/******************************************************************************
 *  rawverse.cpp   - code for class 'RawVerse'- a module that reads raw text
 *			files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *			and provides lookup and parsing functions based on
 *			class VerseKey
 */


#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#ifndef __GNUC__
#include <io.h>
#include <sys/stat.h>
#else
#include <unistd.h>
#endif

#include <utilfuns.h>
#include <rawverse.h>
#include <versekey.h>
#include <sysdata.h>

#ifndef O_BINARY		// O_BINARY is needed in Borland C++ 4.53
#define O_BINARY 0		// If it hasn't been defined than we probably
#endif				// don't need it.

SWORD_NAMESPACE_START

/******************************************************************************
 * RawVerse Statics
 */

int RawVerse::instance = 0;
const char *RawVerse::nl = "\r\n";


/******************************************************************************
 * RawVerse Constructor - Initializes data for instance of RawVerse
 *
 * ENT:	ipath - path of the directory where data and index files are located.
 *		be sure to include the trailing separator (e.g. '/' or '\')
 *		(e.g. 'modules/texts/rawtext/webster/')
 */

RawVerse::RawVerse(const char *ipath, int fileMode)
{
	char *buf;

	path = 0;
	stdstr(&path, ipath);
     buf = new char [ strlen(path) + 80 ];
	if ((path[strlen(path)-1] == '/') || (path[strlen(path)-1] == '\\'))
		path[strlen(path)-1] = 0;

	if (fileMode == -1) { // try read/write if possible
		fileMode = O_RDWR;
	}
		
	sprintf(buf, "%s/ot.vss", path);
	idxfp[0] = FileMgr::getSystemFileMgr()->open(buf, fileMode|O_BINARY, true);

	sprintf(buf, "%s/nt.vss", path);
	idxfp[1] = FileMgr::getSystemFileMgr()->open(buf, fileMode|O_BINARY, true);

	sprintf(buf, "%s/ot", path);
	textfp[0] = FileMgr::getSystemFileMgr()->open(buf, fileMode|O_BINARY, true);

	sprintf(buf, "%s/nt", path);
	textfp[1] = FileMgr::getSystemFileMgr()->open(buf, fileMode|O_BINARY, true);

	delete [] buf;
	instance++;
}


/******************************************************************************
 * RawVerse Destructor - Cleans up instance of RawVerse
 */

RawVerse::~RawVerse()
{
	int loop1;

	if (path)
		delete [] path;

	--instance;

	for (loop1 = 0; loop1 < 2; loop1++) {
		FileMgr::getSystemFileMgr()->close(idxfp[loop1]);
		FileMgr::getSystemFileMgr()->close(textfp[loop1]);
	}
}


/******************************************************************************
 * RawVerse::findoffset	- Finds the offset of the key verse from the indexes
 *
 * ENT: testmt	- testament to find (0 - Bible/module introduction)
 *	idxoff	- offset into .vss
 *	start	- address to store the starting offset
 *	size	- address to store the size of the entry
 */

void RawVerse::findOffset(char testmt, long idxoff, long *start, unsigned short *size) {
	idxoff *= 6;
	if (!testmt)
		testmt = ((idxfp[1]) ? 1:2);
		
	if (idxfp[testmt-1]->getFd() >= 0) {
		lseek(idxfp[testmt-1]->getFd(), idxoff, SEEK_SET);
		read(idxfp[testmt-1]->getFd(), start, 4);
		long len = read(idxfp[testmt-1]->getFd(), size, 2); 		// read size

		*start = swordtoarch32(*start);
		*size  = swordtoarch16(*size);

		if (len < 2) {
			*size = (unsigned short)((*start) ? (lseek(textfp[testmt-1]->getFd(), 0, SEEK_END) - (long)*start) : 0);	// if for some reason we get an error reading size, make size to end of file
		}
	}
	else {
		*start = 0;
		*size = 0;
	}
}


/******************************************************************************
 * RawVerse::preptext	- Prepares the text before returning it to external
 *				objects
 *
 * ENT:	buf	- buffer where text is stored and where to store the prep'd
 *			text.
 */

void RawVerse::prepText(SWBuf &buf) {
	unsigned int to, from; 
	char space = 0, cr = 0, realdata = 0, nlcnt = 0;
	char *rawBuf = buf.getRawData();
	for (to = from = 0; rawBuf[from]; from++) {
		switch (rawBuf[from]) {
		case 10:
			if (!realdata)
				continue;
			space = (cr) ? 0 : 1;
			cr = 0;
			nlcnt++;
			if (nlcnt > 1) {
//				*to++ = nl;
				rawBuf[to++] = 10;
//				*to++ = nl[1];
//				nlcnt = 0;
			}
			continue;
		case 13:
			if (!realdata)
				continue;
//			*to++ = nl[0];
			rawBuf[to++] = 10;
			space = 0;
			cr = 1;
			continue;
		}
		realdata = 1;
		nlcnt = 0;
		if (space) {
			space = 0;
			if (rawBuf[from] != ' ') {
				rawBuf[to++] = ' ';
				from--;
				continue;
			}
		}
		rawBuf[to++] = rawBuf[from];
	}
	buf.setSize(to);

	while (to > 1) {			// remove trailing excess
		to--;
		if ((rawBuf[to] == 10) || (rawBuf[to] == ' '))
			buf.setSize(to);
		else break;
	}
}


/******************************************************************************
 * RawVerse::readtext	- gets text at a given offset
 *
 * ENT:	testmt	- testament file to search in (0 - Old; 1 - New)
 *	start	- starting offset where the text is located in the file
 *	size	- size of text entry + 2 (null)(null)
 *	buf	- buffer to store text
 *
 */

void RawVerse::readText(char testmt, long start, unsigned short size, SWBuf &buf) {
	buf = "";
	buf.setFillByte(0);
	buf.setSize(size + 1);
	if (!testmt)
		testmt = ((idxfp[1]) ? 1:2);
	if (size) {
		if (textfp[testmt-1]->getFd() >= 0) {
			lseek(textfp[testmt-1]->getFd(), start, SEEK_SET);
			read(textfp[testmt-1]->getFd(), buf.getRawData(), (int)size); 
		}
	}
}


/******************************************************************************
 * RawVerse::settext	- Sets text for current offset
 *
 * ENT: testmt	- testament to find (0 - Bible/module introduction)
 *	idxoff	- offset into .vss
 *	buf	- buffer to store
 *      len     - length of buffer (0 - null terminated)
 */

void RawVerse::doSetText(char testmt, long idxoff, const char *buf, long len)
{
	long start, outstart;
	unsigned short size;
	unsigned short outsize;

	idxoff *= 6;
	if (!testmt)
		testmt = ((idxfp[1]) ? 1:2);

	size = outsize = (len < 0) ? strlen(buf) : len;

	start = outstart = lseek(textfp[testmt-1]->getFd(), 0, SEEK_END);
	lseek(idxfp[testmt-1]->getFd(), idxoff, SEEK_SET);

	if (size) {
		lseek(textfp[testmt-1]->getFd(), start, SEEK_SET);
		write(textfp[testmt-1]->getFd(), buf, (int)size);

		// add a new line to make data file easier to read in an editor
		write(textfp[testmt-1]->getFd(), nl, 2);
	}
	else {
		start = 0;
	}

	outstart = archtosword32(start);
	outsize  = archtosword16(size);

	write(idxfp[testmt-1]->getFd(), &outstart, 4);
	write(idxfp[testmt-1]->getFd(), &outsize, 2);


}


/******************************************************************************
 * RawVerse::linkentry	- links one entry to another
 *
 * ENT: testmt	- testament to find (0 - Bible/module introduction)
 *	destidxoff	- dest offset into .vss
 *	srcidxoff		- source offset into .vss
 */

void RawVerse::doLinkEntry(char testmt, long destidxoff, long srcidxoff) {
	long start;
	unsigned short size;

	destidxoff *= 6;
	srcidxoff  *= 6;

	if (!testmt)
		testmt = ((idxfp[1]) ? 1:2);

	// get source
	lseek(idxfp[testmt-1]->getFd(), srcidxoff, SEEK_SET);
	read(idxfp[testmt-1]->getFd(), &start, 4);
	read(idxfp[testmt-1]->getFd(), &size, 2);

	// write dest
	lseek(idxfp[testmt-1]->getFd(), destidxoff, SEEK_SET);
	write(idxfp[testmt-1]->getFd(), &start, 4);
	write(idxfp[testmt-1]->getFd(), &size, 2);
}


/******************************************************************************
 * RawVerse::CreateModule	- Creates new module files
 *
 * ENT: path	- directory to store module files
 * RET: error status
 */

char RawVerse::createModule(const char *ipath)
{
	char *path = 0;
	char *buf = new char [ strlen (ipath) + 20 ];
	FileDesc *fd, *fd2;

	stdstr(&path, ipath);

	if ((path[strlen(path)-1] == '/') || (path[strlen(path)-1] == '\\'))
		path[strlen(path)-1] = 0;

	sprintf(buf, "%s/ot", path);
	FileMgr::removeFile(buf);
	fd = FileMgr::getSystemFileMgr()->open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd->getFd();
	FileMgr::getSystemFileMgr()->close(fd);

	sprintf(buf, "%s/nt", path);
	FileMgr::removeFile(buf);
	fd = FileMgr::getSystemFileMgr()->open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd->getFd();
	FileMgr::getSystemFileMgr()->close(fd);

	sprintf(buf, "%s/ot.vss", path);
	FileMgr::removeFile(buf);
	fd = FileMgr::getSystemFileMgr()->open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd->getFd();

	sprintf(buf, "%s/nt.vss", path);
	FileMgr::removeFile(buf);
	fd2 = FileMgr::getSystemFileMgr()->open(buf, O_CREAT|O_WRONLY|O_BINARY, S_IREAD|S_IWRITE);
	fd2->getFd();

	VerseKey vk;
	vk.Headings(1);
	long offset = 0;
	short size = 0;
	for (vk = TOP; !vk.Error(); vk++) {
		write((vk.Testament() == 1) ? fd->getFd() : fd2->getFd(), &offset, 4);
		write((vk.Testament() == 1) ? fd->getFd() : fd2->getFd(), &size, 2);
	}

	FileMgr::getSystemFileMgr()->close(fd);
	FileMgr::getSystemFileMgr()->close(fd2);

	delete [] path;
	delete [] buf;
/*
	RawVerse rv(path);
	VerseKey mykey("Rev 22:21");
*/
	
	return 0;
}

SWORD_NAMESPACE_END

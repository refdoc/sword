/******************************************************************************
 *
 *  osismarkdown.cpp -	Render filter for MarkDown of an OSIS module
 *
 * $Id: osismarkdown.cpp 3386 2015-09-03 11:54:51Z refdoc $
 *
 * Copyright 2011-2014 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#include <stdlib.h>
#include <ctype.h>
#include <osismarkdown.h>
#include <utilxml.h>
#include <utilstr.h>
#include <versekey.h>
#include <swmodule.h>
#include <url.h>
#include <stringmgr.h>
#include <stack>

SWORD_NAMESPACE_START

const char *OSISMarkDown::getHeader() const {
// can be used to return static start-up info, like packages to load. Not sure yet if I want to retain it.

	const static char *header = "MarkDown 1.01\n";
	return header;
}


namespace {

// though this might be slightly slower, possibly causing an extra bool check, this is a renderFilter
// so speed isn't the absolute highest priority, and this is a very minor possible hit
static inline void outText(const char *t, SWBuf &o, BasicFilterUserData *u) { if (!u->suspendTextPassThru) o += t; else u->lastSuspendSegment += t; }
static inline void outText(char t, SWBuf &o, BasicFilterUserData *u) { if (!u->suspendTextPassThru) o += t; else u->lastSuspendSegment += t; }

void processLemma(bool suspendTextPassThru, XMLTag &tag, SWBuf &buf) {
	const char *attrib;
	if ((attrib = tag.getAttribute("lemma"))) {
		int count = tag.getAttributePartCount("lemma", ' ');
		int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
		do {
			attrib = tag.getAttribute("lemma", i, ' ');
			if (i < 0) i = 0;	// to handle our -1 condition
			if (!suspendTextPassThru) {
					buf.appendFormatted("(%s)",
							attrib);
				}
			
			
		} while (++i < count);
	}
}



void processMorph(bool suspendTextPassThru, XMLTag &tag, SWBuf &buf) {
	const char * attrib;
	if ((attrib = tag.getAttribute("morph"))) { // && (show)) {
		SWBuf savelemma = tag.getAttribute("savlm");
		//if ((strstr(savelemma.c_str(), "3588")) && (lastText.length() < 1))
		//	show = false;
		//if (show) {
			int count = tag.getAttributePartCount("morph", ' ');
			int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
			do {
				attrib = tag.getAttribute("morph", i, ' ');
				if (i < 0) i = 0;	// to handle our -1 condition
				if (!suspendTextPassThru) {
					buf.appendFormatted("(%s)",
							attrib
							);
				}
			} while (++i < count);
		//}
	}
}


}	// end anonymous namespace

BasicFilterUserData *OSISMarkDown::createUserData(const SWModule *module, const SWKey *key) {
	return new MyUserData(module, key);
}


OSISMarkDown::OSISMarkDown() {
	setTokenStart("<");
	setTokenEnd(">");

	setEscapeStart("&");
	setEscapeEnd(";");

	setEscapeStringCaseSensitive(true);
	setPassThruNumericEscapeString(true);

	addAllowedEscapeString("quot");
	addAllowedEscapeString("apos");
	addAllowedEscapeString("amp");
	addAllowedEscapeString("lt");
	addAllowedEscapeString("gt");

	setTokenCaseSensitive(true);
	
	//	addTokenSubstitute("lg",  "<br />");
	//	addTokenSubstitute("/lg", "<br />");

	morphFirst = false;
	renderNoteNumbers = false;
}

class OSISMarkDown::TagStack : public std::stack<SWBuf> {
};

OSISMarkDown::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key), quoteStack(new TagStack()), hiStack(new TagStack()), titleStack(new TagStack()), lineStack(new TagStack()) {
	inXRefNote    = false;
	suspendLevel = 0;
	divLevel = "module";
	wordsOfChristStart = "";
	wordsOfChristEnd   = "";
	consecutiveNewlines = 0;
	firstCell = false;
}

OSISMarkDown::MyUserData::~MyUserData() {
	delete quoteStack;
	delete hiStack;
	delete titleStack;
	delete lineStack;
}

void OSISMarkDown::MyUserData::outputNewline(SWBuf &buf) {
	if (++consecutiveNewlines <= 2) {
		outText("\n", buf, this);
		supressAdjacentWhitespace = true;
	}
}
bool OSISMarkDown::handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData) {
	MyUserData *u = (MyUserData *)userData;
	SWBuf scratch;
	
	bool sub = (u->suspendTextPassThru) ? substituteToken(scratch, token) : substituteToken(buf, token);
	if (!sub) {
  // manually process if it wasn't a simple substitution
		XMLTag tag(token);
		
		// <w> tag
		if (!strcmp(tag.getName(), "w")) {
 
			// start <w> tag
			if ((!tag.isEmpty()) && (!tag.isEndTag())) {
				u->w = token;
			}

			// end or empty <w> tag
			else {
				bool endTag = tag.isEndTag();
				SWBuf lastText;
				//bool show = true;	// to handle unplaced article in kjv2003-- temporary till combined

				if (endTag) {
					tag = u->w.c_str();
					lastText = u->lastTextNode.c_str();
				}
				else lastText = "stuff";

				const char *attrib;
				const char *val;
				if ((attrib = tag.getAttribute("xlit"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					outText(" ", buf, u);
					outText(val, buf, u);
				}
				if ((attrib = tag.getAttribute("gloss"))) {
					// I'm sure this is not the cleanest way to do it, but it gets the job done
					// for rendering ruby chars properly ^_^
					buf -= lastText.length();
					
					outText("(", buf, u);
					outText(lastText, buf, u);
					outText(")(", buf, u);
					outText(attrib, buf, u);
					outText(")", buf, u);
				}
				if (!morphFirst) {
					processLemma(u->suspendTextPassThru, tag, buf);
					processMorph(u->suspendTextPassThru, tag, buf);
				}
				else {
					processMorph(u->suspendTextPassThru, tag, buf);
					processLemma(u->suspendTextPassThru, tag, buf);
				}
				if ((attrib = tag.getAttribute("POS"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					outText(" ", buf, u);
					outText(val, buf, u);
				}

				
			}
		}

		// <note> tag
		
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEndTag()) {
				SWBuf type = tag.getAttribute("type");
				bool strongsMarkup = (type == "x-strongsMarkup" || type == "strongsMarkup");	// the latter is deprecated
				if (strongsMarkup) {
					tag.setEmpty(false);	// handle bug in KJV2003 module where some note open tags were <note ... />
				}

				if (!tag.isEmpty()) {

					if (!strongsMarkup) {	// leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
						SWBuf footnoteNumber = tag.getAttribute("swordFootnote");
						SWBuf footnoteBody = "";
						if (u->module){
							footnoteBody += u->module->getEntryAttributes()["Footnote"][footnoteNumber]["body"];
						}
						SWBuf noteName = tag.getAttribute("n");


						u->inXRefNote = true; // Why this change? Ben Morgan: Any note can have references in, so we need to set this to true for all notes
//						u->inXRefNote = (ch == 'x');

						buf.appendFormatted("[%s][",
								 
							footnoteNumber.c_str()); 
						if (u->module) {
							outText( u->module->renderText(footnoteBody).c_str(), buf, u);
						}
					}
				}
				u->suspendTextPassThru = (++u->suspendLevel);
			}
			if (tag.isEndTag()) {
				u->suspendTextPassThru = (--u->suspendLevel);
				u->inXRefNote = false;
				u->lastSuspendSegment = ""; // fix/work-around for nasb divineName in note bug
				outText("]", buf, u);
			}
		}

		// <p> paragraph and <lg> linegroup tags
		else if (!strcmp(tag.getName(), "p") || !strcmp(tag.getName(), "lg")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {	// non-empty start tag
				u->outputNewline(buf);
			}
			else if (tag.isEndTag()) {	// end tag
				u->outputNewline(buf);
			}
			else {					// empty paragraph break marker
				u->outputNewline(buf);
			}
		}

		// Milestoned paragraphs, created by osis2mod
		// <div type="paragraph" sID.../>
		// <div type="paragraph" eID.../>
		else if (tag.isEmpty() && !strcmp(tag.getName(), "div") && tag.getAttribute("type") && (!strcmp(tag.getAttribute("type"), "x-p") || !strcmp(tag.getAttribute("type"), "paragraph"))) {
			// <div type="paragraph"  sID... />
			if (tag.getAttribute("sID")) {	// non-empty start tag
				u->outputNewline(buf);
			}
			// <div type="paragraph"  eID... />
			else if (tag.getAttribute("eID")) {
				u->outputNewline(buf);
			}
		}

		// <reference> tag
		else if (!strcmp(tag.getName(), "reference")) {	
			if (!u->inXRefNote) {	// only show these if we're not in an xref note				
				if (!tag.isEndTag()) {
					SWBuf target;
					SWBuf work;
					SWBuf ref;
					bool is_scripRef = false;

					target = tag.getAttribute("osisRef");
					const char* the_ref = strchr(target, ':');
					
					if(!the_ref) {
						// No work
						ref = target;
						is_scripRef = true;
					}
					else {
						// Compensate for starting :
						ref = the_ref + 1;

						int size = target.size() - ref.size() - 1;
						work.setSize(size);
						strncpy(work.getRawData(), target, size);

						// For Bible:Gen.3.15 or Bible.vulgate:Gen.3.15
						if(!strncmp(work, "Bible", 5))
							is_scripRef = true;
					}

					if(is_scripRef)
					{
						outText( "(", buf, u);
						outText( ref.c_str(), buf, u);
						outText( " - ", buf, u);
//							(work.size()) ? URL::encode(work.c_str()).c_str() : "")
						
					}
					else
					{
						// Dictionary link, or something
						buf.appendFormatted("(%s - %s - ", // work, entry
							work.c_str(),
							ref.c_str()
							);
					}
				}
				else {
					outText(") ", buf, u);
				}
			}
		}

		// <l> poetry, etc
		else if (!strcmp(tag.getName(), "l")) {
			// start line marker
			if (tag.getAttribute("sID") || (!tag.isEndTag() && !tag.isEmpty())) {
				// nested lines plus if the line itself has an x-indent type attribute value
				outText("\\swordpoetryline{", buf, u);
				u->lineStack->push(tag.toString());
			}
			// end line marker
			else if (tag.getAttribute("eID") || tag.isEndTag()) {
				outText("}", buf, u);
				u->outputNewline(buf);
				if (u->lineStack->size()) u->lineStack->pop();
			}
			// <l/> without eID or sID
			// Note: this is improper osis. This should be <lb/>
			else if (tag.isEmpty() && !tag.getAttribute("sID")) {
				u->outputNewline(buf);
			}
		}

		// <lb.../>
		else if (!strcmp(tag.getName(), "lb") && (!tag.getAttribute("type") || strcmp(tag.getAttribute("type"), "x-optional"))) {
				u->outputNewline(buf);
		}
		// <milestone type="line"/>
		// <milestone type="x-p"/>
		// <milestone type="cQuote" marker="x"/>
		else if ((!strcmp(tag.getName(), "milestone")) && (tag.getAttribute("type"))) {
			if (!strcmp(tag.getAttribute("type"), "line")) {
				u->outputNewline(buf);
				if (tag.getAttribute("subType") && !strcmp(tag.getAttribute("subType"), "x-PM")) {
					u->outputNewline(buf);
				}
			}
			else if (!strcmp(tag.getAttribute("type"),"x-p"))  {
				if (tag.getAttribute("marker"))
					outText(tag.getAttribute("marker"), buf, u);
				else outText("\n\n", buf, u);
			}
			else if (!strcmp(tag.getAttribute("type"), "cQuote")) {
				const char *tmp = tag.getAttribute("marker");
				bool hasMark    = tmp;
				SWBuf mark      = tmp;
				tmp             = tag.getAttribute("level");
				int level       = (tmp) ? atoi(tmp) : 1;

				// first check to see if we've been given an explicit mark
				if (hasMark)
					outText(mark, buf, u);
				// finally, alternate " and ', if config says we should supply a mark
				else if (u->osisQToTick)
					outText((level % 2) ? '\"' : '\'', buf, u);
			}
		}

		// <title>
		
		else if (!strcmp(tag.getName(), "title")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				const char *tmp = tag.getAttribute("type");
				SWBuf type      = tmp;
				
				outText("\n#", buf, u);
				outText(u->divLevel, buf, u);
				
				outText("", buf, u);
			}
			else if (tag.isEndTag()) {
				outText( "\n\n", buf, u);
				++u->consecutiveNewlines;
				u->supressAdjacentWhitespace = true;	
			}
		}
		
		// <list>
		else if (!strcmp(tag.getName(), "list")) {
			if((!tag.isEndTag()) && (!tag.isEmpty())) {
				outText("\n\n", buf, u);
			}
			else if (tag.isEndTag()) {
				outText("\n\n", buf, u);
				++u->consecutiveNewlines;
				u->supressAdjacentWhitespace = true;
			}
		}

		// <item>
		else if (!strcmp(tag.getName(), "item")) {
			if((!tag.isEndTag()) && (!tag.isEmpty())) {
				outText("\n  *  ", buf, u);
			}
			else if (tag.isEndTag()) {
				++u->consecutiveNewlines;
				u->supressAdjacentWhitespace = true;
			}
		}
		// <catchWord> & <rdg> tags (italicize)
		else if (!strcmp(tag.getName(), "rdg") || !strcmp(tag.getName(), "catchWord")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				outText("*", buf, u);
			}
			else if (tag.isEndTag()) {
				outText("*", buf, u);
			}
		}

		// divineName  
		else if (!strcmp(tag.getName(), "divineName")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				outText( "\\sworddivinename{", buf, u);
				u->suspendTextPassThru = (++u->suspendLevel);
			}
			else if (tag.isEndTag()) {
				SWBuf lastText = u->lastSuspendSegment.c_str();
				u->suspendTextPassThru = (--u->suspendLevel);
				if (lastText.size()) {
					scratch.setFormatted("%s}", lastText.c_str());
					outText(scratch.c_str(), buf, u);
				}               
			} 
		}

		// <hi> text highlighting
		else if (!strcmp(tag.getName(), "hi")) {
			SWBuf type = tag.getAttribute("type");

			// handle tei rend attribute if type doesn't exist
			if (!type.length()) type = tag.getAttribute("rend");

			if (type == "bold" || type == "b" || type == "x-b") {
				outText("**", buf, u);
			}
			else {	// all other types
					outText("*", buf, u);
			}
		}

		// <q> quote
		// Rules for a quote element:
		// If the tag is empty with an sID or an eID then use whatever it specifies for quoting.
		//    Note: empty elements without sID or eID are ignored.
		// If the tag is <q> then use it's specifications and push it onto a stack for </q>
		// If the tag is </q> then use the pushed <q> for specification
		// If there is a marker attribute, possibly empty, this overrides osisQToTick.
		// If osisQToTick, then output the marker, using level to determine the type of mark.
		else if (!strcmp(tag.getName(), "q")) {
			SWBuf type      = tag.getAttribute("type");
			SWBuf who       = tag.getAttribute("who");
			const char *tmp = tag.getAttribute("level");
			int level       = (tmp) ? atoi(tmp) : 1;
			tmp             = tag.getAttribute("marker");
			bool hasMark    = tmp;
			SWBuf mark      = tmp;

			// open <q> or <q sID... />
			if ((!tag.isEmpty() && !tag.isEndTag()) || (tag.isEmpty() && tag.getAttribute("sID"))) {
				// if <q> then remember it for the </q>
				if (!tag.isEmpty()) {
					u->quoteStack->push(tag.toString());
				}

				// Do this first so quote marks are included as WoC
				if (who == "Jesus")
					outText(u->wordsOfChristStart, buf, u);

				// first check to see if we've been given an explicit mark
				if (hasMark)
					outText(mark, buf, u);
				//alternate " and '
				else if (u->osisQToTick)
					outText((level % 2) ? '\"' : '\'', buf, u);
			}
			// close </q> or <q eID... />
			else if ((tag.isEndTag()) || (tag.isEmpty() && tag.getAttribute("eID"))) {
				// if it is </q> then pop the stack for the attributes
				if (tag.isEndTag() && !u->quoteStack->empty()) {
					XMLTag qTag(u->quoteStack->top());
					if (u->quoteStack->size()) u->quoteStack->pop();

					type    = qTag.getAttribute("type");
					who     = qTag.getAttribute("who");
					tmp     = qTag.getAttribute("level");
					level   = (tmp) ? atoi(tmp) : 1;
					tmp     = qTag.getAttribute("marker");
					hasMark = tmp;
					mark    = tmp;
				}

				// first check to see if we've been given an explicit mark
				if (hasMark)
					outText(mark, buf, u);
				// finally, alternate " and ', if config says we should supply a mark
				else if (u->osisQToTick)
					outText((level % 2) ? '\"' : '\'', buf, u);

				// Do this last so quote marks are included as WoC
				if (who == "Jesus")
					outText(u->wordsOfChristEnd, buf, u);
			}
		}

		// <transChange>
		else if (!strcmp(tag.getName(), "transChange")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				SWBuf type = tag.getAttribute("type");
				u->lastTransChange = type;

				// just do all transChange tags this way for now
				if ((type == "added") || (type == "supplied"))
					outText("\\swordtranschange{supplied}{", buf, u);
				else if (type == "tenseChange")
					outText( "\\swordtranschange{tense}{", buf, u);
			}
			else if (tag.isEndTag()) {
				outText("}", buf, u);
			}
			else {	// empty transChange marker?
			}
		}

		// image
		else if (!strcmp(tag.getName(), "figure")) {
			const char *src = tag.getAttribute("src");
			if (src) {		// assert we have a src attribute 
				SWBuf filepath;
				if (userData->module) {
					filepath = userData->module->getConfigEntry("AbsoluteDataPath");
					if ((filepath.size()) && (filepath[filepath.size()-1] != '/') && (src[0] != '/'))
						filepath += '/';
				}
				filepath += src;

				outText("\\figure{", buf, u);
				outText("\\includegraphics{", buf, u);
				outText(filepath.c_str(), buf, u);
				outText("}}", buf, u);
				
			}
		}

		// ok to leave these in
		else if (!strcmp(tag.getName(), "div")) {
			SWBuf type = tag.getAttribute("type");
			if (type == "module") {
				u->divLevel = type;
				outText("\n", buf, u);
			}			
			else if (type == "testament") {
				u->divLevel = type;
				outText("\n", buf, u);
			}
			else if (type == "bookGroup") {
				u->divLevel = type;
				outText("\n", buf, u);
			}
			else if (type == "book") {
				u->divLevel = type;
				outText("\n", buf, u);
			}
			else if (type == "majorSection") {
				u->divLevel = type;
				outText("\n", buf, u);
			}
			else if (type == "section") {
				u->divLevel = type;
				outText("\n", buf, u);
			}
			else if (type == "paragraph") {
				u->divLevel = type;
				outText("\n", buf, u);
			}
		}
		else if (!strcmp(tag.getName(), "span")) {
			outText( "", buf, u);
		}
		else if (!strcmp(tag.getName(), "br")) {
			outText( "\\", buf, u);
		}
		else if (!strcmp(tag.getName(), "table")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				outText( "\n\\begin{tabular}", buf, u);
			}
			else if (tag.isEndTag()) {
				outText( "\n\\end{tabular}", buf, u);
				++u->consecutiveNewlines;
				u->supressAdjacentWhitespace = true;
			}
			
		}
		else if (!strcmp(tag.getName(), "row")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				outText( "\n", buf, u);
				u->firstCell = true;
			}
			else if (tag.isEndTag()) {
				outText( "//", buf, u);
				u->firstCell = false;
			}
			
		}
		else if (!strcmp(tag.getName(), "cell")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (u->firstCell == false) {
					outText( " & ", buf, u);
				}
				else {
					u->firstCell = false;
				}
			}
			else if (tag.isEndTag()) {
				outText( "", buf, u);
			}
		}
		else {
			if (!u->supressAdjacentWhitespace) u->consecutiveNewlines = 0;
			return false;  // we still didn't handle token
		}
	}
	if (!u->supressAdjacentWhitespace) u->consecutiveNewlines = 0;
	return true;
}


SWORD_NAMESPACE_END

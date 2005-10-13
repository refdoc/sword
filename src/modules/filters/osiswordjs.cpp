/******************************************************************************
 *
 * osisstrongs -	SWFilter descendant to hide or show strongs number
 *			in a OSIS module.
 */


#include <stdlib.h>
#include <osiswordjs.h>
#include <swmodule.h>
#include <ctype.h>
#include <utilxml.h>
#include <utilstr.h>
#include <versekey.h>

SWORD_NAMESPACE_START

const char oName[] = "Word Javascript";
const char oTip[] = "Toggles Word Javascript data";

const SWBuf choices[3] = {"Off", "On", ""};
const StringList oValues(&choices[0], &choices[2]);


OSISWordJS::OSISWordJS() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");

     defaultGreekLex   = 0;
     defaultHebLex     = 0;
     defaultGreekParse = 0;
     defaultHebParse   = 0;
     mgr               = 0;
}


OSISWordJS::~OSISWordJS() {
}


char OSISWordJS::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	if (option) {
		char token[2112]; // cheese.  Fix.
		int tokpos = 0;
		bool intoken = false;
		bool lastspace = false;
		int wordNum = 1;
		char val[128];
		char *valto;
		char *ch;
		SWBuf modName = (module)?module->Name():"";
		// add TR to w src in KJV then remove this next line
		SWBuf wordSrcPrefix = (modName == "KJV")?SWBuf("TR"):modName;
		
		const SWBuf orig = text;
		const char * from = orig.c_str();

		for (text = ""; *from; ++from) {
			if (*from == '<') {
				intoken = true;
				tokpos = 0;
				token[0] = 0;
				token[1] = 0;
				token[2] = 0;
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				if ((*token == 'w') && (token[1] == ' ')) {	// Word
					XMLTag wtag(token);
					SWBuf lemma = wtag.getAttribute("lemma");
					SWBuf strong = "";
					SWBuf morph = wtag.getAttribute("morph");
					SWBuf xlit = wtag.getAttribute("xlit");
					SWBuf src = wtag.getAttribute("src");
					if (!src.length()) src.appendFormatted("%d", wordNum);
					src.insert(0, wordSrcPrefix);
					char gh = 0;
					VerseKey *vkey = 0;
					if (key) {
						vkey = SWDYNAMIC_CAST(VerseKey, key);
					}
					SWBuf lexName = "";
					if ((!strncmp(lemma.c_str(), "x-Strongs:", 10)) || (!strncmp(lemma.c_str(), "strong:", 7))) {
						const char *num = strstr(lemma.c_str(), ":");
						num++;
						gh = isdigit(*num) ? 0:*num;
						if (!gh) {
							if (vkey) {
								gh = vkey->Testament() ? 'H' : 'G';
							}
						}
						else num++;
						strong = num;
						SWModule *sLex = 0;
						if (gh == 'G') {
							sLex = defaultGreekLex;
						}
						if (gh == 'H') {
							sLex = defaultHebLex;
						}
						if (sLex) {
							// we can pass the real lex name in, but we have some
							// aliases in the javascript to optimize bandwidth
							lexName = sLex->Name();
							if (lexName == "StrongsGreek")
								lexName = "G";
							if (lexName == "StrongsHebrew")
								lexName = "H";
						}
					}
					else if (!strncmp(xlit.c_str(), "betacode:", 9)) {
						lexName = "betacode";
//						const char *m = strchr(xlit.c_str(), ':');
//						strong = ++m;
						strong = lemma;
					}
					SWBuf wordID;
					if (vkey) {
						// optimize for bandwidth and use only the verse as the unique entry id
						wordID.appendFormatted("%d", vkey->Verse());
					}
					else {
						wordID = key->getText();
					}
					for (int i = 0; i < wordID.size(); i++) {
						if ((!isdigit(wordID[i])) && (!isalpha(wordID[i]))) {
							wordID[i] = '_';
						}
					}
					wordID.appendFormatted("_%s", src.c_str());
					// 'p' = 'fillpop' to save bandwidth
					const char *m = strchr(morph.c_str(), ':');
					if (m) m++;
					else m = morph.c_str();
					text.appendFormatted("<span class=\"clk\" onclick=\"p(\'%s\',\'%s\','%s','%s');\" >", lexName.c_str(), strong.c_str(), wordID.c_str(), m);
					wordNum++;
				}
				if ((*token == '/') && (token[1] == 'w') && option) {	// Word
					text += "</w></span>";
					continue;
				}
				
				// if not a strongs token, keep token in text
				text.append('<');
				text.append(token);
				text.append('>');
				
				continue;
			}
			if (intoken) {
				if (tokpos < 2045)
					token[tokpos++] = *from;
					token[tokpos+2] = 0;
			}
			else	{
				text.append(*from);
				lastspace = (*from == ' ');
			}
		}
	}
	return 0;
}

SWORD_NAMESPACE_END

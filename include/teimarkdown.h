/******************************************************************************
 *
 *  teimarkdown.h -	Implementation of TEIMarkDown
 *
 * $Id: teimarkdown.h 3392 2015-12-19 21:00:55Z refdoc $
 *
 * Copyright 2012-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef TEIMARKDOWN_H
#define TEIMARKDOWN_H

#include <swbasicfilter.h>

SWORD_NAMESPACE_START

/** this filter converts TEI text to MarkDown text
 */
class SWDLLEXPORT TEIMarkDown : public SWBasicFilter {
private:
	bool renderNoteNumbers;

protected:
	class MyUserData : public BasicFilterUserData {
	public:
		bool BiblicalText;
		SWBuf lastHi;
		bool firstCell; // for tables, indicates whether a cell is the first one in a row
		int consecutiveNewlines;
		
		SWBuf version;
		MyUserData(const SWModule *module, const SWKey *key);
	};
	virtual BasicFilterUserData *createUserData(const SWModule *module, const SWKey *key) {
		return new MyUserData(module, key);
	}
	virtual bool handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData);
public:
	TEIMarkDown();
	void setRenderNoteNumbers(bool val = true) { renderNoteNumbers = val; }
};

SWORD_NAMESPACE_END
#endif

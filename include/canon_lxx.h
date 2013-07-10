/******************************************************************************
 *
 *  canon_lxx.h -	Versification data for the LXX system
 *
 * $Id$
 *
 * Copyright 2013 CrossWire Bible Society (http://www.crosswire.org)
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
 */

#ifndef CANON_LXX_H
#define CANON_LXX_H

SWORD_NAMESPACE_START


// Versification system: LXX
// Book order: Gen Exod Lev Num Deut Josh Judg Ruth 1Sam 2Sam 1Kgs 2Kgs 1Chr 2Chr 1Esd Ezra Esth Jdt Tob 1Macc 2Macc 3Macc 4Macc Ps PrMan Prov Eccl Song Job Wis Sir PssSol Hos Amos Mic Joel Obad Jonah Nah Hab Zeph Hag Zech Mal Isa Jer Bar Lam EpJer Ezek Sus SusTh Dan DanTh Bel Matt Mark Luke John Acts Rom 1Cor 2Cor Gal Eph Phil Col 1Thess 2Thess 1Tim 2Tim Titus Phlm Heb Jas 1Pet 2Pet 1John 2John 3John Jude Rev

// This is a compromise versification, intended to allow encoding of Bibles using a number of slightly varying LXX-based versification systems. In particular, it was produced in consultation with Rahlfs' & Swete's editions of the LXX, the NETS and OSB. This is not intended for critical editions that separately encode variant traditions of Joshua, Judges, Tobit, etc. that are already served by the Rahlfs versification system (q.v.).
// In addition to the LXX OT, the GNT has been included in this system to facilitate production of modern Bibles such as the OSB, which are translated from the LXX but include a NT translation.

/******************************************************************************
 * [on]tbooks_lxx - initialize static instance for all canonical
 *		 text names and chapmax
 */
struct sbook otbooks_lxx[] = {
  {"Genesis", "Gen", "Gen", 50},
  {"Exodus", "Exod", "Exod", 40},
  {"Leviticus", "Lev", "Lev", 27},
  {"Numbers", "Num", "Num", 36},
  {"Deuteronomy", "Deut", "Deut", 34},
  {"Joshua", "Josh", "Josh", 24},
  {"Judges", "Judg", "Judg", 21},
  {"Ruth", "Ruth", "Ruth", 4},
  {"I Samuel", "1Sam", "1Sam", 31},
  {"II Samuel", "2Sam", "2Sam", 24},
  {"I Kings", "1Kgs", "1Kgs", 22},
  {"II Kings", "2Kgs", "2Kgs", 25},
  {"I Chronicles", "1Chr", "1Chr", 29},
  {"II Chronicles", "2Chr", "2Chr", 36},
  {"I Esdras", "1Esd", "1Esd", 9},
  {"Ezra", "Ezra", "Ezra", 10},
  {"Nehemiah", "Neh", "Neh", 13},
  {"Esther", "Esth", "Esth", 10},
  {"Judith", "Jdt", "Jdt", 16},
  {"Tobit", "Tob", "Tob", 14},
  {"I Maccabees", "1Macc", "1Macc", 16},
  {"II Maccabees", "2Macc", "2Macc", 15},
  {"III Maccabees", "3Macc", "3Macc", 7},
  {"IV Maccabees", "4Macc", "4Macc", 18},
  {"Psalms", "Ps", "Ps", 151},
  {"Prayer of Manasses", "PrMan", "PrMan", 14},
  {"Proverbs", "Prov", "Prov", 31},
  {"Ecclesiastes", "Eccl", "Eccl", 12},
  {"Song of Solomon", "Song", "Song", 8},
  {"Job", "Job", "Job", 42},
  {"Wisdom", "Wis", "Wis", 19},
  {"Sirach", "Sir", "Sir", 51},
  {"Psalms of Solomon", "PssSol", "PssSol", 18},
  {"Hosea", "Hos", "Hos", 14},
  {"Amos", "Amos", "Amos", 9},
  {"Micah", "Mic", "Mic", 7},
  {"Joel", "Joel", "Joel", 4},
  {"Obadiah", "Obad", "Obad", 1},
  {"Jonah", "Jonah", "Jonah", 4},
  {"Nahum", "Nah", "Nah", 3},
  {"Habakkuk", "Hab", "Hab", 3},
  {"Zephaniah", "Zeph", "Zeph", 3},
  {"Haggai", "Hag", "Hag", 2},
  {"Zechariah", "Zech", "Zech", 14},
  {"Malachi", "Mal", "Mal", 3},
  {"Isaiah", "Isa", "Isa", 66},
  {"Jeremiah", "Jer", "Jer", 52},
  {"Baruch", "Bar", "Bar", 5},
  {"Lamentations", "Lam", "Lam", 5},
  {"Epistle of Jeremiah", "EpJer", "EpJer", 1},
  {"Ezekiel", "Ezek", "Ezek", 48},
  {"Susanna", "Sus", "Sus", 1},
  {"Daniel", "Dan", "Dan", 12},
  {"Bel and the Dragon", "Bel", "Bel", 1},
  {"I Enoch", "1En", "1En", 108},
  {"Odes", "Odes", "Odes", 14},
  {"", "", "", 0}
};

/******************************************************************************
 *	Maximum verses per chapter
 */

int vm_lxx[] = {
  // Genesis
  31, 25, 24, 26, 32, 22, 24, 22, 29, 32, 32, 20, 18, 24, 21, 16, 27, 33, 38, 18, 34, 24, 20, 67, 34, 35, 46, 22, 35, 43, 55, 33, 20, 31, 29, 43, 36, 30, 23, 23, 57, 38, 34, 34, 28, 34, 31, 22, 33, 26,
  // Exodus
  22, 25, 22, 31, 23, 30, 29, 32, 35, 29, 10, 51, 22, 31, 27, 36, 16, 27, 25, 26, 37, 31, 33, 18, 40, 37, 21, 43, 46, 38, 18, 35, 23, 35, 35, 40, 21, 27, 23, 38,
  // Leviticus
  17, 16, 17, 35, 26, 40, 38, 36, 24, 20, 47, 8, 59, 57, 33, 34, 16, 30, 37, 27, 24, 33, 44, 23, 55, 46, 34,
  // Numbers
  54, 34, 51, 49, 31, 26, 89, 26, 23, 36, 35, 16, 34, 45, 41, 50, 28, 32, 22, 29, 35, 41, 30, 25, 18, 65, 23, 31, 39, 17, 54, 42, 56, 29, 34, 13,
  // Deuteronomy
  46, 37, 29, 49, 33, 25, 26, 20, 29, 22, 32, 32, 19, 29, 23, 22, 20, 22, 21, 20, 23, 30, 26, 22, 19, 19, 26, 69, 29, 20, 30, 52, 29, 12,
  // Joshua
  18, 24, 17, 24, 15, 27, 26, 29, 33, 42, 23, 24, 32, 15, 63, 10, 18, 28, 51, 9, 45, 34, 16, 33,
  // Judges
  36, 23, 31, 24, 31, 40, 25, 35, 57, 18, 40, 15, 25, 20, 20, 31, 13, 31, 30, 48, 25,
  // Ruth
  22, 23, 18, 22,
  // I Samuel
  28, 36, 21, 22, 12, 21, 17, 22, 27, 27, 15, 25, 23, 52, 35, 23, 54, 29, 24, 43, 16, 23, 28, 23, 44, 25, 12, 25, 11, 31, 13,
  // II Samuel
  27, 32, 39, 12, 25, 23, 29, 18, 13, 19, 27, 31, 39, 33, 37, 23, 29, 33, 44, 26, 22, 51, 39, 25,
  // I Kings
  53, 46, 28, 33, 32, 34, 50, 66, 28, 33, 43, 33, 34, 31, 34, 34, 24, 46, 21, 29, 43, 54,
  // II Kings
  18, 25, 27, 44, 27, 33, 20, 29, 37, 36, 21, 22, 25, 29, 38, 20, 41, 37, 37, 21, 26, 20, 37, 20, 30,
  // I Chronicles
  54, 55, 24, 43, 41, 81, 40, 40, 44, 14, 47, 41, 14, 17, 29, 43, 27, 17, 19, 8, 30, 19, 32, 31, 31, 32, 34, 21, 30,
  // II Chronicles
  18, 18, 17, 22, 14, 42, 22, 18, 31, 19, 23, 16, 23, 15, 19, 14, 19, 34, 11, 37, 20, 12, 21, 27, 28, 23, 9, 27, 36, 27, 21, 33, 25, 33, 27, 23,
  // I Esdras
  55, 26, 24, 63, 71, 33, 15, 92, 55,
  // Ezra
  11, 70, 13, 24, 17, 22, 28, 36, 15, 44,
  // Nehemiah
  11, 20, 37, 23, 19, 19, 73, 18, 38, 40, 36, 47, 31,
  // Esther (chapters 11-16 represent the additions, whether identified as chapters 11-16 or additions A-F)
  22, 23, 15, 17, 14, 14, 10, 17, 32, 13, 17, 7, 30, 19, 24, 24
  // Judith
  16, 28, 10, 15, 24, 21, 32, 36, 14, 23, 23, 20, 20, 19, 14, 25,
  // Tobit
  22, 14, 17, 21, 23, 19, 17, 21, 6, 14, 19, 22, 18, 15,
  // I Maccabees
  64, 70, 60, 61, 68, 63, 50, 32, 73, 89, 74, 53, 53, 49, 41, 24,
  // II Maccabees
  36, 32, 40, 50, 27, 31, 42, 36, 29, 38, 38, 45, 26, 46, 39,
  // III Maccabees
  29, 33, 30, 21, 51, 41, 23,
  // IV Maccabees
  35, 24, 21, 26, 38, 35, 24, 29, 32, 21, 27, 20, 27, 20, 32, 25, 24, 24,
  // Psalms
  6, 12, 9, 9, 13, 11, 18, 10, 39, 7, 9, 6, 7, 5, 11, 15, 51, 15, 10, 14, 32, 6, 10, 22, 12, 14, 9, 11, 13, 25, 11, 22, 23, 28, 13, 40, 23, 14, 18, 14, 12, 5, 27, 18, 12, 10, 15, 21, 23, 21, 11, 7, 9, 24, 14, 12, 12, 18, 14, 9, 13, 12, 11, 14, 20, 8, 36, 37, 6, 24, 20, 28, 23, 11, 13, 21, 72, 13, 20, 17, 8, 19, 13, 14, 17, 7, 19, 53, 17, 16, 16, 5, 23, 11, 13, 12, 9, 9, 5, 8, 29, 22, 35, 45, 48, 43, 14, 31, 7, 10, 10, 9, 26, 9, 10, 2, 29, 176, 7, 8, 9, 4, 8, 5, 6, 5, 6, 8, 8, 3, 18, 3, 3, 21, 26, 9, 8, 24, 14, 10, 8, 12, 15, 21, 10, 11, 9, 14, 9, 6, 7,
  // Prayer of Manasses
  15,
  // Proverbs
  33, 22, 35, 27, 23, 35, 27, 36, 18, 32, 31, 28, 25, 35, 33, 33, 28, 23, 29, 30, 31, 29, 35, 77, 28, 28, 27, 28, 49, 33, 31,
  // Ecclesiastes
  18, 26, 22, 17, 19, 12, 30, 17, 18, 20, 10, 14,
  // Song of Solomon
  17, 17, 11, 16, 17, 12, 14, 14,
  // Job
  22, 13, 26, 21, 27, 30, 21, 22, 35, 22, 20, 25, 28, 22, 35, 23, 16, 21, 29, 29, 34, 30, 17, 25, 6, 14, 23, 28, 25, 31, 40, 22, 33, 37, 16, 33, 24, 41, 35, 32, 26, 17,
  // Wisdom
  16, 24, 19, 20, 23, 25, 30, 21, 18, 21, 26, 27, 19, 31, 19, 29, 21, 25, 22,
  // Sirach
  30, 18, 31, 31, 15, 37, 36, 19, 18, 31, 34, 18, 26, 27, 20, 30, 32, 33, 30, 31, 28, 27, 27, 34, 26, 29, 30, 26, 28, 25, 31, 24, 40, 31, 26, 31, 31, 34, 35, 30, 27, 25, 33, 23, 26, 20, 25, 25, 16, 29, 30,
  // Psalms of Solomon
  8, 41, 16, 29, 22, 9, 10, 40, 20, 9, 9, 8, 12, 10, 15, 15, 51, 14,
  // Hosea
  11, 25, 5, 19, 15, 11, 16, 14, 17, 15, 12, 15, 15, 10,
  // Amos
  15, 16, 15, 13, 27, 14, 17, 14, 15,
  // Micah
  16, 13, 12, 14, 15, 16, 20,
  // Joel
  20, 32, 21, 21,
  // Obadiah
  21,
  // Jonah
  16, 11, 10, 11,
  // Nahum
  15, 14, 19,
  // Habakkuk
  17, 20, 19,
  // Zephaniah
  18, 15, 20,
  // Haggai
  15, 23,
  // Zechariah
  21, 17, 10, 14, 11, 15, 14, 23, 17, 12, 17, 14, 9, 21,
  // Malachi
  14, 17, 24, 6
  // Isaiah
  31, 21, 26, 6, 30, 13, 25, 23, 21, 34, 16, 6, 22, 32, 9, 14, 14, 7, 25, 6, 17, 25, 18, 23, 12, 21, 13, 29, 24, 33, 9, 20, 24, 17, 10, 22, 38, 22, 8, 31, 29, 25, 28, 28, 25, 13, 15, 22, 26, 11, 23, 15, 12, 17, 13, 11, 21, 14, 21, 22, 11, 12, 19, 12, 25, 24,
  // Jeremiah
  19, 37, 25, 31, 31, 30, 34, 23, 26, 25, 23, 17, 27, 22, 21, 21, 27, 23, 15, 18, 14, 30, 40, 10, 20, 28, 46, 64, 23, 33, 44, 38, 24, 22, 17, 32, 24, 40, 44, 13, 22, 19, 32, 21, 28, 18, 16, 18, 22, 13, 35, 34,
  // Baruch
  22, 35, 38, 37, 9,
  // Lamentations
  22, 22, 66, 22, 22,
  // Epistle of Jeremiah
  72,
  // Ezekiel
  28, 10, 27, 17, 17, 14, 27, 18, 11, 22, 25, 28, 23, 23, 8, 63, 24, 32, 14, 49, 37, 31, 49, 27, 17, 21, 36, 26, 21, 26, 18, 32, 33, 31, 15, 38, 28, 23, 29, 49, 26, 20, 27, 31, 25, 24, 23, 35,
  // Susanna
  64,
  // Daniel
  21, 49, 100, 37, 31, 29, 28, 27, 27, 21, 45, 13,
  // Bel and the Dragon
  42,
  // I Enoch
  9, 3, 1, 1, 9, 8, 6, 4, 11, 22, 2, 6, 10, 25, 12, 4, 8, 16, 3, 8, 10, 14, 4, 6, 7, 6, 5, 3, 2, 3, 3, 6, 4, 3, 1, 4, 5, 6, 14, 9, 9, 3, 4, 1, 6, 8, 4, 10, 4, 5, 5, 9, 7, 10, 4, 8, 3, 6, 3, 24, 13, 16, 12, 2, 12, 3, 13, 4, 29, 4, 17, 37, 8, 17, 9, 14, 8, 17, 6, 8, 10, 20, 11, 6, 9, 6, 4, 3, 76, 42, 19, 5, 14, 17, 7, 8, 10, 18, 16, 13, 9, 11, 15, 13, 2, 19, 3, 15
  // Odes
  19, 43, 10, 20, 20, 19, 45, 88, 79, 88, 55, 32, 79, 46,
  // Matthew
  25, 23, 17, 25, 48, 34, 29, 34, 38, 42, 30, 50, 58, 36, 39, 28, 27, 35, 30, 34, 46, 46, 39, 51, 46, 75, 66, 20,
  // Mark
  45, 28, 35, 41, 43, 56, 37, 38, 50, 52, 33, 44, 37, 72, 47, 20,
  // Luke
  80, 52, 38, 44, 39, 49, 50, 56, 62, 42, 54, 59, 35, 35, 32, 31, 37, 43, 48, 47, 38, 71, 56, 53,
  // John
  51, 25, 36, 54, 47, 71, 53, 59, 41, 42, 57, 50, 38, 31, 27, 33, 26, 40, 42, 31, 25,
  // Acts
  26, 47, 26, 37, 42, 15, 60, 40, 43, 48, 30, 25, 52, 28, 41, 40, 34, 28, 41, 38, 40, 30, 35, 27, 27, 32, 44, 31,
  // Romans
  32, 29, 31, 25, 21, 23, 25, 39, 33, 21, 36, 21, 14, 23, 33, 27,
  // I Corinthians
  31, 16, 23, 21, 13, 20, 40, 13, 27, 33, 34, 31, 13, 40, 58, 24,
  // II Corinthians
  24, 17, 18, 18, 21, 18, 16, 24, 15, 18, 33, 21, 14,
  // Galatians
  24, 21, 29, 31, 26, 18,
  // Ephesians
  23, 22, 21, 32, 33, 24,
  // Philippians
  30, 30, 21, 23,
  // Colossians
  29, 23, 25, 18,
  // I Thessalonians
  10, 20, 13, 18, 28,
  // II Thessalonians
  12, 17, 18,
  // I Timothy
  20, 15, 16, 16, 25, 21,
  // II Timothy
  18, 26, 17, 22,
  // Titus
  16, 15, 15,
  // Philemon
  25,
  // Hebrews
  14, 18, 19, 16, 14, 20, 28, 13, 28, 39, 40, 29, 25,
  // James
  27, 26, 18, 17, 20,
  // I Peter
  25, 25, 22, 19, 14,
  // II Peter
  21, 22, 18,
  // I John
  10, 29, 24, 21, 21,
  // II John
  13,
  // III John
  15,
  // Jude
  25,
  // Revelation of John
  20, 29, 22, 11, 14, 17, 17, 13, 21, 11, 19, 18, 18, 20, 8, 21, 18, 24, 21, 15, 27, 21
};


SWORD_NAMESPACE_END


#endif
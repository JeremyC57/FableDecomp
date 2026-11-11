#ifndef FABLE_ENGINE_CWSTRINGPARSER_H
#define FABLE_ENGINE_CWSTRINGPARSER_H
#include "engine/CBaseClass.h"
#include <stddef.h>
class CWParsedItem;
// Original Ego_r4ebab/FableWin CWStringParser,16 bytes. Native00a0cdd0 offsets4/8/c.
class CWStringParser:public CBaseClass {
public:
#ifdef FABLE_RETAIL_CWSTRINGPARSER_READUNTIL
 // Retail opt-in (chunk 2010, row 0x00492381 CSubtitleRenderer::ProcessEmbeddedGraphicTags): Ego_r.pdb / FableWin.pdb ??0CWStringParser@@QAE@PB_W@Z
 // (retail 0x00437260, landed), ?ReadAsStringUntilCharacter@CWStringParser@@QAE?AVCWideString@@_W@Z (0x00A0CCD0, hidden result) and
 // ?SkipWhitespace@CWStringParser@@QAEJXZ (0x00A0C910, returns long). No user-declared destructor: a stack parser is destroyed in retail
 // by a direct call of ~CBaseClass 0x0099A2E0 with no CWStringParser vtable store first (0x00492912, 0x004928FA), the compiler-generated
 // destructor. Wide text is unsigned short (the landed spelling, PBG / G).
 CWStringParser(const unsigned short* text);                         // 0x00437260
 class CWideString ReadAsStringUntilCharacter(unsigned short c);     // 0x00A0CCD0
 long SkipWhitespace();                                              // 0x00A0C910
#else
 virtual ~CWStringParser();
#endif
private:
 const wchar_t* String;
 unsigned long StringPos,Line;
 bool ReadNextNumber(CWParsedItem*);
};
FABLE_STATIC_ASSERT(sizeof(CWStringParser)==16);
#endif

#ifndef FABLE_ENGINE_CWSTRINGPARSER_H
#define FABLE_ENGINE_CWSTRINGPARSER_H
#include "engine/CBaseClass.h"
#include <stddef.h>
#ifdef FABLE_RETAIL_CWSTRINGPARSER_LINES
#include "fable_string.h"
#endif
class CWParsedItem;
// Original Ego_r4ebab/FableWin CWStringParser,16 bytes. Native00a0cdd0 offsets4/8/c.
class CWStringParser:public CBaseClass {
public:
#ifdef FABLE_RETAIL_CWSTRINGPARSER_LINES
 // Retail opt-in (chunk Q4, row 0x00497FD0 CProgressDisplay::DisplayCenteredText): Ego_r.pdb / FableWin.pdb ??0CWStringParser@@QAE@PB_W@Z
 // (an out-of-line COMDAT copy at 0x00437260; DisplayCenteredText expands it: CBaseClass() 0x0099A2D0, the CWStringParser vtable
 // 0x01231718, then Init); no user destructor (the implicit one: retail calls ~CBaseClass 0x0099A2E0 straight after the parser's
 // scope, 0x004983ED / 0x004983F6); ?Init@CWStringParser@@QAEXPB_W@Z 0x00A0C8E0, ?SkipWhitespace@CWStringParser@@QAEJXZ 0x00A0C910,
 // ?ReadAsStringUntilCharacter@CWStringParser@@QAE?AVCWideString@@_W@Z 0x00A0CCD0 (declarations; all landed).
 CWStringParser(const wchar_t* string){Init(string);}
 void Init(const wchar_t* string);                                       // 0x00A0C8E0
 long SkipWhitespace();                                                  // 0x00A0C910
 CWideString ReadAsStringUntilCharacter(wchar_t character);              // 0x00A0CCD0
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

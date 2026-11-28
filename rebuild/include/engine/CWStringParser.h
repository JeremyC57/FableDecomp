#ifndef FABLE_ENGINE_CWSTRINGPARSER_H
#define FABLE_ENGINE_CWSTRINGPARSER_H
#include "engine/CBaseClass.h"
#include <stddef.h>
class CWParsedItem;
// Original Ego_r4ebab/FableWin CWStringParser,16 bytes. Native00a0cdd0 offsets4/8/c.
class CWStringParser:public CBaseClass {
public:
#ifdef FABLE_RETAIL_CWSTRINGPARSER_ICONTEXT_PASS30
 // FableWin 018B4C30 ctor and 030F28A0 ReadNextChunkAsString.
 CWStringParser(const wchar_t* text) { Init(text); }
 void Init(const wchar_t* text);
 bool ReadNextChunkAsString(CWParsedItem*);
#elif defined(FABLE_RETAIL_CWSTRINGPARSER_READUNTIL)
 // Retail opt-in (chunk 2010, row 0x00492381 CSubtitleRenderer::ProcessEmbeddedGraphicTags): Ego_r.pdb / FableWin.pdb ??0CWStringParser@@QAE@PB_W@Z
 // (retail 0x00437260, landed), ?ReadAsStringUntilCharacter@CWStringParser@@QAE?AVCWideString@@_W@Z (0x00A0CCD0, hidden result) and
 // ?SkipWhitespace@CWStringParser@@QAEJXZ (0x00A0C910, returns long). No user-declared destructor: a stack parser is destroyed in retail
 // by a direct call of ~CBaseClass 0x0099A2E0 with no CWStringParser vtable store first (0x00492912, 0x004928FA), the compiler-generated
 // destructor. Wide text is unsigned short (the landed spelling, PBG / G).
#if defined(FABLE_RETAIL_CWSTRINGPARSER_INLINE_CTOR) && defined(FABLE_RETAIL_CWSTRINGPARSER_INIT)
 // Nested opt-in (egod e12, row 0x00497FD0 CProgressDisplay::DisplayCenteredText): the same constructor as a header inline -- Ego_d.pdb emits
 // CWStringParser::CWStringParser in the user's object (progress_debug_display.obj, 90 bytes /Od; the out-of-line COMDAT copy is retail 0x00437260)
 // and DisplayCenteredText expands it twice: CBaseClass() 0x0099A2D0, the CWStringParser vptr 0x01231718, then Init 0x00A0C8E0
 // (0x00497FFD..0x0049801F, 0x00498034..0x00498051). Body: Init(text) (the members are set by Init). Default path unchanged.
 CWStringParser(const unsigned short* text) { Init(text); }
#else
 CWStringParser(const unsigned short* text);                         // 0x00437260
#endif
 class CWideString ReadAsStringUntilCharacter(unsigned short c);     // 0x00A0CCD0
 long SkipWhitespace();                                              // 0x00A0C910
#ifdef FABLE_RETAIL_CWSTRINGPARSER_INIT
 // Retail opt-in (NF35 2026-11-16, row 0x0048F50C): Ego_r.pdb ?Init@CWStringParser@@QAEXPB_W@Z (0x00A0C8E0, catalog CWStringParser::Init).
 void Init(const unsigned short* text);                              // 0x00A0C8E0
#endif
#else
 virtual ~CWStringParser();
#endif
private:
 const wchar_t* String;
 unsigned long StringPos,Line;
 bool ReadNextNumber(CWParsedItem*);
};
FABLE_STATIC_ASSERT_AT(26, sizeof(CWStringParser)==16);
#endif

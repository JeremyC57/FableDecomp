// CWStringParser::CWStringParser(const unsigned short*) -- base CBaseClass(), this vtable, then Init(text)
// Leaf: ??0CWStringParser@@QAE@PBG@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00437260.
// One-argument constructor: default-constructs the base @ 0x0099A2D0, installs this class's vtable (0x01231718, rtti-vtables.tsv)
// and forwards the argument to Init @ 0x00A0C8E0 from the constructor body.
// NF39 (2026-11-16): moved off its local CWStringParser stand-in (a `wchar_t*` parameter and Set_00a0c8e0) onto the shared
//   engine/CWStringParser.h (opt-ins FABLE_RETAIL_CWSTRINGPARSER_READUNTIL, which names 0x00437260, and _INIT): Ego_r.pdb + FableWin.pdb
//   ??0CWStringParser@@QAE@PB_W@Z (const pointer; PBG on the lane, no /Zc:wchar_t) and ?Init@CWStringParser@@QAEXPB_W@Z 0x00A0C8E0.
#include <stddef.h>      // wchar_t under VC7.1 (not a keyword without /Zc:wchar_t)
#define FABLE_RETAIL_CWSTRINGPARSER_READUNTIL
#define FABLE_RETAIL_CWSTRINGPARSER_INIT
#include "engine/CWStringParser.h"

CWStringParser::CWStringParser(const unsigned short* text)
    : CBaseClass()
{
    Init(text);
}

#ifdef FABLE_RETAIL_CPERSISTCONTEXTREADSECTION
#ifndef FABLE_ENGINE_CPERSISTCONTEXTREADSECTION_H
#define FABLE_ENGINE_CPERSISTCONTEXTREADSECTION_H
// Retail opt-in (chunk 735, rows 0x0047F58B / 0x00597228 AddRegionAndTimeInfo): the section a CPersistContext::ReadSection
// 0x009BAE70 fills -- its own context (transfers go through it), the memory stream over the binary slice, the parser and the
// text of the "START_<name>" .. "END_<name>" block. Ego_r.pdb layout (class key V): Context +0x00 (0x2C), InputStream +0x2C,
// Parser +0x48, Text +0x70 with the retail 0x28-byte CStringParser (sizeof 0x74; Ego_r's 0x7C carries the 0x30 parser).
// Ego_r.pdb / FableWin.pdb ??0CPersistContextReadSection@@QAE@XZ (0x00485CD2) and ??1CPersistContextReadSection@@QAE@XZ
// (0x00485CF1) are out of line (both landed). Needs FABLE_RETAIL_CPERSISTCONTEXT, FABLE_RETAIL_CMEMORYDATAINPUTSTREAM,
// FABLE_RETAIL_CDATAINPUTSTREAM, FABLE_RETAIL_CSTRINGPARSER.
#include "fable_string.h"
#include "engine/CPersistContext.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CStringParser.h"
class CPersistContextReadSection {
public:
    CPersistContextReadSection();                                   // 0x00485CD2
#ifndef FABLE_RETAIL_CPERSISTCONTEXTREADSECTION_IMPLICIT_DTOR
    ~CPersistContextReadSection();                                  // 0x00485CF1
#endif
#ifdef FABLE_RETAIL_CPERSISTCONTEXTREADSECTION_IMPLICIT_DTOR
    // Per-TU inline opt-in (chunk 848, row 0x005253F0 CThingManager::LoadGameState; like FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY): retail
    // has one class, and the thing-manager TU expands the destructor in place (0x00525C2C..0x00525CB9: ~Text, the parser's comment list
    // freed node by node, its two strings, ~CBaseClass 0x0099A2E0, then ~CDataStream 0x00993B10 for the stream) where other TUs call
    // the COMDAT 0x00485CF1. Leaves the destructor implicit.
#endif
    CPersistContext        Context;                                 // +0x00
    CMemoryDataInputStream InputStream;                             // +0x2c
    CStringParser          Parser;                                  // +0x48
    CCharString            Text;                                    // +0x70
};
FABLE_STATIC_ASSERT(sizeof(CPersistContextReadSection) == 0x74);
#endif
#endif

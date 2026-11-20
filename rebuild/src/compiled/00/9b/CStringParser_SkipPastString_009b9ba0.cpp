// CStringParser::SkipPastString(const CCharString& text) -- find the next occurrence of `text` and move the cursor past it; false (cursor unchanged) when there is none
// Leaf: ?SkipPastString@CStringParser@@QAE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B9BA0, 81 bytes, thiscall, `ret 4`.
// Naming (NF129, 2026-11-20, q4656): Ego_r.pdb 0x00B0E710 + FableWin.pdb 0x03057FD0 ?SkipPastString@CStringParser@@QAE_NABVCCharString@@@Z;
//   Ego_r donor order SkipUntilWholeString 0x00B0E630 -> SkipPastString 0x00B0E710 -> SkipPastWholeString 0x00B0E750 = retail 0x009B9AA0 /
//   0x009B9BA0 / 0x009B9C00 (hand621 NAME_FIXES), and all 27 landed callers bind this spelling (exe scan + probe). The previous landed
//   spelling (CThingCreatureBase::SerialiseGameState over CThingBase::SerialiseGameState / SerialiseElements) was a propagated label over
//   these bytes, the same one 0x009B9C00 carried; the local models are replaced by engine/CStringParser.h (retail 0x28-byte layout).
// Callees: CStringParser::SkipUntilString(const CCharString&) 0x009B9A20 (landed), CStringParser::CalcInQuotes(bool, const char*,
//   const char*) const 0x009B9570 (protected, out of line here; FABLE_RETAIL_CSTRINGPARSER_CALCINQUOTES_DECL).
// Shape: the 0x009B9C00 SkipPastWholeString body with SkipUntilString in place of SkipUntilWholeString (byte-identical apart from the
//   first call): the string's length (null data: no move) is consumed in place, StringPos stored before the CalcInQuotes result.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_CALCINQUOTES_DECL
#include "engine/CStringParser.h"

bool CStringParser::SkipPastString(const CCharString& text)
{
    if (SkipUntilString(text)) {
        unsigned long n = text.GetLength();
        if (n > 0) {
            unsigned long pos = CursorPos.StringPos;
            const char* from = String + pos;
            bool inQuotes = CalcInQuotes(CursorPos.InQuotes, from, from + n);
            CursorPos.StringPos = pos + n;
            CursorPos.InQuotes = inQuotes;
        }
        return true;
    }
    return false;
}

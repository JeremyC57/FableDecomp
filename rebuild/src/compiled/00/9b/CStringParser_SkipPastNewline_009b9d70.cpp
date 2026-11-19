// CStringParser::SkipPastNewline() -- step the cursor past the rest of the line and its CR/LF run (keeping the in-quotes flag current) and return the number of characters skipped
// Leaf: ?SkipPastNewline@CStringParser@@QAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B9D70, 132 bytes, `ret`; Ego_r.pdb / FableWin.pdb ?SkipPastNewline@CStringParser@@QAEJXZ
//   (lib_string_parser.obj #28). The protected MoveStringPos(unsigned long) and CalcInQuotes(bool, const char*, const char*) const are
//   expanded inline.
// Shape: FableWin 0x03058200 (/Od): pstring_start = GetStringPos() + String; pstring_end the same; a VC6 for-scope loop (`xor eax,eax ;
//   je`) over *pstring_end that on CR / LF skips the whole CR/LF run and breaks; no_chars_skipped = pstring_end - pstring_start;
//   MoveStringPos(no_chars_skipped). Ego_d locals pstring_start, pstring_end (const char*), no_chars_skipped (long).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_MOVESTRINGPOS
#define FABLE_RETAIL_CSTRINGPARSER_MOVESTRINGPOS_EGOD
#include "engine/CStringParser.h"

long CStringParser::SkipPastNewline()
{
    const char* pstring_start = GetStringPos() + String;
    const char* pstring_end = GetStringPos() + String;
    if (false) {} else for (; *pstring_end; ++pstring_end) {
        if (*pstring_end == '\n' || *pstring_end == '\r') {
            while (*pstring_end == '\n' || *pstring_end == '\r')
                ++pstring_end;
            break;
        }
    }
    long no_chars_skipped = pstring_end - pstring_start;
    MoveStringPos(no_chars_skipped);
    return no_chars_skipped;
}
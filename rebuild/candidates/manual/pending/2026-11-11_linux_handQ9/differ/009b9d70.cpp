// CStringParser::SkipPastNewline() -- step the cursor past the rest of the line and its CR/LF run (keeping the in-quotes flag current) and return the number of characters skipped
// Grade: DIFFER(ours v retail) -- DIFFER(127v132): retail recomputes String+StringPos inside the expanded MoveStringPos (0x009B9D9A `mov eax,[ecx+4]` / 0x009B9D9F `mov edi,[ecx+20h]`) and pushes ebx/edi only after the n==0 test (0x009B9D9D); ours CSEs the start pointer from the scan loop (edi/ebp live across it, three pushes at entry). Probes TOKEN_MOVE opt-in, first-char local, (long) StringPos cast, `start = String; start += StringPos` all unchanged.
// Earlier grade (prior draft): DIFFER(ours v retail) -- DIFFER(127v132) (try c): entry register choice: ours keeps String in ebx (push ebx at entry) and folds the first character load (mov dl,[esi+ebx]); retail computes start in eax/edx (lea esi,[eax+edx] ; mov eax,esi ; mov dl,[eax]) and pushes ebx only inside the inlined MoveStringPos. Tried 14 spellings (start/p order, &String[pos], GetStringPos, two-loop and for forms, unsigned count).
// Leaf: ?SkipPastNewline@CStringParser@@QAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B9D70, 132 bytes; Ego_r.pdb ?SkipPastNewline@CStringParser@@QAEJXZ. No callees: the protected
//   MoveStringPos(unsigned long) and CalcInQuotes(bool, const char*, const char*) const (Ego_r.pdb) are expanded inline.
// Layout: engine/CStringParser.h retail branch (String +4, CursorPos {InQuotes +0x1C, StringPos +0x20, Line +0x24}).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_MOVESTRINGPOS
#include "engine/CStringParser.h"

long CStringParser::SkipPastNewline()
{
    const char* start = String + CursorPos.StringPos;
    const char* p = start;
    while (*p) {
        if (*p == '\n' || *p == '\r') {
            while (*p == '\n' || *p == '\r')
                ++p;
            break;
        }
        ++p;
    }
    long n = p - start;
    MoveStringPos(n);
    return n;
}

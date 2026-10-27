// GFRemoveComments(char* text, const char* start, const char* end) -- blank every start..end comment out of a text buffer in place, keeping one newline per newline the comment held so line numbers survive
// Grade: DIFFER(262v245) -- VC7.1 inverts the strstr loop (second strstr call duplicated at the bottom, start kept in edi) where retail jumps back to one loop head with p in ebx and start reloaded from its slot; for(;;)/while/goto forms all invert
// Leaf: ?GFRemoveComments@@YIXPADPBD1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A60410, 245 bytes, __fastcall (text ecx, start edx, end on the stack), `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb `?GFRemoveComments@@YIXPADPBD1@Z` (void __fastcall (char*, const char*,
//   const char*)); the landed CDefinitionManager callers 0x009AEF60 / 0x009AF480 bind it at this address. Its sibling
//   0x00A60200 (the longer body that also counts carriage returns) is GFRemoveCommentsLeavingNewlines.
// Callees: CRT strstr @ 0x00BFEB3A, memmove @ 0x00BFEAE6 (strlen is the inline intrinsic).
// Shape: the buffer end (text + strlen + 1, the terminator included) is kept in ebp and slides back by the bytes each
//   removal saves; an empty end marker blanks the rest of the text and terminates it. The newline fill is a byte loop
//   that VC7.1 turns into its guarded `rep stosd ; rep stosb` with 0x0A0A0A0A.
#include <string.h>

void __fastcall GFRemoveComments(char* text, const char* start, const char* end)
{
    char* textEnd = text + strlen(text) + 1;
    char* p = text;
    for (;;) {
        char* s = strstr(p, start);
        if (s == 0)
            return;
        if (*end == 0) {
            long n = 0;
            char* q;
            for (q = s; q < textEnd; ++q)
                if (*q == '\n')
                    ++n;
            char* d = s + n;
            for (q = s; q < d; ++q)
                *q = '\n';
            *d = 0;
            return;
        }
        char* e = strstr(s, end);
        if (e == 0)
            return;
        e += strlen(end);
        long n = 0;
        char* q;
        for (q = s; q < e + 1; ++q)
            if (*q == '\n')
                ++n;
        char* d = s + n;
        for (q = s; q < d; ++q)
            *q = '\n';
        memmove(d, e, textEnd - e);
        textEnd += d - e;
        p = d;
    }
}

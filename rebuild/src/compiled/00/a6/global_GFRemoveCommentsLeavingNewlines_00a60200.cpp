// GFRemoveCommentsLeavingNewlines(char* text, const char* commentStart, const char* commentEnd) -- cut every commentStart..commentEnd block out of text in place, keeping the block's '\n' and '\r' characters so line numbers survive
// Leaf: ?GFRemoveCommentsLeavingNewlines@@YIXPADPBD1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A60200, 386 bytes, `__fastcall` (text ecx, commentStart edx), `ret 4`.
// Identity: both PDBs name `?GFRemoveCommentsLeavingNewlines@@YIXPADPBD1@Z` (void, fastcall, char*, const char*,
//   const char*), the exact convention; the catalog label agrees. No direct callers (reached through a pointer or
//   inlined elsewhere).
// Callees: strncmp 0x00BFEAF8 and memmove 0x00BFEAE6 (MSVCR71 imports); strlen is the inline intrinsic.
// Shape: three intrinsic strlens; for each position that starts a comment, scan for the end marker; when found, count
//   the block's '\n' and '\r', overwrite the block's head with that many '\n' then '\r' (VC7.1 turns both fill loops into
//   guarded `rep stosd ; rep stosb`), and memmove the tail (with its terminator) down behind them. The text length is
//   not updated after a cut (retail re-reads the original length).
#include <string.h>

void __fastcall GFRemoveCommentsLeavingNewlines(char* text, const char* commentStart, const char* commentEnd)
{
    unsigned long textLength = strlen(text);
    unsigned long startLength = strlen(commentStart);
    unsigned long endLength = strlen(commentEnd);
    for (unsigned long i = 0; i < textLength; ++i) {
        if (strncmp(commentStart, text + i, startLength) == 0) {
            for (unsigned long j = i + startLength; j < textLength; ++j) {
                if (strncmp(text + j, commentEnd, endLength) == 0) {
                    unsigned long end = j + endLength;
                    unsigned long newlines = 0;
                    unsigned long returns = 0;
                    unsigned long k;
                    for (k = i; k < end; ++k) {
                        if (text[k] == '\n')
                            ++newlines;
                        if (text[k] == '\r')
                            ++returns;
                    }
                    for (k = i; k < i + newlines; ++k)
                        text[k] = '\n';
                    for (k = i + newlines; k < i + newlines + returns; ++k)
                        text[k] = '\r';
                    memmove(text + i + newlines + returns, text + end, textLength - returns - newlines - end + 1);
                    break;
                }
            }
        }
    }
}
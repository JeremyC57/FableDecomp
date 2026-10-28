// libogg::oggpack_readinit(oggpack_buffer* b, unsigned char* buf, int bytes) -- clear the 20-byte bit reader and point it at buf with bytes of storage
// Leaf: @oggpack_readinit@12
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C91360, 36 bytes, b in ecx, buf in edx, bytes on the stack (`ret 4`).
//   libogg src/bitwise.c, compiled as C with /Gr (`__fastcall`), as the landed bitwise rows (oggpack_reset 0x00C912F0,
//   oggpack_look 0x00C913A0, oggpack_write 0x00C90FC0) and the existing binding `@oggpack_readinit@12`.
// Callers: the vorbis packet readers 0x00C8E5AF, 0x00C8EDD1, 0x00C8EF1B, 0x00C8EFD1 and the jump 0x00C91390.
//   Label correction: the manifest row "CMemoryDataInputStream::OpenMemorySource" is propagated.
// Shape: the shipped source: memset(b, 0, sizeof(*b)) expands inline to five dword stores through a copy of b in esi,
//   then `b->buffer = b->ptr = buf` stores ptr first, then storage.
#include <string.h>
typedef struct oggpack_buffer {
    long           endbyte;                        // +0x00
    int            endbit;                         // +0x04
    unsigned char* buffer;                         // +0x08
    unsigned char* ptr;                            // +0x0c
    long           storage;                        // +0x10
} oggpack_buffer;

extern "C" void __fastcall oggpack_readinit(oggpack_buffer* b, unsigned char* buf, int bytes)
{
    memset(b, 0, sizeof(*b));
    b->buffer = b->ptr = buf;
    b->storage = bytes;
}
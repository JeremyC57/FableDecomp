// libogg::oggpack_writeinit(oggpack_buffer* b) -- zero the buffer record, then allocate the first 256-byte block as both buffer and write pointer, empty it and set the storage
// Leaf: @oggpack_writeinit@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C90F10, 51 bytes, plain `ret`. libogg src/bitwise.c. Callee: malloc (MSVCR71
//   import [0x01440158]). A one-jump thunk at 0x00C90F50 forwards to it (bitwise.c's oggpackB_writeinit).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): the library past libpk is Xiph libogg +
//   libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240); bitwise.c is libogg's oggpack_* over the 20-byte
//   oggpack_buffer {endbyte, endbit, buffer, ptr, storage} with the shared mask[] table @ 0x0139DDB8, and the
//   neighbouring landed agent447 rows oggpackB_write 0x00C910C0, oggpack_look 0x00C913A0, oggpack_adv 0x00C91530,
//   oggpack_bytes 0x00C917B0 use the same model. Compiled as C with /Gr: `__fastcall`, b in ecx.
// Label: catalog "SStringBuffer_00c90f10::Init" / manifest "CVsyncCallbackTimerInfo" ctor are descriptive/propagated.
// Shape: the shipped source; the record clear is the memset intrinsic (its zero register is private to the expansion,
//   so the first buffer byte is cleared with an immediate `mov byte [eax],0`).
#include <string.h>
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int size);   // [0x01440158]

typedef struct oggpack_buffer {
    long           endbyte;                        // +0x00
    int            endbit;                         // +0x04
    unsigned char* buffer;                         // +0x08
    unsigned char* ptr;                            // +0x0c
    long           storage;                        // +0x10
} oggpack_buffer;

#define BUFFER_INCREMENT 256

extern "C" void __fastcall oggpack_writeinit(oggpack_buffer* b)
{
    memset(b, 0, sizeof(*b));
    b->ptr = b->buffer = (unsigned char*)malloc(BUFFER_INCREMENT);
    b->buffer[0] = '\0';
    b->storage = BUFFER_INCREMENT;
}

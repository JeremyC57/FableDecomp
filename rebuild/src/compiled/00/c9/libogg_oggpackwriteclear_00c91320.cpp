// libogg::oggpack_writeclear(oggpack_buffer* b) -- free the writer's buffer and zero the record
// Leaf: @oggpack_writeclear@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C91320, 34 bytes, plain `ret`. libogg src/bitwise.c. Callee: free (MSVCR71
//   import [0x01440154]). agent447's landed vorbis_bitrate_clear 0x00C9CF10 calls it on each queued packet buffer.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): the library past libpk is Xiph libogg +
//   libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240); bitwise.c is libogg's oggpack_* over the 20-byte
//   oggpack_buffer {endbyte, endbit, buffer, ptr, storage} with the shared mask[] table @ 0x0139DDB8, and the
//   neighbouring landed agent447 rows oggpackB_write 0x00C910C0, oggpack_look 0x00C913A0, oggpack_adv 0x00C91530,
//   oggpack_bytes 0x00C917B0 use the same model. Compiled as C with /Gr: `__fastcall`, b in ecx.
// Label: catalog "FreeBufferHeader" / manifest "_Dest_val<...CSmallVector<NAnimationEvents::CEvent,32>...>" are
//   descriptive/propagated.
#include <string.h>
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]

typedef struct oggpack_buffer {
    long           endbyte;                        // +0x00
    int            endbit;                         // +0x04
    unsigned char* buffer;                         // +0x08
    unsigned char* ptr;                            // +0x0c
    long           storage;                        // +0x10
} oggpack_buffer;

extern "C" void __fastcall oggpack_writeclear(oggpack_buffer* b)
{
    free(b->buffer);
    memset(b, 0, sizeof(*b));
}

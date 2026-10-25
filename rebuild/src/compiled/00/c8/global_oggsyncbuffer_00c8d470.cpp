// ogg_sync_buffer(ogg_sync_state* oy, long size) -- libogg framing: slide out the bytes already returned, grow the sync buffer (size + fill + 4096) when fewer than `size` bytes are free, and return the write pointer at the fill mark
// Leaf: ogg_sync_buffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8D470, length 121 bytes (128 with padding).
// Library: libpk's statically linked Xiph libogg (framing.c, libVorbis I 20030909 era). No PDB/RTTI: libogg API name.
// Convention: LTCG __fastcall (oy in ecx, size in edx). Callees: MSVCR71 memmove (IAT 0x01440144), realloc
// (IAT 0x01440054), malloc (IAT 0x01440158).
// Shape: the libogg body verbatim; the realloc arm keeps its own copy of the `data + fill` return.
extern "C" __declspec(dllimport) void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // IAT 0x01440144
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int size);                 // IAT 0x01440158
extern "C" __declspec(dllimport) void* __cdecl realloc(void* block, unsigned int size);   // IAT 0x01440054
struct ogg_sync_state {                 // 0x1c bytes
    unsigned char* data;                // +0x00
    int            storage;             // +0x04
    int            fill;                // +0x08
    int            returned;            // +0x0c
    int            unsynced;            // +0x10
    int            headerbytes;         // +0x14
    int            bodybytes;           // +0x18
};

extern "C" char* __fastcall ogg_sync_buffer(ogg_sync_state* oy, long size)
{
    /* first, clear out any space that has been previously returned */
    if (oy->returned) {
        oy->fill -= oy->returned;
        if (oy->fill > 0)
            memmove(oy->data, oy->data + oy->returned, oy->fill);
        oy->returned = 0;
    }

    if (size > oy->storage - oy->fill) {
        /* We need to extend the internal buffer */
        long newsize = size + oy->fill + 4096; /* an extra page to be nice */

        if (oy->data)
            oy->data = (unsigned char*)realloc(oy->data, newsize);
        else
            oy->data = (unsigned char*)malloc(newsize);
        oy->storage = newsize;
    }

    /* expose a segment at least as large as requested at the fill mark */
    return ((char*)oy->data + oy->fill);
}
// _global::z_crc32(uLong crc, const Bytef* buf, uInt len) -- zlib 1.2.1 crc32.c crc32: return 0 for a NULL buffer, else tail-call the little-endian BYFOUR worker crc32_little
// Grade: DIFFER(18v24) -- one tail call: retail jumps to the LTCG crc32_little with crc in eax, buf in ecx, len in edx; a cdecl worker is reached by a stack-reusing jmp; lane gap: LTCG custom register convention (not reproducible from one VC7.1 TU)
// Leaf: _z_crc32
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C05FD0, 24 bytes. Reference body only: retail: __cdecl itself, but it tail-jumps to the LTCG static crc32_little 0x00C059D0 with crc in EAX, buf in ECX and len in EDX (the endian test folded away), which no C++ convention emits.
// The library sources are zlib 1.2.1 (ZLIB_VERSION at 0x0129CFE0, Z_PREFIX build) and LZO 1.08 (lzo1x_9x.c + lzo_swd.ch); layouts
// from the Ego_r PDB. Written as an ordinary __cdecl function so the TU emits it; the lane cannot reproduce the
// whole-program register convention, so this row is parked with this body as the reference.
// ---- zlib 1.2.1 model (zconf.h/zlib.h/deflate.h, Z_PREFIX build; layouts from the Ego_r PDB) ----
typedef unsigned char  Byte;
typedef unsigned int   uInt;
typedef unsigned long  uLong;
typedef Byte           Bytef;
typedef unsigned char  uch;
typedef unsigned short ush;
typedef unsigned long  ulg;
typedef ush            Pos;
typedef Pos            Posf;
typedef unsigned       IPos;
typedef void*          voidpf;
typedef voidpf (__cdecl *alloc_func)(voidpf opaque, uInt items, uInt size);
typedef void   (__cdecl *free_func)(voidpf opaque, voidpf address);

struct internal_state;
typedef struct z_stream_s {
    Bytef*    next_in;      // +0x00
    uInt      avail_in;     // +0x04
    uLong     total_in;     // +0x08
    Bytef*    next_out;     // +0x0c
    uInt      avail_out;    // +0x10
    uLong     total_out;    // +0x14
    char*     msg;          // +0x18
    struct internal_state* state; // +0x1c
    alloc_func zalloc;      // +0x20
    free_func  zfree;       // +0x24
    voidpf     opaque;      // +0x28
    int        data_type;   // +0x2c
    uLong      adler;       // +0x30
    uLong      reserved;    // +0x34
} z_stream;                 // 0x38
typedef z_stream* z_streamp;

typedef unsigned int u4;
extern "C" unsigned long __cdecl crc32_little(unsigned long crc, const unsigned char* buf, unsigned len);   // 0x00C059D0 (retail: eax/ecx/edx)
extern "C" unsigned long __cdecl crc32_big(unsigned long crc, const unsigned char* buf, unsigned len);
extern "C" unsigned long __cdecl z_crc32(unsigned long crc, const unsigned char* buf, unsigned len)
{
    if (buf == 0) return 0UL;
    if (sizeof(void *) == sizeof(int)) {
        u4 endian;

        endian = 1;
        if (*((unsigned char *)(&endian)))
            return crc32_little(crc, buf, len);
        else
            return crc32_big(crc, buf, len);
    }
    return crc;
}

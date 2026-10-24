// _global::z_inflateInit2_(z_streamp strm, int windowBits, const char* version, int stream_size) -- zlib 1.2.1 inflateInit2_: check the library version and z_stream size, install the default allocator, allocate the inflate state, decode windowBits (raw / zlib / gzip wrap) and reset
// Leaf: _z_inflateInit2_
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C02240, 199 bytes. zlib 1.2.1, Z_PREFIX build, C linkage, __cdecl.
// Callees: zcalloc 0x00C06030 and zcfree 0x00C06050 (stored as the default allocator), strm->zalloc / strm->zfree
// through the stream, z_inflateReset 0x00C021F0.
// Shape: `version[0] != ZLIB_VERSION[0]` is the byte compare with '1'; sizeof(z_stream) = 0x38; the state is
// ZALLOC(strm, 1, sizeof(struct inflate_state)) = 0x1BA8 bytes; GUNZIP is defined, so a positive windowBits keeps
// `wrap = (windowBits >> 4) + 1` and masks with 15 below 48. edi holds 0 for every null test and store.
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
typedef struct {
    unsigned char op;          // operation, extra bits, table bits
    unsigned char bits;        // bits in this part of the code
    unsigned short val;        // offset in table or code value
} code;                        // 4 bytes (inftrees.h)

typedef enum {
    HEAD = 0, FLAGS, TIME, OS, EXLEN, EXTRA, NAME, COMMENT, HCRC, DICTID, DICT,
    TYPE, TYPEDO, STORED, COPY, TABLE, LENLENS, CODELENS, LEN, LENEXT, DIST, DISTEXT,
    MATCH, LIT, CHECK, LENGTH, DONE, BAD, MEM, SYNC
} inflate_mode;

struct inflate_state {
    inflate_mode mode;         // +0x00
    int last;                  // +0x04
    int wrap;                  // +0x08
    int havedict;              // +0x0c
    int flags;                 // +0x10
    unsigned long check;       // +0x14
    unsigned long total;       // +0x18
    unsigned wbits;            // +0x1c
    unsigned wsize;            // +0x20
    unsigned whave;            // +0x24
    unsigned write;            // +0x28
    unsigned char* window;     // +0x2c
    unsigned long hold;        // +0x30
    unsigned bits;             // +0x34
    unsigned length;           // +0x38
    unsigned offset;           // +0x3c
    unsigned extra;            // +0x40
    code const* lencode;       // +0x44
    code const* distcode;      // +0x48
    unsigned lenbits;          // +0x4c
    unsigned distbits;         // +0x50
    unsigned ncode;            // +0x54
    unsigned nlen;             // +0x58
    unsigned ndist;            // +0x5c
    unsigned have;             // +0x60
    code* next;                // +0x64
    unsigned short lens[320];  // +0x68
    unsigned short work[288];  // +0x2e8
    code codes[1440];          // +0x528 (ENOUGH = 1440 in 1.2.1)
};                             // 0x1ba8

#define Z_OK            0
#define Z_STREAM_ERROR (-2)
#define Z_MEM_ERROR    (-4)
#define Z_VERSION_ERROR (-6)
#define ZLIB_VERSION "1.2.1"
#define ZALLOC(strm, items, size) (*((strm)->zalloc))((strm)->opaque, (items), (size))
#define ZFREE(strm, addr)  (*((strm)->zfree))((strm)->opaque, (voidpf)(addr))

extern "C" voidpf __cdecl zcalloc(voidpf opaque, unsigned items, unsigned size);   // 0x00C06030
extern "C" void   __cdecl zcfree(voidpf opaque, voidpf ptr);                      // 0x00C06050
extern "C" int    __cdecl z_inflateReset(z_streamp strm);                         // 0x00C021F0

extern "C" int __cdecl z_inflateInit2_(z_streamp strm, int windowBits, const char* version, int stream_size)
{
    struct inflate_state* state;

    if (version == 0 || version[0] != ZLIB_VERSION[0] ||
        stream_size != (int)(sizeof(z_stream)))
        return Z_VERSION_ERROR;
    if (strm == 0) return Z_STREAM_ERROR;
    strm->msg = 0;                 /* in case we return an error */
    if (strm->zalloc == (alloc_func)0) {
        strm->zalloc = zcalloc;
        strm->opaque = (voidpf)0;
    }
    if (strm->zfree == (free_func)0) strm->zfree = zcfree;
    state = (struct inflate_state*)
            ZALLOC(strm, 1, sizeof(struct inflate_state));
    if (state == 0) return Z_MEM_ERROR;
    strm->state = (struct internal_state*)state;
    if (windowBits < 0) {
        state->wrap = 0;
        windowBits = -windowBits;
    }
    else {
        state->wrap = (windowBits >> 4) + 1;
        if (windowBits < 48) windowBits &= 15;
    }
    if (windowBits < 8 || windowBits > 15) {
        ZFREE(strm, state);
        strm->state = 0;
        return Z_STREAM_ERROR;
    }
    state->wbits = (unsigned)windowBits;
    state->window = 0;
    return z_inflateReset(strm);
}
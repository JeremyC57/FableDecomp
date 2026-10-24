// _global::z_inflateEnd(z_streamp strm) -- zlib 1.2.1 inflateEnd: free the sliding window (if any) and the inflate state through strm->zfree and clear strm->state
// Leaf: _z_inflateEnd
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C038D0, 72 bytes. zlib 1.2.1, Z_PREFIX build, C linkage, __cdecl.
// Callees: strm->zfree (+0x24) through the stream with strm->opaque (+0x28).
// Shape: the guard tests strm, strm->state and strm->zfree; the zfree pointer tested there stays in ecx for the window
// free (`call ecx`), while the state free after it re-reads both state and zfree (the first call may change them).
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
#define ZFREE(strm, addr)  (*((strm)->zfree))((strm)->opaque, (voidpf)(addr))

extern "C" int __cdecl z_inflateEnd(z_streamp strm)
{
    struct inflate_state* state;
    if (strm == 0 || strm->state == 0 || strm->zfree == (free_func)0)
        return Z_STREAM_ERROR;
    state = (struct inflate_state*)strm->state;
    if (state->window != 0) ZFREE(strm, state->window);
    ZFREE(strm, strm->state);
    strm->state = 0;
    return Z_OK;
}
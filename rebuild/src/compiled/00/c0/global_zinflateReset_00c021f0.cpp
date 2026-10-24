// _global::z_inflateReset(z_streamp strm) -- zlib 1.2.1 inflateReset: zero the stream totals and message, reset the inflate state machine to HEAD and point the code tables at the state's codes[] area
// Leaf: _z_inflateReset
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C021F0, 73 bytes. zlib 1.2.1, Z_PREFIX build, C linkage, __cdecl. No callees.
// Shape: the 1.2.1 body (no `adler = 1`, `dmax`, `head` or `write = 0` -- those arrived in 1.2.2/1.2.3); one zero
// register (ecx) feeds every store, `state->total` is stored first (the chained assignment evaluates right to
// left), then the codes[] address (+0x528) is formed in ecx for the three-way chained pointer assignment.
// Layout: inflate_state from the Ego_r PDB (7080 = 0x1BA8 bytes).
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

extern "C" int __cdecl z_inflateReset(z_streamp strm)
{
    struct inflate_state* state;

    if (strm == 0 || strm->state == 0) return Z_STREAM_ERROR;
    state = (struct inflate_state*)strm->state;
    strm->total_in = strm->total_out = state->total = 0;
    strm->msg = 0;
    state->mode = HEAD;
    state->last = 0;
    state->havedict = 0;
    state->wsize = 0;
    state->whave = 0;
    state->hold = 0;
    state->bits = 0;
    state->lencode = state->distcode = state->next = state->codes;
    return Z_OK;
}
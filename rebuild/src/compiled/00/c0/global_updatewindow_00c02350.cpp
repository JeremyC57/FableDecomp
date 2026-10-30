// updatewindow(z_streamp strm, unsigned out) -- zlib 1.2.1 inflate.c updatewindow: allocate the sliding window on first use, then copy the last wsize (or fewer) output bytes into the circular window
// Leaf: _updatewindow
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C02350, 277 bytes. zlib 1.2.1 (Z_PREFIX build, the landed z_inflateReset 0x00C021F0 /
// z_inflateInit2_ 0x00C02240 neighbours). updatewindow is an inflate.c `local` (static) function; its two retail callers are
// z_inflate (call at 0x00C03756) and z_inflateSetDictionary (call at 0x00C03977), both passing `out` in eax and strm on the
// stack with `add esp,4` after the call: VC7.1's private register convention for a TU-local static with a same-TU caller
// (chunk 585/586 rule). The same-TU caller here is the real 1.2.1 z_inflateSetDictionary body (only the static leaf is graded).
// No calls of its own except the zalloc function pointer (ZALLOC(strm, 1U << wbits, 1)); zmemcpy is the memcpy intrinsic
// (rep movsd / rep movsb).
// Layout: z_stream and inflate_state as the landed zlib 1.2.1 rows (Ego_r PDB layouts; no `head` member in 1.2.1).
typedef unsigned char  Byte;
typedef unsigned int   uInt;
typedef unsigned long  uLong;
typedef Byte           Bytef;
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
    unsigned char op;
    unsigned char bits;
    unsigned short val;
} code;

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
    code codes[1440];          // +0x528
};                             // 0x1ba8

extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
#define zmemcpy memcpy
#define Z_NULL 0
#define Z_OK            0
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR   (-3)
#define Z_MEM_ERROR    (-4)
#define ZALLOC(strm, items, size) (*((strm)->zalloc))((strm)->opaque, (items), (size))

extern "C" uLong __cdecl z_adler32(uLong adler, const Bytef* buf, uInt len);   // 0x00C03CB0

extern "C" {
static __declspec(noinline) int updatewindow(z_streamp strm, unsigned out)
{
    struct inflate_state* state;
    unsigned copy, dist;

    state = (struct inflate_state*)strm->state;

    /* if it hasn't been done already, allocate space for the window */
    if (state->window == Z_NULL) {
        state->window = (unsigned char*)
                        ZALLOC(strm, 1U << state->wbits,
                               sizeof(unsigned char));
        if (state->window == Z_NULL) return 1;
    }

    /* if window not in use yet, initialize */
    if (state->wsize == 0) {
        state->wsize = 1U << state->wbits;
        state->write = 0;
        state->whave = 0;
    }

    /* copy state->wsize or less output bytes into the circular window */
    copy = out - strm->avail_out;
    if (copy >= state->wsize) {
        zmemcpy(state->window, strm->next_out - state->wsize, state->wsize);
        state->write = 0;
        state->whave = state->wsize;
    }
    else {
        dist = state->wsize - state->write;
        if (dist > copy) dist = copy;
        zmemcpy(state->window + state->write, strm->next_out - copy, dist);
        copy -= dist;
        if (copy) {
            zmemcpy(state->window, strm->next_out - copy, copy);
            state->write = copy;
            state->whave = state->wsize;
        }
        else {
            state->write += dist;
            if (state->write == state->wsize) state->write = 0;
            if (state->whave < state->wsize) state->whave += dist;
        }
    }
    return 0;
}
}

// Same-TU caller (zlib 1.2.1 inflateSetDictionary), present so updatewindow gets retail's private convention.
extern "C" int __cdecl z_inflateSetDictionary(z_streamp strm, const Bytef* dictionary, uInt dictLength)
{
    struct inflate_state* state;
    unsigned long id;

    if (strm == Z_NULL || strm->state == Z_NULL) return Z_STREAM_ERROR;
    state = (struct inflate_state*)strm->state;
    if (state->mode != DICT) return Z_STREAM_ERROR;

    id = z_adler32(0L, Z_NULL, 0);
    id = z_adler32(id, dictionary, dictLength);
    if (id != state->check) return Z_DATA_ERROR;

    if (updatewindow(strm, strm->avail_out)) {
        state->mode = MEM;
        return Z_MEM_ERROR;
    }
    if (dictLength > state->wsize) {
        zmemcpy(state->window, dictionary + dictLength - state->wsize,
                state->wsize);
        state->whave = state->wsize;
    }
    else {
        zmemcpy(state->window + state->wsize - dictLength, dictionary,
                dictLength);
        state->whave = dictLength;
    }
    state->havedict = 1;
    return Z_OK;
}
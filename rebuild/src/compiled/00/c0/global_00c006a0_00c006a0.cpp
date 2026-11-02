// z_deflate(z_streamp, int) -- advances a deflate stream and writes headers, blocks and trailers.
// Leaf: _z_deflate
// Vendor: zlib 1.2.1 deflate.c, Z_PREFIX and GZIP.
// Layout: zlib 1.2.1 deflate.h, corroborated by native member offsets.
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
struct ct_data_s { union { ush freq; ush code; } fc; union { ush dad; ush len; } dl; };
typedef struct ct_data_s ct_data;
struct static_tree_desc_s;
struct tree_desc_s { ct_data* dyn_tree; int max_code; struct static_tree_desc_s* stat_desc; };

typedef struct internal_state {
    z_streamp strm;            // +0x00
    int    status;             // +0x04
    Bytef* pending_buf;        // +0x08
    ulg    pending_buf_size;   // +0x0c
    Bytef* pending_out;        // +0x10
    int    pending;            // +0x14
    int    wrap;               // +0x18
    Byte   data_type;          // +0x1c
    Byte   method;             // +0x1d
    int    last_flush;         // +0x20
    uInt   w_size;             // +0x24
    uInt   w_bits;             // +0x28
    uInt   w_mask;             // +0x2c
    Bytef* window;             // +0x30
    ulg    window_size;        // +0x34
    Posf*  prev;               // +0x38
    Posf*  head;               // +0x3c
    uInt   ins_h;              // +0x40
    uInt   hash_size;          // +0x44
    uInt   hash_bits;          // +0x48
    uInt   hash_mask;          // +0x4c
    uInt   hash_shift;         // +0x50
    long   block_start;        // +0x54
    uInt   match_length;       // +0x58
    IPos   prev_match;         // +0x5c
    int    match_available;    // +0x60
    uInt   strstart;           // +0x64
    uInt   match_start;        // +0x68
    uInt   lookahead;          // +0x6c
    uInt   prev_length;        // +0x70
    uInt   max_chain_length;   // +0x74
    uInt   max_lazy_match;     // +0x78
    int    level;              // +0x7c
    int    strategy;           // +0x80
    uInt   good_match;         // +0x84
    int    nice_match;         // +0x88
    struct ct_data_s dyn_ltree[573];   // +0x8c
    struct ct_data_s dyn_dtree[61];    // +0x980
    struct ct_data_s bl_tree[39];      // +0xa74
    struct tree_desc_s l_desc;         // +0xb10
    struct tree_desc_s d_desc;         // +0xb1c
    struct tree_desc_s bl_desc;        // +0xb28
    ush    bl_count[16];               // +0xb34
    int    heap[573];                  // +0xb54
    int    heap_len;                   // +0x1448
    int    heap_max;                   // +0x144c
    uch    depth[573];                 // +0x1450
    uch*   l_buf;                      // +0x1690
    uInt   lit_bufsize;                // +0x1694
    uInt   last_lit;                   // +0x1698
    ush*   d_buf;                      // +0x169c
    ulg    opt_len;                    // +0x16a0
    ulg    static_len;                 // +0x16a4
    uInt   matches;                    // +0x16a8
    int    last_eob_len;               // +0x16ac
    ush    bi_buf;                     // +0x16b0
    int    bi_valid;                   // +0x16b4
} deflate_state;                       // 0x16b8

#include <string.h>
#define GZIP
#define Z_NULL 0
#define Z_NO_FLUSH 0
#define Z_PARTIAL_FLUSH 1
#define Z_FULL_FLUSH 3
#define Z_FINISH 4
#define Z_OK 0
#define Z_STREAM_END 1
#define Z_NEED_DICT 2
#define Z_STREAM_ERROR (-2)
#define Z_BUF_ERROR (-5)
#define INIT_STATE 42
#define BUSY_STATE 113
#define FINISH_STATE 666
#define Z_DEFLATED 8
#define Z_HUFFMAN_ONLY 2
#define PRESET_DICT 0x20
#define NIL 0
#define zmemcpy memcpy
#define zmemzero(dest,len) memset(dest,0,len)
#define put_byte(s,c) {s->pending_buf[s->pending++]=(c);}
#define CLEAR_HASH(s) s->head[s->hash_size-1]=NIL; zmemzero((Bytef*)s->head,(unsigned)(s->hash_size-1)*sizeof(*s->head));
#define ERR_RETURN(strm,err) {strm->msg=(char*)z_errmsg[Z_NEED_DICT-(err)];return (err);}
#define Assert(cond,msg)
extern "C" {
typedef enum { need_more, block_done, finish_started, finish_done } block_state;
typedef block_state (*compress_func)(deflate_state*,int);
typedef struct config_s { ush good_length,max_lazy,nice_length,max_chain; compress_func func; } config;
extern const config configuration_table[10];
extern const char* const z_errmsg[10];
uLong __cdecl z_adler32(uLong,const Bytef*,uInt);
uLong __cdecl z_crc32(uLong,const Bytef*,uInt);
void __cdecl _tr_align(deflate_state*);
void __cdecl _tr_stored_block(deflate_state*,char*,ulg,int);
#define adler32 z_adler32
#define crc32 z_crc32
static __declspec(noinline) void putShortMSB(deflate_state* s, uInt b)
{
    put_byte(s, (Byte)(b >> 8));
    put_byte(s, (Byte)(b & 0xff));
}

static __declspec(noinline) void flush_pending(z_streamp strm)
{
    unsigned len = strm->state->pending;

    if (len > strm->avail_out) len = strm->avail_out;
    if (len == 0) return;

    zmemcpy(strm->next_out, strm->state->pending_out, len);
    strm->next_out  += len;
    strm->state->pending_out  += len;
    strm->total_out += len;
    strm->avail_out  -= len;
    strm->state->pending -= len;
    if (strm->state->pending == 0) {
        strm->state->pending_out = strm->state->pending_buf;
    }
}

int __cdecl z_deflate(z_streamp strm, int flush)
{
    int old_flush; /* value of flush param for previous deflate call */
    deflate_state *s;

    if (strm == Z_NULL || strm->state == Z_NULL ||
        flush > Z_FINISH || flush < 0) {
        return Z_STREAM_ERROR;
    }
    s = strm->state;

    if (strm->next_out == Z_NULL ||
        (strm->next_in == Z_NULL && strm->avail_in != 0) ||
        (s->status == FINISH_STATE && flush != Z_FINISH)) {
        ERR_RETURN(strm, Z_STREAM_ERROR);
    }
    if (strm->avail_out == 0) ERR_RETURN(strm, Z_BUF_ERROR);

    s->strm = strm; /* just in case */
    old_flush = s->last_flush;
    s->last_flush = flush;

    /* Write the header */
    if (s->status == INIT_STATE) {
#ifdef GZIP
        if (s->wrap == 2) {
            put_byte(s, 31);
            put_byte(s, 139);
            put_byte(s, 8);
            put_byte(s, 0);
            put_byte(s, 0);
            put_byte(s, 0);
            put_byte(s, 0);
            put_byte(s, 0);
            put_byte(s, s->level == 9 ? 2 :
                        (s->strategy >= Z_HUFFMAN_ONLY || s->level < 2 ?
                         4 : 0));
            put_byte(s, 255);
            s->status = BUSY_STATE;
            strm->adler = crc32(0L, Z_NULL, 0);
        }
        else
#endif
        {
            uInt header = (Z_DEFLATED + ((s->w_bits-8)<<4)) << 8;
            uInt level_flags;

            if (s->strategy >= Z_HUFFMAN_ONLY || s->level < 2)
                level_flags = 0;
            else if (s->level < 6)
                level_flags = 1;
            else if (s->level == 6)
                level_flags = 2;
            else
                level_flags = 3;
            header |= (level_flags << 6);
            if (s->strstart != 0) header |= PRESET_DICT;
            header += 31 - (header % 31);

            s->status = BUSY_STATE;
            putShortMSB(s, header);

            /* Save the adler32 of the preset dictionary: */
            if (s->strstart != 0) {
                putShortMSB(s, (uInt)(strm->adler >> 16));
                putShortMSB(s, (uInt)(strm->adler & 0xffff));
            }
            strm->adler = adler32(0L, Z_NULL, 0);
        }
    }

    /* Flush as much pending output as possible */
    if (s->pending != 0) {
        flush_pending(strm);
        if (strm->avail_out == 0) {
            /* Since avail_out is 0, deflate will be called again with
             * more output space, but possibly with both pending and
             * avail_in equal to zero. There won't be anything to do,
             * but this is not an error situation so make sure we
             * return OK instead of BUF_ERROR at next call of deflate:
             */
            s->last_flush = -1;
            return Z_OK;
        }

    /* Make sure there is something to do and avoid duplicate consecutive
     * flushes. For repeated and useless calls with Z_FINISH, we keep
     * returning Z_STREAM_END instead of Z_BUF_ERROR.
     */
    } else if (strm->avail_in == 0 && flush <= old_flush &&
               flush != Z_FINISH) {
        ERR_RETURN(strm, Z_BUF_ERROR);
    }

    /* User must not provide more input after the first FINISH: */
    if (s->status == FINISH_STATE && strm->avail_in != 0) {
        ERR_RETURN(strm, Z_BUF_ERROR);
    }

    /* Start a new block or continue the current one.
     */
    if (strm->avail_in != 0 || s->lookahead != 0 ||
        (flush != Z_NO_FLUSH && s->status != FINISH_STATE)) {
        block_state bstate;

        bstate = (*(configuration_table[s->level].func))(s, flush);

        if (bstate == finish_started || bstate == finish_done) {
            s->status = FINISH_STATE;
        }
        if (bstate == need_more || bstate == finish_started) {
            if (strm->avail_out == 0) {
                s->last_flush = -1; /* avoid BUF_ERROR next call, see above */
            }
            return Z_OK;
            /* If flush != Z_NO_FLUSH && avail_out == 0, the next call
             * of deflate should use the same flush parameter to make sure
             * that the flush is complete. So we don't have to output an
             * empty block here, this will be done at next call. This also
             * ensures that for a very small output buffer, we emit at most
             * one empty block.
             */
        }
        if (bstate == block_done) {
            if (flush == Z_PARTIAL_FLUSH) {
                _tr_align(s);
            } else { /* FULL_FLUSH or SYNC_FLUSH */
                _tr_stored_block(s, (char*)0, 0L, 0);
                /* For a full flush, this empty block will be recognized
                 * as a special marker by inflate_sync().
                 */
                if (flush == Z_FULL_FLUSH) {
                    CLEAR_HASH(s);             /* forget history */
                }
            }
            flush_pending(strm);
            if (strm->avail_out == 0) {
              s->last_flush = -1; /* avoid BUF_ERROR at next call, see above */
              return Z_OK;
            }
        }
    }
    Assert(strm->avail_out > 0, "bug2");

    if (flush != Z_FINISH) return Z_OK;
    if (s->wrap <= 0) return Z_STREAM_END;

    /* Write the trailer */
#ifdef GZIP
    if (s->wrap == 2) {
        put_byte(s, (Byte)(strm->adler & 0xff));
        put_byte(s, (Byte)((strm->adler >> 8) & 0xff));
        put_byte(s, (Byte)((strm->adler >> 16) & 0xff));
        put_byte(s, (Byte)((strm->adler >> 24) & 0xff));
        put_byte(s, (Byte)(strm->total_in & 0xff));
        put_byte(s, (Byte)((strm->total_in >> 8) & 0xff));
        put_byte(s, (Byte)((strm->total_in >> 16) & 0xff));
        put_byte(s, (Byte)((strm->total_in >> 24) & 0xff));
    }
    else
#endif
    {
        putShortMSB(s, (uInt)(strm->adler >> 16));
        putShortMSB(s, (uInt)(strm->adler & 0xffff));
    }
    flush_pending(strm);
    /* If avail_out is zero, the application will call deflate again
     * to flush the rest.
     */
    if (s->wrap > 0) s->wrap = -s->wrap; /* write the trailer only once! */
    return s->pending != 0 ? Z_OK : Z_STREAM_END;
}


}
// Self-contained C++03 fixture. Block compression and checksums are explicit dependency contract mocks.
#ifndef _MSC_VER
#define __cdecl
#define __declspec(x)
#endif
// 00c006a0 z_deflate
// Flags:
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

#include <stdio.h>
#include <stdlib.h>
static void check_test(bool ok){if(!ok){fprintf(stderr,"deflate fixture failure\n");exit(1);}}
static int block_result,block_calls,align_calls,stored_calls,empty_output;
static block_state fixture_compress(deflate_state* s,int flush){
    check_test(flush>=0 && flush<=4);++block_calls;
    if(empty_output)s->strm->avail_out=0;
    return (block_state)block_result;
}
extern "C" {
const config configuration_table[10]={
{0,0,0,0,fixture_compress},{4,4,8,4,fixture_compress},{4,5,16,8,fixture_compress},
{4,6,32,32,fixture_compress},{4,4,16,16,fixture_compress},{8,16,32,32,fixture_compress},
{8,16,128,128,fixture_compress},{8,32,128,256,fixture_compress},
{32,128,258,1024,fixture_compress},{32,258,258,4096,fixture_compress}};
const char* const z_errmsg[10]={"need dictionary","stream end","","file error","stream error","data error","insufficient memory","buffer error","incompatible version",""};
uLong __cdecl z_adler32(uLong value,const Bytef* p,uInt n){check_test(value==0 && p==0 && n==0);return 1;}
uLong __cdecl z_crc32(uLong value,const Bytef* p,uInt n){check_test(value==0 && p==0 && n==0);return 0;}
void __cdecl _tr_align(deflate_state*){++align_calls;}
void __cdecl _tr_stored_block(deflate_state*,char* p,ulg n,int last){check_test(p==0 && n==0 && last==0);++stored_calls;}
}
struct Fixture {
    z_stream z;deflate_state s;unsigned char input[32],output[512],pending[512];Pos hash[16];
    Fixture(){
        memset(&z,0,sizeof(z));memset(&s,0,sizeof(s));memset(input,0,sizeof(input));
        memset(output,0xcd,sizeof(output));memset(pending,0xab,sizeof(pending));
        for(unsigned i=0;i<16;++i)hash[i]=(Pos)(i+1);
        z.state=&s;z.next_in=input;z.next_out=output;z.avail_out=sizeof(output);
        s.strm=&z;s.status=BUSY_STATE;s.last_flush=-1;s.w_bits=15;s.level=6;
        s.pending_buf=pending;s.pending_out=pending;s.pending_buf_size=sizeof(pending);
        s.hash_size=16;s.head=hash;block_result=need_more;block_calls=align_calls=stored_calls=empty_output=0;
    }
};
static void test_errors(){
    check_test(z_deflate(0,0)==Z_STREAM_ERROR);
    {Fixture f;f.z.state=0;check_test(z_deflate(&f.z,0)==Z_STREAM_ERROR);}
    {Fixture f;check_test(z_deflate(&f.z,-1)==Z_STREAM_ERROR);check_test(z_deflate(&f.z,5)==Z_STREAM_ERROR);}
    {Fixture f;f.z.next_out=0;check_test(z_deflate(&f.z,0)==Z_STREAM_ERROR && f.z.msg==z_errmsg[4]);}
    {Fixture f;f.z.next_in=0;f.z.avail_in=1;check_test(z_deflate(&f.z,0)==Z_STREAM_ERROR);}
    {Fixture f;f.s.status=FINISH_STATE;check_test(z_deflate(&f.z,0)==Z_STREAM_ERROR);}
    {Fixture f;f.z.avail_out=0;check_test(z_deflate(&f.z,0)==Z_BUF_ERROR && f.z.msg==z_errmsg[7]);}
    {Fixture f;f.s.last_flush=0;check_test(z_deflate(&f.z,0)==Z_BUF_ERROR);}
    {Fixture f;f.s.status=FINISH_STATE;f.z.avail_in=1;check_test(z_deflate(&f.z,4)==Z_BUF_ERROR);}
}
static void test_headers(){
    for(int level=1;level<=9;++level)for(int preset=0;preset<2;++preset){
        Fixture f;f.s.status=INIT_STATE;f.s.wrap=1;f.s.level=level;f.s.strstart=preset;f.z.adler=0x12345678UL;
        check_test(z_deflate(&f.z,0)==Z_OK);
        check_test(f.output[0]==0x78 && ((unsigned)f.output[0]*256+f.output[1])%31==0);
        unsigned hint=level<2?0:level<6?1:level==6?2:3;
        check_test((f.output[1]>>6)==hint && ((f.output[1]>>5)&1)==(unsigned)preset);
        if(preset)check_test(f.output[2]==0x12 && f.output[3]==0x34 && f.output[4]==0x56 && f.output[5]==0x78);
        check_test(f.z.total_out==(preset?6U:2U) && f.z.adler==1 && f.s.status==BUSY_STATE);
    }
    for(int level=1;level<=9;++level){
        Fixture f;f.s.status=INIT_STATE;f.s.wrap=2;f.s.level=level;
        check_test(z_deflate(&f.z,0)==Z_OK);
        const unsigned char header[]={31,139,8,0,0,0,0,0};check_test(memcmp(f.output,header,8)==0);
        check_test(f.output[8]==(level==9?2:level<2?4:0) && f.output[9]==255 && f.z.total_out==10 && f.z.adler==0);
    }
}
static void test_pending(){
    for(unsigned available=1;available<=12;++available){
        Fixture f;for(unsigned i=0;i<10;++i)f.pending[i]=(unsigned char)(100+i);
        f.s.pending=10;f.z.avail_out=available;
        check_test(z_deflate(&f.z,0)==Z_OK);
        unsigned n=available<10?available:10;for(unsigned j=0;j<n;++j)check_test(f.output[j]==100+j);
        check_test(f.s.pending==(int)(10-n) && f.z.total_out==n && f.z.avail_out==available-n);
        check_test(f.s.pending_out==(n==10?f.pending:f.pending+n));
        check_test(f.s.last_flush==(available<=10?-1:0));
    }
}
static void test_block_states(){
    for(int state=0;state<4;++state)for(int flush=0;flush<=4;++flush){
        Fixture f;f.z.avail_in=1;block_result=state;
        int r=z_deflate(&f.z,flush);check_test(block_calls==1);
        check_test(f.s.status==((state==finish_started || state==finish_done)?FINISH_STATE:BUSY_STATE));
        check_test(r==((flush==4 && state!=need_more && state!=finish_started)?Z_STREAM_END:Z_OK));
        check_test(align_calls==((state==block_done && flush==1)?1:0));
        check_test(stored_calls==((state==block_done && flush!=1)?1:0));
        if(state==block_done && flush==3)for(unsigned i=0;i<16;++i)check_test(f.hash[i]==0);
    }
    for(int state=0;state<=2;state+=2){Fixture f;block_result=state;empty_output=1;f.z.avail_in=1;check_test(z_deflate(&f.z,4)==Z_OK && f.s.last_flush==-1);}
}
static void test_finish(){
    for(int wrap=1;wrap<=2;++wrap){
        Fixture f;f.s.wrap=wrap;f.z.adler=0x12345678UL;f.z.total_in=0xabcdef01UL;block_result=finish_done;
        check_test(z_deflate(&f.z,4)==Z_STREAM_END && f.s.wrap==-wrap && f.s.status==FINISH_STATE);
        const unsigned char a[]={0x12,0x34,0x56,0x78};const unsigned char b[]={0x78,0x56,0x34,0x12,1,0xef,0xcd,0xab};
        check_test(memcmp(f.output,wrap==1?a:b,wrap==1?4:8)==0);uLong prior=f.z.total_out;
        check_test(z_deflate(&f.z,4)==Z_STREAM_END && f.z.total_out==prior);
    }
}
int main(){test_errors();test_headers();test_pending();test_block_states();test_finish();puts("00c006a0_TEST PASS");return 0;}
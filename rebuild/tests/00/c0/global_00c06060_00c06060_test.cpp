// Self-contained C++03 fixture. Original function body follows.
#ifndef _MSC_VER
#define __cdecl
#define __declspec(x)
#endif
// 00c06060 inflate_fast
// Flags:
// Leaf: _inflate_fast
// Vendor: zlib 1.2.1 inffast.c; C keyword local renamed here for C++ compatibility.
// Layout: original zlib inflate.h and zlib.h; native loads confirm member positions.
#define FAR
#define Tracevv(x)
#define OFF 1
#define PUP(a) *++(a)
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
    unsigned char op;           /* operation, extra bits, table bits */
    unsigned char bits;         /* bits in this part of the code */
    unsigned short val;         /* offset in table or code value */
} code;

/* op values as set by inflate_table():
    00000000 - literal
    0000tttt - table link, tttt != 0 is the number of table index bits
    0001eeee - length or distance, eeee is the number of extra bits
    01100000 - end of block
    01000000 - invalid code
 */

/* Maximum size of dynamic tree.  The maximum found in a long but non-
   exhaustive search was 1004 code structures (850 for length/literals
   and 154 for distances, the latter actually the result of an
   exhaustive search).  The true maximum is not known, but the value
   below is more than safe. */
#define ENOUGH 1440
#define MAXD 154

/* inflate.h -- internal inflate state definition
 * Copyright (C) 1995-2003 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the compression library and is
   subject to change. Applications should only use zlib.h.
 */

/* define NO_GZIP when compiling if you want to disable gzip header and
   trailer decoding by inflate().  NO_GZIP would be used to avoid linking in
   the crc code when it is not needed.  For shared libraries, gzip decoding
   should be left enabled. */
#ifndef NO_GZIP
#  define GUNZIP
#endif

/* Possible inflate modes between inflate() calls */
typedef enum {
    HEAD,       /* i: waiting for magic header */
#ifdef GUNZIP
    FLAGS,      /* i: waiting for method and flags (gzip) */
    TIME,       /* i: waiting for modification time (gzip) */
    OS,         /* i: waiting for extra flags and operating system (gzip) */
    EXLEN,      /* i: waiting for extra length (gzip) */
    EXTRA,      /* i: waiting for extra bytes (gzip) */
    NAME,       /* i: waiting for end of file name (gzip) */
    COMMENT,    /* i: waiting for end of comment (gzip) */
    HCRC,       /* i: waiting for header crc (gzip) */
#endif
    DICTID,     /* i: waiting for dictionary check value */
    DICT,       /* waiting for inflateSetDictionary() call */
        TYPE,       /* i: waiting for type bits, including last-flag bit */
        TYPEDO,     /* i: same, but skip check to exit inflate on new block */
        STORED,     /* i: waiting for stored size (length and complement) */
        COPY,       /* i/o: waiting for input or output to copy stored block */
        TABLE,      /* i: waiting for dynamic block table lengths */
        LENLENS,    /* i: waiting for code length code lengths */
        CODELENS,   /* i: waiting for length/lit and distance code lengths */
            LEN,        /* i: waiting for length/lit code */
            LENEXT,     /* i: waiting for length extra bits */
            DIST,       /* i: waiting for distance code */
            DISTEXT,    /* i: waiting for distance extra bits */
            MATCH,      /* o: waiting for output space to copy string */
            LIT,        /* o: waiting for output space to write literal */
    CHECK,      /* i: waiting for 32-bit check value */
#ifdef GUNZIP
    LENGTH,     /* i: waiting for 32-bit length (gzip) */
#endif
    DONE,       /* finished check, done -- remain here until reset */
    BAD,        /* got a data error -- remain here until reset */
    MEM,        /* got an inflate() memory error -- remain here until reset */
    SYNC        /* looking for synchronization bytes to restart inflate() */
} inflate_mode;

/*
    State transitions between above modes -

    (most modes can go to the BAD or MEM mode -- not shown for clarity)

    Process header:
        HEAD -> (gzip) or (zlib)
        (gzip) -> FLAGS -> TIME -> OS -> EXLEN -> EXTRA -> NAME
        NAME -> COMMENT -> HCRC -> TYPE
        (zlib) -> DICTID or TYPE
        DICTID -> DICT -> TYPE
    Read deflate blocks:
            TYPE -> STORED or TABLE or LEN or CHECK
            STORED -> COPY -> TYPE
            TABLE -> LENLENS -> CODELENS -> LEN
    Read deflate codes:
                LEN -> LENEXT or LIT or TYPE
                LENEXT -> DIST -> DISTEXT -> MATCH -> LEN
                LIT -> LEN
    Process trailer:
        CHECK -> LENGTH -> DONE
 */

/* state maintained between inflate() calls.  Approximately 7K bytes. */
struct inflate_state {
    inflate_mode mode;          /* current inflate mode */
    int last;                   /* true if processing last block */
    int wrap;                   /* bit 0 true for zlib, bit 1 true for gzip */
    int havedict;               /* true if dictionary provided */
    int flags;                  /* gzip header method and flags (0 if zlib) */
    unsigned long check;        /* protected copy of check value */
    unsigned long total;        /* protected copy of output count */
        /* sliding window */
    unsigned wbits;             /* log base 2 of requested window size */
    unsigned wsize;             /* window size or zero if not using window */
    unsigned whave;             /* valid bytes in the window */
    unsigned write;             /* window write index */
    unsigned char FAR *window;  /* allocated sliding window, if needed */
        /* bit accumulator */
    unsigned long hold;         /* input bit accumulator */
    unsigned bits;              /* number of bits in "in" */
        /* for string and stored block copying */
    unsigned length;            /* literal or length of data to copy */
    unsigned offset;            /* distance back to copy string from */
        /* for table and code decoding */
    unsigned extra;             /* extra bits needed */
        /* fixed and dynamic code tables */
    code const FAR *lencode;    /* starting table for length/literal codes */
    code const FAR *distcode;   /* starting table for distance codes */
    unsigned lenbits;           /* index bits for lencode */
    unsigned distbits;          /* index bits for distcode */
        /* dynamic table building */
    unsigned ncode;             /* number of code length code lengths */
    unsigned nlen;              /* number of length code lengths */
    unsigned ndist;             /* number of distance code lengths */
    unsigned have;              /* number of code lengths in lens[] */
    code FAR *next;             /* next available space in codes[] */
    unsigned short lens[320];   /* temporary storage for code lengths */
    unsigned short work[288];   /* work area for code table building */
    code codes[ENOUGH];         /* space for code tables */
};
extern "C" void inflate_fast(z_streamp strm, unsigned start)
{
    struct inflate_state FAR *state;
    unsigned char FAR *in;      /* local strm->next_in */
    unsigned char FAR *last;    /* while in < last, enough input available */
    unsigned char FAR *out;     /* local strm->next_out */
    unsigned char FAR *beg;     /* inflate()'s initial strm->next_out */
    unsigned char FAR *end;     /* while out < end, enough space available */
    unsigned wsize;             /* window size or zero if not using window */
    unsigned whave;             /* valid bytes in the window */
    unsigned write;             /* window write index */
    unsigned char FAR *window;  /* allocated sliding window, if wsize != 0 */
    unsigned long hold;         /* local strm->hold */
    unsigned bits;              /* local strm->bits */
    code const FAR *lcode;      /* local strm->lencode */
    code const FAR *dcode;      /* local strm->distcode */
    unsigned lmask;             /* mask for first level of length codes */
    unsigned dmask;             /* mask for first level of distance codes */
    code here;                  /* retrieved table entry */
    unsigned op;                /* code bits, operation, extra bits, or */
                                /*  window position, window bytes to copy */
    unsigned len;               /* match length, unused bytes */
    unsigned dist;              /* match distance */
    unsigned char FAR *from;    /* where to copy match from */

    /* copy state to local variables */
    state = (struct inflate_state FAR *)strm->state;
    in = strm->next_in - OFF;
    last = in + (strm->avail_in - 5);
    out = strm->next_out - OFF;
    beg = out - (start - strm->avail_out);
    end = out + (strm->avail_out - 257);
    wsize = state->wsize;
    whave = state->whave;
    write = state->write;
    window = state->window;
    hold = state->hold;
    bits = state->bits;
    lcode = state->lencode;
    dcode = state->distcode;
    lmask = (1U << state->lenbits) - 1;
    dmask = (1U << state->distbits) - 1;

    /* decode literals and length/distances until end-of-block or not enough
       input data or output space */
    do {
        if (bits < 15) {
            hold += (unsigned long)(PUP(in)) << bits;
            bits += 8;
            hold += (unsigned long)(PUP(in)) << bits;
            bits += 8;
        }
        here = lcode[hold & lmask];
      dolen:
        op = (unsigned)(here.bits);
        hold >>= op;
        bits -= op;
        op = (unsigned)(here.op);
        if (op == 0) {                          /* literal */
            Tracevv((stderr, here.val >= 0x20 && here.val < 0x7f ?
                    "inflate:         literal '%c'\n" :
                    "inflate:         literal 0x%02x\n", here.val));
            PUP(out) = (unsigned char)(here.val);
        }
        else if (op & 16) {                     /* length base */
            len = (unsigned)(here.val);
            op &= 15;                           /* number of extra bits */
            if (op) {
                if (bits < op) {
                    hold += (unsigned long)(PUP(in)) << bits;
                    bits += 8;
                }
                len += (unsigned)hold & ((1U << op) - 1);
                hold >>= op;
                bits -= op;
            }
            Tracevv((stderr, "inflate:         length %u\n", len));
            if (bits < 15) {
                hold += (unsigned long)(PUP(in)) << bits;
                bits += 8;
                hold += (unsigned long)(PUP(in)) << bits;
                bits += 8;
            }
            here = dcode[hold & dmask];
          dodist:
            op = (unsigned)(here.bits);
            hold >>= op;
            bits -= op;
            op = (unsigned)(here.op);
            if (op & 16) {                      /* distance base */
                dist = (unsigned)(here.val);
                op &= 15;                       /* number of extra bits */
                if (bits < op) {
                    hold += (unsigned long)(PUP(in)) << bits;
                    bits += 8;
                    if (bits < op) {
                        hold += (unsigned long)(PUP(in)) << bits;
                        bits += 8;
                    }
                }
                dist += (unsigned)hold & ((1U << op) - 1);
                hold >>= op;
                bits -= op;
                Tracevv((stderr, "inflate:         distance %u\n", dist));
                op = (unsigned)(out - beg);     /* max distance in output */
                if (dist > op) {                /* see if copy from window */
                    op = dist - op;             /* distance back in window */
                    if (op > whave) {
                        strm->msg = (char *)"invalid distance too far back";
                        state->mode = BAD;
                        break;
                    }
                    from = window - OFF;
                    if (write == 0) {           /* very common case */
                        from += wsize - op;
                        if (op < len) {         /* some from window */
                            len -= op;
                            do {
                                PUP(out) = PUP(from);
                            } while (--op);
                            from = out - dist;  /* rest from output */
                        }
                    }
                    else if (write < op) {      /* wrap around window */
                        from += wsize + write - op;
                        op -= write;
                        if (op < len) {         /* some from end of window */
                            len -= op;
                            do {
                                PUP(out) = PUP(from);
                            } while (--op);
                            from = window - OFF;
                            if (write < len) {  /* some from start of window */
                                op = write;
                                len -= op;
                                do {
                                    PUP(out) = PUP(from);
                                } while (--op);
                                from = out - dist;      /* rest from output */
                            }
                        }
                    }
                    else {                      /* contiguous in window */
                        from += write - op;
                        if (op < len) {         /* some from window */
                            len -= op;
                            do {
                                PUP(out) = PUP(from);
                            } while (--op);
                            from = out - dist;  /* rest from output */
                        }
                    }
                    while (len > 2) {
                        PUP(out) = PUP(from);
                        PUP(out) = PUP(from);
                        PUP(out) = PUP(from);
                        len -= 3;
                    }
                    if (len) {
                        PUP(out) = PUP(from);
                        if (len > 1)
                            PUP(out) = PUP(from);
                    }
                }
                else {
                    from = out - dist;          /* copy direct from output */
                    do {                        /* minimum length is three */
                        PUP(out) = PUP(from);
                        PUP(out) = PUP(from);
                        PUP(out) = PUP(from);
                        len -= 3;
                    } while (len > 2);
                    if (len) {
                        PUP(out) = PUP(from);
                        if (len > 1)
                            PUP(out) = PUP(from);
                    }
                }
            }
            else if ((op & 64) == 0) {          /* 2nd level distance code */
                here = dcode[here.val + (hold & ((1U << op) - 1))];
                goto dodist;
            }
            else {
                strm->msg = (char *)"invalid distance code";
                state->mode = BAD;
                break;
            }
        }
        else if ((op & 64) == 0) {              /* 2nd level length code */
            here = lcode[here.val + (hold & ((1U << op) - 1))];
            goto dolen;
        }
        else if (op & 32) {                     /* end-of-block */
            Tracevv((stderr, "inflate:         end of block\n"));
            state->mode = TYPE;
            break;
        }
        else {
            strm->msg = (char *)"invalid literal/length code";
            state->mode = BAD;
            break;
        }
    } while (in < last && out < end);

    /* return unused bytes (on entry, bits < 8, so in won't go too far back) */
    len = bits >> 3;
    in -= len;
    bits -= len << 3;
    hold &= (1U << bits) - 1;

    /* update state and return */
    strm->next_in = in + OFF;
    strm->next_out = out + OFF;
    strm->avail_in = (unsigned)(in < last ? 5 + (last - in) : 5 - (in - last));
    strm->avail_out = (unsigned)(out < end ?
                                 257 + (end - out) : 257 - (out - end));
    state->hold = hold;
    state->bits = bits;
    return;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void check_test(bool ok){if(!ok){fprintf(stderr,"inflate fixture failure\n");exit(1);}}
static void put_test_bits(unsigned char* data,unsigned& pos,unsigned value,unsigned count){
    for(unsigned j=0;j<count;++j,++pos)if(value&(1U<<j))data[pos/8]|=(unsigned char)(1U<<(pos%8));
}
static void test_decode(unsigned length,unsigned distance,unsigned write_index,unsigned prefix,unsigned kind){
    unsigned char input[32],output[4096],history[16],expected[4096];
    code literals[8],distances[4];inflate_state state;z_stream stream;
    memset(input,0,sizeof(input));memset(output,0xcd,sizeof(output));memset(expected,0xcd,sizeof(expected));
    memset(&state,0,sizeof(state));memset(&stream,0,sizeof(stream));memset(literals,0,sizeof(literals));memset(distances,0,sizeof(distances));
    for(unsigned i=0;i<16;++i)history[i]=(unsigned char)(90+i);
    for(unsigned j=0;j<prefix;++j)output[j]=expected[j]=(unsigned char)(65+j);
    literals[0].bits=2;literals[0].val=65;
    literals[1].bits=2;literals[1].val=66;
    literals[2].bits=2;literals[2].op=16;literals[2].val=(unsigned short)length;
    literals[3].bits=2;literals[3].op=96;
    distances[0].bits=1;distances[0].op=16;distances[0].val=(unsigned short)distance;
    distances[1]=distances[0];
    unsigned bitcount=0,produced=prefix;bool invalid=false;const char* message=0;
    if(kind==0){
        put_test_bits(input,bitcount,2,2);put_test_bits(input,bitcount,0,1);
        if(distance>prefix+16){invalid=true;message="invalid distance too far back";}
        else for(unsigned k=0;k<length;++k){int at=(int)produced-(int)distance;expected[produced++]=at<0?history[(write_index+16+at)%16]:expected[at];}
    }else if(kind==1){
        put_test_bits(input,bitcount,0,2);put_test_bits(input,bitcount,1,2);expected[produced++]=65;expected[produced++]=66;
    }else if(kind==2){
        literals[0].op=1;literals[0].val=4;literals[4].bits=1;literals[4].val=67;
        put_test_bits(input,bitcount,0,2);put_test_bits(input,bitcount,0,1);expected[produced++]=67;
    }else if(kind==3){
        literals[2].op=64;put_test_bits(input,bitcount,2,2);invalid=true;message="invalid literal/length code";
    }else if(kind==4){
        distances[0].op=64;put_test_bits(input,bitcount,2,2);put_test_bits(input,bitcount,0,1);invalid=true;message="invalid distance code";
    }else if(kind==5){
        literals[2].op=17;distances[0].op=17;
        put_test_bits(input,bitcount,2,2);put_test_bits(input,bitcount,1,1);
        put_test_bits(input,bitcount,0,1);put_test_bits(input,bitcount,1,1);
        for(unsigned z=0;z<length+1;++z){int at=(int)produced-(int)(distance+1);expected[produced++]=at<0?history[(write_index+16+at)%16]:expected[at];}
    }else if(kind==6){
        distances[0].op=1;distances[0].val=2;distances[2].bits=1;distances[2].op=16;distances[2].val=(unsigned short)distance;
        put_test_bits(input,bitcount,2,2);put_test_bits(input,bitcount,0,1);put_test_bits(input,bitcount,0,1);
        for(unsigned z=0;z<length;++z){int at=(int)produced-(int)distance;expected[produced++]=at<0?history[(write_index+16+at)%16]:expected[at];}
    }
    if(!invalid)put_test_bits(input,bitcount,3,2);
    state.mode=LEN;state.wsize=16;state.whave=16;state.write=write_index;state.window=history;
    state.lencode=literals;state.distcode=distances;state.lenbits=2;state.distbits=1;
    stream.state=(internal_state*)&state;stream.next_in=input;stream.avail_in=sizeof(input);
    stream.next_out=output+prefix;stream.avail_out=sizeof(output)-prefix;stream.total_in=123;stream.total_out=456;
    inflate_fast(&stream,sizeof(output));
    check_test(state.mode==(invalid?BAD:TYPE));
    check_test(!invalid?stream.msg==0:stream.msg!=0 && strcmp(stream.msg,message)==0);
    check_test(memcmp(output,expected,sizeof(output))==0);
    check_test(stream.next_out==output+produced && stream.avail_out==sizeof(output)-produced);
    check_test(stream.next_in==input+(bitcount+7)/8 && stream.avail_in==sizeof(input)-(bitcount+7)/8);
    check_test(state.bits==((8-bitcount%8)%8) && state.hold==0);
    check_test(stream.total_in==123 && stream.total_out==456);
}
static void test_exhaustion(bool input_limited){
    unsigned char input[32],output[300];code literal[4];inflate_state state;z_stream stream;
    memset(input,0,sizeof(input));memset(output,0xcd,sizeof(output));memset(&state,0,sizeof(state));memset(&stream,0,sizeof(stream));
    for(unsigned i=0;i<4;++i){literal[i].op=0;literal[i].bits=2;literal[i].val=65;}
    state.mode=LEN;state.lencode=literal;state.distcode=literal;state.lenbits=2;state.distbits=2;
    stream.state=(internal_state*)&state;stream.next_in=input;stream.avail_in=input_limited?6:32;
    stream.next_out=output;stream.avail_out=input_limited?300:258;
    inflate_fast(&stream,stream.avail_out);
    check_test(state.mode==LEN && stream.next_out==output+1 && output[0]==65 && output[1]==0xcd);
    check_test(stream.next_in==input+1 && state.bits==6 && state.hold==0);
}
int main(){
    const unsigned lengths[]={3,4,5,16,258};const unsigned writes[]={0,3,10};
    for(unsigned a=0;a<5;++a)for(unsigned b=1;b<=17;++b)for(unsigned c=0;c<3;++c)for(unsigned d=0;d<3;++d)test_decode(lengths[a],b,writes[c],d,0);
    for(unsigned kind=1;kind<=6;++kind)test_decode(3,2,10,3,kind);
    test_exhaustion(false);test_exhaustion(true);puts("00c06060_TEST PASS");return 0;
}
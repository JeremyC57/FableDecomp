#include <stdio.h>
#include <stdlib.h>
// 00c03b80 z_inflateCopy
// Leaf: _z_inflateCopy
// Flags:
// Original zlib1.2.1 compatible source; K&R prototype and final void-pointer assignment adapted to C++03. Algorithm unchanged.
#include <string.h>
#ifndef _MSC_VER
#define __cdecl
#endif
#define FAR
#define GUNZIP
#define Z_NULL 0
#define Z_OK 0
#define Z_STREAM_ERROR (-2)
#define Z_MEM_ERROR (-4)
#define ENOUGH 1440
typedef unsigned char Byte;
typedef Byte Bytef;
typedef unsigned int uInt;
typedef unsigned long uLong;
typedef void*voidpf;
typedef voidpf (__cdecl *alloc_func)(voidpf,uInt,uInt);
typedef void (__cdecl *free_func)(voidpf,voidpf);
struct internal_state;
typedef struct z_stream_s {
    Bytef    *next_in;  /* next input byte */
    uInt     avail_in;  /* number of bytes available at next_in */
    uLong    total_in;  /* total nb of input bytes read so far */

    Bytef    *next_out; /* next output byte should be put there */
    uInt     avail_out; /* remaining free space at next_out */
    uLong    total_out; /* total nb of bytes output so far */

    char     *msg;      /* last error message, NULL if no error */
    struct internal_state FAR *state; /* not visible by applications */

    alloc_func zalloc;  /* used to allocate the internal state */
    free_func  zfree;   /* used to free the internal state */
    voidpf     opaque;  /* private data object passed to zalloc and zfree */

    int     data_type;  /* best guess about the data type: ascii or binary */
    uLong   adler;      /* adler32 value of the uncompressed data */
    uLong   reserved;   /* reserved for future use */
} z_stream;
typedef z_stream FAR*z_streamp;
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
typedef struct {
    unsigned char op;           /* operation, extra bits, table bits */
    unsigned char bits;         /* bits in this part of the code */
    unsigned short val;         /* offset in table or code value */
} code;
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
#define ZALLOC(strm,items,size) (*((strm)->zalloc))((strm)->opaque,(items),(size))
#define ZFREE(strm,addr) (*((strm)->zfree))((strm)->opaque,(voidpf)(addr))
#define zmemcpy memcpy
extern "C" int __cdecl z_inflateCopy(z_streamp dest,z_streamp source)
{
    struct inflate_state FAR *state;
    struct inflate_state FAR *copy;
    unsigned char FAR *window;

    /* check input */
    if (dest == Z_NULL || source == Z_NULL || source->state == Z_NULL ||
        source->zalloc == (alloc_func)0 || source->zfree == (free_func)0)
        return Z_STREAM_ERROR;
    state = (struct inflate_state FAR *)source->state;

    /* allocate space */
    copy = (struct inflate_state FAR *)
           ZALLOC(source, 1, sizeof(struct inflate_state));
    if (copy == Z_NULL) return Z_MEM_ERROR;
    window = Z_NULL;
    if (state->window != Z_NULL) {
        window = (unsigned char FAR *)
                 ZALLOC(source, 1U << state->wbits, sizeof(unsigned char));
        if (window == Z_NULL) {
            ZFREE(source, copy);
            return Z_MEM_ERROR;
        }
    }

    /* copy state */
    *dest = *source;
    *copy = *state;
    copy->lencode = copy->codes + (state->lencode - state->codes);
    copy->distcode = copy->codes + (state->distcode - state->codes);
    copy->next = copy->codes + (state->next - state->codes);
    if (window != Z_NULL)
        zmemcpy(window, state->window, 1U << state->wbits);
    copy->window = window;
    dest->state = (struct internal_state *)copy;
    return Z_OK;
}
typedef char z_stream_size_check[sizeof(z_stream)==56?1:-1];
typedef char inflate_state_size_check[sizeof(inflate_state)==7080?1:-1];

static void check(bool p){if(!p)abort();}
struct Fixture{int attempts,failAt,frees,live;void*blocks[4];unsigned bytes[4];};
static voidpf __cdecl allocate(voidpf opaque,uInt items,uInt size){Fixture*f=(Fixture*)opaque;++f->attempts;if(f->attempts==f->failAt)return 0;void*p=malloc(items*size);check(p!=0&&f->live<4);memset(p,0xcc,items*size);f->blocks[f->live]=p;f->bytes[f->live]=items*size;++f->live;return p;}
static void __cdecl release(voidpf opaque,voidpf p){Fixture*f=(Fixture*)opaque;int index=0;while(index<f->live&&f->blocks[index]!=p)++index;check(index<f->live);--f->live;f->blocks[index]=f->blocks[f->live];f->bytes[index]=f->bytes[f->live];++f->frees;free(p);}
static unsigned char windowBytes[32768];
static void setup(z_stream&s,inflate_state&state,Fixture&f,bool hasWindow,unsigned bits){memset(&s,0,sizeof s);memset(&f,0,sizeof f);for(unsigned i=0;i<sizeof state;++i)((unsigned char*)&state)[i]=static_cast<unsigned char>((i*31+7)&255);state.mode=TYPE;state.wbits=bits;state.wsize=1U<<bits;state.whave=state.wsize/2;state.write=3;state.window=hasWindow?windowBytes:0;state.lencode=state.codes+17;state.distcode=state.codes+500;state.next=state.codes+700;s.next_in=windowBytes+1;s.avail_in=19;s.total_in=47;s.next_out=windowBytes+43;s.avail_out=31;s.total_out=91;s.msg=0;s.state=(internal_state*)&state;s.zalloc=allocate;s.zfree=release;s.opaque=&f;s.data_type=2;s.adler=0x12345678;s.reserved=0x10203040;}
int main(){check(sizeof(z_stream)==56&&sizeof(inflate_state)==7080&&sizeof(code)==4);for(unsigned i=0;i<sizeof windowBytes;++i)windowBytes[i]=static_cast<unsigned char>((i*13+9)&255);z_stream source,dest;inflate_state state;Fixture f;setup(source,state,f,false,15);memset(&dest,0x5a,sizeof dest);z_stream beforeDest=dest;check(z_inflateCopy(0,&source)==Z_STREAM_ERROR);check(z_inflateCopy(&dest,0)==Z_STREAM_ERROR);internal_state*saved=source.state;source.state=0;check(z_inflateCopy(&dest,&source)==Z_STREAM_ERROR);source.state=saved;source.zalloc=0;check(z_inflateCopy(&dest,&source)==Z_STREAM_ERROR);source.zalloc=allocate;source.zfree=0;check(z_inflateCopy(&dest,&source)==Z_STREAM_ERROR);source.zfree=release;check(memcmp(&dest,&beforeDest,sizeof dest)==0&&f.attempts==0);int cases=0;const unsigned bitCounts[5]={0,1,4,9,15};for(int windowFlag=0;windowFlag<2;++windowFlag)for(unsigned bitIndex=0;bitIndex<5;++bitIndex)for(int fail=0;fail<=2;++fail){setup(source,state,f,windowFlag!=0,bitCounts[bitIndex]);f.failAt=fail;memset(&dest,0x5a,sizeof dest);beforeDest=dest;z_stream beforeSource=source;inflate_state beforeState=state;int codeResult=z_inflateCopy(&dest,&source);if(fail==1||(windowFlag&&fail==2)){check(codeResult==Z_MEM_ERROR&&f.live==0&&memcmp(&dest,&beforeDest,sizeof dest)==0);check(f.frees==(fail==2?1:0));}else{check(codeResult==Z_OK);inflate_state*copy=(inflate_state*)dest.state;check(copy!=&state&&f.live==(windowFlag?2:1));z_stream expectedStream=source;expectedStream.state=(internal_state*)copy;check(memcmp(&dest,&expectedStream,sizeof dest)==0);inflate_state expectedState=state;expectedState.lencode=copy->codes+17;expectedState.distcode=copy->codes+500;expectedState.next=copy->codes+700;expectedState.window=copy->window;check(memcmp(copy,&expectedState,sizeof state)==0);if(windowFlag){check(copy->window&&copy->window!=windowBytes&&memcmp(copy->window,windowBytes,1U<<state.wbits)==0);unsigned char first=windowBytes[0];copy->window[0]^=1;check(windowBytes[0]==first);release(&f,copy->window);}else check(copy->window==0);release(&f,copy);check(f.live==0);}check(memcmp(&source,&beforeSource,sizeof source)==0&&memcmp(&state,&beforeState,sizeof state)==0);++cases;}check(cases==30);puts("00c03b80_TEST PASS");return 0;}
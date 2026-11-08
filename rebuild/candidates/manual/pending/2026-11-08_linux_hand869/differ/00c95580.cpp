// libvorbis::vorbis_book_init_decode(codebook* c, const static_codebook* s) -- sharedbook.c: build a decode codebook from a static one: count the used (length > 0) entries, make the codewords, bit-reverse and sort them (qsort over pointers, sort32a) so the book can be decoded without a tree, build the sorted codelist, unquantised value list, dec_index (sorted position -> original entry) and dec_codelengths, then the first-level decode table (dec_firsttablen = clamp(ilog(used)-4, 5, 8) bits: direct hits as entry+1, misses as 0x80000000 | lo<<15 | hi search hints); -1 after vorbis_book_clear when the words cannot be built
// Grade: DIFFER(901v910) after 7 grades (p1 917, p2 990, p3/p4/p5 901 unchanged). Residue (allocation only, same instruction stream): (1) the codes loop's two spilled temps (the codes pointer and the n down-counter) take frame slots [ebp-0Ch]/[ebp-8] in the opposite order; (2) in the hint-table `while(hi<n && word>=(c->codelist[hi]&mask))hi++` loop retail keeps `hi` memory-resident at [ebp-0Ch] (reloaded into ecx each pass, `mov ecx,[eax] ; and ecx,[ebp-1Ch]` codelist first) while VC7.1 /O2 holds hi in edx and ANDs mask-first; the two 0x7fff clamps then read loval/hival from the swapped registers. Swapping the `&` operands, the lo/hi declaration order or the sortindex declaration moved nothing. The 9-byte gap is the hi reload/store pair.
// Next: c2prio.py over the hint loop to see what takes edx in retail's colouring (the codelist base load `mov edx,[esi+14h]`); try the libvorbis 1.1.0 spelling of the hint loop and a same-TU `_book_unquantize` body (its register use may free/bind edx differently).
// Leaf: @vorbis_book_init_decode@8
// Flags: /TC /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C95580, 910 bytes, LTCG `extern "C" __fastcall` (c in ecx, s in edx, plain `ret`), ebp frame for the
//   two allocas (codep, sortindex: `__alloca_probe` 0x00BFEA30). Callees: _make_words 0x00C94FD0 (`__fastcall`, third argument on the
//   stack), bitreverse 0x00C954F0 (sharedbook.c static, x in eax -- the landed libvorbis_bitreverse_00c954f0.cpp), qsort [0x01440060]
//   with sort32a 0x00C95560 (landed, `__cdecl`), malloc [0x01440158] (_ogg_malloc), free [0x01440154], _book_unquantize 0x00C951B0
//   (`__fastcall`), _ilog 0x00C94EE0 (`@_ilog@4`), calloc [0x01440058], vorbis_book_clear 0x00C95440 (`@vorbis_book_clear@4`).
//   Callers: _vds_shared_init 0x00C8F200 (landed, `vorbis_book_init_decode(ci->fullbooks+i, ci->book_param[i])` on the decode side).
// Identity (two sources): (1) the body is Xiph libvorbis 1.0.1 lib/sharedbook.c vorbis_book_init_decode verbatim -- the `rep stosd` of
//   0x2C/4 dwords, the used-entry count, codes/codep/sortindex construction, the two dec_ tables, the ilog-4 clamp to [5,8], the
//   direct-hit fill `orig | (j << len)` and the 0xfffffffe << (31-n) mask hint loop with the 0x7fff saturation; (2) the landed
//   _vds_shared_init binds `@vorbis_book_init_decode@8` here, and sharedbook.c file order places it between sort32a 0x00C95560 and
//   _dist 0x00C95910 / _best 0x00C95A10 (all landed). The catalog label "CTCWound::CWound::Initialize" is a propagated error (libpk's
//   Xiph block has no game classes).
// Shape: a C TU (/TC /Gr, as sharedbook.c was built) with the statics beside their real caller (bitreverse keeps retail's x-in-eax
//   convention that way); sort32a is the `__cdecl` qsort comparator. Constants: the 0x80000000 / 0x7fff hint packing are immediates.
// Original libvorbis sharedbook.c (libvorbis 1.0.1, unmodified function bodies); BSD-style terms in reference COPYING.
typedef unsigned int ogg_uint32_t;
typedef struct static_codebook {                   // libvorbis 1.0.1 codebook.h, 0x34 bytes
    long  dim;                                     // +0x00
    long  entries;                                 // +0x04
    long* lengthlist;                              // +0x08
    int   maptype;                                 // +0x0c
    long  q_min;                                   // +0x10
    long  q_delta;                                 // +0x14
    int   q_quant;                                 // +0x18
    int   q_sequencep;                             // +0x1c
    long* quantlist;                               // +0x20
    struct encode_aux_nearestmatch* nearest_tree;  // +0x24
    struct encode_aux_threshmatch*  thresh_tree;   // +0x28
    struct encode_aux_pigeonhole*   pigeon_tree;   // +0x2c
    int   allocedp;                                // +0x30
} static_codebook;
typedef struct codebook {                          // libvorbis 1.0.1 codebook.h, 0x2c bytes
    long                   dim;                    // +0x00
    long                   entries;                // +0x04
    long                   used_entries;           // +0x08
    const static_codebook* c;                      // +0x0c
    float*                 valuelist;              // +0x10
    ogg_uint32_t*          codelist;               // +0x14
    int*                   dec_index;              // +0x18
    char*                  dec_codelengths;        // +0x1c
    ogg_uint32_t*          dec_firsttable;         // +0x20
    int                    dec_firsttablen;        // +0x24
    int                    dec_maxlength;          // +0x28
} codebook;

void* __cdecl memset(void* dst, int c, unsigned int n);
void* __cdecl _alloca(unsigned int size);
#pragma intrinsic(memset, _alloca)
#define alloca _alloca
#define NULL 0
__declspec(dllimport) void* __cdecl malloc(unsigned int size);                      // IAT [0x01440158]
__declspec(dllimport) void* __cdecl calloc(unsigned int count, unsigned int size);  // IAT [0x01440058]
__declspec(dllimport) void __cdecl free(void* block);                              // IAT [0x01440154]
__declspec(dllimport) void __cdecl qsort(void* base, unsigned int num, unsigned int width,
                                         int (__cdecl* compare)(const void*, const void*));   // IAT [0x01440060]
#define _ogg_malloc malloc
#define _ogg_calloc calloc
#define _ogg_free free

int __fastcall _ilog(unsigned int v);                                                  // 0x00C94EE0
ogg_uint32_t* __fastcall _make_words(long* l, long n, long sparsecount);               // 0x00C94FD0
float* __fastcall _book_unquantize(const static_codebook* b, int n, int* sparsemap);   // 0x00C951B0
void __fastcall vorbis_book_clear(codebook* b);                                        // 0x00C95440

static __declspec(noinline) ogg_uint32_t bitreverse(ogg_uint32_t x){
  x=    ((x>>16)&0x0000ffffUL) | ((x<<16)&0xffff0000UL);
  x=    ((x>> 8)&0x00ff00ffUL) | ((x<< 8)&0xff00ff00UL);
  x=    ((x>> 4)&0x0f0f0f0fUL) | ((x<< 4)&0xf0f0f0f0UL);
  x=    ((x>> 2)&0x33333333UL) | ((x<< 2)&0xccccccccUL);
  return((x>> 1)&0x55555555UL) | ((x<< 1)&0xaaaaaaaaUL);
}

static int __cdecl sort32a(const void *a,const void *b){
  return ( **(ogg_uint32_t **)a>**(ogg_uint32_t **)b)- 
    ( **(ogg_uint32_t **)a<**(ogg_uint32_t **)b);
}

/* decode codebook arrangement is more heavily optimized than encode */
int __fastcall vorbis_book_init_decode(codebook *c,const static_codebook *s){
  int i,j,n=0,tabn;
  int *sortindex;
  memset(c,0,sizeof(*c));
  
  /* count actually used entries */
  for(i=0;i<s->entries;i++)
    if(s->lengthlist[i]>0)
      n++;

  c->entries=s->entries;
  c->used_entries=n;
  c->dim=s->dim;

  /* two different remappings go on here.  

     First, we collapse the likely sparse codebook down only to
     actually represented values/words.  This collapsing needs to be
     indexed as map-valueless books are used to encode original entry
     positions as integers.

     Second, we reorder all vectors, including the entry index above,
     by sorted bitreversed codeword to allow treeless decode. */

  {
    /* perform sort */
    ogg_uint32_t *codes=_make_words(s->lengthlist,s->entries,c->used_entries);
    ogg_uint32_t **codep=alloca(sizeof(*codep)*n);
    
    if(codes==NULL)goto err_out;

    for(i=0;i<n;i++){
      codes[i]=bitreverse(codes[i]);
      codep[i]=codes+i;
    }

    qsort(codep,n,sizeof(*codep),sort32a);

    sortindex=alloca(n*sizeof(*sortindex));
    c->codelist=_ogg_malloc(n*sizeof(*c->codelist));
    /* the index is a reverse index */
    for(i=0;i<n;i++){
      int position=codep[i]-codes;
      sortindex[position]=i;
    }

    for(i=0;i<n;i++)
      c->codelist[sortindex[i]]=codes[i];
    _ogg_free(codes);
  }

  c->valuelist=_book_unquantize(s,n,sortindex);
  c->dec_index=_ogg_malloc(n*sizeof(*c->dec_index));

  for(n=0,i=0;i<s->entries;i++)
    if(s->lengthlist[i]>0)
      c->dec_index[sortindex[n++]]=i;
  
  c->dec_codelengths=_ogg_malloc(n*sizeof(*c->dec_codelengths));
  for(n=0,i=0;i<s->entries;i++)
    if(s->lengthlist[i]>0)
      c->dec_codelengths[sortindex[n++]]=s->lengthlist[i];

  c->dec_firsttablen=_ilog(c->used_entries)-4; /* this is magic */
  if(c->dec_firsttablen<5)c->dec_firsttablen=5;
  if(c->dec_firsttablen>8)c->dec_firsttablen=8;

  tabn=1<<c->dec_firsttablen;
  c->dec_firsttable=_ogg_calloc(tabn,sizeof(*c->dec_firsttable));
  c->dec_maxlength=0;

  for(i=0;i<n;i++){
    if(c->dec_maxlength<c->dec_codelengths[i])
      c->dec_maxlength=c->dec_codelengths[i];
    if(c->dec_codelengths[i]<=c->dec_firsttablen){
      ogg_uint32_t orig=bitreverse(c->codelist[i]);
      for(j=0;j<(1<<(c->dec_firsttablen-c->dec_codelengths[i]));j++)
	c->dec_firsttable[orig|(j<<c->dec_codelengths[i])]=i+1;
    }
  }

  /* now fill in 'unused' entries in the firsttable with hi/lo search
     hints for the non-direct-hits */
  {
    ogg_uint32_t mask=0xfffffffeUL<<(31-c->dec_firsttablen);
    long lo=0,hi=0;

    for(i=0;i<tabn;i++){
      ogg_uint32_t word=i<<(32-c->dec_firsttablen);
      if(c->dec_firsttable[bitreverse(word)]==0){
	while((lo+1)<n && c->codelist[lo+1]<=word)lo++;
	while(    hi<n && word>=(c->codelist[hi]&mask))hi++;
	
	/* we only actually have 15 bits per hint to play with here.
           In order to overflow gracefully (nothing breaks, efficiency
           just drops), encode as the difference from the extremes. */
	{
	  unsigned long loval=lo;
	  unsigned long hival=n-hi;

	  if(loval>0x7fff)loval=0x7fff;
	  if(hival>0x7fff)hival=0x7fff;
	  c->dec_firsttable[bitreverse(word)]=
	    0x80000000UL | (loval<<15) | hival;
	}
      }
    }
  }
  

  return(0);
 err_out:
  vorbis_book_clear(c);
  return(-1);
}

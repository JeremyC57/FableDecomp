// 00c95580 vorbis_book_init_decode
// Leaf: @vorbis_book_init_decode@8
// Flags: /TC /MD /Gr /Ob1
// Extract of Xiph.Org libvorbis 1.0.1 sharedbook.c. Copyright and license in libvorbis/codebook.h.
// The full unchanged upstream translation unit independently reproduces this routine.
#define FABLE_RETAIL_LIBVORBIS_CODEBOOK_PASS27
#include "libvorbis/codebook.h"

#if defined(_MSC_VER)
#define VORBIS_FASTCALL __fastcall
#define VORBIS_CDECL __cdecl
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#else
#include <alloca.h>
#include <stdlib.h>
#include <string.h>
#define VORBIS_FASTCALL
#define VORBIS_CDECL
#endif
#define _ogg_malloc malloc
#define _ogg_calloc calloc
#define _ogg_free free

ogg_uint32_t* VORBIS_FASTCALL _make_words(long* l, long n, long sparsecount);               // 0x00C94FD0
float* VORBIS_FASTCALL _book_unquantize(const static_codebook* b, int n, int* sparsemap);   // 0x00C951B0
void VORBIS_FASTCALL vorbis_book_clear(codebook* b);                                        // 0x00C95440

int VORBIS_FASTCALL _ilog(unsigned int v){
  int ret=0;
  while(v){
    ret++;
    v>>=1;
  }
  return(ret);
}


static ogg_uint32_t bitreverse(ogg_uint32_t x){
  x=    ((x>>16)&0x0000ffffUL) | ((x<<16)&0xffff0000UL);
  x=    ((x>> 8)&0x00ff00ffUL) | ((x<< 8)&0xff00ff00UL);
  x=    ((x>> 4)&0x0f0f0f0fUL) | ((x<< 4)&0xf0f0f0f0UL);
  x=    ((x>> 2)&0x33333333UL) | ((x<< 2)&0xccccccccUL);
  return((x>> 1)&0x55555555UL) | ((x<< 1)&0xaaaaaaaaUL);
}

static int VORBIS_CDECL sort32a(const void *a,const void *b){
  return ( **(ogg_uint32_t **)a>**(ogg_uint32_t **)b)-
    ( **(ogg_uint32_t **)a<**(ogg_uint32_t **)b);
}

/* decode codebook arrangement is more heavily optimized than encode */
int VORBIS_FASTCALL vorbis_book_init_decode(codebook *c,const static_codebook *s){
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
    ogg_uint32_t **codep=(ogg_uint32_t**)alloca(sizeof(*codep)*n);

    if(codes==NULL)goto err_out;

    for(i=0;i<n;i++){
      codes[i]=bitreverse(codes[i]);
      codep[i]=codes+i;
    }

    qsort(codep,n,sizeof(*codep),sort32a);

    sortindex=(int*)alloca(n*sizeof(*sortindex));
    c->codelist=(ogg_uint32_t*)_ogg_malloc(n*sizeof(*c->codelist));
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
  c->dec_index=(int*)_ogg_malloc(n*sizeof(*c->dec_index));

  for(n=0,i=0;i<s->entries;i++)
    if(s->lengthlist[i]>0)
      c->dec_index[sortindex[n++]]=i;

  c->dec_codelengths=(char*)_ogg_malloc(n*sizeof(*c->dec_codelengths));
  for(n=0,i=0;i<s->entries;i++)
    if(s->lengthlist[i]>0)
      c->dec_codelengths[sortindex[n++]]=(char)s->lengthlist[i];

  c->dec_firsttablen=_ilog(c->used_entries)-4; /* this is magic */
  if(c->dec_firsttablen<5)c->dec_firsttablen=5;
  if(c->dec_firsttablen>8)c->dec_firsttablen=8;

  tabn=1<<c->dec_firsttablen;
  c->dec_firsttable=(ogg_uint32_t*)_ogg_calloc(tabn,sizeof(*c->dec_firsttable));
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
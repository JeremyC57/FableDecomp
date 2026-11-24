#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
#include <malloc.h>
#else
#include <alloca.h>
#endif
#define FABLE_RETAIL_LIBVORBIS_CODEBOOK_PASS27
#include "libvorbis/codebook.h"

#define VORBIS_FASTCALL
#define VORBIS_CDECL
#define _ogg_malloc malloc
#define _ogg_calloc calloc
#define _ogg_free free
#ifdef _MSC_VER
#define alloca _alloca
#endif

ogg_uint32_t *_make_words(long *l,long n,long sparsecount){
  long i,j,count=0;
  ogg_uint32_t marker[33];
  ogg_uint32_t *r=(ogg_uint32_t*)_ogg_malloc((sparsecount?sparsecount:(n?n:1))*sizeof(*r));
  memset(marker,0,sizeof(marker));

  for(i=0;i<n;i++){
    long length=l[i];
    if(length>0){
      ogg_uint32_t entry=marker[length];
      
      /* when we claim a node for an entry, we also claim the nodes
	 below it (pruning off the imagined tree that may have dangled
	 from it) as well as blocking the use of any nodes directly
	 above for leaves */
      
      /* update ourself */
      if(length<32 && (entry>>length)){
	/* error condition; the lengths must specify an overpopulated tree */
	_ogg_free(r);
	return(NULL);
      }
      r[count++]=entry;
    
      /* Look to see if the next shorter marker points to the node
	 above. if so, update it and repeat.  */
      {
	for(j=length;j>0;j--){
	  
	  if(marker[j]&1){
	    /* have to jump branches */
	    if(j==1)
	      marker[1]++;
	    else
	      marker[j]=marker[j-1]<<1;
	    break; /* invariant says next upper marker would already
		      have been moved if it was on the same path */
	  }
	  marker[j]++;
	}
      }
      
      /* prune the tree; the implicit invariant says all the longer
	 markers were dangling from our just-taken node.  Dangle them
	 from our *new* node. */
      for(j=length+1;j<33;j++)
	if((marker[j]>>1) == entry){
	  entry=marker[j];
	  marker[j]=marker[j-1]<<1;
	}else
	  break;
    }else
      if(sparsecount==0)count++;
  }
    
  /* bitreverse the words because our bitwise packer/unpacker is LSb
     endian */
  for(i=0,count=0;i<n;i++){
    ogg_uint32_t temp=0;
    for(j=0;j<l[i];j++){
      temp<<=1;
      temp|=(r[count]>>j)&1;
    }

    if(sparsecount){
      if(l[i])
	r[count++]=temp;
    }else
      r[count++]=temp;
  }

  return(r);
}

static float* _book_unquantize(const static_codebook* s, int n, int* index)
{
    if (s->maptype == 0) return 0;
    float* values = (float*)malloc(n * s->dim * sizeof(*values));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < s->dim; ++j)
            values[index[i] * s->dim + j] = (float)(i * 10 + j);
    return values;
}
static void vorbis_book_clear(codebook* b)
{
    free(b->valuelist);
    free(b->codelist);
    free(b->dec_index);
    free(b->dec_codelengths);
    free(b->dec_firsttable);
    memset(b, 0, sizeof(*b));
}


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

#define CHECK(expr) do { if (!(expr)) { printf("00c95580_TEST failure at %d\n", __LINE__); return 1; } } while (0)
int main()
{
    codebook book;
    static_codebook config;
    memset(&config, 0, sizeof(config));
    config.dim = 2;

    long sparse[] = { 1, 0, 2, 3, 0 };
    config.entries = sizeof(sparse) / sizeof(*sparse);
    config.lengthlist = sparse;
    config.maptype = 2;
    memset(&book, 0xa5, sizeof(book));
    CHECK(vorbis_book_init_decode(&book, &config) == 0);
    CHECK(book.entries == 5 && book.used_entries == 3 && book.dim == 2 && !book.c);
    CHECK(book.dec_firsttablen == 5 && book.dec_maxlength == 3);
    CHECK(book.dec_index[0] == 0 && book.dec_index[1] == 2 && book.dec_index[2] == 3);
    CHECK(book.codelist[0] == 0 && book.codelist[1] == 0x80000000UL && book.codelist[2] == 0xc0000000UL);
    CHECK(book.dec_codelengths[0] == 1 && book.dec_codelengths[1] == 2 && book.dec_codelengths[2] == 3);
    CHECK(book.valuelist[0] == 0.0f && book.valuelist[2] == 10.0f && book.valuelist[5] == 21.0f);
    CHECK(book.dec_firsttable[0] == 1 && book.dec_firsttable[1] == 2 && book.dec_firsttable[3] == 3);
    CHECK(book.dec_firsttable[30] == 1 && book.dec_firsttable[29] == 2 && book.dec_firsttable[27] == 3);
    CHECK(book.dec_firsttable[7] == 0x80010000UL);
    vorbis_book_clear(&book);

    long shuffled[] = { 3, 1, 3, 0 };
    config.entries = sizeof(shuffled) / sizeof(*shuffled);
    config.lengthlist = shuffled;
    config.maptype = 0;
    CHECK(vorbis_book_init_decode(&book, &config) == 0);
    CHECK(book.dec_index[0] == 0 && book.dec_index[1] == 2 && book.dec_index[2] == 1);
    CHECK(book.dec_codelengths[0] == 3 && book.dec_codelengths[1] == 3 && book.dec_codelengths[2] == 1);
    CHECK(!book.valuelist);
    vorbis_book_clear(&book);

    long invalid[] = { 1, 1, 1 };
    config.entries = sizeof(invalid) / sizeof(*invalid);
    config.lengthlist = invalid;
    CHECK(vorbis_book_init_decode(&book, &config) == -1);
    CHECK(!book.entries && !book.used_entries && !book.codelist && !book.dec_firsttable);

    config.entries = 0;
    config.lengthlist = 0;
    CHECK(vorbis_book_init_decode(&book, &config) == 0);
    CHECK(!book.used_entries && book.dec_firsttablen == 5 && book.dec_maxlength == 0);
    for (int i = 0; i < 32; ++i)
        CHECK(book.dec_firsttable[i] == 0x80000000UL);
    vorbis_book_clear(&book);

    config.entries = 512;
    config.lengthlist = (long*)malloc(config.entries * sizeof(*config.lengthlist));
    CHECK(config.lengthlist != 0);
    for (long i = 0; i < config.entries; ++i)
        config.lengthlist[i] = 9;
    CHECK(vorbis_book_init_decode(&book, &config) == 0);
    CHECK(book.dec_firsttablen == 6 && book.dec_maxlength == 9);
    CHECK(book.dec_firsttable[0] == (0x80000000UL | (512 - 8)));
    CHECK(book.dec_firsttable[63] == (0x80000000UL | (504UL << 15)));
    vorbis_book_clear(&book);
    free(config.lengthlist);

    config.entries = 65536;
    config.lengthlist = (long*)malloc(config.entries * sizeof(*config.lengthlist));
    CHECK(config.lengthlist != 0);
    for (long i = 0; i < config.entries; ++i)
        config.lengthlist[i] = 16;
    CHECK(vorbis_book_init_decode(&book, &config) == 0);
    CHECK(book.dec_firsttablen == 8 && book.dec_maxlength == 16);
    CHECK(book.dec_firsttable[0] == 0x80007fffUL);
    CHECK(book.dec_firsttable[255] == 0xbfff8000UL);
    vorbis_book_clear(&book);
    free(config.lengthlist);
    puts("00c95580_TEST PASS");
    return 0;
}
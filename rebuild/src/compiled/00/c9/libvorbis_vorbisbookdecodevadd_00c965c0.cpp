// libvorbis::vorbis_book_decodev_add(codebook* book, float* a, oggpack_buffer* b, int n) -- codebook.c: decode n residue values and add them into a, one codeword per dim-vector: for books wider than 8 a plain inner loop over the entry's values, otherwise the dim-way switch with fall-through (8 down to 1 adds); -1 when a codeword fails to decode, else 0
// Vendor-proof: libvorbis-1.0.1(lib/codebook.c) @vorbis_book_decodev_add@16 -- 316 bytes verified against Fable.exe 0x00C965C0
// Leaf: @vorbis_book_decodev_add@16
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C965C0, 316 bytes, LTCG `extern "C" __fastcall` (book in ecx, a in edx, b and n on the stack,
//   `ret 8`). Callee: decode_packed_entry_number 0x00C96320 (`push b ; push book ; call`, callee-pop, the landed
//   `_decode_packed_entry_number@8`). Caller: res1_inverse / _01inverse 0x00CA12B0 (decodepart, landed libvorbis_res1class TU).
// Identity (two sources): (1) the body is Xiph libvorbis 1.0.1 lib/codebook.c vorbis_book_decodev_add verbatim -- the `dim > 8`
//   split, the entry * dim valuelist index, and the 8-case fall-through switch VC7.1 lays out as the jump table at 0x00C966DC with
//   the eight `fld ; fadd ; fstp` adds; (2) codebook.c file order among the landed decode entry points: vorbis_book_decode
//   0x00C96430, vorbis_book_decodevs_add 0x00C96450, vorbis_book_decodev_add 0x00C965C0, vorbis_book_decodev_set 0x00C96700,
//   vorbis_book_decodevv_add 0x00C96760, all `@name@16` __fastcall. The catalog's gap-scan name sub_00c965c0 is a placeholder.
// Shape: the landed vorbis_book_decodevs_add TU (codebook / oggpack_buffer models, decode_packed_entry_number declared
//   `__stdcall`) with this function appended.
#ifdef _MSC_VER
#include <malloc.h>
#else
#include <alloca.h>
#define __fastcall
#define __stdcall
#endif
typedef unsigned int ogg_uint32_t;
struct static_codebook;
typedef struct codebook{
  long dim;           /* codebook dimensions (elements per vector) */
  long entries;       /* codebook entries */
  long used_entries;  /* populated codebook entries */
  const static_codebook *c;

  /* for encode, the below are entry-ordered, fully populated */
  /* for decode, the below are ordered by bitreversed codeword and only
     used entries are populated */
  float        *valuelist;  /* list of dim*entries actual entry values */  
  ogg_uint32_t *codelist;   /* list of bitstream codewords for each entry */

  int          *dec_index;  /* only used if sparseness collapsed */
  char         *dec_codelengths;
  ogg_uint32_t *dec_firsttable;
  int           dec_firsttablen;
  int           dec_maxlength;

} codebook;
typedef struct {
  long endbyte;
  int  endbit;

  unsigned char *buffer;
  unsigned char *ptr;
  long storage;
} oggpack_buffer;
extern "C" long __stdcall decode_packed_entry_number(codebook*,oggpack_buffer*);
extern "C" long __fastcall vorbis_book_decodevs_add(codebook *book,float *a,oggpack_buffer *b,int n){
  int step=n/book->dim;
  long *entry = (long*)alloca(sizeof(*entry)*step);
  float **t = (float**)alloca(sizeof(*t)*step);
  int i,j,o;

  for (i = 0; i < step; i++) {
    entry[i]=decode_packed_entry_number(book,b);
    if(entry[i]==-1)return(-1);
    t[i] = book->valuelist+entry[i]*book->dim;
  }
  for(i=0,o=0;i<book->dim;i++,o+=step)
    for (j=0;j<step;j++)
      a[o+j]+=t[j][i];
  return(0);
}
extern "C" long __fastcall vorbis_book_decodev_add(codebook *book,float *a,oggpack_buffer *b,int n){
  int i,j,entry;
  float *t;

  if(book->dim>8){
    for(i=0;i<n;){
      entry = decode_packed_entry_number(book,b);
      if(entry==-1)return(-1);
      t     = book->valuelist+entry*book->dim;
      for (j=0;j<book->dim;)
	a[i++]+=t[j++];
    }
  }else{
    for(i=0;i<n;){
      entry = decode_packed_entry_number(book,b);
      if(entry==-1)return(-1);
      t     = book->valuelist+entry*book->dim;
      j=0;
      switch((int)book->dim){
      case 8:
	a[i++]+=t[j++];
      case 7:
	a[i++]+=t[j++];
      case 6:
	a[i++]+=t[j++];
      case 5:
	a[i++]+=t[j++];
      case 4:
	a[i++]+=t[j++];
      case 3:
	a[i++]+=t[j++];
      case 2:
	a[i++]+=t[j++];
      case 1:
	a[i++]+=t[j++];
      case 0:
	break;
      }
    }
  }    
  return(0);
}
typedef char codebook_size_check[sizeof(codebook)==44?1:-1];
// 00c96450 vorbis_book_decodevs_add
// Leaf: @vorbis_book_decodevs_add@16
// Flags: /Gr /Oy-
// Original libvorbis codebook.c and codebook.h; BSD-style terms in reference COPYING.
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
typedef char codebook_size_check[sizeof(codebook)==44?1:-1];
#include <stdio.h>
#include <string.h>
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

extern "C" void abort();
static unsigned cases;static int failAt,decodeCalls,seedValue;static codebook*expectedBook;static oggpack_buffer*expectedBuffer;
static void check(bool p){if(!p){fprintf(stderr,"vector decode case %u failed\n",cases);abort();}}
// Packed-entry dependency contract: predetermined indices or one EOF; candidate does all vector arithmetic.
extern "C" long __stdcall decode_packed_entry_number(codebook*b,oggpack_buffer*p){check(b==expectedBook&&p==expectedBuffer);int at=decodeCalls++;return at==failAt?-1:(at*3+seedValue)%8;}
int main(){const int dimensions[]={1,2,3,8,9};for(int d=0;d<5;++d){int dim=dimensions[d];for(int n=0;n<=40;++n){int step=n/dim;for(int seed=0;seed<4;++seed)for(int failure=0;failure<=step;++failure){
 codebook book;oggpack_buffer buffer;memset(&book,0,sizeof book);memset(&buffer,0,sizeof buffer);float values[72],copyValues[72],output[128],original[128];for(int i=0;i<72;++i)values[i]=copyValues[i]=((i*7+seed)%31-15)*0.125f;for(int i=0;i<128;++i)output[i]=original[i]=((i*3+seed)%17-8)*0.25f;book.dim=dim;book.valuelist=values;expectedBook=&book;expectedBuffer=&buffer;failAt=failure;seedValue=seed;decodeCalls=0;bool success=failure==step;
 long result=vorbis_book_decodevs_add(&book,output+1,&buffer,n);check(result==(success?0:-1)&&decodeCalls==(success?step:failure+1));
 for(int i=0;i<128;++i){float expected=original[i];if(success&&i>=1&&i<=step*dim){int at=i-1;int component=at/step;int token=(at%step*3+seed)%8;expected+=values[token*dim+component];}check(output[i]==expected);}check(memcmp(values,copyValues,sizeof values)==0);++cases;
 }}}check(cases==7376);puts("00c96450_TEST PASS");return 0;}
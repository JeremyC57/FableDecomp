// 00c951b0 _book_unquantize
// Leaf: @_book_unquantize@12
// Flags: /Gr
// Original libvorbis sharedbook.c and codebook.h; BSD-style terms in reference COPYING.
#include <stddef.h>
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(fabs)
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int,unsigned int);
#else
#define __fastcall
#include <stdlib.h>
#endif
#define _ogg_calloc calloc
typedef struct static_codebook{
  long   dim;            /* codebook dimensions (elements per vector) */
  long   entries;        /* codebook entries */
  long  *lengthlist;     /* codeword lengths in bits */

  /* mapping ***************************************************************/
  int    maptype;        /* 0=none
			    1=implicitly populated values from map column 
			    2=listed arbitrary values */

  /* The below does a linear, single monotonic sequence mapping. */
  long     q_min;       /* packed 32 bit float; quant value 0 maps to minval */
  long     q_delta;     /* packed 32 bit float; val 1 - val 0 == delta */
  int      q_quant;     /* bits: 0 < quant <= 16 */
  int      q_sequencep; /* bitflag */

  long     *quantlist;  /* map == 1: (int)(entries^(1/dim)) element column map
			   map == 2: list of dim*entries quantized entry vals
			*/

  /* encode helpers ********************************************************/
  struct encode_aux_nearestmatch *nearest_tree;
  struct encode_aux_threshmatch  *thresh_tree;
  struct encode_aux_pigeonhole  *pigeon_tree;

  int allocedp;
} static_codebook;
extern "C" float __fastcall _float32_unpack(long val);
extern "C" long __fastcall _book_maptype1_quantvals(const static_codebook *b);
extern "C" float* __fastcall _book_unquantize(const static_codebook *b,int n,int *sparsemap){
  long j,k,count=0;
  if(b->maptype==1 || b->maptype==2){
    int quantvals;
    float mindel=_float32_unpack(b->q_min);
    float delta=_float32_unpack(b->q_delta);
    float *r=(float*)_ogg_calloc(n*b->dim,sizeof(*r));

    /* maptype 1 and 2 both use a quantized value vector, but
       different sizes */
    switch(b->maptype){
    case 1:
      /* most of the time, entries%dimensions == 0, but we need to be
	 well defined.  We define that the possible vales at each
	 scalar is values == entries/dim.  If entries%dim != 0, we'll
	 have 'too few' values (values*dim<entries), which means that
	 we'll have 'left over' entries; left over entries use zeroed
	 values (and are wasted).  So don't generate codebooks like
	 that */
      quantvals=_book_maptype1_quantvals(b);
      for(j=0;j<b->entries;j++){
	if((sparsemap && b->lengthlist[j]) || !sparsemap){
	  float last=0.f;
	  int indexdiv=1;
	  for(k=0;k<b->dim;k++){
	    int index= (j/indexdiv)%quantvals;
	    float val=b->quantlist[index];
	    val=fabs(val)*delta+mindel+last;
	    if(b->q_sequencep)last=val;	  
	    if(sparsemap)
	      r[sparsemap[count]*b->dim+k]=val;
	    else
	      r[count*b->dim+k]=val;
	    indexdiv*=quantvals;
	  }
	  count++;
	}

      }
      break;
    case 2:
      for(j=0;j<b->entries;j++){
	if((sparsemap && b->lengthlist[j]) || !sparsemap){
	  float last=0.f;
	  
	  for(k=0;k<b->dim;k++){
	    float val=b->quantlist[j*b->dim+k];
	    val=fabs(val)*delta+mindel+last;
	    if(b->q_sequencep)last=val;	  
	    if(sparsemap)
	      r[sparsemap[count]*b->dim+k]=val;
	    else
	      r[count*b->dim+k]=val;
	  }
	  count++;
	}
      }
      break;
    }

    return(r);
  }
  return(NULL);
}
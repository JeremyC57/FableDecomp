// 00c95ca0 vorbis_staticbook_pack
// Leaf: @vorbis_staticbook_pack@8
// Flags: /Gr
// Original libvorbis codebook.c/headers; BSD-style terms in reference COPYING.
// C local this is renamed thisLength because it is a C++ keyword.
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#endif
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
typedef struct {
  long endbyte;
  int  endbit;

  unsigned char *buffer;
  unsigned char *ptr;
  long storage;
} oggpack_buffer;
extern "C" void __fastcall oggpack_write(oggpack_buffer*,unsigned long,int);
extern "C" int __fastcall _ilog(unsigned int);
extern "C" long __fastcall _book_maptype1_quantvals(const static_codebook*);
extern "C" int __fastcall vorbis_staticbook_pack(const static_codebook *c,oggpack_buffer *opb){
  long i,j;
  int ordered=0;

  /* first the basic parameters */
  oggpack_write(opb,0x564342,24);
  oggpack_write(opb,c->dim,16);
  oggpack_write(opb,c->entries,24);

  /* pack the codewords.  There are two packings; length ordered and
     length random.  Decide between the two now. */
  
  for(i=1;i<c->entries;i++)
    if(c->lengthlist[i-1]==0 || c->lengthlist[i]<c->lengthlist[i-1])break;
  if(i==c->entries)ordered=1;
  
  if(ordered){
    /* length ordered.  We only need to say how many codewords of
       each length.  The actual codewords are generated
       deterministically */

    long count=0;
    oggpack_write(opb,1,1);  /* ordered */
    oggpack_write(opb,c->lengthlist[0]-1,5); /* 1 to 32 */

    for(i=1;i<c->entries;i++){
      long thisLength=c->lengthlist[i];
      long last=c->lengthlist[i-1];
      if(thisLength>last){
	for(j=last;j<thisLength;j++){
	  oggpack_write(opb,i-count,_ilog(c->entries-count));
	  count=i;
	}
      }
    }
    oggpack_write(opb,i-count,_ilog(c->entries-count));
    
  }else{
    /* length random.  Again, we don't code the codeword itself, just
       the length.  This time, though, we have to encode each length */
    oggpack_write(opb,0,1);   /* unordered */
    
    /* algortihmic mapping has use for 'unused entries', which we tag
       here.  The algorithmic mapping happens as usual, but the unused
       entry has no codeword. */
    for(i=0;i<c->entries;i++)
      if(c->lengthlist[i]==0)break;

    if(i==c->entries){
      oggpack_write(opb,0,1); /* no unused entries */
      for(i=0;i<c->entries;i++)
	oggpack_write(opb,c->lengthlist[i]-1,5);
    }else{
      oggpack_write(opb,1,1); /* we have unused entries; thus we tag */
      for(i=0;i<c->entries;i++){
	if(c->lengthlist[i]==0){
	  oggpack_write(opb,0,1);
	}else{
	  oggpack_write(opb,1,1);
	  oggpack_write(opb,c->lengthlist[i]-1,5);
	}
      }
    }
  }

  /* is the entry number the desired return value, or do we have a
     mapping? If we have a mapping, what type? */
  oggpack_write(opb,c->maptype,4);
  switch(c->maptype){
  case 0:
    /* no mapping */
    break;
  case 1:case 2:
    /* implicitly populated value mapping */
    /* explicitly populated value mapping */
    
    if(!c->quantlist){
      /* no quantlist?  error */
      return(-1);
    }
    
    /* values that define the dequantization */
    oggpack_write(opb,c->q_min,32);
    oggpack_write(opb,c->q_delta,32);
    oggpack_write(opb,c->q_quant-1,4);
    oggpack_write(opb,c->q_sequencep,1);
    
    {
      int quantvals;
      switch(c->maptype){
      case 1:
	/* a single column of (c->entries/c->dim) quantized values for
	   building a full value list algorithmically (square lattice) */
	quantvals=_book_maptype1_quantvals(c);
	break;
      case 2:
	/* every value (c->entries*c->dim total) specified explicitly */
	quantvals=c->entries*c->dim;
	break;
      default: /* NOT_REACHABLE */
	quantvals=-1;
      }

      /* quantized values */
      for(i=0;i<quantvals;i++)
	oggpack_write(opb,labs(c->quantlist[i]),c->q_quant);

    }
    break;
  default:
    /* error case; we don't have any other map types now */
    return(-1);
  }

  return(0);
}
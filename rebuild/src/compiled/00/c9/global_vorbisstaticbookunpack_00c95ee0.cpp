// 00c95ee0 vorbis_staticbook_unpack
// Leaf: @vorbis_staticbook_unpack@8
// Flags: /Gr
// Original libvorbis codebook.c/headers; BSD-style terms in reference COPYING.
#include <stddef.h>
#include <string.h>
#ifdef _MSC_VER
#pragma intrinsic(memset)
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int);
#else
#include <stdlib.h>
#endif
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

#define _ogg_malloc malloc
extern "C" long __fastcall oggpack_read(oggpack_buffer*,int);
extern "C" int __fastcall _ilog(unsigned int);
extern "C" long __fastcall _book_maptype1_quantvals(const static_codebook*);
extern "C" void __fastcall vorbis_staticbook_clear(static_codebook*);
extern "C" int __fastcall vorbis_staticbook_unpack(oggpack_buffer *opb,static_codebook *s){
  long i,j;
  memset(s,0,sizeof(*s));
  s->allocedp=1;

  /* make sure alignment is correct */
  if(oggpack_read(opb,24)!=0x564342)goto _eofout;

  /* first the basic parameters */
  s->dim=oggpack_read(opb,16);
  s->entries=oggpack_read(opb,24);
  if(s->entries==-1)goto _eofout;

  /* codeword ordering.... length ordered or unordered? */
  switch((int)oggpack_read(opb,1)){
  case 0:
    /* unordered */
    s->lengthlist=(long*)_ogg_malloc(sizeof(*s->lengthlist)*s->entries);

    /* allocated but unused entries? */
    if(oggpack_read(opb,1)){
      /* yes, unused entries */

      for(i=0;i<s->entries;i++){
	if(oggpack_read(opb,1)){
	  long num=oggpack_read(opb,5);
	  if(num==-1)goto _eofout;
	  s->lengthlist[i]=num+1;
	}else
	  s->lengthlist[i]=0;
      }
    }else{
      /* all entries used; no tagging */
      for(i=0;i<s->entries;i++){
	long num=oggpack_read(opb,5);
	if(num==-1)goto _eofout;
	s->lengthlist[i]=num+1;
      }
    }
    
    break;
  case 1:
    /* ordered */
    {
      long length=oggpack_read(opb,5)+1;
      s->lengthlist=(long*)_ogg_malloc(sizeof(*s->lengthlist)*s->entries);

      for(i=0;i<s->entries;){
	long num=oggpack_read(opb,_ilog(s->entries-i));
	if(num==-1)goto _eofout;
	for(j=0;j<num && i<s->entries;j++,i++)
	  s->lengthlist[i]=length;
	length++;
      }
    }
    break;
  default:
    /* EOF */
    return(-1);
  }
  
  /* Do we have a mapping to unpack? */
  switch((s->maptype=oggpack_read(opb,4))){
  case 0:
    /* no mapping */
    break;
  case 1: case 2:
    /* implicitly populated value mapping */
    /* explicitly populated value mapping */

    s->q_min=oggpack_read(opb,32);
    s->q_delta=oggpack_read(opb,32);
    s->q_quant=oggpack_read(opb,4)+1;
    s->q_sequencep=oggpack_read(opb,1);

    {
      int quantvals=0;
      switch(s->maptype){
      case 1:
	quantvals=_book_maptype1_quantvals(s);
	break;
      case 2:
	quantvals=s->entries*s->dim;
	break;
      }
      
      /* quantized values */
      s->quantlist=(long*)_ogg_malloc(sizeof(*s->quantlist)*quantvals);
      for(i=0;i<quantvals;i++)
	s->quantlist[i]=oggpack_read(opb,s->q_quant);
      
      if(quantvals&&s->quantlist[quantvals-1]==-1)goto _eofout;
    }
    break;
  default:
    goto _errout;
  }

  /* all set */
  return(0);
  
 _errout:
 _eofout:
  vorbis_staticbook_clear(s);
  return(-1); 
}
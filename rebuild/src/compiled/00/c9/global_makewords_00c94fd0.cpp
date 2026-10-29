// 00c94fd0 _make_words
// Leaf: @_make_words@12
// Flags: /Gr
// Original libvorbis sharedbook.c; BSD-style terms in reference libvorbis COPYING.
#include <string.h>
#ifdef _MSC_VER
#pragma intrinsic(memset)
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int);
extern "C" __declspec(dllimport) void __cdecl free(void*);
#else
#define __fastcall
#include <stdlib.h>
#endif
typedef unsigned int ogg_uint32_t;
#define _ogg_malloc malloc
#define _ogg_free free
extern "C" ogg_uint32_t* __fastcall _make_words(long *l,long n,long sparsecount){
  long i,j,count=0;
  ogg_uint32_t marker[33];
  ogg_uint32_t *r=(ogg_uint32_t*)_ogg_malloc((sparsecount?sparsecount:n)*sizeof(*r));
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
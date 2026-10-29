// 00c92f10 seed_chase
// Leaf: _seed_chase@12
// Flags:
// Original libvorbis psy.c; BSD-style terms in reference COPYING.
#ifdef _MSC_VER
#include <malloc.h>
#else
#include <alloca.h>
#define __stdcall
#endif
extern "C" void __stdcall seed_chase(float *seeds, int linesper, long n){
  long  *posstack=(long*)alloca(n*sizeof(*posstack));
  float *ampstack=(float*)alloca(n*sizeof(*ampstack));
  long   stack=0;
  long   pos=0;
  long   i;

  for(i=0;i<n;i++){
    if(stack<2){
      posstack[stack]=i;
      ampstack[stack++]=seeds[i];
    }else{
      while(1){
	if(seeds[i]<ampstack[stack-1]){
	  posstack[stack]=i;
	  ampstack[stack++]=seeds[i];
	  break;
	}else{
	  if(i<posstack[stack-1]+linesper){
	    if(stack>1 && ampstack[stack-1]<=ampstack[stack-2] &&
	       i<posstack[stack-2]+linesper){
	      /* we completely overlap, making stack-1 irrelevant.  pop it */
	      stack--;
	      continue;
	    }
	  }
	  posstack[stack]=i;
	  ampstack[stack++]=seeds[i];
	  break;

	}
      }
    }
  }

  /* the stack now contains only the positions that are relevant. Scan
     'em straight through */

  for(i=0;i<stack;i++){
    long endpos;
    if(i<stack-1 && ampstack[i+1]>ampstack[i]){
      endpos=posstack[i+1];
    }else{
      endpos=posstack[i]+linesper+1; /* +1 is important, else bin 0 is
					discarded in short frames */
    }
    if(endpos>n)endpos=n;
    for(;pos<endpos;pos++)
      seeds[pos]=ampstack[i];
  }
  
  /* there.  Linear time.  I now remember this was on a problem set I
     had in Grad Skool... I didn't solve it at the time ;-) */

}
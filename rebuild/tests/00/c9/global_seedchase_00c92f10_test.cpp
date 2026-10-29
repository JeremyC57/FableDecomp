#include <stdio.h>
#include <stdlib.h>
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

static unsigned cases;
static void check(bool p){if(!p){fprintf(stderr,"seed chase case %u failed\n",cases);abort();}}
// Independent fixed-point list reduction, followed by interval-priority evaluation.
static void reference(const float*input,float*out,int n,int span){int keep[10],count=n;for(int k=0;k<n;++k)keep[k]=k;bool changed=true;while(changed){changed=false;for(int k=1;k+1<count;++k){int a=keep[k-1],b=keep[k],c=keep[k+1];if(c<a+span&&input[b]<=input[a]&&input[b]<=input[c]){for(int j=k;j+1<count;++j)keep[j]=keep[j+1];--count;changed=true;break;}}}for(int x=0;x<n;++x){bool assigned=false;for(int k=0;k<count;++k){int at=keep[k];int end=k+1<count&&input[keep[k+1]]>input[at]?keep[k+1]:at+span+1;if(x<end){out[x]=input[at];assigned=true;break;}}check(assigned);}}
int main(){int patterns=1;for(int n=0;n<=6;++n){if(n)patterns*=5;for(int pattern=0;pattern<patterns;++pattern)for(int span=0;span<=n+2;++span){float data[10],before[10];int v=pattern;for(int i=0;i<10;++i)data[i]=before[i]=64.0f;for(int i=0;i<n;++i){data[i+1]=before[i+1]=(v%5-2)*0.5f;v/=5;}seed_chase(data+1,span,n);float expected[10];reference(before+1,expected,n,span);for(int i=0;i<n;++i)check(data[i+1]==expected[i]);check(data[0]==64.0f);for(int i=n+1;i<10;++i)check(data[i]==64.0f);++cases;}}check(cases==170898);puts("00c92f10_TEST PASS");return 0;}
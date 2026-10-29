#include <stdio.h>
#ifndef _MSC_VER
#include <stdlib.h>
extern "C" void* test_malloc(size_t);
extern "C" void test_free(void*);
#define malloc test_malloc
#define free test_free
#endif
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

#ifndef _MSC_VER
#undef malloc
#undef free
#define __cdecl
#endif
extern "C" void __cdecl abort();
static unsigned cases,allocated,freed,requested;
static unsigned storage[66];
static void check(bool p){if(!p){fprintf(stderr,"codeword case %u failed\n",cases);abort();}}
extern "C" void* __cdecl test_malloc(unsigned bytes){check(allocated==0&&bytes<=64*sizeof(unsigned));allocated++;requested=bytes;for(int i=0;i<66;++i)storage[i]=0x5a7b9cdeU;return storage+1;}
extern "C" void __cdecl test_free(void*p){check(p==storage+1&&allocated==1&&freed==0);++freed;}
#ifdef _MSC_VER
extern "C" {void* (__cdecl* _imp__malloc)(unsigned)=test_malloc;void (__cdecl* _imp__free)(void*)=test_free;}
#endif
// Independent binary-tree allocation: choose the smallest unused prefix for each leaf.
static bool reference(const long*lengths,int n,unsigned*out){unsigned chosen[8];for(int i=0;i<n;++i){if(lengths[i]==0){chosen[i]=out[i]=0;continue;}unsigned value=0;for(;;++value){if(lengths[i]<32&&value>=(1U<<lengths[i]))return false;bool collision=false;for(int k=0;k<i;++k)if(lengths[k]){if(lengths[k]<=lengths[i])collision=((value>>(lengths[i]-lengths[k]))==chosen[k]);else collision=((chosen[k]>>(lengths[k]-lengths[i]))==value);if(collision)break;}if(!collision)break;}chosen[i]=value;unsigned reversed=0;for(int bit=0;bit<lengths[i];++bit){reversed=(reversed<<1)|(value&1);value>>=1;}out[i]=reversed;}return true;}
static void run(long*lengths,int n,bool sparse){unsigned expected[8];long copy[8];long used=0;for(int i=0;i<n;++i){copy[i]=lengths[i];if(lengths[i])++used;}bool valid=reference(lengths,n,expected);allocated=freed=requested=0;unsigned*actual=_make_words(lengths,n,sparse?used:0);check(allocated==1&&requested==unsigned((sparse&&used?used:n)*sizeof(unsigned)));if(!valid){check(actual==0&&freed==1);}else{check(actual==storage+1&&freed==0);int at=0;for(int i=0;i<n;++i)if(!sparse||!used||lengths[i])check(actual[at++]==expected[i]);test_free(actual);}check(freed==1&&storage[0]==0x5a7b9cdeU&&storage[1+requested/4]==0x5a7b9cdeU);for(int i=0;i<n;++i)check(lengths[i]==copy[i]);++cases;}
int main(){long lengths[8];int combinations=1;for(int n=0;n<=5;++n){if(n)combinations*=6;for(int pattern=0;pattern<combinations;++pattern){int v=pattern;for(int i=0;i<n;++i){lengths[i]=v%6;v/=6;}run(lengths,n,false);run(lengths,n,true);}}lengths[0]=32;lengths[1]=32;run(lengths,1,false);run(lengths,2,false);run(lengths,2,true);check(cases==18665);puts("00c94fd0_TEST PASS");return 0;}
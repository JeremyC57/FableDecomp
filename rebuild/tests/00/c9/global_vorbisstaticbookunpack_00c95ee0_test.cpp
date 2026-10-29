#include <stdio.h>
#ifndef _MSC_VER
#include <stdlib.h>
extern "C" void* test_malloc(size_t);
#define malloc test_malloc
#endif
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

#ifndef _MSC_VER
#undef malloc
#define __cdecl
#endif
extern "C" void __cdecl abort();
static unsigned cases,allocated,freed,used,fullBits,readBits,limitBits;
static unsigned memory[128],*blocks[3];static bool releases[3];
static unsigned char input[256],saved[256];
static void check(bool p){if(!p){fprintf(stderr,"book unpack case %u failed\n",cases);abort();}}
extern "C" void* __cdecl test_malloc(unsigned bytes){check(bytes%4==0&&allocated<3&&used+bytes/4+2<128);unsigned*p=memory+used+1;used+=bytes/4+2;blocks[allocated]=p;releases[allocated++]=false;return p;}
#ifdef _MSC_VER
extern "C" {void* (__cdecl* _imp__malloc)(unsigned)=test_malloc;}
#endif
static void release(void*p){for(unsigned i=0;i<allocated;++i)if(p==blocks[i]){check(!releases[i]);releases[i]=true;++freed;return;}check(false);}
// Contract-only cleanup helper: these test books own only their two decoded arrays.
extern "C" void __fastcall vorbis_staticbook_clear(static_codebook*b){check(b->allocedp==1&&!b->nearest_tree&&!b->thresh_tree&&!b->pigeon_tree);if(b->quantlist)release(b->quantlist);if(b->lengthlist)release(b->lengthlist);memset(b,0,sizeof(*b));}
extern "C" long __fastcall oggpack_read(oggpack_buffer*b,int count){check(count>=0&&count<=32);if(readBits+(unsigned)count>limitBits){readBits+=count;b->endbit=readBits%8;b->endbyte=readBits/8;return -1;}unsigned value=0;for(int k=0;k<count;++k){value|=((input[readBits/8]>>(readBits%8))&1U)<<k;++readBits;}b->endbit=readBits%8;b->endbyte=readBits/8;return (long)value;}
extern "C" int __fastcall _ilog(unsigned value){int count=0;while(value){++count;value>>=1;}return count;}
extern "C" long __fastcall _book_maptype1_quantvals(const static_codebook*b){int q=1;for(;;){long n=1;for(int i=0;i<b->dim;++i)n*=q+1;if(n>b->entries)return q;++q;}}
static void put(unsigned value,int count){for(int k=0;k<count;++k){if((value>>k)&1U)input[fullBits/8]|=(unsigned char)(1U<<(fullBits%8));++fullBits;}check(fullBits<2048);}
// Independent fixture encoder emits canonical ordered or explicitly sparse length records.
static void encode(const static_codebook*b,bool ordered){memset(input,0,sizeof input);fullBits=0;put(0x564342,24);put(b->dim,16);put(b->entries,24);put(ordered,1);if(ordered){put(b->lengthlist[0]-1,5);int pos=0;for(long length=b->lengthlist[0];pos<b->entries;++length){int start=pos;while(pos<b->entries&&b->lengthlist[pos]==length)++pos;put(pos-start,_ilog(b->entries-start));}}else{put(1,1);for(int i=0;i<b->entries;++i){put(b->lengthlist[i]!=0,1);if(b->lengthlist[i])put(b->lengthlist[i]-1,5);}}put(b->maptype,4);if(b->maptype==1||b->maptype==2){put(b->q_min,32);put(b->q_delta,32);put(b->q_quant-1,4);put(b->q_sequencep,1);int count=b->maptype==1?_book_maptype1_quantvals(b):b->dim*b->entries;for(int i=0;i<count;++i)put(b->quantlist[i],b->q_quant);}memcpy(saved,input,sizeof input);}
static void run(const static_codebook*expected,unsigned limit,bool magicBad){for(int i=0;i<128;++i)memory[i]=0x89abcdefU;allocated=freed=used=readBits=0;limitBits=limit;static_codebook result;memset(&result,0xa5,sizeof result);oggpack_buffer opb;memset(&opb,0,sizeof opb);int status=vorbis_staticbook_unpack(&opb,&result);bool valid=limit==fullBits&&!magicBad&&(expected->maptype<=2);if(valid){check(status==0&&result.dim==expected->dim&&result.entries==expected->entries&&result.maptype==expected->maptype&&result.allocedp==1);for(int i=0;i<result.entries;++i)check(result.lengthlist[i]==expected->lengthlist[i]);if(expected->maptype){check(result.q_min==expected->q_min&&result.q_delta==expected->q_delta&&result.q_quant==expected->q_quant&&result.q_sequencep==expected->q_sequencep);int n=expected->maptype==1?_book_maptype1_quantvals(expected):expected->entries*expected->dim;for(int i=0;i<n;++i)check(result.quantlist[i]==expected->quantlist[i]);}check(!freed&&readBits==fullBits);vorbis_staticbook_clear(&result);}else{check(status==-1);if(allocated)check(freed==allocated);}
check(freed==allocated&&memory[0]==0x89abcdefU&&memory[used]==0x89abcdefU);check(memcmp(input,saved,sizeof input)==0||magicBad);++cases;}
int main(){const int sizes[]={1,2,5,16};for(int si=0;si<4;++si)for(int shape=0;shape<4;++shape)for(int dim=1;dim<=3;++dim)for(int kind=0;kind<3;++kind)for(int seq=0;seq<2;++seq){static_codebook b;memset(&b,0,sizeof b);long lengths[16],quant[48];for(int i=0;i<16;++i)lengths[i]=shape==0?3:(shape==1?1+i/2:(shape==2?1+(i*3+1)%5:(i%3?4:0)));for(int i=0;i<48;++i)quant[i]=(i*7+3)%31;b.entries=sizes[si];b.dim=dim;b.lengthlist=lengths;b.maptype=kind;b.q_min=0x60123456;b.q_delta=0x61234567;b.q_quant=5;b.q_sequencep=seq;b.quantlist=quant;encode(&b,shape<2);for(unsigned limit=0;limit<=fullBits;++limit)run(&b,limit,false);input[0]^=1;run(&b,fullBits,true);memcpy(input,saved,sizeof input);}check(cases>30000);puts("00c95ee0_TEST PASS");return 0;}
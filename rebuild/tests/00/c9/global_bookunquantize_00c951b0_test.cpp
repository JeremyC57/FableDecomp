#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#include <stdlib.h>
extern "C" void* test_calloc(size_t,size_t);
#define calloc test_calloc
#endif
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

#ifndef _MSC_VER
#undef calloc
#define __cdecl
#endif
extern "C" void __cdecl abort();
static unsigned cases,allocations,requested,unpackCalls,rootCalls;
static float arena[130];
static void check(bool p){if(!p){fprintf(stderr,"unquantize case %u failed\n",cases);abort();}}
extern "C" void* __cdecl test_calloc(unsigned count,unsigned bytes){check(!allocations&&bytes==sizeof(float)&&count<=128);++allocations;requested=count;for(int i=0;i<130;++i)arena[i]=64.0f;for(unsigned i=0;i<count;++i)arena[i+1]=0.0f;return arena+1;}
#ifdef _MSC_VER
extern "C" {void* (__cdecl* _imp__calloc)(unsigned,unsigned)=test_calloc;}
#endif
// Dependency contracts: packed scalar decoding and integral codebook radix only.
static float decode(long v){long mant=v&0x1fffff;int e=((v&0x7fe00000L)>>21)-788;float result=(float)mant;while(e>0){result*=2.0f;--e;}while(e<0){result*=0.5f;++e;}return (v&0x80000000UL)?-result:result;}
extern "C" float __fastcall _float32_unpack(long value){++unpackCalls;return decode(value);}
static int radix(const static_codebook*b){int q=1;for(;;){long result=1;for(int k=0;k<b->dim;++k)result*=q+1;if(result>b->entries)return q;++q;}}
extern "C" long __fastcall _book_maptype1_quantvals(const static_codebook*b){++rootCalls;return radix(b);}
static long packPower(int exponent,bool negative){return ((long)(768+exponent)<<21)|(1L<<20)|(negative?0x80000000UL:0UL);}
int main(){
 for(int entries=1;entries<=17;++entries)for(int dim=1;dim<=4;++dim)for(int kind=0;kind<4;++kind)for(int sequence=0;sequence<2;++sequence)for(int sparse=0;sparse<2;++sparse)for(int seed=0;seed<4;++seed){
 static_codebook b;memset(&b,0,sizeof b);long lengths[20],quant[72],oldQuant[72],oldLengths[20];int remap[20];for(int i=0;i<20;++i)lengths[i]=oldLengths[i]=((i+seed)%3)?3:0;for(int i=0;i<72;++i)quant[i]=oldQuant[i]=(i*7+seed*3)%19-9;
 int active=0;for(int i=0;i<entries;++i)if(!sparse||lengths[i])++active;for(int i=0;i<active;++i)remap[i]=active-i-1;
 b.dim=dim;b.entries=entries;b.lengthlist=lengths;b.maptype=kind;b.q_min=packPower((seed%3)-1,(seed&1)!=0);b.q_delta=packPower(-1-(seed&1),false);b.q_sequencep=sequence;b.quantlist=quant;static_codebook original=b;
 allocations=requested=unpackCalls=rootCalls=0;float*result=_book_unquantize(&b,active,sparse?remap:0);
 if(kind!=1&&kind!=2){check(!result&&!allocations&&!unpackCalls&&!rootCalls);}else{check(result==arena+1&&allocations==1&&requested==(unsigned)(active*dim)&&unpackCalls==2&&rootCalls==(unsigned)(kind==1));int q=radix(&b),at=0;for(int i=0;i<entries;++i)if(!sparse||lengths[i]){int dest=sparse?remap[at]:at;long cumulative=0,power=1;for(int k=0;k<dim;++k){int index=kind==1?(i/power)%q:i*dim+k;long value=quant[index]<0?-quant[index]:quant[index];cumulative+=value;float expected=(sequence?cumulative:value)*decode(b.q_delta)+(sequence?k+1:1)*decode(b.q_min);check(result[dest*dim+k]==expected);power*=q;}++at;}check(arena[0]==64.0f&&arena[requested+1]==64.0f);}
 check(memcmp(&b,&original,sizeof b)==0&&memcmp(quant,oldQuant,sizeof quant)==0&&memcmp(lengths,oldLengths,sizeof lengths)==0);++cases;
 }
 check(cases==4352);puts("00c951b0_TEST PASS");return 0;
}
// Self-contained original target, no external engine mocks required.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
static void fail(int line){std::fprintf(stderr,"failure %d\n",line);std::exit(1);}
#define CHECK(x) do{if(!(x))fail(__LINE__);}while(0)
// 00c0acc0 MultiplyMaskedFactors
// Flags:
// Leaf: ?MultiplyMaskedFactors@@YIMMPBMKK@Z
// Fidelity: descriptive binding MultiplyMaskedFactors has no PDB identity.
float __fastcall MultiplyMaskedFactors(float value,const float* factors,unsigned long mask,unsigned long count)
{
    if(value != 0.0f){
        const float* cursor=factors;
        unsigned long remaining=count;
        do{
            if(mask==0 || remaining==0)break;
            if(mask&1)value*=*cursor;
            --remaining; mask>>=1; ++cursor;
        }while(value!=0.0f);
    }
    return value;
}

static float bits(unsigned int n){float f;CHECK(sizeof(f)==sizeof(n));std::memcpy(&f,&n,4);return f;}
int main(){
 const float factors[8]={2.0f,0.5f,-1.0f,4.0f,0.0f,0.25f,8.0f,-2.0f};
 const float starts[6]={0.0f,-0.0f,1.0f,-1.0f,8.0f,-8.0f};
 const unsigned long masks[16]={0,1,2,3,5,8,15,16,31,32,63,85,128,170,255,0x80000000UL};
 int cases=0;
 for(int v=0;v<6;++v)for(int m=0;m<16;++m)for(unsigned long n=0;n<=8;++n){
  float expected=starts[v];
  for(unsigned long bit=0;bit<n;++bit)if((masks[m]&(1UL<<bit))!=0)expected=expected*factors[bit];
  float got=MultiplyMaskedFactors(starts[v],factors,masks[m],n);
  CHECK(got==expected);++cases;
 }
 float wide[32];for(int w=0;w<32;++w)wide[w]=1.0f;wide[31]=2.0f;
 CHECK(MultiplyMaskedFactors(4.0f,wide,0x80000000UL,31)==4.0f);
 CHECK(MultiplyMaskedFactors(4.0f,wide,0x80000000UL,32)==8.0f);
 CHECK(MultiplyMaskedFactors(4.0f,wide,0x80000000UL,40)==8.0f);
 // Every no-work guard must tolerate an absent factor buffer.
 CHECK(MultiplyMaskedFactors(0.0f,0,0xffffffffUL,32)==0.0f);
 CHECK(MultiplyMaskedFactors(-8.0f,0,0xffffffffUL,0)==-8.0f);
 CHECK(MultiplyMaskedFactors(8.0f,0,0,32)==8.0f);
 float quiet=bits(0x7fc00000U),infinity=bits(0x7f800000U);
 CHECK(MultiplyMaskedFactors(quiet,0,0,32)!=MultiplyMaskedFactors(quiet,0,0,32));
 float x=MultiplyMaskedFactors(quiet,factors,1,1);CHECK(x!=x);
 x=MultiplyMaskedFactors(infinity,factors,1,1);CHECK(x==infinity);
 float exceptional[3]={0.0f,quiet,infinity};
 CHECK(MultiplyMaskedFactors(1.0f,exceptional,7,3)==0.0f);
 CHECK(MultiplyMaskedFactors(-0.0f,exceptional+1,3,2)==0.0f);
 x=MultiplyMaskedFactors(1.0f,exceptional+1,1,1);CHECK(x!=x);
 CHECK(cases==864);std::puts("00c0acc0_TEST PASS");return 0;
}
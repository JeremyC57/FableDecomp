// Complete selected production source, shared class headers, no fixture class models.
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_ACCESSORS
#define FABLE_RETAIL_CFLOATECHO_TOKEN
#define FABLE_RETAIL_CFLOATPITCH_PROCESS
#define FABLE_RETAIL_REVERBMATH_ORIGINAL_DS_FTOI_PASS26
// Original GameCODA2.10 CodeView and native CreateComponents calls prove class/API and all names.
// Leaf: ?Create@CFloatPitch@@QAEXIAAPAXPAHH@Z
// Flags: (none)
// Retail-asm: 00C76DFB fistp dword ptr [esp+8]; original ds_ftoi uses bare dword conversion, shared LIBPK_FTOL portable twin.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATPITCH_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_CREATE
#define FABLE_RETAIL_CFLOATCYCLICBUFFER_CLEAR
#include "engine/CFloatPitch.h"
#include "engine/ReverbMath.h"
#pragma intrinsic(memset)
void CFloatPitch::Create(unsigned int uiSamplesPerSecond,void*& pvStaticData,int* piSizeOfStaticData,int iFaderToken)
{
    unsigned int iNumSamples=ds_ftoi(uiSamplesPerSecond*0.5f);
    unsigned int uiBufferLength=(iNumSamples+3)&~3;
    if (pvStaticData) {
        m_Buffer.Create((float*)pvStaticData,uiBufferLength);
        m_Buffer.Clear();
        m_iMyFaderToken=iFaderToken;
        m_fControlPoint=0;
        pvStaticData=(float*)pvStaticData+uiBufferLength;
    }
    *piSizeOfStaticData+=uiBufferLength*sizeof(float);
}

#include <cstdio>
#include <cstdlib>
#include <cstring>
CReverbDspProcessor::~CReverbDspProcessor() {}
CFloatPitch::~CFloatPitch() {}
static int failed=0;
#define CHECK(x) do {if(!(x)){++failed;std::printf("FAIL %d\n",__LINE__);}}while(0)
// Copy a trivially copyable shared buffer subobject to inspect its public API.
static CFloatCyclicBuffer Buffer(const CFloatPitch& effect) {
 CFloatCyclicBuffer copy;
 std::memcpy(&copy,(const unsigned char*)&effect+sizeof(CReverbDspProcessor),sizeof(copy));
 return copy;
}
int main() {
 struct Case {unsigned rate;unsigned samples;};
 // Literal expectations include the source helper's nearest-even half-step ties.
 const Case cases[]={{1,0},{2,0},{4,4},{6,4},{8,4},{10,4},{12,8},{16,8},{100,52},{44100,22052},{48000,24000}};
 for(unsigned n=0;n<sizeof(cases)/sizeof(cases[0]);++n) {
  CFloatPitch effect;
  // First establish valid internal state, so the dry-run unchanged check reads initialized object bytes.
  float* memory=(float*)std::malloc((cases[n].samples+10)*sizeof(float));CHECK(memory!=0);if(!memory)return 1;
  for(unsigned i=0;i<cases[n].samples+10;++i)memory[i]=123.25f;
  void* cursor=memory+3;int total=17;
  effect.Create(cases[n].rate,cursor,&total,37);
  CHECK(cursor==memory+3+cases[n].samples);CHECK(total==17+(int)cases[n].samples*4);
  CFloatCyclicBuffer copy=Buffer(effect);
  CHECK(copy.m_pBase==memory+3&&copy.GetLength()==cases[n].samples&&copy.GetWriteCursor()==0);
  CHECK(effect.Token()==37);
  for(unsigned i=0;i<cases[n].samples;++i)CHECK(memory[3+i]==0.0f);
  CHECK(memory[0]==123.25f&&memory[2]==123.25f&&memory[3+cases[n].samples]==123.25f);
  unsigned char before[sizeof(effect)];std::memcpy(before,&effect,sizeof(effect));
  cursor=0;total=23;effect.Create(cases[n].rate,cursor,&total,-2);
  CHECK(cursor==0&&total==23+(int)cases[n].samples*4);CHECK(std::memcmp(before,&effect,sizeof(effect))==0);
  cursor=memory+3;total=0;effect.Create(cases[n].rate,cursor,&total,-11);CHECK(effect.Token()==-11);CHECK(Buffer(effect).GetWriteCursor()==0);
  float control;std::memcpy(&control,(const unsigned char*)&effect+sizeof(CReverbDspProcessor)+sizeof(CFloatCyclicBuffer),sizeof(control));CHECK(control==0.0f);

  std::free(memory);
 }
 std::puts(failed?"00c76dd0_TEST FAIL":"00c76dd0_TEST PASS");return failed?1:0;
}
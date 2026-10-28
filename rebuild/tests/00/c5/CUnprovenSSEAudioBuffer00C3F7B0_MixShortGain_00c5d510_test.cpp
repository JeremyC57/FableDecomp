#include <stdio.h>
#ifndef _MSC_VER
#define __cdecl
#endif
class CUnprovenLibraryObject_012B5868{public:virtual ~CUnprovenLibraryObject_012B5868();};
class CUnprovenAudioBuffer_00C591F0:public CUnprovenLibraryObject_012B5868{public:int* Buffers[6];unsigned int OutputShift,ChannelCount;unsigned char UnalignedStorage[6*512+16];virtual ~CUnprovenAudioBuffer_00C591F0();virtual void Reset();virtual void Clear();virtual void MixShortGain(const short*,int*,unsigned short);virtual void MixIntGain(const int*,int*,unsigned short);virtual void MixInt(const int*,int*);virtual void MixFloatGain(const float*,int*,float);virtual void MixFloatQ15Gain(const float*,int*,unsigned short);virtual void MixFloat(const float*,int*);virtual void Route(unsigned int,const void*);virtual void Output(short*,unsigned int);void RouteFloat4(const float*);void RouteInt4(const int*);};
class CUnprovenFilteredAudioBuffer_00C3EFA0:public CUnprovenAudioBuffer_00C591F0{public:virtual ~CUnprovenFilteredAudioBuffer_00C3EFA0();};
class CUnprovenMMXAudioBuffer_00C3F660:public CUnprovenFilteredAudioBuffer_00C3EFA0{public:virtual ~CUnprovenMMXAudioBuffer_00C3F660();virtual void Clear();virtual void MixShortGain(const short*,int*,unsigned short);};
class CUnprovenSSEAudioBuffer_00C3F7B0:public CUnprovenFilteredAudioBuffer_00C3EFA0{public:virtual ~CUnprovenSSEAudioBuffer_00C3F7B0();virtual void MixShortGain(const short*,int*,unsigned short);};
#ifdef _MSC_VER
typedef char ExactBufferSize[sizeof(CUnprovenAudioBuffer_00C591F0)==0xC34?1:-1];
#endif
extern "C" void __cdecl CUnprovenMixShortBlocks_00C5D150(const short*,unsigned int,int*,unsigned short);
void CUnprovenSSEAudioBuffer_00C3F7B0::MixShortGain(const short* source,int* destination,unsigned short gain){CUnprovenMixShortBlocks_00C5D150(source,16,destination,gain);}
CUnprovenLibraryObject_012B5868::~CUnprovenLibraryObject_012B5868(){}CUnprovenAudioBuffer_00C591F0::~CUnprovenAudioBuffer_00C591F0(){}CUnprovenFilteredAudioBuffer_00C3EFA0::~CUnprovenFilteredAudioBuffer_00C3EFA0(){}CUnprovenMMXAudioBuffer_00C3F660::~CUnprovenMMXAudioBuffer_00C3F660(){}CUnprovenSSEAudioBuffer_00C3F7B0::~CUnprovenSSEAudioBuffer_00C3F7B0(){}
void CUnprovenAudioBuffer_00C591F0::Reset(){}
void CUnprovenAudioBuffer_00C591F0::Clear(){}
void CUnprovenAudioBuffer_00C591F0::MixShortGain(const short*,int*,unsigned short){}
void CUnprovenAudioBuffer_00C591F0::MixIntGain(const int*,int*,unsigned short){}
void CUnprovenAudioBuffer_00C591F0::MixInt(const int*,int*){}
void CUnprovenAudioBuffer_00C591F0::MixFloatGain(const float*,int*,float){}
void CUnprovenAudioBuffer_00C591F0::MixFloatQ15Gain(const float*,int*,unsigned short){}
void CUnprovenAudioBuffer_00C591F0::MixFloat(const float*,int*){}
void CUnprovenAudioBuffer_00C591F0::Route(unsigned int,const void*){}
void CUnprovenAudioBuffer_00C591F0::Output(short*,unsigned int){}
void CUnprovenMMXAudioBuffer_00C3F660::Clear(){}
void CUnprovenMMXAudioBuffer_00C3F660::MixShortGain(const short*,int*,unsigned short){}
static const short* expectedSource;static int* expectedDestination;static unsigned short expectedGain;static int calls,bad;extern "C" void __cdecl CUnprovenMixShortBlocks_00C5D150(const short* s,unsigned int count,int* d,unsigned short g){if(s!=expectedSource||d!=expectedDestination||count!=16||g!=expectedGain)bad=1;++calls;d[0]+=7;}int main(){CUnprovenSSEAudioBuffer_00C3F7B0 owner;short input[128]={0};int output[128]={0};expectedSource=input;expectedDestination=output;unsigned short gains[]={0,1,0x7FFF,0x8000,0xFFFF};CUnprovenAudioBuffer_00C591F0* base=&owner;for(int i=0;i<5;++i){expectedGain=gains[i];base->MixShortGain(input,output,gains[i]);}if(bad||calls!=5||output[0]!=35||output[1]){puts("00c5d510_TEST FAIL");return 1;}puts("00c5d510_TEST PASS");return 0;}
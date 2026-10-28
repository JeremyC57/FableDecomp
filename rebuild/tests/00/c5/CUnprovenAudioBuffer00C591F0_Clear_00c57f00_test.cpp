#include <stdio.h>
#include <string.h>
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

#pragma intrinsic(memset)
void CUnprovenAudioBuffer_00C591F0::Clear(){memset(Buffers[0],0,ChannelCount<<9);}
CUnprovenLibraryObject_012B5868::~CUnprovenLibraryObject_012B5868(){}CUnprovenAudioBuffer_00C591F0::~CUnprovenAudioBuffer_00C591F0(){}CUnprovenFilteredAudioBuffer_00C3EFA0::~CUnprovenFilteredAudioBuffer_00C3EFA0(){}CUnprovenMMXAudioBuffer_00C3F660::~CUnprovenMMXAudioBuffer_00C3F660(){}CUnprovenSSEAudioBuffer_00C3F7B0::~CUnprovenSSEAudioBuffer_00C3F7B0(){}
void CUnprovenAudioBuffer_00C591F0::Reset(){}
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
void CUnprovenSSEAudioBuffer_00C3F7B0::MixShortGain(const short*,int*,unsigned short){}
int main(){CUnprovenAudioBuffer_00C591F0 owner;unsigned char bytes[6*512+16];for(unsigned int n=0;n<=6;++n){memset(bytes,0xA5,sizeof(bytes));owner.Buffers[0]=(int*)(bytes+8);owner.ChannelCount=n;owner.OutputShift=3;CUnprovenAudioBuffer_00C591F0* base=&owner;base->Clear();for(unsigned int i=0;i<sizeof(bytes);++i)if(bytes[i]!=(i>=8&&i<8+n*512?0:0xA5))goto fail;if(owner.ChannelCount!=n||owner.OutputShift!=3||owner.Buffers[0]!=(int*)(bytes+8))goto fail;}puts("00c57f00_TEST PASS");return 0;fail:puts("00c57f00_TEST FAIL");return 1;}
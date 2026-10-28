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
void CUnprovenAudioBuffer_00C591F0::RouteInt4(const int* source){if(ChannelCount==1){MixIntGain(source,Buffers[0],8192);MixIntGain(source+128,Buffers[0],8192);MixIntGain(source+256,Buffers[0],8192);MixIntGain(source+384,Buffers[0],8192);}else if(ChannelCount==2){MixIntGain(source,Buffers[0],16384);MixIntGain(source+128,Buffers[1],16384);MixIntGain(source+256,Buffers[0],16384);MixIntGain(source+384,Buffers[1],16384);}else{MixInt(source,Buffers[0]);MixInt(source+128,Buffers[1]);MixInt(source+256,Buffers[2]);MixInt(source+384,Buffers[3]);}}
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
void CUnprovenSSEAudioBuffer_00C3F7B0::MixShortGain(const short*,int*,unsigned short){}
static const int* inputBase;static int* expectedInitial[4];static int* expectedChanged[4];static unsigned int initialChannels;static int count,bad;class Fixture:public CUnprovenAudioBuffer_00C591F0{public:void Check(const int* source,int* dest,float gain,bool scaled){unsigned int d=initialChannels==1?0:initialChannels==2?(count&1):count;if(source!=inputBase+count*128||dest!=(count?expectedChanged[d]:expectedInitial[d])||scaled!=(initialChannels==1||initialChannels==2)||gain!=(initialChannels==1?8192.0f:initialChannels==2?16384.0f:0.0f))bad=1;++count;if(count==1){for(int i=0;i<4;++i)Buffers[i]=expectedChanged[i];ChannelCount=99;}}virtual void MixIntGain(const int* s,int* d,unsigned short g){Check(s,d,(float)g,true);}virtual void MixInt(const int* s,int* d){Check(s,d,0,false);}};int main(){Fixture owner;int input[512];int destinations[8][128];inputBase=input;unsigned int choices[]={0,1,2,4,6};for(int scenario=0;scenario<5;++scenario){for(int i=0;i<4;++i){expectedInitial[i]=destinations[i];expectedChanged[i]=destinations[i+4];owner.Buffers[i]=expectedInitial[i];}initialChannels=choices[scenario];owner.ChannelCount=initialChannels;count=bad=0;owner.RouteInt4(input);if(count!=4||bad||owner.ChannelCount!=99){puts("00c58630_TEST FAIL");return 1;}}puts("00c58630_TEST PASS");return 0;}
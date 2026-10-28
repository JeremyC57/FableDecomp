// CVtable_012bede0::SetupVoice(...) -- Quantize and clamp mixer gains, derive the impulse delay and prepare one voice.
// Leaf: ?SetupVoice@CVtable_012bede0@@QAEXPAUSFilteredVoice@@MPBUSUnprovenImpulse52@@@Z
// Retail-asm: 0x00c70f80 uses three bare fistp dword conversions after float stores; ordinary C++ casts call __ftol2 and truncate rather than obey the current x87 rounding mode. Only that original conversion is preserved, C++ reconstructs all surrounding arithmetic/branches.
// Identity: descriptive. Own common base constructor00c6e6a0 initializes polymorphic lock at4, Win32 section8,volume32/crossfade36. Full destructor00c6e630 resets common object root012b5868 at00c6e64a; lock deleting destructor00c5d5b0 has no further base. Sibling derived constructor00c6d9d0 starts its own six gain/step pairs at40, proving base40 rather than168. Concrete constructors00c70120/00c70b00 round the scratch raw address168 upward to16-byte alignment and store pointer440. Derived scratch raw combines64short history+128short input+8short alignment slack at40; pointer440. Own block copy reads/writes128bytes of short history. Typed two voice pairs start444; constructor aligned coefficient pointers and SetupVoice stores prove voice fields, sizes292/676, and short coefficient data. Class/field names for this library remain descriptive; no library PDB identity claimed. Global0143bc2c installed at00c4d3af after allocating1d8 and calling ctor00c3e520. True ctor chain00c3e520->00c4d000 initializes base012b5b80 and four20byte pool objects at4/18/2c/40; pool ctor00c3c8a0 and full dtor00c3caa0 show true commonroot012b5868. Full base dtor00c3e2a0 resets root at00c3e308; master dtor00c3f380 chains base. Own ctor zeros TapTable1b8 and outputChannels1bc; inactive derived fields54..1b7 and1c0..1d7 explicitly unproven. Descriptive master name retained from existing library models, no original PDB identity claimed. SetupVoice converts gains with original bare FISTP32, clamps unsigned results32767, splits impulse Delay word50 into quotient and low7 bits, prepares short coefficients through 00c7ba00. Original helper descriptive catalog spelling retained.
// Layout: true root/mixer and master ancestry, typed short coefficients and impulse25shorts+ushortDelay.
#include <stddef.h>
class CUnprovenLibraryObject_012B5868{public:virtual ~CUnprovenLibraryObject_012B5868(){}};
struct CRITICAL_SECTION{void* DebugInfo;long LockCount,RecursionCount;void* OwningThread;void* LockSemaphore;unsigned long SpinCount;};
class CUnprovenLock_012B8FC8{public:virtual ~CUnprovenLock_012B8FC8(){}CRITICAL_SECTION Section;};
class CVtable_012beda4:public CUnprovenLibraryObject_012B5868{public:virtual ~CVtable_012beda4(){}virtual void Clear(){}virtual void Process(const short*,long* const*){}virtual void SetParameters(float,const float*,bool)=0;virtual void Update(float,float,float,float,const float*,bool,bool){}CUnprovenLock_012B8FC8 Lock;float m_volume;bool m_crossfade;};
struct SUnprovenHistoryScratch{short Raw[200];short* Aligned;};
struct SResampleVoice{short Raw[136];short* Coefficients;unsigned long SampleDelay;long m_offset;unsigned long m_increment;bool m_muted,m_swapOutputs;unsigned short m_pan;};
struct SFilteredVoice{short Raw[328];short* Coefficients;unsigned long SampleDelay;long m_offset;unsigned long m_increment;bool m_muted,m_swapOutputs;unsigned short m_pan;};
template<class Voice>struct SUnprovenVoicePair{Voice A,B;};
#ifdef _MSC_VER
typedef char MixerBaseSize[sizeof(CVtable_012beda4)==40?1:-1];typedef char VoiceSize[sizeof(SResampleVoice)==0x124?1:-1];typedef char FilteredVoiceSize[sizeof(SFilteredVoice)==0x2a4?1:-1];
#endif
class CUnprovenPool_012B586C:public CUnprovenLibraryObject_012B5868{public:virtual ~CUnprovenPool_012B586C(){}void* Allocation;void* FreeHead;long Capacity;unsigned long Count;};
class CUnprovenAudioMasterBase_012B5B80:public CUnprovenLibraryObject_012B5868{public:virtual ~CUnprovenAudioMasterBase_012B5B80(){}CUnprovenPool_012B586C Pools[4];};
class CAudioMaster:public CUnprovenAudioMasterBase_012B5B80{public:unsigned char UnprovenFields54[356];void* TapTable;unsigned long m_outputChannels;unsigned char UnprovenTail1C0[24];};extern CAudioMaster* g_pAudioMaster;
#ifdef _MSC_VER
typedef char MasterBaseSize[sizeof(CUnprovenAudioMasterBase_012B5B80)==84?1:-1];typedef char MasterChannelsOffset[offsetof(CAudioMaster,m_outputChannels)==0x1bc?1:-1];typedef char MasterSize[sizeof(CAudioMaster)==0x1d8?1:-1];
#endif
struct SUnprovenImpulse52{short Values[25];unsigned short Delay;};
#ifndef _MSC_VER
#define __forceinline inline
#include <math.h>
#endif
static __forceinline unsigned long RoundGain(float value){
#ifdef _MSC_VER
long rounded;
__asm {fld value}
__asm {fistp rounded}
return (unsigned long)rounded;
#else
double whole=floor((double)value);double fraction=(double)value-whole;if(fraction>0.5||(fraction==0.5&&((long)whole%2)!=0))whole+=1.0;return (unsigned long)(long)whole;
#endif
}
class CVtable_012bede0:public CVtable_012beda4{public:SUnprovenHistoryScratch Scratch;SUnprovenVoicePair<SFilteredVoice> Block0,Block1;SUnprovenVoicePair<SFilteredVoice>* PFront;SUnprovenVoicePair<SFilteredVoice>* PBack;float m_gains[2];float m_front,m_rear;const void* m_taps[2];virtual void SetParameters(float,const float*,bool);void UpdateVoices(bool);void SetupVoice(SFilteredVoice*,float,const SUnprovenImpulse52*);};
void Audio_RenderMultichannelData_Alt(const SUnprovenImpulse52*,unsigned long,short*,unsigned long);
void CVtable_012bede0::SetupVoice(SFilteredVoice* voice,float gain,const SUnprovenImpulse52* taps){gain*=m_volume;if(g_pAudioMaster->m_outputChannels>=4){gain*=32767.0f;unsigned long front=RoundGain(gain*m_front);if(front>32767)front=32767;unsigned long rear=RoundGain(gain*m_rear);if(rear>32767)rear=32767;voice->SampleDelay=taps->Delay;voice->m_increment=voice->SampleDelay&7;voice->m_offset=voice->SampleDelay-voice->m_increment;if(voice->m_swapOutputs){voice->m_muted=rear==0;if(!voice->m_muted)voice->m_pan=(unsigned short)(front*32767/rear);Audio_RenderMultichannelData_Alt(taps,voice->m_increment,voice->Coefficients,rear);}else{voice->m_muted=front==0;if(!voice->m_muted)voice->m_pan=(unsigned short)(rear*32767/front);Audio_RenderMultichannelData_Alt(taps,voice->m_increment,voice->Coefficients,front);}}else{gain*=32767.0f;unsigned long mono=RoundGain(gain);if(mono>32767)mono=32767;voice->SampleDelay=taps->Delay;voice->m_increment=voice->SampleDelay&7;voice->m_offset=voice->SampleDelay-voice->m_increment;voice->m_muted=mono==0;Audio_RenderMultichannelData_Alt(taps,voice->m_increment,voice->Coefficients,mono);}}
// CVtable_012bede0::Process(const short* source,long* const* outputs) -- Mix the current or transitioning voices and retain audio history.
// Leaf: ?Process@CVtable_012bede0@@UAEXPBFPBQAJ@Z
// Identity: descriptive. Own common base constructor00c6e6a0 initializes polymorphic lock at4, Win32 section8,volume32/crossfade36. Full destructor00c6e630 resets common object root012b5868 at00c6e64a; lock deleting destructor00c5d5b0 has no further base. Sibling derived constructor00c6d9d0 starts its own six gain/step pairs at40, proving base40 rather than168. Concrete constructors00c70120/00c70b00 round the scratch raw address168 upward to16-byte alignment and store pointer440. Derived scratch raw combines64short history+128short input+8short alignment slack at40; pointer440. Own block copy reads/writes128bytes of short history. Typed two voice pairs start444; constructor aligned coefficient pointers and SetupVoice stores prove voice fields, sizes292/676, and short coefficient data. Class/field names for this library remain descriptive; no library PDB identity claimed. Own Process slot2 copies128 short samples, locks section, fades out old voices/swaps/fades in new ones on crossfade, otherwise mixes current voices; fadeOut00c70cb0/fadeIn00c70c10/steady00c70d60/copy00c70a60. Copy helper catalog CPreTransposedBoneMatrix::Set is a propagated placeholder: own caller passes adjacent short audio history blocks and helper copies exactly128bytes. Distinct descriptive copy names prevent binding one symbol to two targets; no original copy identifier claimed.
// Layout: actual root/base40, derived short history/scratch member and two typed voice pairs; complete active prefix, no artificial nonpolymorphic base.
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
#include <string.h>
#ifdef _MSC_VER
#pragma intrinsic(memcpy)
#define IMPORT __declspec(dllimport)
#else
#define IMPORT
#define __stdcall
#endif
extern "C" IMPORT void __stdcall EnterCriticalSection(CRITICAL_SECTION*);extern "C" IMPORT void __stdcall LeaveCriticalSection(CRITICAL_SECTION*);
class CVtable_012bede0:public CVtable_012beda4{public:SUnprovenHistoryScratch Scratch;SUnprovenVoicePair<SFilteredVoice> Block0,Block1;SUnprovenVoicePair<SFilteredVoice>* PFront;SUnprovenVoicePair<SFilteredVoice>* PBack;float m_gains[2];float m_front,m_rear;const void* m_taps[2];virtual void SetParameters(float,const float*,bool);void UpdateVoices(bool);virtual void Process(const short*,long* const*);};
void MixFilteredVoiceFadeOut(const short*,const SFilteredVoice*,long*,long*);void MixFilteredVoiceFadeIn(const short*,const SFilteredVoice*,long*,long*);void MixFilteredVoice(const short*,unsigned long,const SFilteredVoice*,long*,long*);void CopyHistory128SSE(short*,const short*);
void CVtable_012bede0::Process(const short* source,long* const* outputs){memcpy(Scratch.Aligned,source,256);EnterCriticalSection(&Lock.Section);if(m_crossfade){m_crossfade=false;MixFilteredVoiceFadeOut(Scratch.Aligned,&PFront->A,outputs[0],outputs[2]);MixFilteredVoiceFadeOut(Scratch.Aligned,&PFront->B,outputs[1],outputs[3]);SUnprovenVoicePair<SFilteredVoice>* previous=PFront;PFront=PBack;PBack=previous;MixFilteredVoiceFadeIn(Scratch.Aligned,&PFront->A,outputs[0],outputs[2]);MixFilteredVoiceFadeIn(Scratch.Aligned,&PFront->B,outputs[1],outputs[3]);}else{MixFilteredVoice(Scratch.Aligned,16,&PFront->A,outputs[0],outputs[2]);MixFilteredVoice(Scratch.Aligned,16,&PFront->B,outputs[1],outputs[3]);}LeaveCriticalSection(&Lock.Section);CopyHistory128SSE(Scratch.Aligned-64,Scratch.Aligned+64);}
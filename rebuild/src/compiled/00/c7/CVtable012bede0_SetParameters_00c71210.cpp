// CVtable_012bede0::SetParameters(float volume,const float* gains,bool crossfade) -- Update changed mixer gains and rebuild the voices.
// Leaf: ?SetParameters@CVtable_012bede0@@UAEXMPBM_N@Z
// Identity: descriptive. Own common base constructor00c6e6a0 initializes polymorphic lock at4, Win32 section8,volume32/crossfade36. Full destructor00c6e630 resets common object root012b5868 at00c6e64a; lock deleting destructor00c5d5b0 has no further base. Sibling derived constructor00c6d9d0 starts its own six gain/step pairs at40, proving base40 rather than168. Concrete constructors00c70120/00c70b00 round the scratch raw address168 upward to16-byte alignment and store pointer440. Derived scratch raw combines64short history+128short input+8short alignment slack at40; pointer440. Own block copy reads/writes128bytes of short history. Typed two voice pairs start444; constructor aligned coefficient pointers and SetupVoice stores prove voice fields, sizes292/676, and short coefficient data. Class/field names for this library remain descriptive; no library PDB identity claimed. Target compares volume/gains, calls UpdateVoices00c71120 only when one changes.
// Layout: true root/mixer base40 and lock member; derived history/scratch member and typed voice records, float gains; no active engine header.
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
class CVtable_012bede0:public CVtable_012beda4{public:SUnprovenHistoryScratch Scratch;SUnprovenVoicePair<SFilteredVoice> Block0,Block1;SUnprovenVoicePair<SFilteredVoice>* PFront;SUnprovenVoicePair<SFilteredVoice>* PBack;float m_gains[2];float m_front,m_rear;const void* m_taps[2];virtual void SetParameters(float,const float*,bool);void UpdateVoices(bool);};
void CVtable_012bede0::SetParameters(float volume,const float* gains,bool crossfade){if(m_volume!=volume||m_gains[0]!=gains[0]||m_gains[1]!=gains[1]){m_volume=volume;m_gains[0]=gains[0];m_gains[1]=gains[1];UpdateVoices(crossfade);}}
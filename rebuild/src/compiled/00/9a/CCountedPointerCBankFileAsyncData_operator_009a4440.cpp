// CCountedPointer<CBankFileAsyncData>::operator=(const CCountedPointer&) -- share the other pointer: ShareData(Data, Info), return *this
// Leaf: ??4?$CCountedPointer@VCBankFileAsyncData@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A4440, 72 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Set_009a4440 (stand-in name).
// T = CBankFileAsyncData, proven by: NParticleEngine::CEmitterData BeginLoading 0x00AF1360 binds CCountedPointer<CBankFileAsyncData>::operator=; the BeginLoading callers (CPhysicsMeshInfo, C3DAnimationInfo, CEmitterData) assign their FileData member (Ego_r: CCountedPointer<CBankFileAsyncData> in C3DMeshLODInfo / CAnimationData / CEmitterData); it sits next to ~CCountedPointer<CBankFileAsyncData> 0x009A4400; FableWin.pdb ??4 name.
// Member: the in-class operator= of engine/CCountedPointer.h (loads Data / Info, then the ShareData body), emitted as its own COMDAT.
// Model: opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData zeroes Data first; its stores die under ShareData's own).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CBankFileAsyncData;
template CCountedPointer<CBankFileAsyncData>& CCountedPointer<CBankFileAsyncData>::operator=(const CCountedPointer<CBankFileAsyncData>&);

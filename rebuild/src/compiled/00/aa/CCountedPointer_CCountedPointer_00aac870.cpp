// CCountedPointer<CSoundEmitterLH::CActiveSound>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCActiveSound@CSoundEmitterLH@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAC870, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T (agentNF31, 2026-11-12): CSoundEmitterLH::CActiveSound. Its only retail callers, 0x00AABF30 in PlaySoundEvent 0x00AABD50 and
//   0x00AAC1A7 in PlayAtmos, release the CCountedPointer<CActiveSound> temporary built by 0x00AAC820, and the landed
//   PlaySoundEvent row binds this exact symbol (reloc_bindings). No engine header models CSoundEmitterLH, so the nesting is
//   forward-declared here.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
class CSoundEmitterLH { public: class CActiveSound; };
template CCountedPointer<CSoundEmitterLH::CActiveSound>::~CCountedPointer();

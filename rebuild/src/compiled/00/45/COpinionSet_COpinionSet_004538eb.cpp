// COpinionSet::COpinionSet() -- default constructor: the opinion values start as one zero per opinion type (five floats)
// Leaf: ??0COpinionSet@@QAE@XZ
// Inline-budget: FABLE_RETAIL_CARRAY_SIZE_CTOR_FORCEINLINE (engine/CArray.h per-TU opt-in) -- retail expands CArray<float>(n) in this /Os TU (the 0.0f and allocator temporaries at [ebp-8] / [ebp-1] of this frame) and calls only the vector fill constructor; without it VC7.1 calls the wrapper (`push 5 ; call`). FABLE_RETAIL_STLP_VECTOR_FILL_OUTOFLINE keeps that constructor a call, as retail.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004538EB, 33 bytes, thiscall, plain `ret` (returns this); a size-optimised TU with a frame
//   pointer (`push ebp ; mov ebp,esp ; push ecx ; push ecx`). Callee: the STLport vector<float>(size_type n, const float& value,
//   const allocator<float>& a) constructor 0x004516E7 (thiscall, `ret 0Ch`: n = 5, the 0.0f temporary at [ebp-8], the empty
//   allocator temporary at [ebp-1]).
// Names (agentFAM10, 2026-11-12; was _global::sub_004538eb with the module CApprenticeSpeedTest and the stand-in
//   CApprenticeSpeedTest_OnPersist(CApprenticeSpeedTest*) over a free __stdcall Persist_Serialize(int, float*, unsigned char*)):
//   Ego_r.pdb / FableWin.pdb ??0COpinionSet@@QAE@XZ; the landed COpinionSourceDef ctor 0x00456FFE already binds this address to that
//   symbol (reloc_bindings), and its other callers (TransferIn 0x00678760, COpinionReactionManager GetOpinions 0x00700AE0 /
//   GetAttitudeOrReactionType 0x00701010 / GetReactionMatchStrength 0x00701590) build local opinion sets; Ego_r.pdb COpinionSet is
//   one member, CArray<float> OpinionValues (retail 12 bytes), and the landed CTCOpinionSource::OnCreate 0x00771660 expands the same
//   constructor (`COpinionSet() : OpinionValues(5) { }`, the five-float buffer).
// Callee name: the vector fill constructor is STLport 4.6's vector(size_type, const _Tp&, const allocator_type&) =
//   ??0?$vector@MV?$allocator@M@std@@@std@@QAE@IABMABV?$allocator@M@1@@Z (CArray<float>(n) forwards n and T(), engine/CArray.h);
//   the landed row at 0x004516E7 is still labelled CTCEnemy::SetFaction and binders spell it Persist_Serialize / GetLookDirHelper
//   (NAME_FIXES.tsv).
#pragma optimize("s",on)
#define FABLE_RETAIL_CARRAY_SIZE_CTOR_FORCEINLINE
#define FABLE_RETAIL_STLP_VECTOR_FILL_OUTOFLINE

#include "engine/CArray.h"

class COpinionSet {
public:
    COpinionSet();
    CArray<float> OpinionValues;                                         // +0x00 (Ego_r.pdb; retail STLport vector 12 bytes)
};

COpinionSet::COpinionSet() : OpinionValues(5)
{
}

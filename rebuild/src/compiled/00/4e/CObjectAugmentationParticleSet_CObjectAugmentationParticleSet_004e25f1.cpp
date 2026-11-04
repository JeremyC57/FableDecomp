// CObjectAugmentationParticleSet::~CObjectAugmentationParticleSet() -- release the EndPoint then the StartPoint arrays
// Leaf: ??1CObjectAugmentationParticleSet@@QAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E25F1, 33 bytes, plain `ret` (__thiscall).
// Identity: Ego_r.pdb ??1CObjectAugmentationParticleSet@@QAE@XZ (non-virtual) and layout {float PeriodSecs,
// long ParticleEffect, ParticleEffectToBlendTo, AttackParticleEffect, AttackParticleEffectToBlendTo,
// CArray<CDefString> StartPoint, CArray<CDefString> EndPoint}; with retail's 12-byte STLport vectors the arrays sit at
// +0x14 and +0x20, the two blocks freed here in reverse member order. Callers include the
// CObjectAugmentationParticleSet transfer helper 0x004E9550 (its traits value) and the
// map<EObjectAugmentationType, CObjectAugmentationParticleSet> serialisers (0x004EC8B8, 0x004EC996..). CDefString
// (Ego_r.pdb: long TablePos) needs no destruction, so each array's destructor reduces to the block free. The old
// CBufferPair_004e25f1 name was descriptive.
// Callees: free @ 0x00BFEA14 (the STLport malloc allocator's deallocate).
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

class CDefString {
public:
    long TablePos;                                       // +0x00
};

#include "engine/CArray.h"


class CObjectAugmentationParticleSet {
public:
    float              PeriodSecs;                       // +0x00
    long               ParticleEffect;                   // +0x04
    long               ParticleEffectToBlendTo;          // +0x08
    long               AttackParticleEffect;             // +0x0C
    long               AttackParticleEffectToBlendTo;    // +0x10
    CArray<CDefString> StartPoint;                       // +0x14
    CArray<CDefString> EndPoint;                         // +0x20
    ~CObjectAugmentationParticleSet();
};

CObjectAugmentationParticleSet::~CObjectAugmentationParticleSet()
{
}

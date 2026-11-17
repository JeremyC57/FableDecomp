// CObjectAugmentationParticleSet::operator=(const CObjectAugmentationParticleSet& other) -- memberwise assignment: the period and the four particle-effect indices, then the start and end point arrays through vector<CDefString>::operator=; returns *this
// Leaf: ??4CObjectAugmentationParticleSet@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E4D3B, length 67 bytes (`ret 4`, eax = this). Callee: vector<CDefString>::operator=
// @ 0x0046078C (manifest "CActiveFile::operator=", propagated), twice; CArray<CDefString>'s implicit operator= is that call.
// Identity (NF48, round 7): the assignment operator, not the copy constructor the row was landed as. The body stores the five scalars
// and then calls the vector ASSIGNMENT 0x0046078C on both arrays without first constructing them (no zero stores), which only an
// assignment does; the landed caller CWeaponDef::operator= 0x004E493E binds this address as ??4CObjectAugmentationParticleSet@@QAEAAV0@ABV0@@Z;
// the real copy constructor is the twin 0x004EC46D (bound as ??0CObjectAugmentationParticleSet@@QAE@ABV0@@Z by _M_create_node 0x004EC441).
// engine/CObjectAugmentationParticleSet.h declares both.
// Layout from the PDB (engine/CObjectAugmentationParticleSet.h): PeriodSecs +0x00 (float), ParticleEffect +0x04, ParticleEffectToBlendTo +0x08,
// AttackParticleEffect +0x0C, AttackParticleEffectToBlendTo +0x10, StartPoint +0x14 and EndPoint +0x20 (CArray<CDefString>,
// 12 bytes in retail). Shape: a size-optimised module (#pragma optimize("s", on)): every scalar member goes through eax in
// declaration order; the last array's source address is formed with `add edi,20h` (the argument register is dead afterwards).
#pragma optimize("s", on)
// NF48: the arrays are the shared CArray<CDefString> (a public STLport 4.6.2 std::vector<CDefString>, was the local stand-in
// CArrayDefString); FABLE_RETAIL_STLP_VECTOR_ASSIGN declares vector<CDefString>::operator= (0x0046078C) out of line.
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CObjectAugmentationParticleSet.h"

CObjectAugmentationParticleSet& CObjectAugmentationParticleSet::operator=(const CObjectAugmentationParticleSet& other)
{
    PeriodSecs = other.PeriodSecs;
    ParticleEffect = other.ParticleEffect;
    ParticleEffectToBlendTo = other.ParticleEffectToBlendTo;
    AttackParticleEffect = other.AttackParticleEffect;
    AttackParticleEffectToBlendTo = other.AttackParticleEffectToBlendTo;
    StartPoint = other.StartPoint;
    EndPoint = other.EndPoint;
    return *this;
}

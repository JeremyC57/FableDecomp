// CEnginePrimitiveManagerParticleGroup::CEnginePrimitiveManagerParticleGroup(CArray<CEngineScene*>&) -- particle-group primitive manager: CEnginePrimitiveManager(scenes), four state bytes cleared, then ReadZBuffer (+0x21) set
// Leaf: ??0CEnginePrimitiveManagerParticleGroup@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBBF40, length 43 bytes (+5 int3 padding).
// Callee: CEnginePrimitiveManager::CEnginePrimitiveManager @ 0x00BACDB0. Vtable 0x012A5BB8 = CEnginePrimitiveManagerParticleGroup
// (rtti-vtables.tsv).
// NF39 (2026-11-16): moved off its local CEngine* stand-in onto the shared engine/CEnginePrimitiveManagerParticleGroup.h (opt-in
// FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEGROUP_CTOR, which names 0x00BBBF40): Ego_r.pdb 0x00A71660 + FableWin.pdb 0x02EA4AD0
// ??0CEnginePrimitiveManagerParticleGroup@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z; row 0x00B4AC10 binds it so.
// Shape: speed TU; the four cleared bytes (+0x20, +0x22, +0x23, +0x24, one `xor al,al`) are initialisers scheduled
// BEFORE the vtable store; the +0x21 byte (ReadZBuffer, Ego_r name) stored after the vtable store is a body assignment.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEGROUP_CTOR
#include "engine/CArray.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEnginePrimitiveManagerParticleGroup.h"

CEnginePrimitiveManagerParticleGroup::CEnginePrimitiveManagerParticleGroup(CArray<CEngineScene*>& scenes)
    : CEnginePrimitiveManager(scenes)
    , ClipEffectToWeatherMask(false)
    , WriteZBuffer(false)
    , DisplayBoundingBox(false)
    , DisplayBoundingSphere(false)
{
    ReadZBuffer = true;
}

// NParticleEngine::CPSCSingleSprite::ClearAllPrimitives(CIEngine* pengine, CEnginePrimitiveHandle& system_group) -- when the system group handle is live, clears every sprite child primitive in PrimIDList from it through the engine
// Leaf: ?ClearAllPrimitives@CPSCSingleSprite@NParticleEngine@@UAEXPAVCIEngine@@AAVCEnginePrimitiveHandle@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B07570, length 93 bytes.
// Callees: CIEngine slot 30 (+0x78) ?ClearChildPrimitive@CIEngine@@UAEXAAVCEnginePrimitiveHandle@@K@Z (engine/CIEngine.h), called as
//   (system group handle, primitive id).
// Identity: slot 29 of NParticleEngine::CPSCSingleSprite's vtable 0x0129FB84 (rtti-vtables.tsv); Ego_r.pdb
//   ?ClearAllPrimitives@CPSCSingleSprite@NParticleEngine@@UAEXPAVCIEngine@@AAVCEnginePrimitiveHandle@@@Z; the CPSCBase default for
//   slot 29 (0x00AF2AC0) is an empty `ret 8`. Parameter and loop-index names from Ego_d.pdb (pengine, system_group, c).
// Re-port (Account B, 2026-11-20): the shared engine/CPSCSingleSprite.h / CIEngine.h / CEnginePrimitiveHandle.h replace the local
//   CEngineInterface (`RemovePrimitive` at slot 30), CPrimitiveGroupHandle {m_pLink, m_pGroup} and CArray models; the handle test is
//   the header's inline IsNull() (CIVCountedPointer Data at +4, the word the old model called m_pGroup).
// Layout: PrimIDList (CArray<long>) at retail +0x12c (Ego_r +0x138; the retail object is 0xc bytes smaller before it).
// Shape: /O2 loop (aligned head `lea ebx,[ebx]`), the engine argument loaded into ebx only after the entry test, the element count
//   re-read every iteration (the virtual call may change the array), unsigned index compare (`jb`).
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSINGLESPRITE_MEMBERS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DATA
#define FABLE_RETAIL_CIENGINE
#include "engine/CPSCSingleSprite.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CIEngine.h"
namespace NParticleEngine {
void CPSCSingleSprite::ClearAllPrimitives(CIEngine* pengine, CEnginePrimitiveHandle& system_group)
{
    if (!system_group.IsNull()) {
        for (unsigned long c = 0; c < PrimIDList.size(); ++c)
            pengine->ClearChildPrimitive(system_group, PrimIDList[c]);
    }
}
}

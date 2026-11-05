// CTCBase::GetThingManager() const -- the global thing manager (const overload)
// Leaf: ?GetThingManager@CTCBase@@IBEABVCThingManager@@XZ
// Naming wave 18: the global stand-in g_pThingManager (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686AF0, 6 bytes: mov eax,[0x013B8A1C] ; ret (this unused).
// Name: Ego_r.pdb lists CTCBase's accessors in the same order as this retail run (GetDefinitionManager 0x006869D0,
//   GetWorld 0x00686A70, PeekWorld 0x00686A80, then the GetThingManager pair, non-const before const, after Serialise),
//   and the identical twin at 0x00686AE0 is the other overload (no identical-COMDAT folding in this link). Callers are CTC*
//   component methods reaching CThingManager members (CTCOwnedEntity::OnSerialise -> RemapUIDForSerialisation).
//   The catalog label "GFGetPreMainMemoryUsed" was propagated from a byte-identical getter.
// Data: 0x013B8A1C is the CThingManager pointer (descriptive name; no PDB name).
class CThingManager;

class CThingManager;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#include "engine/CThing.h"

class CTCBase {
protected:
    const CThingManager& GetThingManager() const;
};

const CThingManager& CTCBase::GetThingManager() const
{
    return *CThing::GetPCurrentThingManager();
}

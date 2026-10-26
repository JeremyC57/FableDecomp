// CTCBase::GetThingManager() -- the global thing manager (non-const overload)
// Leaf: ?GetThingManager@CTCBase@@IAEAAVCThingManager@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686AE0, 6 bytes: mov eax,[0x013B8A1C] ; ret (this unused).
// Name: Ego_r.pdb lists CTCBase's accessors in the same order as this retail run (GetDefinitionManager 0x006869D0,
//   GetWorld 0x00686A70, PeekWorld 0x00686A80, then the GetThingManager pair, non-const before const, after Serialise),
//   and the identical twin at 0x00686AF0 is the other overload (no identical-COMDAT folding in this link). Callers are CTC*
//   component methods reaching CThingManager members (CTCOwnedEntity::OnSerialise -> RemapUIDForSerialisation).
//   The catalog label "GFGetPreMainMemoryUsed" was propagated from a byte-identical getter.
// Data: 0x013B8A1C is the CThingManager pointer (descriptive name; no PDB name).
class CThingManager;

extern CThingManager* g_pThingManager;                // 0x013B8A1C

class CTCBase {
protected:
    CThingManager& GetThingManager();
};

CThingManager& CTCBase::GetThingManager()
{
    return *g_pThingManager;
}

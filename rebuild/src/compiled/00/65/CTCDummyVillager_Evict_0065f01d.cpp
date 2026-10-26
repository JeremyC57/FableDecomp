#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCDummyVillager::Evict() -- forgets the dummy's creature: the Creature pointer is reset to null and CreatureUID cleared
// Leaf: Evict
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065F01D, length 49 bytes. Frame-pointer size TU (`push ebp ; mov ebp,esp`,
// `push ecx ; push ecx` for the 8-byte temporary, `and dword ptr [..],0`, `leave`): `optimize("y",off)` + `("s",on)`.
// Label: the catalog name "CTCVillage::InitialiseSecuritySweeps" is propagated. The only caller is
// CTCVillage::DetachFromDummyVillager(CThing*) (0x0065B333), which calls it on the interface-0xD6 component of each
// village dummy whose +0x30 pointer holds the thing; Ego_r.pdb CTCDummyVillager has Creature (CIntelligentPointer
// <CThingAICreature>) at +0x30 and CreatureUID (ulong64) at +0x38, and the PDB lists `?Evict@CTCDummyVillager@@QAEXXZ`.
// Callees: CIntelligentPointer<CThingAICreature>::CIntelligentPointer(CThing*) @ 0x0050F3F0, CIntelligentPointer
// <CThingAICreature>::operator=(const CIntelligentPointer&) @ 0x0050F440, ~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: `Creature = 0` through the implicit converting constructor: the temporary is re-addressed with `lea` for the
// assignment and destroyed after it (the landed NPlayerGui::CDrawHighlightingTargetingCursor::Clear 0x00646709 shape);
// the 64-bit UID is cleared as two `and` stores.
class CThing;

struct CBaseIntelligentPointer {
    void* __vftable;                        // +0x00
    void* PData;                            // +0x04
    ~CBaseIntelligentPointer();             // 0x00A01C10
};

namespace N_0050f3f0 { namespace N_0050f440 { struct CIntelligentPointerCreature : CBaseIntelligentPointer {
    CIntelligentPointerCreature(CThing* thing);   // 0x0050F3F0
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other);   // 0x0050F440
}; } using N_0050f440::CIntelligentPointerCreature; } using N_0050f3f0::CIntelligentPointerCreature;

class CTCDummyVillager { public:
    void*                       __vftable;  // +0x00
    unsigned char               _pad_0x04[0x2C];   // +0x04 CTCBase, DefType, Home/WorkBuilding and their UIDs
    CIntelligentPointerCreature Creature;   // +0x30
    unsigned __int64            CreatureUID;    // +0x38
    void Evict();
};

void CTCDummyVillager::Evict()
{
    Creature = (CThing*)0;
    CreatureUID = 0;
}
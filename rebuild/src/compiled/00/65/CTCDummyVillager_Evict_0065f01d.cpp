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
    void* PData;                            // +0x04
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
};

class CThingAICreature;
template <class T> class CIntelligentPointer;
template <> class CIntelligentPointer<CThingAICreature> : public CBaseIntelligentPointer { public:
    CIntelligentPointer(CThingAICreature* item);   // 0x0050F3F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x0050F440
};

class CTCDummyVillager { public:
    void*                       __vftable;  // +0x00
    unsigned char               _pad_0x04[0x2C];   // +0x04 CTCBase, DefType, Home/WorkBuilding and their UIDs
    CIntelligentPointer<CThingAICreature> Creature;   // +0x30
    unsigned __int64            CreatureUID;    // +0x38
    void Evict();
};

void CTCDummyVillager::Evict()
{
    Creature = (CThingAICreature*)0;
    CreatureUID = 0;
}
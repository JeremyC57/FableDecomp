#pragma optimize("s", on)
// CTCVillage::BuildingContainsCorrectVillagers(CThingBuilding* building) -- false when the building is dead, or when a live villager whose home is this building stands outside it, or one living elsewhere stands inside it
// Grade: DIFFER(126v126) -- register swap only: retail keeps &node->villager in edi and the home building in ebx, ours ebx/edi. Tried: template Get() wrapper (add instead of lea + swap), typed GetPItem (registers right, `mov ecx,eax ; add ecx,168h`), unrelated pointer type, inline CThingAICreature::GetHome() accessor (lea right, registers swapped back).
// Leaf: BuildingContainsCorrectVillagers
// Retail: Fable.exe @ 0x00658EC3, 126 bytes (`ret 4`); size TU (unaligned entry; the frame pointer comes from the address of
//         the `building` argument passed by reference): `optimize("s",on)`.
// Callees: IS_THING_ALIVE<CThingBuilding*>(CThingBuilding* const&) 0x0065D624 (__fastcall); IS_THING_ALIVE(const
//          CIntelligentPointer<CThingAICreature>&) 0x004AB8F0; CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3: the
//          villager, its home building pointer at +0x168, the villager again); CThing::GetPosition() 0x004C73D0 (manifest
//          "CVertexBufferWin32::DoSizeof" is propagated); CThingBuilding::IsPointInside(const C3DVector&) 0x0082D000.
// Layout: retail CTCVillage VillagePeople +0x2c (bare list head pointer, PDB +0x34); the villager's home building is a
//         CIntelligentPointer<CThingBuilding> at +0x168.
// Shape: `this` is spilled to the frame (`mov [ebp-4],esi`) and the list end re-read through it; the two mismatch tests
//        share one `xor al,al` exit.
struct C3DVector { float X, Y, Z; };
class CThingBuilding;

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    void* _vfptr;
    void* m_pItem;
};
template <class T> struct CIntelligentPointer {   // 8 bytes: vptr + item
    void* _vfptr;
    T*    m_pItem;
    T* GetPItem() const;                        // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};

class CThingAICreature {
public:
    unsigned char _pad_0x00[0x168];
    CIntelligentPointer<CThingBuilding> PHome;  // +0x168
    C3DVector* GetPosition();                   // 0x004C73D0 (CThing)
    CThingBuilding* GetHome() const { return PHome.GetPItem(); }
};
class CThingBuilding {
public:
    bool IsPointInside(const C3DVector& pos);   // 0x0082D000
};

bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThingAICreature>& thing);   // 0x004AB8F0
bool __fastcall IS_THING_ALIVE(CThingBuilding* const& thing);           // 0x0065D624

struct SVillagerNode {                          // STLport list node
    SVillagerNode* _M_next;
    SVillagerNode* _M_prev;
    CIntelligentPointer<CThingAICreature> _M_data;
};

struct CTCVillage {
    unsigned char  _pad_0x00[0x2c];
    SVillagerNode* VillagePeople;               // +0x2c list head node
    bool BuildingContainsCorrectVillagers(CThingBuilding* building);
};

bool CTCVillage::BuildingContainsCorrectVillagers(CThingBuilding* building)
{
    if (!IS_THING_ALIVE(building))
        return false;
    for (SVillagerNode* node = VillagePeople->_M_next; node != VillagePeople; node = node->_M_next) {
        CIntelligentPointer<CThingAICreature>& villager = node->_M_data;
        if (IS_THING_ALIVE(villager)) {
            CThingBuilding* home = villager.GetPItem()->GetHome();
            bool inside = building->IsPointInside(*villager.GetPItem()->GetPosition());
            if (home == building) {
                if (!inside)
                    return false;
            } else if (inside) {
                return false;
            }
        }
    }
    return true;
}

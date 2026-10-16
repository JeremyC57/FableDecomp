#pragma optimize("s", on)
// CTCVillage::DetachFromDummyVillager(CThing* villager) -- for every live village dummy whose interface-214 component is attached to this villager, detach it
// Leaf: DetachFromDummyVillager
// Retail: Fable.exe @ 0x0065B2C4, 133 bytes (`ret 4`); size TU (unaligned entry): `optimize("s",on)`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x2); IS_THING_ALIVE<CBaseObject*>(CBaseObject* const&) 0x0040DCE0
//          (__fastcall); std::bitset<>::test(size_t) const 0x00410DE0 (interface 214, edi hoisted); CVectorMap<
//          ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined GetTC); the component's detach method 0x0065F01D
//          (manifest "CTCVillage::InitialiseSecuritySweeps" is wrong: it is called on the component; real name unknown).
// Layout: retail CTCVillage VillageDummies +0x74 (bare list head pointer, PDB +0xf8); list node {next, prev,
//         CIntelligentPointer<CThing> +8}; the interface-214 component holds its villager as CIntelligentPointer<CThing>
//         at +0x30.
// Shape: `this` spilled to [ebp-8] for the end re-read; the dummy binds IS_THING_ALIVE's const reference through a
//        derived-to-base (CThing* -> CBaseObject*) temporary at [ebp-4], later the GetTC key (a const-qualification
//        conversion does not keep the dummy in esi).
class CTCBase;
class CThing;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CBaseObject {
public:
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CThing : public CBaseObject {};
bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing);   // 0x0040DCE0

struct CBaseIntelligentPointer {
    void* _vfptr;
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CTCDummyVillager {                       // the dummy's interface-214 component (class name unknown)
    unsigned char _pad_0x00[0x30];
    CBaseIntelligentPointer PVillager;          // +0x30
    void Detach();                              // 0x0065F01D
};

struct SThingNode {                             // STLport list node
    SThingNode* _M_next;
    SThingNode* _M_prev;
    CBaseIntelligentPointer _M_data;
};

struct CTCVillage {
    unsigned char _pad_0x00[0x74];
    SThingNode*   VillageDummies;               // +0x74 list head node
    void DetachFromDummyVillager(CThing* villager);
};

void CTCVillage::DetachFromDummyVillager(CThing* villager)
{
    for (SThingNode* node = VillageDummies->_M_next; node != VillageDummies; node = node->_M_next) {
        CThing* dummy = node->_M_data.GetPItem();
        if (IS_THING_ALIVE(dummy) && dummy->HasTC(214)) {
            CTCDummyVillager* component = (CTCDummyVillager*)dummy->GetTC(214);
            if (component->PVillager.GetPItem() == villager)
                component->Detach();
        }
    }
}
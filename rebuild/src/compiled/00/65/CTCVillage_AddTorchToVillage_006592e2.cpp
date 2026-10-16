#pragma optimize("s", on)
// CTCVillage::AddTorchToVillage(CThing* torch) -- append the torch to the village's torch list and point its village-member component (interface 35) at this village
// Leaf: AddTorchToVillage
// Retail: Fable.exe @ 0x006592E2, 124 bytes (`ret 4`); size TU (unaligned entry): `optimize("s",on)`.
// Callees: CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) 0x004A96F0; list<CIntelligentPointer<CThing>>::push_back
//          (const CIntelligentPointer<CThing>&) 0x004BA4E0 (manifest "Std_Vector_Push_Back_SmartPtr" is a gap-scan name);
//          CBaseIntelligentPointer::GetPItem 0x00A01B50; std::bitset<>::test(size_t) const 0x00410DE0 (the thing's interface
//          bitset at +0x20, bit 35); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined GetTC, key homed
//          in the dead `torch` argument slot); CTCVillageMember::SetPVillage(CTCVillage*) 0x007EE270;
//          CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10.
// Layout: retail CTCVillage VillageTorches +0x30 (bare list head pointer, PDB +0x40); retail CThing: bitset +0x20, interface
//         map +0x44.
// Shape: `T* member = 0; if (HasTC(35)) member = GetTC(35);` preloads the ebx zero (declared before the GetPItem call, which it precedes); `this` is spilled to [ebp-4] for the
//        SetPVillage argument.
class CTCBase;
class CThing;
struct CTCVillage;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
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

struct CIntelligentPointer {                    // CIntelligentPointer<CThing>: vptr + item
    void* _vfptr;
    CThing* m_pItem;
    CIntelligentPointer(CThing* item);          // 0x004A96F0
    ~CIntelligentPointer();                     // 0x00A01C10 (CBaseIntelligentPointer)
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CThingList {                             // retail std::list<CIntelligentPointer<CThing>>: head pointer only
    void* _M_node;
    void push_back(const CIntelligentPointer& value);   // 0x004BA4E0
};

struct CTCVillageMember {
    void SetPVillage(CTCVillage* village);      // 0x007EE270
};

struct CTCVillage {
    unsigned char _pad_0x00[0x30];
    CThingList    VillageTorches;               // +0x30
    void AddTorchToVillage(CThing* torch);
};

void CTCVillage::AddTorchToVillage(CThing* torch)
{
    CIntelligentPointer pointer(torch);
    VillageTorches.push_back(pointer);
    CTCVillageMember* member = 0;
    CThing* thing = pointer.GetPItem();
    if (thing->HasTC(35))
        member = (CTCVillageMember*)thing->GetTC(35);
    member->SetPVillage(this);
}
#include "fable_compiler.h"
#pragma optimize("s", on)
// CTCVillage::AddTorchToVillage(CThing* torch) -- append the torch to the village's torch list and point its village-member component (interface 35) at this village
// Leaf: AddTorchToVillage
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad24[0x20] removed.
// Naming wave 18: the interface-presence stand-in CBitset is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
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
class CTCVillage;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274>       m_present;                    // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    FABLE_FORCEINLINE CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return it->second;
    }
};

class CBaseObject;
class CBaseIntelligentPointer { public:
    CBaseObject* GetPItem() const;   // 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
};
struct CIntelligentPointer {                    // CIntelligentPointer<CThing>: vptr + item
    void* _vfptr;
    CThing* m_pItem;
    CIntelligentPointer(CThing* item);          // 0x004A96F0
    ~CIntelligentPointer();                     // 0x00A01C10 (CBaseIntelligentPointer)
};
struct CThingList {                             // retail std::list<CIntelligentPointer<CThing>>: head pointer only
    void* _M_node;
    void push_back(const CIntelligentPointer& value);   // 0x004BA4E0
};

class CTCVillageMember { public:
    void SetPVillage(CTCVillage* village);      // 0x007EE270
};

class CTCVillage { public:
    unsigned char _pad_0x00[0x30];
    CThingList    VillageTorches;               // +0x30
    void AddTorchToVillage(CThing* torch);
};

void CTCVillage::AddTorchToVillage(CThing* torch)
{
    CIntelligentPointer pointer(torch);
    VillageTorches.push_back(pointer);
    CTCVillageMember* member = 0;
    CThing* thing = ((CThing*)((CBaseIntelligentPointer*)&pointer)->GetPItem());
    if (thing->HasTC(35))
        member = (CTCVillageMember*)thing->GetTC(35);
    member->SetPVillage(this);
}
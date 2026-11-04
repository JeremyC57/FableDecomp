// CAIStateGroup_VillageTaskPickCrops::Cleanup() -- hand the remembered crop field back to the creature's village
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Grade: RELOCATION_MATCH -- landing retry: named creature local before RemoveThingFromTask (fixes the eax push)
// (RemoveThingFromTask) and clear PTaskThing.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00940240, length 208 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined HasTC +
// entry-returning FindEntry, villager 0x23 = bit 3 of +0x24), GFGetPVillageInterface @ 0x007EE540 (__fastcall),
// CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase>, CThing*) @ 0x00659EFC (the by-value pointer
// built in its argument slot: ctor @ 0x00A01B10, vtable 0x0125A8A4, SetPItem @ 0x00A01B90 from PTaskThing), then
// `PTaskThing = 0` through the converting temporary (vtable 0x0128B0A0) and its dtor @ 0x00A01C10.
// Shape: the bool HasTC result is spilled to a frame byte (`shr ecx,3 ; and cl,1 ; mov [esp+13h],cl`); the
// by-value argument's converting ctor takes the source pointer by reference (GetPItem after the vtable store).
// No engine-header member names beyond the padding; local model.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
struct CTCVillager;
class CTCVillage;
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
    __forceinline CInterfaceMap::value_type* FindEntry(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        CInterfaceMap::value_type* sel;
        if (it == map->end()) sel = map->end();
        else if (it->first <= interfaceType) sel = it;
        else sel = map->end();
        return sel;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject*a0);                // 0x00A01B90
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x0128B0A0
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CIntelligentPointerTask : CBaseIntelligentPointer {   // CIntelligentPointer<CVillageTaskBase>, vtable 0x0125A8A4
    virtual void v00();
    CIntelligentPointerTask(const CBaseIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); }
};

class CTCVillage { public:
    void RemoveThingFromTask(CIntelligentPointerTask task, CThing* creature);   // 0x00659EFC
};

struct CAIStateGroup_VillageTaskPickCrops : CAIStateGroupBase {
    CIntelligentPointerThing PTaskThing;        // +0x14 CIntelligentPointer<CThing>
    virtual void Cleanup();
};

void CAIStateGroup_VillageTaskPickCrops::Cleanup()
{
    if (((CThing*)PTaskThing.GetPItem()) != 0) {
        bool hasVillager = PBrain->PCreature->HasTC(0x23);
        if (hasVillager) {
            CTCVillage* village = ((CTCVillageMember*)((CTCVillager*)PBrain->PCreature->FindEntry(0x23)->second))->GetPVillage();
            CThing* creature = PBrain->PCreature;
            village->RemoveThingFromTask(PTaskThing, creature);
        }
        if (hasVillager)
            PTaskThing = 0;
    }
}
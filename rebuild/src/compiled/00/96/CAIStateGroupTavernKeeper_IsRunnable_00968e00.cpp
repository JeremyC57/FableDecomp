// CAIStateGroup_TavernKeeper::IsRunnable() -- for a village member with a live pub (adopted from the creature's workplace when needed) and a live or newly found bar, register the creature as the bar's keeper.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00968E00, length 416 bytes (+ int3 padding). Vtable slot 11.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 0x23 villager = bit 3 of +0x24; 0xb9 tavern bar =
// bit 25 of +0x34, miss path reads the never-written key slot), GFGetPVillageInterface @ 0x007EE540 (__fastcall),
// CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10
// (CIntelligentPointer<CThingBuilding> vtable 0x01244344, CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0),
// CAIStateGroup_TavernKeeper::GetBar @ 0x00968650.
// Layout: engine/CAIStateGroup_TavernKeeper.h (PJug +0x14, PBar +0x1c, PPub +0x24) over the landed local CAIStateGroupBase
// model; the creature's workplace intelligent pointer at +0x170 as the landed CAIStateGroup_TavernServer::IsRunnable
// (0x00967650) models it.
// Shape: the workplace is reached through an inline `GetCreature()` getter and a cast to the creature view (retail
// `lea ecx,[eax+170h]`; the direct member chain gives `add ecx,170h`); one nested `if` with a single `return false`.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing;
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingAICreature { public:
    unsigned char _pad[0x170];
    CBaseIntelligentPointer PWorkplace;         // +0x170 (descriptive)
};
struct CIntelligentPointerBuilding : CBaseIntelligentPointer {     // CIntelligentPointer<CThingBuilding> (vtable 0x01244344)
    virtual void v00();
    CIntelligentPointerBuilding(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerBuilding& operator=(const CIntelligentPointerBuilding& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem(item); return *this; }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {     // CIntelligentPointer<CThingAICreature> (vtable 0x0123E7E0)
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem(item); return *this; }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCVillage;
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540
class CTCTavernBar { public:                           // interface 0xb9
    unsigned char _pad[0x18];
    CIntelligentPointerCreature PKeeper;        // +0x18
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
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_TavernKeeper : CAIStateGroupBase {
    CBaseIntelligentPointer PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CIntelligentPointerBuilding PPub;           // +0x24
    bool GetBar();                              // 0x00968650
    virtual bool IsRunnable();
};

bool CAIStateGroup_TavernKeeper::IsRunnable()
{
    CTCVillager* villager;
    if (PBrain->PCreature->GetInterface(&villager, 0x23)) {
        CTCVillage* village = GFGetPVillageInterface(villager);
        if (!IS_THING_ALIVE(PPub))
            PPub = CIntelligentPointerBuilding(((CThing*)((CThingAICreature*)GetCreature())->PWorkplace.GetPItem()));
        if (IS_THING_ALIVE(PPub) && village != 0 && (IS_THING_ALIVE(PBar) || GetBar())) {
            CTCTavernBar* bar;
            ((CThing*)PBar.GetPItem())->GetInterface(&bar, 0xb9);
            bar->PKeeper = CIntelligentPointerCreature(PBrain->PCreature);
            return true;
        }
    }
    return false;
}
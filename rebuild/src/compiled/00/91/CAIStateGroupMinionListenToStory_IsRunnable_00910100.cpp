// Grade: RELOCATION_MATCH -- one `return a && b && c && d;` with a long-returning (BOOL) GetInterface gives retail's int-width `mov eax,1` / `xor eax,eax`; the search runs in an inline FindMarker(filter, index) whose block-local out thing shares the key word while the function-scope filter keeps its own word, its index stored right before the call; same-TU LowerBound body
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// CAIStateGroup_MinionListenToStory::IsRunnable() -- runnable during the village's cook time when a STORY_PLACE_MARKER
// thing exists in the world.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00910100, length 180 bytes (+ int3 padding).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("STORY_PLACE_MARKER" @ 0x0128F5F0),
// CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410,
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x23 = CTCVillager, bit 3 of the bitset dword at
// +0x24), GFGetPVillageInterface @ 0x007EE540 (twice), CTCVillage::IsCookTime @ 0x00658A80,
// CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPFirstThing<CThingFilter_HasDefGlobalIndex>
// @ 0x00910740 (filter {def index} and the out thing in the 8-byte frame).
// Shape: the def index is looked up first through a CCharString temporary; the out-thing slot shares the key word.
// The engine header flattens the base, so the class is modelled locally.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;


class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad9[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline long GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return 1;
        }
        return 0;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::value_type* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
        CInterfaceMap::value_type* sel;
        if (it == map->end()) sel = map->end();
        else if (it->first <= interfaceType) sel = it;
        else sel = map->end();
        return sel;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CThingSearchTools;
class CAIStateGroupBase {
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000 (Ego_r.pdb ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ, protected)
private: public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
};

struct CTCVillager;
class CTCVillage { public:
    bool IsCookTime();                          // 0x00658A80
};
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)

class CThingFilter_HasDefGlobalIndex { public:
    long DefGlobalIndex;
};

class CThingSearchTools { public:
    bool GetPFirstThing(const CThingFilter_HasDefGlobalIndex& filter, CThing** ppThing) const;   // 0x00910740
};

struct CAIStateGroup_MinionListenToStory : CAIStateGroupBase {
    bool FindMarker(CThingFilter_HasDefGlobalIndex& filter, long markerIndex) { CThing* marker; filter.DefGlobalIndex = markerIndex; return PeekThingSearchTools().GetPFirstThing(filter, &marker); }
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionListenToStory::IsRunnable()
{
    long markerIndex;
    {
        CCharString name("STORY_PLACE_MARKER");
        markerIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    CThingFilter_HasDefGlobalIndex filter;
    CTCVillager* villager;
    return PBrain->PCreature->GetInterface(&villager, 0x23) && ((CTCVillageMember*)villager)->GetPVillage() != 0
        && ((CTCVillageMember*)villager)->GetPVillage()->IsCookTime() && FindMarker(filter, markerIndex);
}
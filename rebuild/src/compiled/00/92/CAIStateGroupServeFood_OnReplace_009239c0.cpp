// CAIStateGroup_ServeFood::OnReplace() -- give up the table: release our place at it, and if the creature still carries
// live food, drop it, destroy it and tell the table; then forget the table, place and task slot.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009239C0, length 214 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (two inlined GetInterface lookups, miss paths read the shared never-written
// slot: type 0xB8 on the table, bit 24 of the dword at +0x34; type 0x46 = CTCCarrying on the creature, bit 6 of +0x28),
// the table component's place release @ 0x007B9090 (manifest "CTCActionUseAcceptBoast::OnActionUse" is propagated),
// CTCCarrying::GetThingInPrimarySlot @ 0x006A1440 / RemoveThingFromPrimarySlot @ 0x006A1E00, CThing::Kill(bool) @
// 0x004C9B80, the table component's served notification @ 0x007B85F0 (manifest "CVillageTaskCarryCrate::
// AssignCreatureToTask" is propagated).
// Shape: two out-parameter GetInterface calls in the body share one key/out slot; `= -1` stores share `or eax,-1`.
// No engine header for this class; PTable +0x18, TaskIndex +0x1c, PlaceIndex +0x44 named descriptively.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->first <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
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

struct CTCTable {                               // the interface-0xB8 component
    unsigned char _pad[0xc];
    bool Occupied;                              // +0x0c
    void NotifyFoodServed(long taskIndex, long flags);  // 0x007B85F0
    void ReleasePlace(long placeIndex);         // 0x007B9090
};
class CTCCarrying { public:
    CThing* GetThingInPrimarySlot();            // 0x006A1440
public:
    CThing* RemoveThingFromPrimarySlot(bool);   // 0x006A1E00
};
struct CThingKill {
    void Kill(bool silent);                     // 0x004C9B80
};

struct CAIStateGroup_ServeFood : CAIStateGroupBase {
    unsigned char _pad_0x14[0x4];               // +0x14
    CThing* PTable;                             // +0x18
    long TaskIndex;                             // +0x1c
    unsigned char _pad_0x20[0x24];              // +0x20..+0x43
    long PlaceIndex;                            // +0x44
    virtual void OnReplace();
};

void CAIStateGroup_ServeFood::OnReplace()
{
    if (PTable != 0) {
        CTCTable* table;
        PTable->GetInterface(&table, 0xb8);
        table->ReleasePlace(PlaceIndex);
        table->Occupied = false;
        CTCCarrying* carrying;
        PBrain->PCreature->GetInterface(&carrying, 0x46);
        CThing* food = carrying->GetThingInPrimarySlot();
        if (food != 0 && !(food->m_flags91 & 1)) {
            carrying->RemoveThingFromPrimarySlot(0);
            ((CThingKill*)food)->Kill(true);
            table->NotifyFoodServed(TaskIndex, 0);
        }
        TaskIndex = -1;
        PlaceIndex = -1;
        PTable = 0;
    }
}
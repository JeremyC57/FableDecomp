// CAIStateGroup_MinionHawkingFromStall::IsStillValid() -- still hawking while the shopkeeper has a shop, it is work
// hours, and the live hawk target (+0x18) is either the creature itself or the shopkeeper's current thing.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094BDF0, length 173 bytes.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x90 = CTCShopKeeper, bit 16 of the bitset
// dword at +0x30; miss path reads the never-written out slot that shares the key word), CTCShopKeeper::GetShopTC
// @ 0x00798F50, CAIStateGroup_WorkBase::IsInWorkHours @ 0x0094BEF0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (four times), the shopkeeper's thing getter @ 0x00798D80 (manifest "_Cons_val<...>" is propagated and wrong).
// Shape: IS_THING_ALIVE inlined; the creature is a named local read before the third GetPItem (held in edi),
// then the shopkeeper's thing as a named local (held in edi) compared with a fresh GetPItem.
// No engine header for this class; the pointer member is +0x18.
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
    unsigned long HasTC(long interfaceType) const
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

inline bool IS_THING_ALIVE(const CBaseIntelligentPointerCtor& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCShop;
class CTCShopKeeper { public:
    CTCShop* GetShopTC();                       // 0x00798F50
    CThing* GetShopThing();                     // 0x00798D80
};

struct CAIStateGroup_MinionHawkingFromStall : CAIStateGroupBase {
    unsigned char _pad_0x14[0x4];               // +0x14
    CBaseIntelligentPointerCtor PHawkTarget;    // +0x18
    bool IsInWorkHours();                       // 0x0094BEF0 (CAIStateGroup_WorkBase)
    virtual bool IsStillValid();
};

bool CAIStateGroup_MinionHawkingFromStall::IsStillValid()
{
    CTCShopKeeper* keeper;
    PBrain->PCreature->GetInterface(&keeper, 0x90);
    if (keeper->GetShopTC() != 0 && IsInWorkHours() && IS_THING_ALIVE(PHawkTarget)) {
        CThing* self = PBrain->PCreature;
        if (PHawkTarget.GetPItem() == self)
            return true;
        CThing* shopThing = keeper->GetShopThing();
        if (shopThing == PHawkTarget.GetPItem())
            return true;
    }
    return false;
}
// CAIStateGroup_SummonerSummon::IsRunnable() -- runnable when the creature's interface-8 component names a live
// target and its summon component (interface 0xFA) still allows creatures; remember the target in PTarget.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096E540, length 208 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 / 0x004365B0 (inlined HasTC + entry-returning FindEntry, types 8
// and 0xFA; two byte-identical instantiations), the interface-8 component's virtual slot 29 (+0x74),
// CTCAISummon::GetMaxCreatureCount @ 0x007FDF60, CIntelligentPointer<const CThing>::CIntelligentPointer(CThing*)
// @ 0x004A96F0 (temporary), GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90 (on PTarget +0x14), dtor @ 0x00A01C10.
// Shape: as CAIStateGroup_MinionReactToEnemy::IsRunnable (0x00937F70) with a second HasTC/FindEntry and a
// converting temporary built by the out-of-line pointer ctor. The engine header for this class is all padding
// (only PTarget +0x14 named), so the shared local AI state-group model is used.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound: the non-const
// instantiation @ 0x0040F020 and the const one @ 0x004365B0 (two byte-identical bodies).
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020 (non-const instantiation)
    const SEntry* LowerBound(const ETCInterfaceType& type) const; // 0x004365B0 (const instantiation, byte-identical body)
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
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline const CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        const CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->first <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
    // the same lookups through a const CThing (the const LowerBound instantiation)
    template<class T> __forceinline bool GetInterfaceConst(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    __forceinline const CInterfaceMap::SEntry* FindEntryConst(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        const CInterfaceMap::SEntry* sel;
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

struct CIntelligentPointerConstThing : CBaseIntelligentPointerCtor {
    CIntelligentPointerConstThing(CThing* item);    // 0x004A96F0
};

struct CTCTargetSource {                        // the interface-8 component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};

class CTCAISummon { public:                            // interface 0xFA
    long GetMaxCreatureCount();                 // 0x007FDF60
};

struct CAIStateGroup_SummonerSummon : CAIStateGroupBase {
    CBaseIntelligentPointerCtor PTarget;        // +0x14 CIntelligentPointer<CThing>
    virtual bool IsRunnable();
};

bool CAIStateGroup_SummonerSummon::IsRunnable()
{
    if (PBrain->PCreature->HasTC(8)) {
        CThing* target = ((CTCTargetSource*)PBrain->PCreature->FindEntry(8)->second)->GetEnemy();
        if (target != 0 && !(target->m_flags91 & 1) && PBrain->PCreature->HasTC(0xFA)
            && ((CTCAISummon*)PBrain->PCreature->FindEntryConst(0xFA)->second)->GetMaxCreatureCount() != 0) {
            CIntelligentPointerConstThing converted(target);
            PTarget.SetPItem(converted.GetPItem());
            return true;
        }
    }
    return false;
}
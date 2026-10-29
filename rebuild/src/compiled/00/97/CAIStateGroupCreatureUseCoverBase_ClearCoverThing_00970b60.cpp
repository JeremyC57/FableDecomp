// CAIStateGroup_CreatureUseCoverBase::ClearCoverThing() -- if the cover thing is still alive, release its cover-point component (SetUser(0)) and clear PCoverThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00970B60, length 166 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three reads: IS_THING_ALIVE inlined, then the thing),
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inline out-parameter GetInterface, type 0xAC),
// cover-point component call @ 0x0087C390 with 0 (label "AssignCreatureToTask" is propagated),
// CBaseIntelligentPointer ctor/SetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01C10.
// Shape: the reset `PCoverThing = CIntelligentPointer<CThing>(0)` (vtable 0x01238C6C) runs only inside the alive test;
// the out-parameter lookup keeps the component in eax (`mov eax,[eax+4] ; push 0 ; mov ecx,eax`).
// Layout: Ego_r.pdb CAIStateGroup_CreatureUseCoverBase (PCoverThing +0x1f, CoverList +0x28, TimingCritical +0x1e).
class CThing;

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer   // inline ctor stores the template's vtable after the base ctor
{ public:
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    T* Get() const { return (T*)GetPItem(); }
};

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                  // +0x00
    SEntry* last;                                   // +0x04
    SEntry* LowerBound(const ETCInterfaceType& type);           // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing
{ public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead

    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CInterfaceMap::SEntry* FindEntry(long type)    // the entry; `->component` read at the call site
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        CInterfaceMap::SEntry* sel;
        if (it == end)
            sel = end;
        else if (it->first <= type)
            sel = it;
        else
            sel = end;
        return sel;
    }
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CAIBrain
{ public:
    unsigned char _pad0[0x18];
    CThing* Creature;                               // +0x18
};

struct CTCCoverPoint                                // interface type 0xAC component (word 5 bit 12: `test ch,10h` on +0x34)
{
    void SetUser(CThing* user);                     // 0x0087C390 (one argument; manifest "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated)
};

struct CCoverList                                   // vector<CIntelligentPointer<CThing> > at +0x28 {first, last, end}
{
    CIntelligentPointer<CThing>* first;
    CIntelligentPointer<CThing>* last;
    CIntelligentPointer<CThing>* end_of_storage;
    CIntelligentPointer<CThing>* erase(CIntelligentPointer<CThing>* from, CIntelligentPointer<CThing>* to);   // 0x004AC0F0
};

__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0)
    {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

#pragma pack(push, 1)
struct CAIStateGroup_CreatureUseCoverBase           // retail layout = Ego_r.pdb (byte-packed: PCoverThing at the odd +0x1f)
{
    void* __vftable;                                // +0x00
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08 CAIStateGroupBase tail
    CIntelligentPointer<const CThing> PTargetThing; // +0x14
    unsigned char _pad1c[2];                        // +0x1c HasDiveAnim, HasJumpOutAnims
    bool TimingCritical;                            // +0x1e
    CIntelligentPointer<CThing> PCoverThing;        // +0x1f
    unsigned char _pad27;                           // +0x27
    CCoverList CoverList;                           // +0x28
    void ClearCoverThing();
    void CleanupLeaf();
};
#pragma pack(pop)

void CAIStateGroup_CreatureUseCoverBase::ClearCoverThing()
{
    if (IS_THING_ALIVE(PCoverThing))
    {
        CTCCoverPoint* cover;
        if (PCoverThing.GetPItem()->GetInterface(0xac, &cover))
            cover->SetUser(0);
        PCoverThing.SetPItem(CIntelligentPointer<CThing>(0).GetPItem());
    }
}
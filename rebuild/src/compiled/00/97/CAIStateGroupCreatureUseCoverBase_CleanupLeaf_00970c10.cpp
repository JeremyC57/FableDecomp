// CAIStateGroup_CreatureUseCoverBase::CleanupLeaf() -- release a live cover thing's cover point, empty the cover list, clear PCoverThing and TimingCritical
// Leaf: CleanupLeaf@CAIStateGroup_CreatureUseCoverBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00970C10, length 192 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020
// (type 0xAC), cover-point call @ 0x0087C390 with 0, vector<CIntelligentPointer<CThing> >::erase(first, last) @ 0x004AC0F0
// (an inline clear() member -- written in the caller it swaps ebx/edi), CBaseIntelligentPointer ctor/SetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01C10.
// Shape: the component pointer is deliberately unset when the interface is absent (retail calls through the dead
// key slot `mov eax,[esp+8]`), so the lookup is `T* tc; if (HasTC(k)) tc = GetTC(k); tc->SetUser(0);`.
struct CThing;

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer   // inline ctor stores the template's vtable after the base ctor
{
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    T* Get() const { return (T*)GetPItem(); }
};

struct CTCBase;

struct CInterfaceMap                                // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                  // +0x00
    SEntry* last;                                   // +0x04
    SEntry* LowerBound(const long* type);           // 0x004365B0
};

struct CThing
{
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
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        CInterfaceMap::SEntry* sel;
        if (it == end)
            sel = end;
        else if (it->type <= type)
            sel = it;
        else
            sel = end;
        return sel;
    }
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CAIBrain
{
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
    void clear() { erase(first, last); }
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

void CAIStateGroup_CreatureUseCoverBase::CleanupLeaf()
{
    if (IS_THING_ALIVE(PCoverThing))
    {
        CThing* thing = PCoverThing.GetPItem();
        CTCCoverPoint* cover;
        if (thing->HasTC(0xac))
            cover = (CTCCoverPoint*)thing->FindEntry(0xac)->component;
        cover->SetUser(0);
    }
    CoverList.clear();
    PCoverThing.SetPItem(CIntelligentPointer<CThing>(0).GetPItem());
    TimingCritical = false;
}
// CAIStateGroup_CreatureUseCoverBase::IsRunnable() -- the creature's interface-8 component names an attack target that IsToAttackTarget accepts: remember it in PTargetThing and defer to the leaf test (slot 15)
// Leaf: IsRunnable@CAIStateGroup_CreatureUseCoverBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00970AB0, length 168 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inline CThing::GetInterface,
// type 8 = word 0 bit 8: `test ch,1`), the component's virtual slot 29 (+0x74) returning the target thing,
// IsToAttackTarget(CThing* creature, CThing* target) @ 0x00864D00 (__fastcall, bool),
// CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10, own slot 15 (+0x3c).
// Shape: `PTargetThing = CIntelligentPointer<const CThing>(target)` -- the temporary (vtable 0x0123E7D8)
// shares the 8-byte frame with the lookup key; its inline operator= is SetPItem(GetPItem()).
// The leaf test's result is returned directly (bool in al). PTargetThing sits at +0x14 (first member after
// the 0x14-byte CAIStateGroupBase); engine header is byte-flattened so a local model is used.
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
    SEntry* LowerBound(const long* type) const;           // 0x004365B0
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

struct CTCInterface8                                // interface type 8 component
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetTarget();                    // slot 29 (+0x74)
};

bool __fastcall IsToAttackTarget(CThing* creature, CThing* target);   // 0x00864D00

struct CAIStateGroup_CreatureUseCoverBase
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual bool IsRunnable();                      // slot 11 (+0x2c)
    virtual void v12(); virtual void v13(); virtual void v14();
    virtual bool IsRunnableLeaf();                  // slot 15 (+0x3c)
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08 priority, name, flags
    CIntelligentPointer<const CThing> PTargetThing; // +0x14
};

bool CAIStateGroup_CreatureUseCoverBase::IsRunnable()
{
    CThing* creature = PBrain->Creature;
    if (creature->HasTC(8))
    {
        CThing* target = ((CTCInterface8*)creature->FindEntry(8)->component)->GetTarget();
        if (target && IsToAttackTarget(PBrain->Creature, target))
        {
            PTargetThing.SetPItem(CIntelligentPointer<const CThing>(target).GetPItem());
            return IsRunnableLeaf();
        }
    }
    return false;
}
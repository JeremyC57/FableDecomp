// CAIStateGroupBehaviourBase::IsRunnable() -- clear PTargetThing; if it is (still) a live thing and the leaf test (slot 15) passes, take the perception component's current thing as the new target
// Grade: DIFFER(181v181) -- scheduling only: `lea esi,[edi+14h]` (&PTargetThing) is placed before the first temporary GetPItem call, retail after it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009577E0, length 181 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10,
// own virtual slot 15 (+0x3c), CTCPerceiveThing query @ 0x007446A0 on PTCPerception +0x1c (returns the thing; the
// manifest's "_Cons_val<...CParticleMorphs...>" label is propagated).
// Shape: two `PTargetThing = CIntelligentPointer<const CThing>(x)` assignments (inline template ctor, vtable 0x01237F1C)
// sharing one 8-byte frame slot; &PTargetThing is kept in esi from the first assignment on; IS_THING_ALIVE inlined.
// Layout: Ego_r.pdb CAIStateGroupBehaviourBase = retail (PTargetThing +0x14, PTCPerception +0x1c).
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
    __forceinline CIntelligentPointer() { }
    __forceinline CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
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

struct CTCPerceiveThing
{
    CThing* GetCurrentThing();                      // 0x007446A0
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

struct CAIStateGroupBehaviourBase
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual bool IsRunnable();                      // slot 11 (+0x2c)
    virtual void v12(); virtual void v13(); virtual void v14();
    virtual bool IsRunnableLeaf();                  // slot 15 (+0x3c)
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CIntelligentPointer<const CThing> PTargetThing; // +0x14
    CTCPerceiveThing* PTCPerception;                // +0x1c
};

bool CAIStateGroupBehaviourBase::IsRunnable()
{
    PTargetThing = CIntelligentPointer<const CThing>(0);
    if (IS_THING_ALIVE(PTargetThing) && IsRunnableLeaf())
    {
        PTargetThing = CIntelligentPointer<const CThing>(PTCPerception->GetCurrentThing());
        return true;
    }
    return false;
}

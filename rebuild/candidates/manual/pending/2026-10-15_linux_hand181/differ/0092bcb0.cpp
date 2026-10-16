// CAIStateGroup_AvoidRegionExit::IsRunnable() -- blocked when the creature's interface-0xAA component is inactive (+0x7b clear or +0x68 set); runnable when its region-exit component (0x105) holds a live thing: store that component's avoid position
// Grade: DIFFER(207v207) -- register swap only: the 0x105 component lands in esi and &PThing in edi (retail edi/esi, reusing the creature register)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092BCB0, length 207 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (types 0xAA = word 5 bit 10, 0x105 = word 8 bit 5),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (IS_THING_ALIVE inlined on the component's pointer +0x0c),
// CTCAvoidRegionExit::GetPosition() const @ 0x0092BD80 (C3DVector by hidden pointer).
// Shape: both lookups are out-parameter GetInterface calls written in the body (one shared key slot); the creature is
// kept in edi; the returned vector is copied dword-wise into the member at +0x14.
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

struct C3DVector { float X, Y, Z; };

struct CTCInterfaceAA                               // interface type 0xAA
{
    unsigned char _pad00[0x68];
    bool Suppressed;                                // +0x68
    unsigned char _pad69[0x7b - 0x69];
    bool Active;                                    // +0x7b
};

struct CTCAvoidRegionExit                           // interface type 0x105
{
    unsigned char _pad00[0x0c];
    CIntelligentPointer<CThing> PThing;             // +0x0c
    C3DVector GetPosition() const;                  // 0x0092BD80
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

struct CAIStateGroup_AvoidRegionExit
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual bool IsRunnable();                      // slot 11
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    C3DVector Position;                             // +0x14
};

bool CAIStateGroup_AvoidRegionExit::IsRunnable()
{
    CThing* creature = PBrain->Creature;
    CTCInterfaceAA* tcAA;
    if (creature->GetInterface(0xaa, &tcAA))
    {
        if (!tcAA->Active || tcAA->Suppressed)
            return false;
    }
    CTCAvoidRegionExit* avoid;
    if (creature->GetInterface(0x105, &avoid) && IS_THING_ALIVE(avoid->PThing))
    {
        Position = avoid->GetPosition();
        return true;
    }
    return false;
}

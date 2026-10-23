// CAIStateGroup_CreatureAdvanceInCover::IsStillRunnable() -- the target lives, is not incapacitated and holds a projectile weapon
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00978f10, length 187 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTargetThing +0x14, IS_THING_ALIVE inlined), CThingCreatureBase::IsIncapacitated @ 0x00661E80,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (twice: carrying 0x46 on the target, weapon 0x48 on the
// primary-slot thing), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CTCWeapon::IsProjectileWeapon @ 0x005DAA40.
// Shape: one && chain with the shared `xor al,al` false exit; both lookups are the out-parameter GetInterface form
// written in the body, so they share one key slot (inside an inline helper, or as HasTC + FindEntry, they get two). Layout: PTargetThing +0x14, PCoverThing +0x1f (Ego_r.pdb CAIStateGroup_CreatureUseCoverBase, byte-packed).
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

struct CTCCarrying                                  // interface type 0x46 (word 2 bit 6)
{
    CThing* GetThingInPrimarySlot();                // 0x006A1440
};

struct CTCWeapon                                    // interface type 0x48 (word 2 bit 8)
{
    bool IsProjectileWeapon();                      // 0x005DAA40
};

struct CThingCreatureBase : CThing
{
    bool IsIncapacitated();                         // 0x00661E80 (flags byte +0x15c: bit 2, else bit 3; manifest "CText::GetAlignment" is propagated)
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
struct CAIStateGroup_CreatureAdvanceInCover
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CIntelligentPointer<const CThing> PTargetThing; // +0x14
    bool IsStillRunnable();
};
#pragma pack(pop)

bool CAIStateGroup_CreatureAdvanceInCover::IsStillRunnable()
{
    if (IS_THING_ALIVE(PTargetThing)
        && !((CThingCreatureBase*)PTargetThing.GetPItem())->IsIncapacitated())
    {
        CTCCarrying* carrying;
        if (PTargetThing.GetPItem()->GetInterface(0x46, &carrying))
        {
            CThing* weapon = carrying->GetThingInPrimarySlot();
            if (weapon) {
                CTCWeapon* tcWeapon;
                if (weapon->GetInterface(0x48, &tcWeapon) && tcWeapon->IsProjectileWeapon())
                    return true;
            }
        }
    }
    return false;
}
// CAIStateGroup_CreatureRangedCombat2::IsAbleToFire() -- a creature with interface 0x34 and a live target may fire, unless it is a village guard (0xB4 + villager 0x23) of a village where the hero is a criminal and the target's fine dialogue (0xF3) is allowed
// Grade: DIFFER(240v240) -- register choice only: creature loaded into ecx (retail eax) for the 0x34 test, PBrain into edx (retail ecx), key-slot address in eax/ecx (retail edx/eax)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091EA70, length 240 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (IS_THING_ALIVE inlined on PTargetThing +0x14, then the target),
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (types 0x23 and 0xF3), GFGetPVillageInterface(CTCVillager*)
// @ 0x007EE540 (__fastcall, called twice; the manifest's IS_THING_ALIVE label is propagated), CTCVillage::IsHeroCriminal
// @ 0x0065BFCE, CTCFineDialogue::IsAllowed @ 0x007E2790.
// Shape: interface bits tested as whole dwords (`test dword ptr [eax+24h],100000h` = type 0x34, +0x34 bit 20 = 0xB4,
// `test byte ptr [eax+24h],8` = 0x23, +0x3c bit 19 = 0xF3); the villager component is kept in esi across both village
// calls; both lookups share one key slot (out-parameter GetInterface written in the body). Every inner failure falls to `mov al,1`.
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

struct CTCVillage
{
    bool IsHeroCriminal();                          // 0x0065BFCE
};
struct CTCVillager;                                 // interface type 0x23
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);   // 0x007EE540

struct CTCFineDialogue                              // interface type 0xF3
{
    bool IsAllowed();                               // 0x007E2790
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

struct CAIStateGroup_CreatureRangedCombat2
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CBaseIntelligentPointer PTargetThing;           // +0x14 CIntelligentPointer<CThingCreatureBase>
    bool IsAbleToFire();
};

bool CAIStateGroup_CreatureRangedCombat2::IsAbleToFire()
{
    if (PBrain->Creature->HasTC(0x34) && IS_THING_ALIVE(PTargetThing))
    {
        CThing* creature = PBrain->Creature;
        CTCVillager* villager;
        if (creature->HasTC(0xb4) && creature->GetInterface(0x23, &villager))
        {
            if (GFGetPVillageInterface(villager) && GFGetPVillageInterface(villager)->IsHeroCriminal())
            {
                CTCFineDialogue* fine;
                if (PTargetThing.GetPItem()->GetInterface(0xf3, &fine) && fine->IsAllowed())
                    return false;
            }
        }
        return true;
    }
    return false;
}

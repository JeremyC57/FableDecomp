// CCreatureAction_PickUpGenericBox::PerformAction() -- give up (virtual slot 4) when the box is dead or already held
// Grade: DIFFER(416v402) -- register allocation: retail gives the carrying component esi and the dropped thing edi (edi then reused for the ability def), ours swaps them, needs ebx for the def, spills &PThingToPickUp and grows the frame to 8; branch layout, slot sharing via same-TU LowerBound/GetSubDef stand-ins otherwise match; tried scoped held, GetTC helper
// (its 0x47 component has an owner); otherwise drop whatever the creature carries in its primary slot (killing a live
// 0x48 thing when the creature's +0x10 state is 1), put the box in the carry slot named by the creature's ability def,
// and add creature mode 0x22
// Leaf: ?PerformAction@CCreatureAction_PickUpGenericBox
// Layout: retail CCreatureActionBase is 0xa8, so PThingToPickUp sits at +0xa8 (Ego_r.pdb +0xb4); CThing interface bitset
//   +0x20, map +0x44, def +0x70, flags +0x91; CAbilityDef carry slot +0x28.
// Shape: the interface lookups go through same-TU LowerBound stand-ins (const 0x004365B0 and 0x0040F020), so VC7.1 shares
//   each key word with the out-pointer (and the 0x31 key with the dead def slot); GetSubDef<CAbilityDef> 0x005CC10E has a
//   same-TU stand-in too (the def stays in edi, the release has no null test).
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CAbilityDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long          RefCount;                           // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long          CarrySlot;                          // +0x28
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (--Object->RefCount == 0)
            Object->Destroy();
    }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CAbilityDef>* out);    // 0x005CC10E
};
void* FindSubDef(CDefClassBase* def);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAbilityDef>* out)
{
    void* found = FindSubDef(this);
    if (!found)
        return false;
    *(void**)out = found;
    return true;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char m_state10;                          // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                   // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* Def;                               // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    CDefClassBase* GetDef() const { return Def; }
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    void Kill(bool removeNow);                        // 0x004C9B80
    template <class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCOwnable {                                   // interface 0x47 on the box
    unsigned char _pad00[0x10];
    CThing*       Owner;                              // +0x10
};
struct CTCCarrying {                                  // interface 0x46 on the creature
    CThing* RemoveThingFromPrimarySlot(bool drop);    // 0x006A1E00
    void PutThingInCarrySlot(CThing* thing, long slot, bool show);   // 0x006A1F80
};
struct CTCCreatureModeManager {                       // interface 0x31
    bool AddMode(unsigned long mode);                 // 0x0073AE10
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                         // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCreatureActionBase {                          // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void GiveUp();                            // slot 4 (+0x10)
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
struct CCreatureAction_PickUpGenericBox : CCreatureActionBase {
    CBaseIntelligentPointer PThingToPickUp;           // +0xa8
    void PerformAction();
};

void CCreatureAction_PickUpGenericBox::PerformAction()
{
    if (IS_THING_ALIVE(PThingToPickUp)) {
        const CTCOwnable* ownable;
        PThingToPickUp.GetPItem()->GetInterface(&ownable, 0x47);
        if (!ownable->Owner) {
            CTCCarrying* carrying;
            GetCreature()->GetInterface(&carrying, 0x46);
            CThing* held = carrying->RemoveThingFromPrimarySlot(false);
            if (held && !(held->m_flags91 & 1) && GetCreature()->m_state10 == 1 && held->HasTC(0x48))
                held->Kill(true);
            CDefPointer<CAbilityDef> def;
            PThingToPickUp.GetPItem()->GetDef()->GetSubDef(&def);
            carrying->PutThingInCarrySlot(PThingToPickUp.GetPItem(), def.Object->CarrySlot, true);
            CTCCreatureModeManager* modes;
            GetCreature()->GetInterface(&modes, 0x31);
            modes->AddMode(0x22);
        } else
            GiveUp();
    } else
        GiveUp();
}

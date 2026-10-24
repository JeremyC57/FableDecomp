// NCreatureMode::CModeCrouch::OnActivate() -- point the two-group filter at ("CROUCH", the primary-slot weapon's animation group), raise the crouch byte of the interface-0x36 component, then the base activation
// Leaf: ?OnActivate@CModeCrouch
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D89C0, 228 bytes (+ int3 padding), `ret`; vtable 0x01267B34 slot 5.
// Callees: CVectorMap::LowerBound (const) @ 0x004365B0 (inlined HasTC + GetTC for CTCCarrying 0x46 and CTCWeapon 0x48),
//   CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CCharString(const char*, long) @ 0x0099EBF0 ("CROUCH" @ 0x01267B28),
//   CTCWeapon::GetAnimationGroupName @ 0x005DABE0, CAnimFilterOf2Groups::SetGroups @ 0x0073E040, ~CCharString @ 0x0099EAE0,
//   CVectorMap::LowerBound @ 0x0040F020 (inlined out-parameter GetInterface, type 0x36 = bit 22 of the +0x24 dword),
//   CModeBase::OnActivate @ 0x0073AAC0 (qualified).
// Name: FableWin.pdb `?OnActivate@CModeCrouch@NCreatureMode@@UAEXXZ` (slot 5).
// Layout: Ego_r.pdb CModeCrouch (0x34): CModeBase 0x1c, SneakAnimName +0x1c, Filter +0x20 (CAnimFilterOf2Groups).
// Shape: the carrying/weapon lookups as the sibling CModeCarryingWeaponStand::OnActivate (0x008D87C0): byte presence test
//   then the dword `test ch,1`, one-ternary returned GetTC, a same-TU lower_bound stand-in so every key shares the one frame
//   word; the weapon component is a named local (esi); the "CROUCH" temporary is built before the group-name call (a
//   literal bound to a const CCharString& parameter); the 0x36 component is the out-parameter GetInterface (`mov eax,[eax+4]`).
struct CTCBase;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CAnimFilterOf2Groups {                  // Ego_r.pdb 0x14
    virtual void v00();
    CCharString   Group1;                      // +4
    CCharString   Group2;                      // +8
    unsigned long Group1CRC;                   // +0xc
    unsigned long Group2CRC;                   // +0x10
    void SetGroups(const CCharString& group1, const CCharString& group2);   // 0x0073E040
};

struct CInterfaceMap {                         // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;  // 0x004365B0 (the const instantiation)
    SEntry* LowerBound(const long* type);              // 0x0040F020
};

// Same-TU stand-in body for the out-of-line const lower_bound (GOTCHAS I-3): the key's address does not escape, so the
// lookups and the string temporary share one frame word.
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
#pragma auto_inline(on)

struct CThing {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];           // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;               // +0x44
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        const CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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

struct CTCCarrying {                           // interface type 0x46
    CThing* GetThingInPrimarySlot() const;     // 0x006A1440
};
struct CTCWeapon {                             // interface type 0x48
    const CCharString& GetAnimationGroupName() const;   // 0x005DABE0
};
struct CTCInterface36 {                        // interface type 0x36 (two byte flags, cleared again by OnDeactivate)
    unsigned char _pad00[0x0f];
    bool          m_crouching;                 // +0x0f
    bool          m_flag10;                    // +0x10
};

struct CModeBase {                             // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;                 // +0x04
    unsigned char _body[0x14];
    void OnActivate();                         // 0x0073AAC0
};

namespace NCreatureMode {
struct CModeCrouch : CModeBase {               // vtable 0x01267B34
    CCharString          SneakAnimName;        // +0x1c
    CAnimFilterOf2Groups Filter;               // +0x20
    void OnActivate();
};
}

void NCreatureMode::CModeCrouch::OnActivate()
{
    if (ParentThing->HasTC(0x46)) {
        CThing* weapon = ParentThing->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
        if (weapon != 0 && weapon->HasTC(0x48)) {
            CTCWeapon* weaponTC = weapon->GetTC<CTCWeapon>(0x48);
            Filter.SetGroups("CROUCH", weaponTC->GetAnimationGroupName());
        }
    }
    CTCInterface36* tc;
    if (ParentThing->GetInterface(&tc, 0x36))
        tc->m_crouching = true;
    CModeBase::OnActivate();
}
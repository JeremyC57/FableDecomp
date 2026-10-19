// NCreatureMode::CModeStrafe::UpdateWeaponState() -- track the weapon in the creature's primary carrying slot: switch the strafe anims when the carried weapon (or its absence) differs from ActiveWeapon, and again whenever the weapon's usability for the creature flips
// Grade: RELOCATION_MATCH -- the usability flip is one `(usable && !LastUsable) || (!usable && LastUsable)` test: VC7.1 lays it out as two tests entering one shared SetChangeOfWeapon block (the nested if/else gave two blocks, `usable != LastUsable` and the ternary fold it)
// Leaf: UpdateWeaponState
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E9950, 215 bytes (`ret`).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x004365B0 (the const instantiation: inlined const GetInterface for
// CTCCarrying 0x46 and CTCWeapon 0x48); CTCCarrying::GetThingInPrimarySlot 0x006A1440; CTCWeapon::IsUsableBy(CThing*) 0x005DAB10;
// CModeStrafe::SetChangeOfWeapon(CThing*) 0x008E9850.
// Layout (PDB CModeStrafe 0x70, matches retail; see the ctor 0x00741FE0): ParentThing +0x04, LastUsable +0x28, ActiveWeapon +0x38.
// Thing: interface bitset +0x20, map +0x44, the weapon's 16-bit id +0x8c (compared with ActiveWeapon), dead bit 0 of +0x91.
// Shape: the change test is one ternary on the weapon pointer (ActiveWeapon read before the branch); usability defaults to
// true and is replaced by IsUsableBy only for a live weapon thing; `if (usable != LastUsable) SetChangeOfWeapon(weapon);
// LastUsable = usable;` is duplicated by VC7.1 per known value of the bool (bl).
struct CTCBase;
struct CThing;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;  // 0x004365B0 (the const instantiation)
};

struct CThing {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];       // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;           // +0x44
    unsigned char  _pad4c[0x8c - 0x4c];
    unsigned short m_weaponId;             // +0x8c
    unsigned char  _pad8e[3];
    unsigned char  m_lifeFlags;            // +0x91 (bit 0: dead)
    bool IsDead() const { return (m_lifeFlags & 1) != 0; }
    template<class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
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
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        const CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (const T*)sel->component;
    }
};

struct CTCCarrying {                       // interface type 0x46
    CThing* GetThingInPrimarySlot() const; // 0x006A1440
};

struct CTCWeapon {                         // interface type 0x48
    bool IsUsableBy(CThing* user) const;   // 0x005DAB10
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
};

namespace NCreatureMode {

struct CModeStrafe : CModeBase {           // vtable 0x01268588
    float         AnimMovementDistance;    // +0x1c
    void*         StrafeAnimName;          // +0x20 (CCharString)
    long          AnimIndex;               // +0x24
    bool          LastUsable;              // +0x28
    unsigned char _filter[0x0c];           // +0x2c CAnimFilterOfGroup
    long          ActiveWeapon;            // +0x38
    void SetChangeOfWeapon(CThing* weapon);   // 0x008E9850
    void UpdateWeaponState();
};

void CModeStrafe::UpdateWeaponState()
{
    const CTCCarrying* carrying;
    if (!ParentThing->GetInterface(&carrying, 0x46))
        return;
    CThing* weapon = carrying->GetThingInPrimarySlot();
    if ((weapon == 0 && ActiveWeapon != -1) || (weapon != 0 && weapon->m_weaponId != ActiveWeapon))
        SetChangeOfWeapon(weapon);
    bool usable = true;
    if (weapon && !weapon->IsDead() && weapon->HasTC(0x48))
        usable = weapon->GetTC<CTCWeapon>(0x48)->IsUsableBy(ParentThing);
    if ((usable && !LastUsable) || (!usable && LastUsable))
        SetChangeOfWeapon(weapon);
    LastUsable = usable;
}

}
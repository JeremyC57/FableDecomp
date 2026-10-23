// CTCSpecialAbilities::CancelAbilitiesForCutscene() -- cancel every special ability the parent thing carries for a cutscene
// Leaf: CancelAbilitiesForCutscene
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072BC60, length 596 bytes; one caller (0x0089D6E0). The catalog names it
// CancelAllAbilities (propagated); the PDB lists ?CancelAbilitiesForCutscene@CTCSpecialAbilities@@QAEXXZ and every callee
// is a CancelForCutscene member, so this is CancelAbilitiesForCutscene.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020; the per-ability CancelForCutscene members:
// CTCBulletTime 0x0079C660 (0x92), CTCPhysicalShieldSpell 0x0079E500 (0x93, catalog mislabels it
// CSystemManager::UninitWindow), CTCBerserk 0x0078D210 (0x84), CTCSummonSpell 0x0079A660 (0x91), CTCGhostSword 0x007C5480
// (0x9a), CTCMultiArrow 0x007CC190 (0x88), CTCAssassinRush 0x007AA5C0 (0xa9, word 5 = +0x34), CTCMultiStrike 0x007CC9C0
// (0x87), CTCDivineWrath 0x0078F8F0 (0x85), CTCUnholyPower 0x00794E60 (0x86).
// Layout: the parent thing's interface bitset at +0x20 (types 0x84..0x9a in word 4 = +0x30, 0xa9 in word 5) and its sorted
// interface map at +0x44, as in the landed 0x0072C370 / 0x007504E0 GetInterface shape.
// Shape: ten inline `if (HasTC(type)) GetTC(type)->CancelForCutscene();` blocks; the lookup is the out-parameter
// GetInterface with the ternary entry pick, the key homed in the one frame word (`push ecx` prologue).
struct CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 std::bitset of thing-component interface types
    CInterfaceMap m_interfaces;                             // +0x44
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T>
    void GetInterface(long type, T** out)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        *out = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T>
    T* GetTC(long type, T*)
    {
        T* tc;
        GetInterface(type, &tc);
        return tc;
    }
};

struct CTCBulletTime          { void CancelForCutscene(); };  // 0x0079C660
struct CTCPhysicalShieldSpell { void CancelForCutscene(); };  // 0x0079E500
struct CTCBerserk             { void CancelForCutscene(); };  // 0x0078D210
struct CTCSummonSpell         { void CancelForCutscene(); };  // 0x0079A660
struct CTCGhostSword          { void CancelForCutscene(); };  // 0x007C5480
struct CTCMultiArrow          { void CancelForCutscene(); };  // 0x007CC190
struct CTCAssassinRush        { void CancelForCutscene(); };  // 0x007AA5C0
struct CTCMultiStrike         { void CancelForCutscene(); };  // 0x007CC9C0
struct CTCDivineWrath         { void CancelForCutscene(); };  // 0x0078F8F0
struct CTCUnholyPower         { void CancelForCutscene(); };  // 0x00794E60

struct CTCSpecialAbilities {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    void CancelAbilitiesForCutscene();
};

void CTCSpecialAbilities::CancelAbilitiesForCutscene()
{
    if (ParentThing->HasTC(0x92)) ParentThing->GetTC(0x92, (CTCBulletTime*)0)->CancelForCutscene();
    if (ParentThing->HasTC(0x93)) ParentThing->GetTC(0x93, (CTCPhysicalShieldSpell*)0)->CancelForCutscene();
    if (ParentThing->HasTC(0x84)) ParentThing->GetTC(0x84, (CTCBerserk*)0)->CancelForCutscene();
    if (ParentThing->HasTC(0x91)) ParentThing->GetTC(0x91, (CTCSummonSpell*)0)->CancelForCutscene();
    if (ParentThing->HasTC(0x9a)) ParentThing->GetTC(0x9a, (CTCGhostSword*)0)->CancelForCutscene();
    if (ParentThing->HasTC(0x88)) ParentThing->GetTC(0x88, (CTCMultiArrow*)0)->CancelForCutscene();
    if (ParentThing->HasTC(0xa9)) ParentThing->GetTC(0xa9, (CTCAssassinRush*)0)->CancelForCutscene();
    if (ParentThing->HasTC(0x87)) ParentThing->GetTC(0x87, (CTCMultiStrike*)0)->CancelForCutscene();
    if (ParentThing->HasTC(0x85)) ParentThing->GetTC(0x85, (CTCDivineWrath*)0)->CancelForCutscene();
    if (ParentThing->HasTC(0x86)) ParentThing->GetTC(0x86, (CTCUnholyPower*)0)->CancelForCutscene();
}

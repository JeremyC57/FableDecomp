// CTCSpecialAbilities::CancelAllAbilities() -- cancel every special-ability move component the parent thing carries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072BA40, length 537 bytes; called from CTCSpecialAbilities::FrameUpdate
// (0x0072C370, landed).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020; the per-ability CancelMove / CancelAbilities
// members: CTCBulletTime 0x0079C640 (0x92), CTCPhysicalShieldSpell 0x0079E300 (0x93), CTCBerserk 0x0078C610 (0x84),
// CTCSummonSpell 0x0079A820 (0x91), CTCGhostSword 0x007C5710 (0x9a), CTCMultiStrike 0x007CC9B0 (0x87), CTCMultiArrow
// 0x007CBEE0 (0x88), CTCDivineWrath 0x0078F820 (0x85), CTCUnholyPower 0x00794D90 (0x86).
// Layout: the parent thing's interface bitset at +0x20 (all nine types live in word 4 = +0x30) and its sorted
// interface map at +0x44, as in the landed 0x0072C370 / 0x007504E0 GetInterface shape.
// Shape: nine inline `if (HasTC(type)) GetTC(type)->CancelMove();` blocks; the lookup is the out-parameter
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

struct CTCBulletTime          { void CancelMove(); };       // 0x0079C640
struct CTCPhysicalShieldSpell { void CancelMove(); };       // 0x0079E300
struct CTCBerserk             { void CancelMove(); };       // 0x0078C610
struct CTCSummonSpell         { void CancelMove(); };       // 0x0079A820
struct CTCGhostSword          { void CancelMove(); };       // 0x007C5710
struct CTCMultiStrike         { void CancelMove(); };       // 0x007CC9B0
struct CTCMultiArrow          { void CancelMove(); };       // 0x007CBEE0
struct CTCDivineWrath         { void CancelAbilities(); };  // 0x0078F820
struct CTCUnholyPower         { void CancelAbilities(); };  // 0x00794D90

struct CTCSpecialAbilities {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    void CancelAllAbilities();
};

void CTCSpecialAbilities::CancelAllAbilities()
{
    if (ParentThing->HasTC(0x92)) ParentThing->GetTC(0x92, (CTCBulletTime*)0)->CancelMove();
    if (ParentThing->HasTC(0x93)) ParentThing->GetTC(0x93, (CTCPhysicalShieldSpell*)0)->CancelMove();
    if (ParentThing->HasTC(0x84)) ParentThing->GetTC(0x84, (CTCBerserk*)0)->CancelMove();
    if (ParentThing->HasTC(0x91)) ParentThing->GetTC(0x91, (CTCSummonSpell*)0)->CancelMove();
    if (ParentThing->HasTC(0x9a)) ParentThing->GetTC(0x9a, (CTCGhostSword*)0)->CancelMove();
    if (ParentThing->HasTC(0x87)) ParentThing->GetTC(0x87, (CTCMultiStrike*)0)->CancelMove();
    if (ParentThing->HasTC(0x88)) ParentThing->GetTC(0x88, (CTCMultiArrow*)0)->CancelMove();
    if (ParentThing->HasTC(0x85)) ParentThing->GetTC(0x85, (CTCDivineWrath*)0)->CancelAbilities();
    if (ParentThing->HasTC(0x86)) ParentThing->GetTC(0x86, (CTCUnholyPower*)0)->CancelAbilities();
}

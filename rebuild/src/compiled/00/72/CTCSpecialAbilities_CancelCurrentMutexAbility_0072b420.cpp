// CTCSpecialAbilities::CancelCurrentMutexAbility() -- while the ability mutex is held, cancel the move component of the mutex ability currently in use
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072B420, 1100 bytes + a 19-entry jump table (0x0072B820) inside the row.
//   Ego_r.pdb / FableWin.pdb: ?CancelCurrentMutexAbility@CTCSpecialAbilities@@QAEXXZ.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetTC) and one cancel member per ability:
//   1 CTCForcePushPower 0x007A5420 (0x96; catalog label "_Dest_val", landed as NotifyOwnerProperty150: it is RemoveTC(0x96)
//   at the head of the CTCForcePushPower TU), 2 CTCBulletTime 0x0079C640 (0x92), 3 CTCWillEnflame 0x007A44C0 (0x95; inside
//   the CTCWillEnflame TU), 4 CTCPhysicalShieldSpell 0x0079E300 (0x93), 5 CTCTurncoatSpell 0x007C6A40 (0x9E; landed as
//   CTCWillEnflame::CancelMove, but it sits right before CTCTurncoatSpell's vtable functions), 6 CTCDrainLife 0x007C3000
//   (0x97; an empty `ret` at the head of the CTCDrainLife TU), 10 CTCSummonSpell 0x0079A820 (0x91), 11 CTCWillLightning
//   0x007A03B0 (0x94), 12 CTCBattleCharge 0x007A7D10 (0xA8; PDB CancelMove, landed as the descriptive CancelCharge),
//   13 CTCAssassinRush 0x007AA480 (0xA9), 14 CTCHealLife 0x007C3E90 (0x99; RemoveTC(0x99) between the DrainLife and
//   HealLife vtable functions, class unproven), 15 CTCGhostSword 0x007C5710 (0x9A), 16 CTCFireballSpell 0x007C8FD0 (0x9D),
//   18 CTCDivineWrath 0x0078F820 (0x85), 19 CTCUnholyPower 0x00794D90 (0x86); abilities 7, 8, 9 and 17 have no mutex move.
// Layout (Ego_r.pdb CTCSpecialAbilities, 56 bytes): ParentThing (CTCBase) +0x04, AbilityMutexLock +0x0C, CurrentAbility
//   +0x10; the thing's interface bitset at +0x20 and its sorted interface map at +0x44 (landed 0x0072BA40 model).
// Shape: a dense switch over CurrentAbility (1..19, default merged with the four empty cases); each case reads ParentThing
//   once into a local and runs the inline `if (HasTC(t)) GetTC(t)->Cancel();` pair, the key homed in the `push ecx` word.
//   In a body this large VC7.1 keeps HasTC/GetInterface/GetTC out of line unless they are `__forceinline`.
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
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T>
    __forceinline void GetInterface(long type, T** out)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        *out = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T>
    __forceinline T* GetTC(long type, T*)
    {
        T* tc;
        GetInterface(type, &tc);
        return tc;
    }
};

struct CTCForcePushPower      { void CancelMove(); };       // 0x007A5420
struct CTCBulletTime          { void CancelMove(); };       // 0x0079C640
struct CTCWillEnflame         { void CancelMove(); };       // 0x007A44C0
struct CTCPhysicalShieldSpell { void CancelMove(); };       // 0x0079E300
struct CTCTurncoatSpell       { void CancelMove(); };       // 0x007C6A40
struct CTCDrainLife           { void CancelMove(); };       // 0x007C3000
struct CTCSummonSpell         { void CancelMove(); };       // 0x0079A820
struct CTCWillLightning       { void CancelMove(); };       // 0x007A03B0
struct CTCBattleCharge        { void CancelMove(); };       // 0x007A7D10
struct CTCAssassinRush        { void CancelMove(); };       // 0x007AA480
struct CTCHealLife            { void CancelMove(); };       // 0x007C3E90
struct CTCGhostSword          { void CancelMove(); };       // 0x007C5710
struct CTCFireballSpell       { void CancelMove(); };       // 0x007C8FD0
struct CTCDivineWrath         { void CancelAbilities(); };  // 0x0078F820
struct CTCUnholyPower         { void CancelAbilities(); };  // 0x00794D90

struct CTCSpecialAbilities {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    long    _base08;                                        // +0x08 (CTCBase)
    bool    AbilityMutexLock;                               // +0x0C
    bool    UsingBed;                                       // +0x0D
    long    CurrentAbility;                                 // +0x10 EHeroAbility
    void CancelCurrentMutexAbility();
};

void CTCSpecialAbilities::CancelCurrentMutexAbility()
{
    if (!AbilityMutexLock)
        return;
    switch (CurrentAbility) {
    case 1: { CThing* t = ParentThing; if (t->HasTC(0x96)) t->GetTC(0x96, (CTCForcePushPower*)0)->CancelMove(); break; }
    case 2: { CThing* t = ParentThing; if (t->HasTC(0x92)) t->GetTC(0x92, (CTCBulletTime*)0)->CancelMove(); break; }
    case 3: { CThing* t = ParentThing; if (t->HasTC(0x95)) t->GetTC(0x95, (CTCWillEnflame*)0)->CancelMove(); break; }
    case 4: { CThing* t = ParentThing; if (t->HasTC(0x93)) t->GetTC(0x93, (CTCPhysicalShieldSpell*)0)->CancelMove(); break; }
    case 5: { CThing* t = ParentThing; if (t->HasTC(0x9e)) t->GetTC(0x9e, (CTCTurncoatSpell*)0)->CancelMove(); break; }
    case 6: { CThing* t = ParentThing; if (t->HasTC(0x97)) t->GetTC(0x97, (CTCDrainLife*)0)->CancelMove(); break; }
    case 10: { CThing* t = ParentThing; if (t->HasTC(0x91)) t->GetTC(0x91, (CTCSummonSpell*)0)->CancelMove(); break; }
    case 11: { CThing* t = ParentThing; if (t->HasTC(0x94)) t->GetTC(0x94, (CTCWillLightning*)0)->CancelMove(); break; }
    case 12: { CThing* t = ParentThing; if (t->HasTC(0xa8)) t->GetTC(0xa8, (CTCBattleCharge*)0)->CancelMove(); break; }
    case 13: { CThing* t = ParentThing; if (t->HasTC(0xa9)) t->GetTC(0xa9, (CTCAssassinRush*)0)->CancelMove(); break; }
    case 14: { CThing* t = ParentThing; if (t->HasTC(0x99)) t->GetTC(0x99, (CTCHealLife*)0)->CancelMove(); break; }
    case 15: { CThing* t = ParentThing; if (t->HasTC(0x9a)) t->GetTC(0x9a, (CTCGhostSword*)0)->CancelMove(); break; }
    case 16: { CThing* t = ParentThing; if (t->HasTC(0x9d)) t->GetTC(0x9d, (CTCFireballSpell*)0)->CancelMove(); break; }
    case 18: { CThing* t = ParentThing; if (t->HasTC(0x85)) t->GetTC(0x85, (CTCDivineWrath*)0)->CancelAbilities(); break; }
    case 19: { CThing* t = ParentThing; if (t->HasTC(0x86)) t->GetTC(0x86, (CTCUnholyPower*)0)->CancelAbilities(); break; }
    }
}
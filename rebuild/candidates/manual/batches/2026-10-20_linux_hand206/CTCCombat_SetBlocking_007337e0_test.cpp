// Behaviour test for CTCCombat::SetBlocking (0x007337E0): ranged users stop aiming / reset / drop player mode 0x22; the mode
// manager gets mode 0xc only when not already in it; Blocking is set.
#include <cstdio>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CPlayer {
    long Removed; int Calls;
    void RemoveMode(long mode) { Removed = mode; ++Calls; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CPlayer* Player;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) { *ppOut = (T*)GetTC(interfaceType); return true; }
        return false;
    }
    CPlayer* GetPlayer() { return Player; }
};
static char g_order[8]; static int g_n = 0;
struct CTCHeroRangedAttacks {
    void CancelAimingWeapon() { g_order[g_n++] = 'c'; }
    void ResetProjectileWeaponMode() { g_order[g_n++] = 'r'; }
};
struct CMode {
    long Type;
    virtual long GetModeType() { return Type; }
};
struct CTCCreatureModeManager {
    CMode* Current; int Adds; unsigned long Added;
    CMode* GetCurrentMode() { return Current; }
    bool AddMode(unsigned long mode) { ++Adds; Added = mode; return true; }
};
struct CTCCombat {
    virtual void v00() {}
    CThing* ParentThing;
    bool Blocking;
    void SetBlocking();
};

void CTCCombat::SetBlocking()
{
    if (ParentThing->HasTC(0x81)) {
        CTCHeroRangedAttacks* ranged = (CTCHeroRangedAttacks*)ParentThing->GetTC(0x81);
        ranged->CancelAimingWeapon();
        ranged->ResetProjectileWeaponMode();
        ParentThing->GetPlayer()->RemoveMode(0x22);
    }
    CTCCreatureModeManager* modes;
    ParentThing->GetInterface(&modes, 0x31);
    if (modes->GetCurrentMode()->GetModeType() != 0xc)
        modes->AddMode(0xc);
    Blocking = true;
}

int main()
{
    int bad = 0;
    CTCHeroRangedAttacks ranged; CMode mode; mode.Type = 3;
    CTCCreatureModeManager modes; modes.Current = &mode; modes.Adds = 0; modes.Added = 0;
    CPlayer player; player.Removed = 0; player.Calls = 0;
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0; thing.Player = &player;
    CInterfaceMap::SEntry e[2] = { { 0x31, (CTCBase*)&modes }, { 0x81, (CTCBase*)&ranged } };
    thing.m_interfaces.first = e; thing.m_interfaces.last = e + 2;
    thing.m_presentBits[1] = 1UL << 17; thing.m_presentBits[4] = 1UL << 1;
    CTCCombat combat; combat.ParentThing = &thing; combat.Blocking = false;
    combat.SetBlocking();
    if (g_n != 2 || g_order[0] != 'c' || g_order[1] != 'r' || player.Removed != 0x22 || modes.Adds != 1 || modes.Added != 0xc || !combat.Blocking) bad |= 1;
    mode.Type = 0xc; thing.m_presentBits[4] = 0; combat.Blocking = false;
    combat.SetBlocking();
    if (g_n != 2 || player.Calls != 1 || modes.Adds != 1 || !combat.Blocking) bad |= 2;
    if (bad) { std::printf("007337e0_TEST FAIL\n"); return 1; }
    std::printf("007337e0_TEST PASS\n");
    return 0;
}

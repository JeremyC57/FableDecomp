// behaviour test for CThingCreatureBase::SheatheAllWeapons (0x006647F0): gates (player, carrying a weapon, TC 0x46), kill each live weapon not held
#include <cstdio>
#include <cstdlib>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing;
struct CThingVector {
    CThing** _M_start; CThing** _M_finish; CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) std::free(_M_start); }
    CThing** begin() { return _M_start; } CThing** end() { return _M_finish; }
};
static CThing* g_weapons[3]; static int g_numWeapons = 0; static CThing* g_held = 0;
struct CTCCarrying {
    int m_calls;
    void GetWeapons_006a22d0(CThingVector* v, bool) { ++m_calls; v->_M_start = (CThing**)std::malloc(sizeof(CThing*) * 3);
        for (int i = 0; i < g_numWeapons; ++i) v->_M_start[i] = g_weapons[i]; v->_M_finish = v->_M_start + g_numWeapons; v->_M_end_of_storage = v->_M_start + 3; }
    bool IsCarryingThing(CThing* t) { return t == g_held; }
};
class CThing {
public:
    bool m_player, m_carryingWeapon; int m_killed;
    unsigned long UsingComponents[9]; CInterfaceMap Components; unsigned char Dead : 1;
    virtual ~CThing() {}
    virtual bool IsPlayer() const { return m_player; }
    void Kill(bool immediately) { if (immediately) ++m_killed; }
    bool HasTC(long type) const { if (UsingComponents[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template<class T> T* GetTC(long interfaceType)
    { long type = interfaceType; CInterfaceMap* map = &Components; CInterfaceMap::SEntry* it = map->LowerBound(&type);
      if (it == map->last || interfaceType < it->type) it = map->last; return (T*)it->component; }
};
static bool IsThingCarryingWeapon(const CThing* t) { return t->m_carryingWeapon; }
inline bool IS_THING_ALIVE(CThing* t) { return t && !t->Dead; }
class CThingCreatureBase : public CThing {
public:
    void SheatheAllWeapons()
    {
        if (IsPlayer()) return;
        if (!IsThingCarryingWeapon(this)) return;
        if (!HasTC(0x46)) return;
        CTCCarrying* carrying = GetTC<CTCCarrying>(0x46);
        CThingVector weapons;
        carrying->GetWeapons_006a22d0(&weapons, false);
        for (CThing** it = weapons.begin(); it != weapons.end(); ++it) {
            CThing* weapon = *it;
            if (IS_THING_ALIVE(weapon) && !carrying->IsCarryingThing(weapon)) weapon->Kill(true);
        }
    }
};
static void Init(CThing& t) { t.m_player = false; t.m_carryingWeapon = false; t.m_killed = 0; t.Dead = 0; for (int i = 0; i < 9; ++i) t.UsingComponents[i] = 0; t.Components.first = t.Components.last = 0; }
int main()
{
    CTCCarrying carrying; carrying.m_calls = 0;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x46, (CTCBase*)&carrying } };
    CThingCreatureBase c; Init(c); c.m_carryingWeapon = true; c.UsingComponents[2] = 1UL << 6;
    c.Components.first = entries; c.Components.last = entries + 2;
    CThing w1, w2, w3; Init(w1); Init(w2); Init(w3); w2.Dead = 1;
    g_weapons[0] = &w1; g_weapons[1] = &w2; g_weapons[2] = &w3; g_numWeapons = 3; g_held = &w3;
    int bad = 0;
    c.SheatheAllWeapons();
    if (carrying.m_calls != 1 || w1.m_killed != 1 || w2.m_killed != 0 || w3.m_killed != 0) bad = 1;
    c.m_player = true; c.SheatheAllWeapons(); if (carrying.m_calls != 1) bad = 1;
    c.m_player = false; c.m_carryingWeapon = false; c.SheatheAllWeapons(); if (carrying.m_calls != 1) bad = 1;
    c.m_carryingWeapon = true; c.UsingComponents[2] = 0; c.SheatheAllWeapons(); if (carrying.m_calls != 1) bad = 1;
    if (bad) { std::printf("006647f0_TEST FAIL\n"); return 1; }
    std::printf("006647f0_TEST PASS\n");
    return 0;
}
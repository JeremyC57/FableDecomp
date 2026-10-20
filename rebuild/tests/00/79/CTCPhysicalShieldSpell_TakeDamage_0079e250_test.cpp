// Behaviour test for CTCPhysicalShieldSpell::TakeDamage (0x0079E250): only an untimed shield (DurationTimer == -1) reacts; with
// hero stats (interface 4) the stamina changes by (long)(damage * ManaDamage[level]) and an empty stamina ends the shield,
// otherwise ShieldHealth absorbs the hit and a non-positive health ends it.
#include <cstdio>
struct CTCBase;
static int g_lowerBoundCalls = 0;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        ++g_lowerBoundCalls;
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
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
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CSpecialAbilitiesPhysicalShieldDef { vector<float> ManaDamage; };
template <class T> struct CDefPointer {
    const T* Object;
    const T* operator->() const { return Object; }
};
static long g_lastModify = 0; static int g_modifyCalls = 0;
struct CTCHeroStats {
    long Stamina;
    void ModifyStaminaBy(long amount) { ++g_modifyCalls; g_lastModify = amount; Stamina += amount; if (Stamina < 0) Stamina = 0; }
};
struct CTCPhysicalShieldSpell {
    CThing* ParentThing;
    CDefPointer<CSpecialAbilitiesPhysicalShieldDef> PDef;
    long    SpellLevel;
    long    DurationTimer;
    float   ShieldHealth;
    void TakeDamage(float damage);
};

void CTCPhysicalShieldSpell::TakeDamage(float damage)
{
    if (DurationTimer == -1) {
        CTCHeroStats* stats;
        if (ParentThing->GetInterface(4, &stats)) {
            stats->ModifyStaminaBy((long)(damage * PDef->ManaDamage[SpellLevel]));
            if (stats->Stamina <= 0)
                DurationTimer = 0;
        } else {
            ShieldHealth += damage * PDef->ManaDamage[SpellLevel];
            if (ShieldHealth <= 0.0f)
                DurationTimer = 0;
        }
    }
}

int main()
{
    int bad = 0;
    float mana[3] = { 1.0f, 2.0f, 4.0f };
    CSpecialAbilitiesPhysicalShieldDef def; def.ManaDamage._M_start = mana; def.ManaDamage._M_finish = mana + 3;
    CTCHeroStats stats; stats.Stamina = 50;
    CInterfaceMap::SEntry entries[3] = { { 1, 0 }, { 4, (CTCBase*)&stats }, { 9, 0 } };
    CThing hero; for (int i = 0; i < 9; ++i) hero.m_presentBits[i] = 0;
    hero.m_presentBits[0] = (1UL << 4) | 2 | (1UL << 9);
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 3;
    CTCPhysicalShieldSpell s; s.ParentThing = &hero; s.PDef.Object = &def; s.SpellLevel = 1; s.DurationTimer = -1; s.ShieldHealth = 3.0f;
    s.TakeDamage(-10.0f);
    if (g_modifyCalls != 1 || g_lastModify != -20 || stats.Stamina != 30 || s.DurationTimer != -1 || s.ShieldHealth != 3.0f) bad |= 1;
    s.TakeDamage(-20.0f);                               // stamina hits 0: shield over
    if (stats.Stamina != 0 || s.DurationTimer != 0) bad |= 2;
    s.TakeDamage(-1.0f);                                // timed shield: ignored
    if (g_modifyCalls != 2) bad |= 4;
    CThing rock; for (int i = 0; i < 9; ++i) rock.m_presentBits[i] = 0;
    rock.m_interfaces.first = entries; rock.m_interfaces.last = entries;
    s.ParentThing = &rock; s.DurationTimer = -1; s.SpellLevel = 2; s.ShieldHealth = 10.0f;
    s.TakeDamage(-2.0f);
    if (s.ShieldHealth != 2.0f || s.DurationTimer != -1 || g_lowerBoundCalls != 2) bad |= 8;
    s.TakeDamage(-0.5f);
    if (s.ShieldHealth != 0.0f || s.DurationTimer != 0) bad |= 16;
    if (bad) { std::printf("0079e250_TEST FAIL\n"); return 1; }
    std::printf("0079e250_TEST PASS\n");
    return 0;
}
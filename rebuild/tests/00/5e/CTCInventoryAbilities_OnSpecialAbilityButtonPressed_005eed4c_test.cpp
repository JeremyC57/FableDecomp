// Behaviour test of CTCInventoryAbilities::OnSpecialAbilityButtonPressed(long button) (retail 0x005EED4C)
// The model and the function body are the source's own; the retail callees are given test bodies below.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

#pragma optimize("s", on)

enum EHeroAbility { HERO_ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class K, class V> class CKeyPairCompareLess { public:   // empty
};

template <class K, class V, class C> class CVectorMap { public:   // STLport vector (12 bytes) + Compare + Dirty
    typedef pair<K, V> value_type;
    value_type*   m_first;                                 // +0x00
    value_type*   m_last;                                  // +0x04
    value_type*   m_end;                                   // +0x08
    C             Compare;                                 // +0x0C
    bool          Dirty;                                   // +0x0D
    value_type* end() { return m_last; }
    value_type* LowerBound(const K& key);                  // 0x005F7F4C
    __forceinline value_type* Find(const K& key)
    {
        value_type* it = LowerBound(key);
        return (it == end() || key < it->first) ? end() : it;
    }
};

class CTCInventoryAbilities { public:                             // retail layout (see above)
    unsigned char _pad_0x000[0x160];
    unsigned char Abilities[0x0c];                         // +0x160 vector<pair<long, CDefPointer<const CHeroAbilityDef> > >
    CVectorMap<long, EHeroAbility, CKeyPairCompareLess<long, EHeroAbility> > AbilityButtons;   // +0x16C
    void OnSpecialAbilityUsed(EHeroAbility ability, bool fromQuickAccess);   // 0x005EEA9B
    void OnSpecialAbilityButtonPressed(long button);
};

void CTCInventoryAbilities::OnSpecialAbilityButtonPressed(long button)
{
    pair<long, EHeroAbility>* it = AbilityButtons.Find(button);
    if (it != AbilityButtons.end())
        OnSpecialAbilityUsed(it->second, false);
}

// ---- test bodies of the retail callees ----
template <class K, class V, class C> typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::LowerBound(const K& key)   // 0x005F7F4C
{
    value_type* it = m_first;
    while (it != m_last && it->first < key)
        ++it;
    return it;
}
static int g_used = 0;
static EHeroAbility g_usedAbility = HERO_ABILITY_NULL;
static bool g_usedFlag = true;
void CTCInventoryAbilities::OnSpecialAbilityUsed(EHeroAbility ability, bool fromQuickAccess)   // 0x005EEA9B
{
    ++g_used;
    g_usedAbility = ability;
    g_usedFlag = fromQuickAccess;
}
static CTCInventoryAbilities g_abilities;
int main()
{
    static pair<long, EHeroAbility> buttons[3] = { { 0, (EHeroAbility)3 }, { 2, (EHeroAbility)5 }, { 4, (EHeroAbility)7 } };
    g_abilities.AbilityButtons.m_first = buttons;
    g_abilities.AbilityButtons.m_last = buttons + 3;
    g_abilities.AbilityButtons.m_end = buttons + 3;
    g_abilities.OnSpecialAbilityButtonPressed(2);
    bool ok = g_used == 1 && g_usedAbility == (EHeroAbility)5 && !g_usedFlag;
    g_abilities.OnSpecialAbilityButtonPressed(1);           // between two buttons: nothing assigned
    g_abilities.OnSpecialAbilityButtonPressed(5);           // past the last button
    ok = ok && g_used == 1;
    g_abilities.OnSpecialAbilityButtonPressed(4);
    ok = ok && g_used == 2 && g_usedAbility == (EHeroAbility)7;
    if (!ok) { std::printf("005eed4c_TEST FAIL\n"); return 1; }
    std::printf("005eed4c_TEST PASS\n");
    return 0;
}
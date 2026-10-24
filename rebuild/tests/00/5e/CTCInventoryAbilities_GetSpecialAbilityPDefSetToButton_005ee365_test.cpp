// Behaviour test of CTCInventoryAbilities::GetSpecialAbilityPDefSetToButton(long button) const (retail 0x005EE365)
// The model and the function body are the source's own; the retail callees are given test bodies below.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

#pragma optimize("y", off)
#pragma optimize("s", on)

enum EHeroAbility { HERO_ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };

struct CDefPointeeBase {                                   // counted game definition: vtable + reference count
    virtual void v00();
    virtual void Release() const;                          // slot 1
    mutable long RefCount;                                 // +0x04
};
struct CHeroAbilityDef : CDefPointeeBase { };

template <class T> struct CDefPointer {                    // one word; returned through a hidden result pointer
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Release(); }
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class K, class V> struct CKeyPairCompareLess {   // empty
};

template <class K, class V, class C> struct CVectorMap {   // STLport vector (12 bytes) + Compare + Dirty
    typedef pair<K, V> value_type;
    value_type*   m_first;                                 // +0x00
    value_type*   m_last;                                  // +0x04
    value_type*   m_end;                                   // +0x08
    C             Compare;                                 // +0x0C
    bool          Dirty;                                   // +0x0D
    const value_type* end() const { return m_last; }
    const value_type* LowerBound(const K& key) const;      // 0x005F7F89
    __forceinline const value_type* Find(const K& key) const
    {
        const value_type* it = LowerBound(key);
        return (it == end() || key < it->first) ? end() : it;
    }
};

struct CTCInventoryAbilities {                             // retail layout (see above)
    unsigned char _pad_0x000[0x160];
    unsigned char Abilities[0x0c];                         // +0x160 vector<pair<long, CDefPointer<const CHeroAbilityDef> > >
    CVectorMap<long, EHeroAbility, CKeyPairCompareLess<long, EHeroAbility> > AbilityButtons;   // +0x16C
    CDefPointer<const CHeroAbilityDef> GetAbilityPDef(EHeroAbility ability) const;            // 0x005EDFD9
    CDefPointer<const CHeroAbilityDef> GetSpecialAbilityPDefSetToButton(long button) const;
};

CDefPointer<const CHeroAbilityDef> CTCInventoryAbilities::GetSpecialAbilityPDefSetToButton(long button) const
{
    const pair<long, EHeroAbility>* it = AbilityButtons.Find(button);
    if (it != AbilityButtons.end())
        return GetAbilityPDef(it->second);
    return CDefPointer<const CHeroAbilityDef>();
}

// ---- test bodies of the retail callees ----
template <class K, class V, class C> const typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::LowerBound(const K& key) const   // 0x005F7F89
{
    const value_type* it = m_first;
    while (it != m_last && it->first < key)
        ++it;
    return it;
}
static int g_released = 0;
void CDefPointeeBase::v00() { }
void CDefPointeeBase::Release() const { ++g_released; }
static CHeroAbilityDef g_defs[8];
CDefPointer<const CHeroAbilityDef> CTCInventoryAbilities::GetAbilityPDef(EHeroAbility ability) const   // 0x005EDFD9
{
    CDefPointer<const CHeroAbilityDef> def;
    def.Object = &g_defs[ability];
    ++g_defs[ability].RefCount;
    return def;
}
static CTCInventoryAbilities g_abilities;
int main()
{
    for (int i = 0; i < 8; ++i)
        g_defs[i].RefCount = 1;
    static pair<long, EHeroAbility> buttons[2] = { { 1, (EHeroAbility)3 }, { 3, (EHeroAbility)5 } };
    g_abilities.AbilityButtons.m_first = buttons;
    g_abilities.AbilityButtons.m_last = buttons + 2;
    g_abilities.AbilityButtons.m_end = buttons + 2;
    bool ok = true;
    {
        CDefPointer<const CHeroAbilityDef> def = g_abilities.GetSpecialAbilityPDefSetToButton(3);
        ok = def.Object == &g_defs[5] && g_defs[5].RefCount == 2;
        CDefPointer<const CHeroAbilityDef> none = g_abilities.GetSpecialAbilityPDefSetToButton(2);
        ok = ok && none.Object == 0;
        CDefPointer<const CHeroAbilityDef> past = g_abilities.GetSpecialAbilityPDefSetToButton(9);
        ok = ok && past.Object == 0;
    }
    ok = ok && g_defs[5].RefCount == 1 && g_released == 0;
    if (!ok) { std::printf("005ee365_TEST FAIL\n"); return 1; }
    std::printf("005ee365_TEST PASS\n");
    return 0;
}
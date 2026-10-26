// CTCInventoryAbilities::OnSpecialAbilityButtonPressed(long button) -- cast the special ability assigned to a quick-access button, if the button has one
// Leaf: OnSpecialAbilityButtonPressed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EED4C, 64 bytes, `ret 4` (__fastcall).
//   Ego_r.pdb: `?OnSpecialAbilityButtonPressed@CTCInventoryAbilities@@QAEXJ@Z`.
// Callees: CVectorMap<long, EHeroAbility>::LowerBound(const long&) @ 0x005F7F4C (the non-const copy; the const one is
//   0x005F7F89), CTCInventoryAbilities::OnSpecialAbilityUsed(EHeroAbility, bool) @ 0x005EEA9B (landed).
// Callers: 0x00690104 (the player's special-ability button handler).
// Layout: engine/CTCInventoryAbilities.h carries the Ego_r.pdb offsets (16-byte vectors, 20-byte CVectorMaps); the
//   shipped class has 12-byte STLport vectors, so Abilities sits at +0x160 (PDB +0x180) and AbilityButtons
//   (CVectorMap<long button, EHeroAbility>: vector + Compare +0xC + Dirty +0xD) at +0x16C (PDB +0x190), as in the
//   landed siblings 0x005EE31D / 0x005EDFD9 -- the header cannot be used here.
// Shape: CVectorMap::Find inlined (`it = LowerBound(key); return (it == end() || key < it->first) ? end() : it;`, map
//   pointer in esi overwritten by end()), the caller's end() test re-reads the member, then
//   OnSpecialAbilityUsed(it->second, false). Size TU (`optimize("s", on)`) like the sibling 0x005EE31D.
// Label correction: catalogued as `CTCInventoryAbilities_TriggerSpecialAbility`; the PDB name is
//   OnSpecialAbilityButtonPressed.
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
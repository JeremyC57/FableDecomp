// CTCInventoryAbilities::SetAbilityToButton(EHeroAbility ability, long button, bool flag) -- assign a special ability to a quick-access button (moving it off any other button), or clear the button for the null ability
// Leaf: SetAbilityToButton
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F6423, 186 bytes, `ret 0Ch` (__fastcall; the bool is not read here).
//   Ego_r.pdb: `?SetAbilityToButton@CTCInventoryAbilities@@QAEXW4EHeroAbility@@J_N@Z`.
// Callees (AbilityButtons, CVectorMap<long button, EHeroAbility>): LowerBound(const long&) @ 0x005F7F4C (non-const copy),
//   vector::erase(iterator) @ 0x005F87C0, vector::push_back(const pair&) @ 0x005F9C4F, CVectorMap::Sort() @ 0x005FA47D
//   (sort + `Dirty = false`; the out-of-line Update() `if (Dirty) Sort();` is 0x005FA4AF).
// Callers: 0x005F650E, 0x005F6629, 0x005F6D4F, 0x005F7245 (CTCInventoryAbilities).
// Layout: retail AbilityButtons at +0x16C (PDB +0x190; engine/CTCInventoryAbilities.h carries the PDB offsets, see the
//   landed siblings 0x005EE31D / 0x005EDF33).
// Shape: frame-pointer size TU (`optimize("y", off)` + `("s", on)`); FableWin.pdb names the CVectorMap members Find /
//   Insert(const K&, const V&) / Update() / Sort(): Find inlined (`(it == end() || key < it->first) ? end() : it`),
//   the value search is the landed 0x005EDF33 helper (an early `return it`, then `return last`), Insert builds the
//   pair temporary in the 8-byte frame, push_back, `Dirty = true`, and the inline Update() re-reads Dirty before Sort().
// Label correction: catalogued as `AbilityMap_SetData`; the PDB name is SetAbilityToButton.
#pragma optimize("y", off)
#pragma optimize("s", on)

enum EHeroAbility { HERO_ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class K, class V> struct CKeyPairCompareLess {   // empty
};

template <class T> struct vector {                         // STLport vector: 12 bytes
    T* _M_start;                                           // +0x00
    T* _M_finish;                                          // +0x04
    T* _M_end_of_storage;                                  // +0x08
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* position);                                 // 0x005F87C0
    void push_back(const T& x);                            // 0x005F9C4F
};

template <class K, class V, class C> struct CVectorMap : vector<pair<K, V> > {
    typedef pair<K, V> value_type;
    C    Compare;                                          // +0x0C
    bool Dirty;                                            // +0x0D
    value_type* LowerBound(const K& key);                  // 0x005F7F4C
    __forceinline value_type* Find(const K& key)
    {
        value_type* it = LowerBound(key);
        return (it == this->end() || key < it->first) ? this->end() : it;
    }
    __forceinline void Insert(const K& key, const V& value)   // a plain inline stays out of line in the size TU
    {
        this->push_back(value_type(key, value));
        Dirty = true;
    }
    void Sort();                                           // 0x005FA47D
    void Update()
    {
        if (Dirty)
            Sort();
    }
};

typedef CVectorMap<long, EHeroAbility, CKeyPairCompareLess<long, EHeroAbility> > CAbilityButtonMap;

__forceinline pair<long, EHeroAbility>* FindAbility(CAbilityButtonMap& map, EHeroAbility ability)
{
    pair<long, EHeroAbility>* it = map.begin();
    pair<long, EHeroAbility>* last = map.end();
    for (; it != last; ++it) {
        if (it->second == ability)
            return it;
    }
    return last;
}

struct CTCInventoryAbilities {                             // retail layout (see above)
    unsigned char _pad_0x000[0x160];
    unsigned char Abilities[0x0c];                         // +0x160 vector<pair<long, CDefPointer<const CHeroAbilityDef> > >
    CAbilityButtonMap AbilityButtons;                      // +0x16C
    void SetAbilityToButton(EHeroAbility ability, long button, bool flag);
};

void CTCInventoryAbilities::SetAbilityToButton(EHeroAbility ability, long button, bool flag)
{
    if (ability == HERO_ABILITY_NULL) {
        pair<long, EHeroAbility>* it = AbilityButtons.Find(button);
        if (it != AbilityButtons.end())
            AbilityButtons.erase(it);
    }
    else {
        pair<long, EHeroAbility>* it = FindAbility(AbilityButtons, ability);
        if (it != AbilityButtons.end())
            AbilityButtons.erase(it);
        it = AbilityButtons.Find(button);
        if (it == AbilityButtons.end()) {
            AbilityButtons.Insert(button, ability);
            AbilityButtons.Update();
        }
        else
            it->second = ability;
    }
}
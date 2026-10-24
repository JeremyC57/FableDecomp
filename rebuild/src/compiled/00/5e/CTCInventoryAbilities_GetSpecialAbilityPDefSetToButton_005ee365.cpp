// CTCInventoryAbilities::GetSpecialAbilityPDefSetToButton(long button) const -- the counted def of the special ability assigned to a quick-access button, a null pointer when the button has none
// Leaf: GetSpecialAbilityPDefSetToButton
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE365, 78 bytes, `ret 8` (__fastcall: hidden CDefPointer result, button).
//   Ego_r.pdb: `?GetSpecialAbilityPDefSetToButton@CTCInventoryAbilities@@QBE?AV?$CDefPointer@$$CBVCHeroAbilityDef@@@@J@Z`.
// Callees: CVectorMap<long, EHeroAbility>::LowerBound(const long&) const @ 0x005F7F89 (the const copy; the non-const one
//   is 0x005F7F4C), CTCInventoryAbilities::GetAbilityPDef(EHeroAbility) const @ 0x005EDFD9 (landed; hidden result).
// Callers: 0x00652AEB.
// Layout: retail AbilityButtons (CVectorMap<long button, EHeroAbility>) at +0x16C (PDB +0x190); engine/CTCInventoryAbilities.h
//   carries the PDB offsets, so the retail-layout model of the landed siblings 0x005EE31D / 0x005EED4C is used.
// Shape: frame-pointer size TU (`optimize("y", off)` + `("s", on)`; the null result is the size TU's `and [eax],0`);
//   CVectorMap::Find inlined (`(it == end() || key < it->first) ? end() : it`, the key re-read from its argument slot),
//   the found arm returns GetAbilityPDef's result straight into the hidden result (no copy), the empty arm a default
//   CDefPointer.
// Label correction: catalogued as `CTCInventoryAbilities_FindAbility`; the PDB name is GetSpecialAbilityPDefSetToButton.
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
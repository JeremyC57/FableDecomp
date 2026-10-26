// CTCInventoryAbilities::GetSpecialAbilityGraphicSetToButton(long) const -- the icon graphic of the ability assigned to a button, 0 when the button has none
// Leaf: ?GetSpecialAbilityGraphicSetToButton@CTCInventoryAbilities@@QBEJJ@Z
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE31D, length 72 bytes.
// Signature: Ego_r.pdb / FableWin.pdb ?GetSpecialAbilityGraphicSetToButton@CTCInventoryAbilities@@QBEJJ@Z (a const member).
// Callees: CVectorMap<long, EHeroAbility, CKeyPairCompareLess<long, EHeroAbility> >::LowerBound(const long&) const @ 0x005F7F89
//          (on AbilityButtons). Retail keeps both LowerBound instances: the const one 0x5F7F89 is called only from this const
//          getter, the const GetSpecialAbilityPDefSetToButton 0x5EE365 and the out-of-line const Find 0x5F879E; the non-const byte twin 0x5F7F4C is called
//          from the non-const SetAbilityToButton 0x5F6423, OnSpecialAbilityButtonPressed 0x5EED4C, SetAbilityAsAvailable and
//          OnSerialise (FableWin.pdb has both QAE and QBE LowerBound for this map; FINDINGS chunk 380).
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout (16-byte vectors); in the shipped class Abilities
//         (vector<pair<long level, CDefPointer<const CHeroAbilityDef>>>) is at +0x160 and AbilityButtons
//         (CVectorMap<long button, EHeroAbility>: {first, last, end_of_storage} of 8-byte pairs) at +0x16C.
//         Retail CHeroAbilityDef keeps IconGraphicIndex at +0x44 (PDB +0x48).
// Pragma: optimize("s", on) -- size TU (one shared epilogue reached by `jmp`); under it Find must be
//        `__forceinline` (plain inline is called out of line: 51v72).
// Shape: CVectorMap::Find inlined as `it = LowerBound(key); return (it == end() || key < it->key) ? end() : it;`
//        (the map pointer held in esi, then overwritten by end()), and the caller compares the result with
//        end() re-read from the member.
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
}
enum EHeroAbility { HERO_ABILITY_NULL };

class CHeroAbilityDef { public:
    unsigned char _pad_0x00[0x44];
    long          IconGraphicIndex;               // +0x44 (PDB +0x48)
};

struct SAbilityEntry {                            // pair<long, CDefPointer<const CHeroAbilityDef>>
    long                   Level;                 // +0x00
    const CHeroAbilityDef* PDef;                  // +0x04 CDefPointer::Object
};

struct CAbilityEntryVector {
    SAbilityEntry* first;                         // +0x00
    SAbilityEntry* last;                          // +0x04
    SAbilityEntry* end_of_storage;                // +0x08
    const SAbilityEntry& operator[](unsigned long n) const { return first[n]; }
};

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
};

template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                            // +0x00
    value_type* Last;                             // +0x04
    value_type* EndOfStorage;                     // +0x08
    const value_type* end() const { return Last; }
    const value_type* LowerBound(const K& key) const;   // 0x005F7F89
    __forceinline const value_type* Find(const K& key) const
    {
        const value_type* it = LowerBound(key);
        return (it == end() || key < it->first) ? end() : it;
    }
};

class CTCInventoryAbilities {
public:
    unsigned char       _pad_0x000[0x160];
    CAbilityEntryVector Abilities;                // +0x160 (PDB +0x180)
    CVectorMap<long, EHeroAbility, CKeyPairCompareLess<long, EHeroAbility> > AbilityButtons;   // +0x16C (PDB +0x190)

    long GetSpecialAbilityGraphicSetToButton(long button) const;
};

long CTCInventoryAbilities::GetSpecialAbilityGraphicSetToButton(long button) const
{
    const std::pair<long, EHeroAbility>* it = AbilityButtons.Find(button);
    if (it != AbilityButtons.end())
        return Abilities[it->second].PDef->IconGraphicIndex;
    return 0;
}

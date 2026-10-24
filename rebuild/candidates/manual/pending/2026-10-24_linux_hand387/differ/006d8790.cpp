// CTCHeroExperience::AddSuccessfulStrikeOfType(EDamageAttribute type, float damage, bool special) -- record the attack, rearm the experience-attenuation timer and, unless a multi-strike is running, raise the combo multiplier param by the damage scaled by the attack-variation and per-attribute combo multipliers
// Leaf: ?AddSuccessfulStrikeOfType@
// Grade: DIFFER(124v124) -- register allocation only: retail keeps the found node in ebx (`push ebx` inside the
//        branch, `mov ebx,[esp+0Ch]`) and the parent thing in eax / its flag byte in cl after the timer store; ours
//        reuses edi (the dead combo-data reference) for the node, loads the thing into ecx ahead of the store and
//        passes the find arguments in edx/eax instead of ecx/edx. Tried: early `return` on HasTC (identical), a
//        default-constructed iterator assigned from find (gets ebx but reads the node through find's returned pointer,
//        122 bytes), the full STLport iterator / _Rb_tree / map layering (125 bytes).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D8790, length 124 bytes, thiscall `ret 0Ch`. Ego_r.pdb
// `?AddSuccessfulStrikeOfType@CTCHeroExperience@@AAEXW4EDamageAttribute@@M_N@Z`; callers 0x006D8D00
// (AddSuccessfulStrikeWithHitParams, this chunk) and the other strike entry points. The manifest label
// "CTCHeroExperience_ApplyDamageMultiplier" is descriptive.
// Callees: AddAttackOfTypeToAttackHistory(EDamageAttribute) @ 0x006D7EC0 (this chunk, parked),
// GetActiveComboMultiplierData() const @ 0x006D72D0, __ftol2 @ 0x00BFEA70, map<EDamageAttribute,float>::find const
// @ 0x006DAB90 (landed; iterator through the hidden pointer, `ret 8`), GetAttackVariationMultiplier() const @ 0x006D7120
// (landed), UpdateComboMultiplier(bool) @ 0x006D7F20 (landed). Global: the integer frame rate @ 0x01375550 (`fild`).
// Layout: CTCHeroExperience in its retail layout (12-byte STLport vectors, 4-byte lists; engine/CTCHeroExperience.h
// carries the Ego_r.pdb debug-STL offsets): ParentThing +0x04, ExperienceMultiplierParam +0x30,
// ExperienceAttenuationTimer +0x38, as in the landed 0x006D7120 / 0x006D7F20. CComboMultiplierData: ComboMultiplier map
// +0x00, ExperienceMultiplierNumSecondsForAttenuation +0x0C (as in this chunk's 0x006DB060). The parent thing's
// component bitset is at +0x20; type 0x87 (CTCMultiStrike) is bit 7 of the byte at +0x30.
// Shape: the timer is `(long)(fps * seconds)`; `!HasTC(0x87)` guards the find; the damage magnitude is a named float
// local (homed in the dead `type` slot, `fst` kept) multiplied into `variation * |damage| * multiplier`.
#include <math.h>

enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

namespace std {

struct _Rb_tree_node_base {
    unsigned char       _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};

template <class K, class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    K first;                                                // +0x10
    V second;                                               // +0x14
};

template <class K, class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    bool operator!=(const _Rb_tree_iterator& x) const { return _M_node != x._M_node; }
    _Rb_tree_node<K, V>* operator->() const { return (_Rb_tree_node<K, V>*)_M_node; }
};

template <class K, class V> class map {
public:
    typedef _Rb_tree_iterator<K, V> const_iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    unsigned char       _M_key_compare[4];                  // +0x08
    const_iterator end() const { return const_iterator(_M_header); }
    const_iterator find(const K& key) const;                // 0x006DAB90
};

}

class CComboMultiplierData {
public:
    std::map<EDamageAttribute, float> ComboMultiplier;      // +0x00
    float ExperienceMultiplierNumSecondsForAttenuation;     // +0x0C
    float ExperienceMultiplierParamDecayDeltaPerSecond;     // +0x10
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20 bitset of thing-component interface types
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};

extern long g_framesPerSecond;                              // 0x01375550

class CTCHeroExperience {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    long FishingLevel;                                      // +0x0C
    long TotalExperiencePoints;                             // +0x10
    long ExperiencePointsAvailableToSpend;                  // +0x14
    long* StatExperiencePoints[3];                          // +0x18
    long* TrainableStatLevels[3];                           // +0x24
    float ExperienceMultiplierParam;                        // +0x30
    float LastExperienceMultiplierParam;                    // +0x34
    long ExperienceAttenuationTimer;                        // +0x38

    const CComboMultiplierData& GetActiveComboMultiplierData() const;   // 0x006D72D0
    float GetAttackVariationMultiplier() const;             // 0x006D7120
    void AddAttackOfTypeToAttackHistory(EDamageAttribute type);         // 0x006D7EC0
    void UpdateComboMultiplier(bool special);               // 0x006D7F20
    void AddSuccessfulStrikeOfType(EDamageAttribute type, float damage, bool special);
};

void CTCHeroExperience::AddSuccessfulStrikeOfType(EDamageAttribute type, float damage, bool special)
{
    AddAttackOfTypeToAttackHistory(type);
    const CComboMultiplierData& data = GetActiveComboMultiplierData();
    ExperienceAttenuationTimer = (long)(g_framesPerSecond * data.ExperienceMultiplierNumSecondsForAttenuation);
    if (!ParentThing->HasTC(0x87)) {
        std::map<EDamageAttribute, float>::const_iterator it = data.ComboMultiplier.find(type);
        if (it != data.ComboMultiplier.end()) {
            float magnitude = (float)fabs(damage);
            ExperienceMultiplierParam += GetAttackVariationMultiplier() * magnitude * it->second;
            UpdateComboMultiplier(special);
        }
    }
}

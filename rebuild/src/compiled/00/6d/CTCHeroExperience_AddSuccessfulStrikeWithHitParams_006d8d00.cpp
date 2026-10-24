// CTCHeroExperience::AddSuccessfulStrikeWithHitParams(const CHitParameters& params) -- credit one successful strike per damage attribute in the hit's damage map, flagged special when the hit was a block counter, a knock-down or a flourish
// Leaf: ?AddSuccessfulStrikeWithHitParams@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D8D00, length 131 bytes, thiscall `ret 4`. Ego_r.pdb
// `?AddSuccessfulStrikeWithHitParams@CTCHeroExperience@@QAEXABVCHitParameters@@@Z`. The manifest label
// "Experience_ProcessHitCombo" is descriptive.
// Callees: CTCHeroExperience::AddSuccessfulStrikeOfType(EDamageAttribute, float, bool) @ 0x006D8790 (Ego_r.pdb
// `?AddSuccessfulStrikeOfType@CTCHeroExperience@@AAEXW4EDamageAttribute@@M_N@Z`; `ret 0Ch`, this chunk's row).
// Layout: CHitParameters (Ego_r.pdb, matches retail up to the map; pack(1) because Position/Direction are unaligned): KnockDown +0x1A, Flourish +0x1D, BlockCounter +0x1F,
// DamageMap map<EDamageAttribute, float> +0x30 (STLport header pointer; node key +0x10, float +0x14).
// Shape: `for (it = DamageMap.begin(); it != DamageMap.end(); ++it) AddSuccessfulStrikeOfType(it->first, it->second,
// BlockCounter || KnockDown || Flourish);` -- the last argument is evaluated first (right to left) every pass, the
// `||` chain giving `xor eax,eax ; mov al,1` / `xor al,al`; end() re-read each pass; the STLport step
// `_M_node = _M_increment(_M_node)` inlined.
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

template <class Dummy> struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* x)
    {
        if (x->_M_right != 0) {
            x = x->_M_right;
            while (x->_M_left != 0)
                x = x->_M_left;
        }
        else {
            _Rb_tree_node_base* y = x->_M_parent;
            while (x == y->_M_right) {
                x = y;
                y = y->_M_parent;
            }
            if (x->_M_right != y)
                x = y;
        }
        return x;
    }
};

template <class K, class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& x) const { return _M_node != x._M_node; }
    _Rb_tree_node<K, V>* operator->() const { return (_Rb_tree_node<K, V>*)_M_node; }
};

template <class K, class V> class map {
public:
    typedef _Rb_tree_iterator<K, V> const_iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    unsigned char       _M_key_compare[4];                  // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
    unsigned long size() const { return _M_node_count; }
};

}

struct C3DVector { float X, Y, Z; };

#pragma pack(push, 1)                                       // the PDB really has the C3DVectors unaligned at +0x02/+0x0E
class CHitParameters {                                      // Ego_r.pdb (prefix; retail-equal up to DamageMap)
public:
    bool Localised;                                         // +0x00
    bool Directional;                                       // +0x01
    C3DVector Position;                                     // +0x02
    C3DVector Direction;                                    // +0x0E
    bool KnockDown;                                         // +0x1A
    bool Decapitate;                                        // +0x1B
    bool Blockable;                                         // +0x1C
    bool Flourish;                                          // +0x1D
    bool EpicSpell;                                         // +0x1E
    bool BlockCounter;                                      // +0x1F
    bool PlayHitResponse;                                   // +0x20
    bool PlayHitResponseOverrideSet;                        // +0x21
    bool MoveBack;                                          // +0x22
    bool CreateParticleEffectOnHit;                         // +0x23
    bool CreateDustParticleEffectOnHit;                     // +0x24
    bool GuaranteeHit;                                      // +0x25
    bool Blocked;                                           // +0x26
    bool HitNegated;                                        // +0x27
    bool CauseRecoil;                                       // +0x28
    unsigned char _pad_0x29[3];                             // +0x29
    float ShotBuildupTime;                                  // +0x2C
    std::map<EDamageAttribute, float> DamageMap;            // +0x30
};
#pragma pack(pop)

class CTCHeroExperience {
public:
    void AddSuccessfulStrikeOfType(EDamageAttribute type, float damage, bool special);   // 0x006D8790
    void AddSuccessfulStrikeWithHitParams(const CHitParameters& params);
};

void CTCHeroExperience::AddSuccessfulStrikeWithHitParams(const CHitParameters& params)
{
    for (std::map<EDamageAttribute, float>::const_iterator it = params.DamageMap.begin(); it != params.DamageMap.end(); ++it)
        AddSuccessfulStrikeOfType(it->first, it->second, params.BlockCounter || params.KnockDown || params.Flourish);
}
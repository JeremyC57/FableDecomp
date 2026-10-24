// CTCHeroExperience::GetOnHitSound() const -- the sound of the first intensity threshold at or above the current on-hit sound intensity (the lowest threshold's sound when none is)
// Leaf: ?GetOnHitSound@
// Grade: DIFFER(150v172) -- retail keeps PDef in esi and the map header in edi, re-reads `PDef->map` header and its
//        leftmost node after the loop and shares the GetString-copy-destroy tail between the two returns (the found path
//        jumps into it); ours keeps the header in esi and CSEs begin() into edi (the after-loop load is `[edi+14h]`) and
//        emits the found path's tail twice. Tried: two block-scope named locals (two frame words, 201 bytes); an inline
//        `CDefString::GetString()` returning the table call directly (RVO, no copy, 120); that helper with a named
//        local (one frame word, temp + copy + destroy as retail, 150); plus the STLport base-iterator / non-member
//        `operator!=` shape (150). next: `CCharString sound = it->second.GetString(); return sound;` with the
//        forwarding inline (no named local inside it) -- the spelling that matched the sibling 0x006D75C0 -- plus a
//        spelling of the default that blocks the begin() CSE.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D8BF0, length 172 bytes (+ int3 padding), thiscall `ret 4` (hidden
// CCharString result). Ego_r.pdb `?GetOnHitSound@CTCHeroExperience@@QBE?AVCCharString@@XZ`. The manifest label
// "CTCHeroExperience_GetFlourishName" is descriptive and wrong.
// Callees: GetOnHitSoundIntensity() const @ 0x006D86D0 (this chunk), CDefStringTable::GetString(CDefString) @ 0x009D49B0
// on g_DefStringTable @ 0x013CA828, CCharString copy constructor @ 0x0099EC30, ~CCharString @ 0x0099EAE0.
// Layout: CTCHeroExperience retail (PDef +0x54); CHeroExperienceDef retail: SoundIntensityThresholds map<long, CDefString>
// +0x4C (STLport header pointer; node key +0x10, CDefString +0x14), as in this chunk's 0x006D86D0.
// Shape: `for (it = begin(); it != end(); ++it) if (intensity <= it->first) return it->second.GetString();` and the same
// for begin() after the loop; CDefString::GetString() is an inline returning a named local (VC7.1 has no NRVO), which gives
// retail's frame-word temporary, the copy into the result and its destructor. The STLport `_M_increment` is inlined.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

class CDefString;

class CDefStringTable {
public:
    CCharString GetString(CDefString name);                 // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                    // 0x013CA828

class CDefString {
public:
    long Index;
    CCharString GetString() const { CCharString text = g_DefStringTable.GetString(*this); return text; }
};

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

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment() { _M_node = _Rb_global<bool>::_M_increment(_M_node); }
};

inline bool operator!=(const _Rb_tree_base_iterator& x, const _Rb_tree_base_iterator& y) { return x._M_node != y._M_node; }

template <class V> struct _Nonconst_traits { };
template <class V> struct _Const_traits { };

template <class K, class V, class Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<K, V, _Nonconst_traits<V> > iterator;
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const iterator& it) { _M_node = it._M_node; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
    _Rb_tree_node<K, V>* operator->() const { return (_Rb_tree_node<K, V>*)_M_node; }
};

template <class K, class V> class map {
public:
    typedef _Rb_tree_iterator<K, V, _Const_traits<V> > const_iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    unsigned char       _M_key_compare[4];                  // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
};

}

class CHeroExperienceDef {
public:
    unsigned char _base[0x28];                              // CDefClassBase
    long* ExperienceLevelThresholds[3];                     // +0x28
    long* HeroLevelTitles[3];                               // +0x34
    long* FallbackMultipliers[3];                           // +0x40
    std::map<long, CDefString> SoundIntensityThresholds;    // +0x4C
};

class CTCHeroExperience {
public:
    virtual void v00();
    unsigned char _pad04[0x50];                             // ParentThing .. AttackHistory
    CHeroExperienceDef* PDef;                               // +0x54

    long GetOnHitSoundIntensity() const;                    // 0x006D86D0
    CCharString GetOnHitSound() const;
};

CCharString CTCHeroExperience::GetOnHitSound() const
{
    long intensity = GetOnHitSoundIntensity();
    for (std::map<long, CDefString>::const_iterator it = PDef->SoundIntensityThresholds.begin(); it != PDef->SoundIntensityThresholds.end(); ++it) {
        if (intensity <= it->first)
            return it->second.GetString();
    }
    return PDef->SoundIntensityThresholds.begin()->second.GetString();
}

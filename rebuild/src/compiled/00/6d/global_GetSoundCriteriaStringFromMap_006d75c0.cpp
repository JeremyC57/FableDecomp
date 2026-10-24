// GetSoundCriteriaStringFromMap(const map<long, CDefString>& sounds, long multiplier, CCharString& sound) -- (anonymous namespace) the sound for the first entry at or above the multiplier (the last entry when none is); false for an empty map
// Leaf: ?GetSoundCriteriaStringFromMap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D75C0, length 117 bytes (+ int3 padding), `__fastcall` (map in ecx,
// multiplier in edx, the CCharString reference on the stack = `ret 4`). Ego_r.pdb `?GetSoundCriteriaStringFromMap@
// ?A0x95990025@@YI_NABV?$map@JVCDefString@@...@std@@JAAVCCharString@@@Z` -- a file-local helper of
// CTCHeroExperience.cpp; single caller PlayCombatMultiplierSound 0x006D768A (this chunk). The manifest label is right.
// Callees: the map const_iterator operator--() @ 0x006DA140 (landed; STLport `_M_decrement`), CDefStringTable::GetString
// (CDefString) @ 0x009D49B0 on g_DefStringTable @ 0x013CA828, CCharString::operator=(const CCharString&) @ 0x0099EFB0,
// ~CCharString @ 0x0099EAE0.
// Layout: STLport map {header pointer +0, node count +4}; node key +0x10, CDefString +0x14.
// Shape: `if (sounds.empty()) return false; it = sounds.lower_bound(multiplier); if (it == sounds.end()) --it;
// sound = it->second.GetString(); return true;` -- `_M_lower_bound` inlined (key < k -> right), the iterator lives in
// the frame word (operator-- takes its address), which the GetString temporary reuses. The string comes through an
// inline `CDefString::GetString()` forwarding to the table: that is what makes VC7.1 re-`lea` the temporary for
// operator= (calling the table directly passes GetString's returned eax instead). operator--'s body sits above under
// `#pragma auto_inline(off)` (retail calls it). Kept by a file-scope pointer (anonymous namespace).
class CCharString {
public:
    void* PStringData;
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
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
    CCharString GetString() const { return g_DefStringTable.GetString(*this); }
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

template <class K, class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator& operator--();                        // 0x006DA140
    bool operator==(const _Rb_tree_iterator& x) const { return _M_node == x._M_node; }
    _Rb_tree_node<K, V>* operator->() const { return (_Rb_tree_node<K, V>*)_M_node; }
};

#pragma auto_inline(off)
template <class K, class V> _Rb_tree_iterator<K, V>& _Rb_tree_iterator<K, V>::operator--()
{
    _Rb_tree_node_base* x = _M_node;
    if (x->_M_color == 0 && x->_M_parent->_M_parent == x)
        x = x->_M_right;
    else if (x->_M_left != 0) {
        _Rb_tree_node_base* y = x->_M_left;
        while (y->_M_right != 0)
            y = y->_M_right;
        x = y;
    }
    else {
        _Rb_tree_node_base* y = x->_M_parent;
        while (x == y->_M_left) {
            x = y;
            y = y->_M_parent;
        }
        x = y;
    }
    _M_node = x;
    return *this;
}
#pragma auto_inline(on)

template <class K, class V> class map {
public:
    typedef _Rb_tree_iterator<K, V> const_iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    unsigned char       _M_key_compare[4];                  // +0x08
    const_iterator end() const { return const_iterator(_M_header); }
    bool empty() const { return _M_node_count == 0; }
    _Rb_tree_node_base* _M_lower_bound(const K& k) const
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        while (x != 0)
            if (!(((_Rb_tree_node<K, V>*)x)->first < k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    const_iterator lower_bound(const K& k) const { return const_iterator(_M_lower_bound(k)); }
};

}

namespace {

bool __fastcall GetSoundCriteriaStringFromMap(const std::map<long, CDefString>& sounds, long multiplier, CCharString& sound)
{
    if (sounds.empty())
        return false;
    std::map<long, CDefString>::const_iterator it = sounds.lower_bound(multiplier);
    if (it == sounds.end())
        --it;
    sound = it->second.GetString();
    return true;
}

}

bool (__fastcall* g_pGetSoundCriteriaStringFromMap)(const std::map<long, CDefString>&, long, CCharString&)
    = &GetSoundCriteriaStringFromMap;
// PlayCombatMultiplierSound(CThing& thing, const map<long, CDefString>& sounds, long multiplier) -- (anonymous namespace) play the sound the combo-multiplier map gives for this multiplier on the thing's sound player, if it has one
// Leaf: ?PlayCombatMultiplierSound@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D7640, length 118 bytes (+ int3 padding), `__fastcall` (thing in ecx,
// sounds in edx, multiplier on the stack = `ret 4`). Ego_r.pdb `?PlayCombatMultiplierSound@?A0x95990025@@YIXAAVCThing@@
// ABV?$map@JVCDefString@@...@std@@J@Z` -- a file-local helper of CTCHeroExperience.cpp, called from the landed
// UpdateComboMultiplier 0x006D7F20 and OnComboMultiplierDecrease 0x006D7E70. The manifest label is right.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CCharString::CCharString() @ 0x0099E4B0,
// GetSoundCriteriaStringFromMap @ 0x006D75C0 (this chunk; its body sits above under `#pragma auto_inline(off)`, as in
// retail's TU), CTCSoundPlayer::PlayAttachedCriteria(const CCharString&, bool, bool, bool, long) @ 0x0066BD00
// (Ego_r.pdb `?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z`), ~CCharString @ 0x0099EAE0.
// Layout: CThing component bitset +0x20 (type 0x10 = CTCSoundPlayer is bit 16 of word 0), interface map +0x44 -- the
// bool GetInterface model of the landed UpdateComboMultiplier 0x006D7F20 (same TU).
// Shape: `if (thing.GetInterface(&player, 0x10)) { CCharString sound; if (GetSoundCriteriaStringFromMap(sounds,
// multiplier, sound)) player->PlayAttachedCriteria(sound, true, false, false, -1); }` -- the lookup key (inside
// GetInterface's own `if`) and the string (inside the caller's) are sibling scopes, so they share the one frame word;
// with HasTC + an unconditional GetTC the string gets a second word (123 bytes).
class CCharString {
public:
    void* PStringData;
    CCharString();                                          // 0x0099E4B0
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
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

#pragma auto_inline(off)
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
#pragma auto_inline(on)

}

class CTCSoundPlayer {
public:
    unsigned long PlayAttachedCriteria(const CCharString& name, bool attach, bool loop, bool b3, long d);   // 0x0066BD00
};

class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 bitset of thing-component interface types
    CInterfaceMap m_interfaces;                             // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

namespace {

void __fastcall PlayCombatMultiplierSound(CThing& thing, const std::map<long, CDefString>& sounds, long multiplier)
{
    CTCSoundPlayer* player;
    if (thing.GetInterface(&player, 0x10)) {
        CCharString sound;
        if (GetSoundCriteriaStringFromMap(sounds, multiplier, sound))
            player->PlayAttachedCriteria(sound, true, false, false, -1);
    }
}

}

void (__fastcall* g_pPlayCombatMultiplierSound)(CThing&, const std::map<long, CDefString>&, long) = &PlayCombatMultiplierSound;
// CMeshDataBank::FindLipSyncAnimation(const CCharString& animation, const CCharString& phoneme) -- animation index of a phoneme in an animation's lip-sync table, 0 when absent
// Leaf: ?FindLipSyncAnimation@CMeshDataBank@@QBEKABVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A26530, 117 bytes, `ret 8`. Only caller 0x00A9F139.
// Identity: Ego_r.pdb `?FindLipSyncAnimation@CMeshDataBank@@QBEKABVCCharString@@0@Z`; LipSyncAnimations
//   (map<CCharString, CArray<CMeshDataBank::CLipSyncEntry>>) is at retail +0x3C8: the PDB order with 12-byte
//   STLport arrays puts MeshesAndAnimations at +0x380 and the four info buffers at +0x38C..+0x3B0 (all used by
//   0x00A26900), AnimationEnumType +0x3BC, AnimationManager +0x3C0. CLipSyncEntry = PhonemeName +0, AnimationIndex +4.
//   The catalog label "animation_entry_search" is descriptive.
// Callees: the map's out-of-line `_M_find` 0x00A2B240 (returns the node; STLport 4.6 `find` wraps it), and
//   Compare 0x00411570 inside the inline CCharString ==.
// Shape: Compare's real body is defined above under auto_inline(off), so VC7.1 knows the call writes no memory:
//   the element count and the phoneme's string block stay in registers across it. The hit returns through the
//   element reference (`[ebx+4]`); indexing `entries[i]` twice re-derives start + i*8 from a spilled start.
class CCharStringData { public:                                 // the shared string block
    const char*   m_pData;                               // +0x00
    unsigned long m_length;                              // +0x04
};

#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

class CCharString {
public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
};
template <class _Value>
inline bool operator!=(const _Rb_tree_iterator<_Value>& __x, const _Rb_tree_iterator<_Value>& __y) { return __x._M_node != __y._M_node; }
template <class _Key, class _Tp> class map {                       // STLport map, 12 bytes
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned long       _M_key_compare;
    _Rb_tree_node_base* _M_find(const _Key& __k) const;           // 0x00A2B240
    const_iterator find(const _Key& __k) const { return const_iterator(_M_find(__k)); }
    const_iterator end() const { return const_iterator(_M_header); }
};
}

template <class T> class CArray {                                   // retail STLport vector, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned long i) const { return _M_start[i]; }
};

class CMeshDataBank {
public:
    class CLipSyncEntry {                                           // 8 bytes
    public:
        CCharString   PhonemeName;                                  // +0x00
        unsigned long AnimationIndex;                               // +0x04
    };
    unsigned char _body[0x3C8];
    std::map<CCharString, CArray<CLipSyncEntry> > LipSyncAnimations;   // +0x3C8
    unsigned long FindLipSyncAnimation(const CCharString& animation, const CCharString& phoneme) const;
};

unsigned long CMeshDataBank::FindLipSyncAnimation(const CCharString& animation, const CCharString& phoneme) const
{
    std::map<CCharString, CArray<CLipSyncEntry> >::const_iterator it = LipSyncAnimations.find(animation);
    if (it != LipSyncAnimations.end()) {
        const CArray<CLipSyncEntry>& entries = it->second;
        for (unsigned long i = 0; i < entries.size(); ++i) {
            const CLipSyncEntry& entry = entries[i];
            if (entry.PhonemeName == phoneme)
                return entry.AnimationIndex;
        }
    }
    return 0;
}
// std::map::operator[](const CPendingObjectSortKey& k) -- STLport subscript of CThreadedFileManager's device-queue map<CPendingObjectSortKey, CFileObject*>: lower_bound the key, insert {key, null} at that hint when absent, return a reference to the mapped pointer
// Leaf: ??A?$map@VCPendingObjectSortKey@CThreadedFileManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00991350, 124 bytes (+ int3 padding), __fastcall `ret 4`.
// Identity: twin of the landed map<unsigned long, CFileObject*>::operator[] 0x009913D0 (its header names this row); callers
//   CThreadedFileManager::SetObjectPriority 0x0098EFF0, AddObject 0x0098F0F0 and 0x0098EC90. Key layout and operator<
//   (FableWin.pdb `CPendingObjectSortKey::operator<(const CPendingObjectSortKey&) const`) as the landed _M_lower_bound
//   0x009901D0 models them.
// Callees: _M_lower_bound @ 0x009901D0 (landed; node pointer in eax), insert_unique(iterator, const value_type&) @ 0x00990DC0
//   (the hint insert; iterator built in its slot, result through the dead `k` slot).
// Shape: STLport 4.6 map::operator[] (`i = lower_bound(k); if (i == end() || key_comp()(k, i->first)) i = insert(i,
//   value_type(k, T())); return i->second;`) with the sort-key operator< inline (`jb`/`ja` on the physical sort keys,
//   then the file positions, then `setb` on the object addresses). The operator< reads both physical sort keys into
//   named locals first, as the landed find 0x0098FE70 spells it (k's chain loads before the node's; the direct
//   expression form interleaves the two chains). Chunk-422 recipe as in the twin: a same-TU noinline
//   stand-in for insert_unique that reads v but never keeps its address lets VC7.1 store the pair's zero second after the
//   hidden-result push, as retail.
#include <stddef.h>

struct CThreadedFile {
    unsigned char _pad00[0x14];
    unsigned long PhysicalSortKey;             // +0x14
};
struct CThreadedFileObjectDesc {
    void*         PDst;                        // +0x00
    unsigned long FilePos;                     // +0x04
    unsigned char _pad08[0x10];
};
class CThreadedFileManager {
public:
    class CFileObject {                        // a class in the PDB name sets
    public:
        CThreadedFileObjectDesc Desc;          // +0x00
        unsigned long  UID;                    // +0x18
        long           LoadingStatus;          // +0x1C
        void*          Overlapped;             // +0x20
        CThreadedFileManager* PManager;        // +0x24
        CThreadedFile* PFile;                  // +0x28
    };
    class CPendingObjectSortKey {
    public:
        CFileObject* Object;
        bool operator<(const CPendingObjectSortKey& o) const
        {
            unsigned long key = Object->PFile->PhysicalSortKey;
            unsigned long otherKey = o.Object->PFile->PhysicalSortKey;
            if (key < otherKey)
                return true;
            if (key > otherKey)
                return false;
            if (Object->Desc.FilePos < o.Object->Desc.FilePos)
                return true;
            if (Object->Desc.FilePos > o.Object->Desc.FilePos)
                return false;
            return Object < o.Object;
        }
    };
};

namespace std {

template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template <class T> class allocator { };

template <class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};

template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;                       // +0x10
};

template <class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) { }
    Value& operator*() const { return static_cast<_Rb_tree_node<Value>*>(_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& other) const { return _M_node == other._M_node; }
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;              // +0x00
    size_t _M_node_count;                       // +0x04
    Compare _M_key_compare;                     // +0x08

    iterator end() { return iterator(_M_header); }
    Compare key_comp() const { return _M_key_compare; }
    _Rb_tree_node_base* _M_lower_bound(const Key& k) const;    // 0x009901D0
    iterator lower_bound(const Key& k) { return iterator(_M_lower_bound(k)); }
    iterator insert_unique(iterator position, const Value& v);   // 0x00990DC0
};

#pragma auto_inline(off)
// Same-TU stand-in for insert_unique(hint, v) 0x00990DC0 (reads v, never keeps its address).
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
__declspec(noinline) typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator position, const Value& v)
{
    if (v.second == 0)
        ++_M_node_count;
    return position;
}
#pragma auto_inline(on)

template <class Key, class T, class Compare = less<Key>, class Alloc = allocator<pair<const Key, T> > > class map {
public:
    typedef Key key_type;
    typedef pair<const Key, T> value_type;
    typedef _Rb_tree<Key, value_type, _Select1st<value_type>, Compare, Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;

    iterator lower_bound(const key_type& k) { return _M_t.lower_bound(k); }
    iterator end() { return _M_t.end(); }
    Compare key_comp() const { return _M_t.key_comp(); }
    iterator insert(iterator position, const value_type& v) { return _M_t.insert_unique(position, v); }
    T& operator[](const key_type& k)
    {
        iterator i = lower_bound(k);
        // i->first is greater than or equivalent to k.
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }
};

}   // namespace std

template CThreadedFileManager::CFileObject*& std::map<CThreadedFileManager::CPendingObjectSortKey, CThreadedFileManager::CFileObject*>::operator[](const CThreadedFileManager::CPendingObjectSortKey&);
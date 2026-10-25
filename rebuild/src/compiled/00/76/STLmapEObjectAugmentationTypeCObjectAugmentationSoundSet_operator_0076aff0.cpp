// std::map<EObjectAugmentationType, CObjectAugmentationSoundSet>::operator[](const key_type& k) -- STLport subscript on an augmentation sound map: lower_bound, insert (k, CObjectAugmentationSoundSet()) when absent, return the sound set
// Leaf: ??A?$map
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076AFF0, 183 bytes, `ret 4`.
// Callers: 0x00766C63 and 0x00766D06 (CTCObjectAugmentations, on InitSounds / LoopingSounds).
// Callees: CCharString(const char*, long) 0x0099EBF0 with the shared empty literal 0x0122D70E (the inline
//   CObjectAugmentationSoundSet default constructor, as the landed out-of-line 0x00765CC0), CCharString copy
//   constructor 0x0099EC30 (the value_type temporary), _Rb_tree::insert_unique(iterator, const value_type&) 0x0076A700
//   (hinted insert, iterator returned through the hidden pointer), ~CCharString 0x0099EAE0 (twice: the temporary pair, then
//   the default sound set).
// Identity: Ego_r.pdb CTCObjectAugmentations::InitSounds / LoopingSounds = map<EObjectAugmentationType,
//   CObjectAugmentationSoundSet>; the value is node + 0x14 (key at node + 0x10, signed `jl`/`jge` compares).
// Shape: STLport 4.x `map::operator[]` verbatim (the landed 0x005B3100 model in a speed TU): the lower_bound walk
//   is inlined; `_Tp()` is a named local of the insert block (Played = false stored before the string constructor,
//   Handle = -1 after it), the value_type temporary below it, the iterator argument built in its slot and the
//   insert result written into the dead `k` slot. map::insert is STLport's inline forwarder to the tree's
//   insert_unique, so the result is re-read from that slot (`mov esi,[esp+2Ch]`, the landed 0x009AC530 lesson).
enum EObjectAugmentationType { OBJECT_AUGMENTATION_TYPE_NONE };

class CCharString {
public:
    void* PStringData;                                      // +0x00
    CCharString(const char* text, long length);             // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

struct CObjectAugmentationSoundSet {                        // Ego_r.pdb, 12 bytes
    bool          Played;                                   // +0x00
    CCharString   Sound;                                    // +0x04
    unsigned long Handle;                                   // +0x08
    CObjectAugmentationSoundSet() : Played(false), Sound("", -1), Handle(0xFFFFFFFF) { }   // 0x00765CC0 out of line
};

namespace std {

template <class T> struct less {
    less() { }
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template <class _T1, class _T2>
class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};

struct _Rb_tree_node_base {
    long                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};

template <class _Value>
struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y) const { return _M_node == y._M_node; }
};

template <class V> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    V& operator*() const { return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
};

template <class K, class T, class C = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned int        _M_node_count;                      // +0x04
    C                   _M_key_compare;                     // +0x08
    iterator insert_unique(iterator position, const value_type& v);   // 0x0076A700 (_Rb_tree::insert_unique)
    iterator insert(iterator position, const value_type& v) { return insert_unique(position, v); }
    iterator lower_bound(const K& k)
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        while (x != 0)
            if (!_M_key_compare(static_cast<_Node*>(x)->_M_value_field.first, k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return iterator(y);
    }
    iterator end() { return iterator(_M_header); }
    C key_comp() const { return _M_key_compare; }
    T& operator[](const K& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first)) {
            T def;
            i = insert(i, value_type(k, def));
        }
        return (*i).second;
    }
};

template CObjectAugmentationSoundSet& map<EObjectAugmentationType, CObjectAugmentationSoundSet>::operator[](const EObjectAugmentationType&);

}
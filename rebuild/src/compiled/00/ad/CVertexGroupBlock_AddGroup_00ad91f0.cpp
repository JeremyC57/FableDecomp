// CVertexGroupBlock::AddGroup(unsigned long group) -- append a bone group to the block and record its index in the group -> index map
// Leaf: ?AddGroup@CVertexGroupBlock@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD91F0, 0x62 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: Ego_r.pdb ?AddGroup@CVertexGroupBlock@@QAEXK@Z; retail layout as the landed CVertexGroupBlock::IsSubsetOf
// 0x00AD8C30 (Groups vector<unsigned long> +0x00, GroupToIndexMap map<unsigned long, long> +0x0C). Callers: the mesh
// code at 0x00AA6D3C, CVertexGroupBlock::AddGroupsFrom 0x00AD9260 and CVertexGroupBlockOptimiser::InsertBlock 0x00AD92E0.
// Catalog "AnimBlockGroup_AddBlock" is descriptive.
// Callees: vector<unsigned long>::_M_insert_overflow(pos, x, __true_type, fill_len, at_end) @ 0x00464ACE (as in the
// landed 0x00AD9D70), map<unsigned long, long>::insert -> _Rb_tree::insert_unique(const value_type&) @ 0x0045CA09
// (pair<iterator,bool> through the hidden pointer).
// Shape: STLport push_back (`if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
// _M_insert_overflow(_M_finish, x, __true_type(), 1, true);` -- x binds to the parameter's own slot, the empty tag is
// homed at that same slot) then `GroupToIndexMap.insert(value_type(group, Groups.size() - 1))` with the pair built
// after the argument pushes.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);

namespace std {

struct __true_type { __true_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template <class T1, class T2> class pair {
public:
    T1 first;
    T2 second;
    pair() { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> class less { };

template <class T> class vector {
public:
    T* _M_start;                                            // +0
    T* _M_finish;                                           // +4
    T* _M_end_of_storage;                                   // +8 (alloc proxy)
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(T* __position, const T& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x00464ACE (same-TU body below)
    void push_back(const T& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
};

#pragma auto_inline(off)
// Same-TU stand-in for the out-of-line vector<unsigned long>::_M_insert_overflow (retail 0x00464ACE, another TU's
// COMDAT): STLport's trivial-copy reallocation. It never touches the __true_type tag, which lets VC7.1 home the
// caller's empty tag on the live `group` argument slot exactly as retail does.
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* __position, const T& __x, const __true_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);
    T* __new_start = __len != 0 ? (T*)malloc(__len * sizeof(T)) : 0;
    memmove(__new_start, _M_start, (char*)__position - (char*)_M_start);
    T* __new_finish = __new_start + (__position - _M_start);
    for (unsigned int __n = __fill_len; __n > 0; --__n, ++__new_finish)
        *__new_finish = __x;
    if (!__atend) {
        memmove(__new_finish, __position, (char*)_M_finish - (char*)__position);
        __new_finish += _M_finish - __position;
    }
    if (_M_start != 0)
        free(_M_start);
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage = __new_start + __len;
}
#pragma auto_inline(on)

struct _Rb_tree_node_base;
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(const _Rb_tree_iterator& x) : _M_node(x._M_node) { }
};

template <class K, class T, class C = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned int        _M_node_count;                      // +0x04
    C                   _M_key_compare;                     // +0x08
    pair<iterator, bool> insert_unique(const value_type& x);   // 0x0045CA09
    pair<iterator, bool> insert(const value_type& x) { return insert_unique(x); }
};

} // namespace std

class CVertexGroupBlock {
public:
    std::vector<unsigned long>    Groups;                  // +0x00
    std::map<unsigned long, long> GroupToIndexMap;         // +0x0C
    void AddGroup(unsigned long group);
};

void CVertexGroupBlock::AddGroup(unsigned long group)
{
    Groups.push_back(group);
    GroupToIndexMap.insert(std::map<unsigned long, long>::value_type(group, Groups.size() - 1));
}
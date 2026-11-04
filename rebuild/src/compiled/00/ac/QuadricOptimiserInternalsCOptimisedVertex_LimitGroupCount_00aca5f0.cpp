// QuadricOptimiserInternals::COptimisedVertex::LimitGroupCount(unsigned long maxGroups) -- keep at most maxGroups bone groups on the vertex: copy the Groups map into a vector, sort it by descending weight (CGroup::CompareWeight), when there are more than maxGroups renormalise the first maxGroups weights to sum to one, then rebuild the map from the kept entries
// Leaf: ?LimitGroupCount@COptimisedVertex@QuadricOptimiserInternals@@QAEXK@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ACA5F0, 570 bytes, thiscall `ret 4`, 0x18-byte frame; the STLport helpers are __fastcall
//   (/Gr, as the landed QuadricOptimiser rows).
// Identity (two sources): (1) `this` is the landed QuadricOptimiserInternals::COptimisedVertex (PDB layout: Groups, a
//   std::map<unsigned long, float> at +0x9C with its node count at +0xA0; PairContractions +0xA8; 0xB8 bytes) and the body is the
//   PDB method ?LimitGroupCount@COptimisedVertex@QuadricOptimiserInternals@@QAEXK@Z; (2) its callees are the landed
//   vector<CGroup>::reserve 0x00AD4010 / _M_insert_overflow 0x00AD4E20 (element type CGroup = {BoneIndex, BoneWeight}),
//   std::sort 0x00AD7550 with the landed CGroup::CompareWeight 0x00AC6B40 (descending weight), the map's _M_erase 0x00AD14E0
//   (inlined clear() loop) and operator[] 0x00AD6420, malloc / free. The catalog's QuadricOptimiser_NormalizeQuadricWeights is
//   descriptive.
// Shape (10 probes, probe/00aca5f0.log): two counters -- `count` (the map size, compared with maxGroups) and `newCount` (what
//   the rebuilt map keeps, homed at +0x10 from the start); the data pointer is taken once before the sort (`first`) and used by
//   every loop, which is what keeps it in ebx across the sort and the map inserts; and the two out-of-line vector members are
//   given same-TU bodies that do not leak `this` (reserve copies with placement new, the growth stand-in only reads x) --
//   without that VC7.1 treats the local vector as escaped and re-reads _M_finish after the element copy and _M_start for the
//   destructor (574v570).
#include <stddef.h>
inline void* operator new(size_t, void* where) { return where; }
extern "C" void* __cdecl malloc(size_t size);           // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    typedef unsigned int size_type;
    typedef _Tp* iterator;
    typedef __false_type _TrivialCopy;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    void _M_clear() { this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start); }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e) { this->_M_start = __s; this->_M_finish = __f; this->_M_end_of_storage._M_data = __e; }
    void reserve(size_type __n);                                                                // 0x00AD4010 (same-TU body below)
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);   // 0x00AD4E20
    void push_back(const _Tp& __x) {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, _TrivialCopy(), 1UL, true);
    }
};
#pragma auto_inline(off)
// Same-TU bodies for the two out-of-line vector members retail instantiated in this TU (reserve 0x00AD4010, the growth
// 0x00AD4E20): VC7.1 sees that neither lets `this` escape, so the local vector stays unaliased in the caller (its data
// pointer survives the sort and the map inserts in a register, and `_M_finish` is not re-read after the element copy).
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::reserve(size_type __n)
{
    if (capacity() < __n) {
        const size_type __old_size = size();
        _Tp* __tmp = this->_M_end_of_storage.allocate(__n);
        _Tp* __d = __tmp;
        for (_Tp* __s = this->_M_start; __s != this->_M_finish; ++__s, ++__d)
            _Construct(__d, *__s);
        _M_clear();
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend)
{
    if (*(const unsigned long*)&__x == 0 && *((const unsigned long*)&__x + 1) == 0 && __atend) this->_M_finish = __position + __fill_len;
}
#pragma auto_inline(on)

template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00AD7550

typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base {
    _Rb_tree_Color_type _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Rb_tree_node_base_ptr;
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* __x) {
        if (__x->_M_right != 0) {
            __x = __x->_M_right;
            while (__x->_M_left != 0)
                __x = __x->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = __x->_M_parent;
            while (__x == __y->_M_right) {
                __x = __y;
                __y = __y->_M_parent;
            }
            if (__x->_M_right != __y)
                __x = __y;
        }
        return __x;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Value, class _Traits> struct _Rb_tree_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Value* operator->() const { return &(operator*()); }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};
template <class _Value> struct _Nonconst_traits { };
template <class _Tp, class _Traits1, class _Traits2>
inline bool __fastcall operator==(const _Rb_tree_iterator<_Tp, _Traits1>& __x, const _Rb_tree_iterator<_Tp, _Traits2>& __y) { return __x._M_node == __y._M_node; }
template <class _Tp, class _Traits1, class _Traits2>
inline bool __fastcall operator!=(const _Rb_tree_iterator<_Tp, _Traits1>& __x, const _Rb_tree_iterator<_Tp, _Traits2>& __y) { return __x._M_node != __y._M_node; }

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef unsigned int size_type;
    _STLP_alloc_proxy<_Base_ptr, _Node, allocator<_Node> > _M_header;   // +0x00
    size_type _M_node_count;                                             // +0x04
    _Compare  _M_key_compare;                                            // +0x08
    _Base_ptr& _M_root() const { return _M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return _M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return _M_header._M_data->_M_right; }
    static _Base_ptr& _S_left(_Base_ptr __x) { return __x->_M_left; }
    static _Base_ptr& _S_right(_Base_ptr __x) { return __x->_M_right; }
    void _M_destroy_node(_Node* __p) {
        _M_header.deallocate(__p, 1);
    }
    void _M_erase(_Base_ptr __x);                                        // 0x00AD14E0
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(_M_header._M_data); }
    size_type size() const { return _M_node_count; }
    void clear() {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = _M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = _M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Base_ptr __x)
{
    // erase without rebalancing
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Base_ptr __y = _S_left(__x);
        _M_destroy_node((_Node*)__x);
        __x = __y;
    }
}
template <class _Pair> struct _Select1st { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    typedef unsigned int size_type;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    size_type size() const { return _M_t.size(); }
    void clear() { _M_t.clear(); }
    _Tp& operator[](const _Key& __k);                                    // 0x00AD6420
};
}

#include "engine/C3DVector.h"
namespace QuadricOptimiserInternals {
class CQuadric { public: double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight; };   // 0x58
class CIndexList { public: unsigned char Indices[0xc]; };           // std::set<unsigned long> (STLport, 12 bytes)
class CContractionReference;
class COptimisedVertex {                                           // Ego_r.pdb layout, STLport sizes (0xb8 bytes)
public:
    class CGroup {
    public:
        unsigned long BoneIndex;                                   // +0x00
        float         BoneWeight;                                  // +0x04
        CGroup(unsigned long index, float weight) : BoneIndex(index), BoneWeight(weight) { }
        static bool __fastcall CompareWeight(const CGroup& a, const CGroup& b);   // 0x00AC6B40
    };
    C3DVector     Position;                                        // +0x00
    C3DVector     Normal;                                          // +0x0c
    C3DVector     TextureCoord;                                    // +0x18
    CQuadric      PositionError;                                   // +0x28
    float         Cost;                                            // +0x80
    CIndexList    FaceList;                                        // +0x84
    bool          Exists;                                          // +0x90
    unsigned long UniqueIndex;                                     // +0x94
    unsigned long OutputIndex;                                     // +0x98
    std::map<unsigned long, float> Groups;                         // +0x9c
    std::map<unsigned long, CContractionReference> PairContractions;   // +0xa8
    void LimitGroupCount(unsigned long maxGroups);
};

void COptimisedVertex::LimitGroupCount(unsigned long maxGroups)
{
    unsigned long count = Groups.size();
    unsigned long newCount = count;
    std::vector<CGroup> groups;
    groups.reserve(count);
    for (std::map<unsigned long, float>::iterator it = Groups.begin(); it != Groups.end(); ++it)
        groups.push_back(CGroup(it->first, it->second));
    CGroup* first = groups.begin();
    std::sort(first, groups.end(), CGroup::CompareWeight);
    if (count > maxGroups)
    {
        float total = 0.0f;
        for (unsigned long i = 0; i < maxGroups; ++i)
            total += first[i].BoneWeight;
        float scale = 1.0f / total;
        for (unsigned long i = 0; i < maxGroups; ++i)
            first[i].BoneWeight *= scale;
        newCount = maxGroups;
    }
    Groups.clear();
    for (unsigned long i = 0; i < newCount; ++i)
        Groups[first[i].BoneIndex] = first[i].BoneWeight;
}
}
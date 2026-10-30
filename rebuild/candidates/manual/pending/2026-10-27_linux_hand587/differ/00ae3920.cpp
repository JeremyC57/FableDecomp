// CParticleProgram::RemapBoneIndices(std::map<unsigned long, unsigned long>& remap, CArray<C3DGroup2>& groups) -- keep only the skinning groups whose bone is in the remap table, each with its bone index rewritten through the table, and assign the survivors back to the array
// Grade: DIFFER(266v270) -- two residues: (1) retail stores the unsigned long key temporary [esp+18h] before the inlined find and reuses it for operator[], ours stores it at the operator[] call; (2) the local array destructor keeps _M_start in ebx and compares with the zero in ebp, ours reloads it into ecx after each free
// Leaf: ?RemapBoneIndices@CParticleProgram@@CIXAAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@AAV?$CArray@VC3DGroup2@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE3920, 270 bytes, private static __fastcall (remap in ecx, groups in edx), `ret`.
// Identity (proven): Ego_r.pdb and FableWin.pdb both list the pair ?RemapBoneIndices@CParticleProgram@@QAEXAAV?$map@KK...@Z
//   (the landed 0x00AE3A30, which calls this row twice: SimGroups, NonSimGroups) and this private static `CI` overload
//   taking (map<unsigned long, unsigned long>&, CArray<C3DGroup2>&); the registers fit (`mov ebx,edx` = groups, ecx
//   spilled = remap) and the body walks C3DGroup2 records (16 bytes, BoneIndex +0, VertexBlends +4).
// Callees: vector<C3DGroup2>::reserve 0x00AE80F0 (landed), map<unsigned long, unsigned long>::operator[] 0x00553E60
//   (landed), C3DGroup2::SetBoneIndex 0x00AC4070 (Ego_r PDB name, bound by the landed AddGroup 0x00AE37D0),
//   CArray<C3DGroup2>::push_back 0x00AE83D0 (Ego_r PDB name), vector<C3DGroup2>::operator= 0x00AE7FE0 (landed),
//   free 0x00BFEA14 (the local array's destructor: each group's blend block, then the group block).
// Shape: `CArray<C3DGroup2> new_groups; new_groups.reserve(groups.size()); for (i < groups.size()) { if
//   (remap.find(group.BoneIndex) != remap.end()) { group.SetBoneIndex(remap[group.BoneIndex]); new_groups.push_back(group); } }
//   groups = new_groups;` with the STLport 4.6 map::find (_Rb_tree::_M_find walk, unsigned `jb`) inlined and the
//   long -> unsigned long key temporary in the third frame word (shared by find and operator[]); the implicit
//   CArray assignment forwards to vector::operator=, the local's destructor (vector -> _Destroy -> ~C3DGroup2 ->
//   deallocate) is inlined.
// Model: real STLport 4.6 templates in namespace std (map over _Rb_tree, vector over _Vector_base +
//   _STLP_alloc_proxy); CArray<T> derives from std::vector<T> (retail 12 bytes). engine/CParticleProgram.h is included
//   under a layout name and the class derives from it to declare the static (the header carries no member functions).
extern "C" void __cdecl free(void* p);                                 // 0x00BFEA14

#define CParticleProgram CParticleProgram_HeaderLayout
#include "engine/CParticleProgram.h"
#undef CParticleProgram

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
};
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }

template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    static const _Key& _S_key(_Link_type __x) { return _KeyOfValue()(__x->_M_value_field); }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    iterator end() { return iterator(this->_M_header._M_data); }
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
private:
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const
    {
        _Link_type __y = this->_M_header._M_data;      // last node which is not less than __k
        _Link_type __x = _M_root();                    // current node
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
            __y = this->_M_header._M_data;
        return __y;
    }
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator end() { return _M_t.end(); }
    iterator find(const key_type& __x) { return _M_t.find(__x); }
    _Tp& operator[](const key_type& __k);                                    // 0x00553E60
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);         // 0x00AE7FE0 (C3DGroup2)
    void reserve(size_type __n);                                             // 0x00AE80F0 (C3DGroup2)
};
}

class C3DVertexBlend2 { public: long VertexIndex; float BlendValue; };   // 8 bytes

template <class T> class CArray : public std::vector<T> {   // retail CArray<T>: an STLport vector (12 bytes)
public:
    void push_back(const T& x);                                              // 0x00AE83D0 (C3DGroup2)
};

class C3DGroup2 { public:                                       // 16 bytes in retail
    long                     BoneIndex;                  // +0x0
    CArray<C3DVertexBlend2>  VertexBlends;               // +0x4
    void SetBoneIndex(long bone);                        // 0x00AC4070
};

typedef std::map<unsigned long, unsigned long> CBoneRemap;

class CParticleProgram : public CParticleProgram_HeaderLayout {
public:
    void RemapBoneIndices(CBoneRemap& remap);                                // 0x00AE3A30
private:
    static void __fastcall RemapBoneIndices(CBoneRemap& remap, CArray<C3DGroup2>& groups);
};

void __fastcall CParticleProgram::RemapBoneIndices(CBoneRemap& remap, CArray<C3DGroup2>& groups)
{
    CArray<C3DGroup2> new_groups;
    new_groups.reserve(groups.size());
    for (unsigned long i = 0; i < groups.size(); ++i) {
        C3DGroup2& group = groups[i];
        if (remap.find(group.BoneIndex) != remap.end()) {
            group.SetBoneIndex(remap[group.BoneIndex]);
            new_groups.push_back(group);
        }
    }
    groups = new_groups;
}

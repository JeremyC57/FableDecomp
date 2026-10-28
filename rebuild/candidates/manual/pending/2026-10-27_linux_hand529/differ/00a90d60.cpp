// N3DPrimitive::CDeviceIndependent::RemapBoneIndices(std::map<unsigned long, unsigned long>& boneMap) -- rebuilds the primitive's group array in the new bone order: each mapped old group moves to its new index (and is told its new bone index), the array is cut or padded to the highest new index + 1, then replaces Groups
// Grade: DIFFER(445v413) -- retail never releases resize()'s zero CCountedPointer temporary (VC elided its destructor: /Oa reproduces the elision but reshuffles the frame, 469v413) and keeps &Groups in ebp where ours uses edi; retail's out-of-line 0x00A985A0 proves this STLport build's resize takes const _Tp& (not 4.6's by-value _Tp)
// Leaf: ?RemapBoneIndices@CDeviceIndependent@N3DPrimitive@@AAEXAAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A90D60, 413 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?RemapBoneIndices@CDeviceIndependent@N3DPrimitive@@AAEXAAV?$map@KK...@std@@@Z (private); the
//   only caller is C3DPrimitive2::RemapBoneIndices 0x00A90F00, which forwards its map to `Data->RemapBoneIndices(map)`
//   (C3DPrimitive2::Data, scoped_ptr<N3DPrimitive::CDeviceIndependent>, at +0x2C). this+0x1C is
//   CDeviceIndependent::Groups (CArray<CCountedPointer<C3DGroup2> >; Ego_r +0x24 minus 4 per Dinkumware vector before it),
//   and the element call is C3DGroup2::SetBoneIndex(long) 0x00AC4070. The catalog's
//   "AnimationSequence_RemapBoneRefs" is descriptive.
// Callees: vector<CCountedPointer<C3DGroup2> >::vector(unsigned int) 0x00A967A0, C3DGroup2::SetBoneIndex 0x00AC4070,
//   operator delete 0x00BFE9BC (inline counted-pointer release), std::__copy 0x00A95C00 and std::__destroy_aux 0x00A969D0
//   (/Gr __fastcall; erase / the local's destructor), vector::_M_fill_insert 0x00A97F70, vector::operator= 0x00A97690,
//   free 0x00BFEA14.
// Shape: STLport 4.6 text: map iteration with _Rb_global<bool>::_M_increment defined in the TU (VC inlines it),
//   `resize(n)` = `resize(n, _Tp())` (erase or _M_fill_insert with the zero temporary), the empty tags homed on the dead
//   map-argument slot through same-TU noinline __copy / __destroy_aux bodies.
extern "C" void __cdecl free(void*);                       // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC

class CCPPointerInfo {
public:
    unsigned long RefCount;                                // +0
    void (__fastcall* DeleteFunc)(void* data);             // +4
    void* Data;                                            // +8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0
    CCPPointerInfo* Info;                                  // +4
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer<T>& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { Release(); Data = 0; Info = 0; }
    T* operator->() const { return Data; }
    CCountedPointer<T>& operator=(const CCountedPointer<T>& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    void Release()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
};
class C3DGroup2 {
public:
    void SetBoneIndex(long index);                         // 0x00AC4070
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: void deallocate(_Tp* __p, unsigned int) { free(__p); } };

#pragma auto_inline(off)
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)        // 0x00A95C00
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{                                                          // 0x00A969D0
    for (; __first != __last; ++__first)
        (&*__first)->~CCountedPointer();
}
#pragma auto_inline(on)
template <class _Tp>
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                               _BidirectionalIter2 __result, const random_access_iterator_tag&, _Distance*);
template <class _Tp>
inline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef _Tp& reference;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    explicit vector(size_type __n);                        // 0x00A967A0
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // 0x00A97690
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    bool empty() const { return this->_M_start == this->_M_finish; }
    reference operator[](size_type __n) { return *(begin() + __n); }
    typedef __false_type _IsPODType;
    // 0x00A97F70: same-TU body of the retail COMDAT (STLport 4.6 _vector.c text) so VC sees that the zero temporary
    // of resize() is only read; its own callees stay declarations.
    __declspec(noinline) void _M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
    {
        if (__n != 0) {
            if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
                _Tp __x_copy = __x;
                const size_type __elems_after = this->_M_finish - __position;
                pointer __old_finish = this->_M_finish;
                if (__elems_after > __n) {
                    __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                    this->_M_finish += __n;
                    __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                    fill(__position, __position + __n, __x_copy);
                }
                else {
                    uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                    this->_M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                    this->_M_finish += __elems_after;
                    fill(__position, __old_finish, __x_copy);
                }
            }
            else
                _M_insert_overflow(__position, __x, _IsPODType(), __n);
        }
    }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
public:
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    __forceinline iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    __forceinline void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    __forceinline void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                    // +0x00
    _Base_ptr _M_parent;                                   // +0x04
    _Base_ptr _M_left;                                     // +0x08
    _Base_ptr _M_right;                                    // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                 // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* __x);
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value> struct _Nonconst_traits { typedef _Value& reference; typedef _Value* pointer; };
template <class _Value, class _Traits>
struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    typename _Traits::pointer operator->() const { return &((_Link_type)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _STLP_alloc_proxy<_Rb_tree_node<_Value>*, _Rb_tree_node<_Value>, _Alloc> _M_header;   // +0x00
    unsigned int _M_node_count;                                                         // +0x04
    _Compare _M_key_compare;                                                            // +0x08
    iterator begin() { return iterator(_M_header._M_data->_M_left); }
    iterator end() { return iterator(_M_header._M_data); }
    unsigned int size() const { return _M_node_count; }
};
template <class _Pair> struct _Select1st { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    unsigned int size() const { return _M_t.size(); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T> {
public:
    explicit CArray(unsigned int n) : std::vector<T>(n) { }
};

namespace N3DPrimitive {
class CDeviceIndependent {
public:
    virtual ~CDeviceIndependent();
private:
    void RemapBoneIndices(std::map<unsigned long, unsigned long>& boneMap);
    unsigned char Vertices[0x0C];                          // +0x04 CArray<C3DVertex2> (not touched here)
    unsigned char TriangleLists[0x0C];                     // +0x10 CArray<CIVCountedPointer<CTriangleList> > (not touched)
    CArray<CCountedPointer<C3DGroup2> > Groups;            // +0x1C
};

void CDeviceIndependent::RemapBoneIndices(std::map<unsigned long, unsigned long>& boneMap)
{
    if (Groups.empty())
        return;
    CArray<CCountedPointer<C3DGroup2> > newGroups(boneMap.size());
    unsigned long maxIndex = 0;
    for (std::map<unsigned long, unsigned long>::iterator it = boneMap.begin(); it != boneMap.end(); ++it) {
        unsigned long oldIndex = it->first;
        unsigned long newIndex = it->second;
        if (oldIndex < Groups.size()) {
            newGroups[newIndex] = Groups[oldIndex];
            newGroups[newIndex]->SetBoneIndex(newIndex);
            if (newIndex >= maxIndex)
                maxIndex = newIndex;
        }
    }
    newGroups.resize(maxIndex + 1);
    Groups = newGroups;
}
}   // namespace N3DPrimitive

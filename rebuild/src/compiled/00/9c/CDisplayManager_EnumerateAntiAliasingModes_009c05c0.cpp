// CDisplayManager::EnumerateAntiAliasingModes(CArray<ESurfaceMultisampleType>& types) -- collect every multisample type that adapter 0 supports (D3D_OK from CheckDeviceMultiSampleType, full-screen) for any back-buffer format into a sorted unique set, then copy the set into types
// Leaf: ?EnumerateAntiAliasingModes@
// Grade: RELOCATION_MATCH -- the multisample type read into a named local ahead of the CheckDeviceMultiSampleType call
//   fixes the argument-setup register plan (sec5 retry)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C05C0, 693 bytes, thiscall `ret 4`. Callers: seven game-side sites
//   (0x004090BA .. 0x0059933E). Name and signature: Ego_r.pdb
//   ?EnumerateAntiAliasingModes@CDisplayManager@@QAEXAAV?$CArray@W4ESurfaceMultisampleType@@@@@Z. The chunk label
//   "CGuiControlTreePane_RebuildTreeFromArray" is wrong.
// Layout (retail; no engine/ header): CDisplayManager D3D (IDirect3D9*) +0x54, D3DDeviceCaps.DeviceType +0x5C,
//   BackBufferFormats CArray<CPixelFormat> +0x298, MultisampleTypes CArray<ESurfaceMultisampleType> +0x2A4 (Ego_r.pdb
//   +0x2A0 / +0x2B0: the PDB's Dinkumware containers are larger). CPixelFormat = {FormatIndex} into the 36-byte pixel-format
//   table at 0x0129BA40 (D3DFORMAT at +0).
// Callees: IDirect3D9::CheckDeviceMultiSampleType (slot 11), set<ESurfaceMultisampleType>::insert (_Rb_tree::insert_unique
//   @ 0x009C29B0), _Rb_tree::_M_erase @ 0x009C1E00 (the recursive right-subtree call; the first level is inline),
//   vector::reserve @ 0x009C2780, 0x00BFEA0E malloc, 0x00BFEA14 free.
// Shape: types.clear() is inline here (STLport erase(first, last) with the counted __copy loop: an enum is not trivially
//   assignable to STLport's type traits); push_back and its __false_type _M_insert_overflow are inline (`__forceinline`),
//   as in EnumerateDisplayModes 0x009C02D0 from the same TU.
extern "C" void* __cdecl malloc(unsigned int size);                 // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                          // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};

template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    void reserve(size_type __n);                                // 0x009C2780 for ESurfaceMultisampleType
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = copy(__last, this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
    __forceinline void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template <class _Tp, class _Alloc>
__forceinline void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};

struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator<_Value>& operator++() { _M_node = _Rb_global::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator<_Value>& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Alloc>
class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Tp> _Node;
    _Rb_tree_base(const _Alloc&) : _M_header(allocator<_Node>(), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    _Rb_tree(const _Compare& __comp, const _Alloc& __a)
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp)
    {
        _M_empty_initialize();
    }
    ~_Rb_tree() { clear(); }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Rb_tree_node_base* __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Rb_tree_node_base* __x) { return (_Link_type&)(__x->_M_right); }
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }
    unsigned int size() const { return _M_node_count; }
    pair<iterator, bool> insert_unique(const _Value& __x);      // 0x009C29B0 for ESurfaceMultisampleType
    void _M_erase(_Link_type __x);
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x)
{
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        this->_M_header.deallocate(__x, 1);
        __x = __y;
    }
}

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    set() : _M_t(_Compare(), _Alloc()) {}
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    unsigned int size() const { return _M_t.size(); }
    pair<iterator, bool> insert(const _Key& __x) { return _M_t.insert_unique(__x); }
    _Rep_type _M_t;
};

}

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };

enum ESurfaceMultisampleType { SURFACE_MULTISAMPLE_NONE = 0 };

struct CPixelFormatInfo {                           // 36 bytes
    long D3DFormat;                                 // +0x00
    long Info[8];
};
extern CPixelFormatInfo g_PixelFormatTable[];       // 0x0129BA40

class CPixelFormat {
public:
    long FormatIndex;
    long GetD3DFormat() const { return g_PixelFormatTable[FormatIndex].D3DFormat; }
};

struct IDirect3D9 {
    virtual long __stdcall QueryInterface(void*, void**) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
    virtual long __stdcall RegisterSoftwareDevice(void*) = 0;
    virtual unsigned int __stdcall GetAdapterCount() = 0;
    virtual long __stdcall GetAdapterIdentifier(unsigned int, unsigned long, void*) = 0;
    virtual unsigned int __stdcall GetAdapterModeCount(unsigned int, long) = 0;
    virtual long __stdcall EnumAdapterModes(unsigned int, long, unsigned int, void*) = 0;
    virtual long __stdcall GetAdapterDisplayMode(unsigned int, void*) = 0;
    virtual long __stdcall CheckDeviceType(unsigned int, long, long, long, int) = 0;
    virtual long __stdcall CheckDeviceFormat(unsigned int, long, long, unsigned long, long, long) = 0;
    virtual long __stdcall CheckDeviceMultiSampleType(unsigned int Adapter, long DeviceType, long SurfaceFormat,
                                                      int Windowed, long MultiSampleType, unsigned long* pQualityLevels) = 0;
};

struct D3DCAPS9 {
    long DeviceType;                                // +0x00
    unsigned int AdapterOrdinal;                    // +0x04
};

class CDisplayManager {
public:
    void EnumerateAntiAliasingModes(CArray<ESurfaceMultisampleType>& types);

    unsigned char _pad_000[0x54];
    IDirect3D9* D3D;                                // +0x54
    unsigned char _pad_058[0x5C - 0x58];
    D3DCAPS9 D3DDeviceCaps;                         // +0x5C (first members only)
    unsigned char _pad_064[0x298 - 0x64];
    CArray<CPixelFormat> BackBufferFormats;         // +0x298
    CArray<ESurfaceMultisampleType> MultisampleTypes;   // +0x2A4
};

void CDisplayManager::EnumerateAntiAliasingModes(CArray<ESurfaceMultisampleType>& types)
{
    std::set<ESurfaceMultisampleType> found;
    for (unsigned int i = 0; i < BackBufferFormats.size(); ++i) {
        for (unsigned int j = 0; j < MultisampleTypes.size(); ++j) {
            ESurfaceMultisampleType type = MultisampleTypes[j];
            if (D3D->CheckDeviceMultiSampleType(0, D3DDeviceCaps.DeviceType, BackBufferFormats[i].GetD3DFormat(), 0,
                                                type, 0) == 0)
                found.insert(MultisampleTypes[j]);
        }
    }
    types.clear();
    types.reserve(found.size());
    for (std::set<ESurfaceMultisampleType>::iterator it = found.begin(); it != found.end(); ++it)
        types.push_back(*it);
}
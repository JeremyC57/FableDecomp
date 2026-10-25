// std::vector<CGraphicInfoFrame>::_M_insert_overflow(CGraphicInfoFrame* pos, const CGraphicInfoFrame& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport grow path of a graphic's frame vector: allocate size + max(size, fill_len), copy the prefix, construct fill_len copies of x, copy the suffix unless atend, destroy (virtually) and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A00C80, 272 bytes, thiscall `ret 14h` (pos, &x, the tag reference, fill_len,
//   atend). Single caller 0x00A0108C (the vector's _M_fill_insert 0x00A00F50). The catalog label
//   "CGameMapCellArray_InsertElements" is a guess: the element is CGraphicInfoFrame (vtable 0x0129C878 stored by the
//   inline copy, as the landed __uninitialized_fill_n 0x00A004D0 and resize 0x00A016F0 say).
// Callees: __uninitialized_copy(first, last, result, const __false_type&) @ 0x00A00750 (`__fastcall`, `ret 8`),
//   __uninitialized_fill_n(first, n, x, const __false_type&) @ 0x00A004D0 (landed), the element's virtual destructor
//   (vtable slot 0, `??_G` 0x00A009A0, called with 0), malloc @ 0x00BFEA0E, free @ 0x00BFEA14.
// Layout: CGraphicInfoFrame (Ego_r.pdb, 0x10 bytes): CBaseClass vptr +0x00, FrameNum +0x04, SmallMipmapLevel +0x06,
//   Data CIVCountedPointer<CFrameData> +0x08 (copy = AddRef of RefCount +4), GraphicInfo +0x0C. The implicit copy
//   constructor stores only the derived vtable. CBaseClass's destructor is virtual (slot 0), so _Destroy's
//   `__pointer->~_Tp()` is a virtual call with a zero flag.
// Shape: STLport 4.6 `_M_insert_overflow(pos, x, const __false_type&, fill_len, atend)` verbatim (the 0x009B8DD0
//   sibling): max() takes the address of the fill_len argument, placement new keeps its null test, _M_clear = the
//   inline _Destroy loop + deallocate. The copy helpers are same-TU noinline bodies that never touch their tag, so
//   VC7.1 homes the __false_type temporaries on the atend argument slot.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CBaseClass {                                  // vtable 0x01231710
public:
    CBaseClass();                                   // 0x0099A2D0
    virtual ~CBaseClass();                          // slot 0 (0x0099A2E0)
};

template <class T>
class CIVCountedPointer {                           // intrusive: the object's RefCount at +4, virtual slot 1 frees it
public:
    T* Object;
    CIVCountedPointer() : Object(0) {}
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->AddRef();
    }
    ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

class CGraphicInfo;

class CGraphicInfoFrame : public CBaseClass {       // vtable 0x0129C878, 0x10 bytes
public:
    class CFrameData {
    public:
        virtual void v000();
        virtual void DeleteThis();                  // slot 1 (+0x04)
        long RefCount;                              // +0x04
        void AddRef() { ++RefCount; }
        void Release()
        {
            if (--RefCount == 0)
                DeleteThis();
        }
    };
    unsigned short FrameNum;                        // +0x04
    signed char SmallMipmapLevel;                   // +0x06
    CIVCountedPointer<CFrameData> Data;             // +0x08
    CGraphicInfo* GraphicInfo;                      // +0x0C
};

namespace std {
struct __false_type { __false_type() {} };

template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                 // 0x00A00750
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                 // 0x00A004D0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

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
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
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
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
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

template void vector<CGraphicInfoFrame>::_M_insert_overflow(CGraphicInfoFrame*, const CGraphicInfoFrame&,
                                                            const __false_type&, unsigned int, bool);
}
// std::vector::vector(unsigned int n) -- STLport vector<CEnginePrimitiveHandle>(size_type n) (size TU): allocate n slots, then copy a default-constructed handle into each
// Leaf: ??0?$vector@VCEnginePrimitiveHandle@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00485D30, 135 bytes, __fastcall `ret 4`, returns this; frame-pointer size TU.
// Label correction: the catalog's map<long, CEnginePrimitiveHandle>::operator[] is wrong: the body builds a vector, not a
//   tree lookup. Callers are member initialisers of CTCInGameMenu 0x0047DF8A, CSubtitleRenderer 0x0049166E (two),
//   CTCWallMountEffects 0x005C3947, CDrawBettingHUD 0x00657D60 and CHeroMorphDef 0x0082A460.
// Callees: _Vector_base(size_type, const allocator&) @ 0x00485779 (zeroes the three words, malloc(n * 8) when n != 0),
//   CBaseClass() @ 0x0099A2D0 / ~CBaseClass() @ 0x0099A2E0, __uninitialized_fill_n<CEnginePrimitiveHandle*, unsigned int,
//   CEnginePrimitiveHandle>(first, n, x, __false_type) @ 0x0043F6C0 (landed; __fastcall, first in ecx, n in edx); the
//   primitive's virtual slots 3 and 1 in the temporary's destructor.
// Shape: STLport 4.6 `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish =
//   uninitialized_fill_n(_M_start, n, _Tp()); }`: the allocator and __false_type tag temporaries share the high byte of
//   the `n` slot (`lea eax,[ebp+0Bh]`) because the _Vector_base ctor and __uninitialized_fill_n bodies are visible
//   (same-TU stand-ins under auto_inline(off): retail's 0x00485779 COMDAT sits beside this function, 0x0043F6C0 is the
//   landed template); the handle temporary (8 bytes) lives in the frame. Under this /Os TU the handle's ctor/dtor and
//   the counted-pointer helpers need `__forceinline` (plain inline: all out of line, 61v135), and the uniqueness test
//   reads the count through a null-tested ternary (`(Object ? Object->RefCount : 0) == 1`: the `xor eax,eax` arm, only
//   here and in 0x005BBFD6 in retail), unlike the `Object != 0 && RefCount == 1` of 0x00BBDCB0.
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    __forceinline void DecRef() { --RefCount; }
    __forceinline long GetRefCount() const { return RefCount; }
    __forceinline void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    __forceinline CIVCountedPointer() : Object(0) { }
    __forceinline CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    __forceinline long GetRefCount() const { return Object ? Object->RefCount : 0; }
    __forceinline ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
        Object = 0;
    }
};

class CBaseClass {                                 // vtable 0x01231710
public:
    virtual void v000();
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                                 // 0x0099A2E0
};

class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001();
    __forceinline CEnginePrimitiveHandle() { }
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& x);   // inline in retail's 0x0043F6C0
    __forceinline ~CEnginePrimitiveHandle()
    {
        if (Data.GetRefCount() == 1)
            Data->OnLastHandleReleased(this);
    }
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    __forceinline allocator() { }
};

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&);   // 0x0043F6C0

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                 // +0
    _Tp* _M_finish;                                // +4
    _Tp* _M_end_of_storage;                        // +8
    _Vector_base(unsigned int __n, const _Alloc& __a);   // 0x00485779 (body below)
};

#pragma auto_inline(off)
// Same-TU bodies (retail COMDATs: _Vector_base 0x00485779 sits beside this function; __uninitialized_fill_n 0x0043F6C0
// is the landed template): VC7.1 sees that neither touches its empty allocator / tag argument, so both temporaries
// overlay the high byte of the `n` slot as in retail.
template <class _Tp, class _Alloc> _Vector_base<_Tp, _Alloc>::_Vector_base(unsigned int __n, const _Alloc&)
    : _M_start(0), _M_finish(0), _M_end_of_storage(0)
{
    _M_start = __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0;
    _M_finish = _M_start;
    _M_end_of_storage = _M_start + __n;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        new (&*__cur) _Tp(__x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(unsigned int __n)
        : _Vector_base<_Tp, _Alloc>(__n, allocator_type())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
};

template class vector<CEnginePrimitiveHandle>;

} // namespace std
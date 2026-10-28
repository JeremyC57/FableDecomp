// std::vector<CTCSmokeGenerator::CGenerationPos>::_M_insert_overflow(pointer position, const CGenerationPos& x, const __false_type&, size_type fill_len, bool atend) -- STLport 4.6 grow-and-insert of the smoke generator's position list
// Grade: DIFFER(300v300) -- register assignment only: retail keeps `this` in edi and the new finish in esi, the lane swaps them (same instructions otherwise). Tried: declared-only STLport helpers, same-TU noinline helper bodies, out-of-class member definition, plain-inline _Destroy.
// Leaf: ?_M_insert_overflow@?$vector@VCGenerationPos@CTCSmokeGenerator@@V?$allocator@VCGenerationPos@CTCSmokeGenerator@@@std@@@std@@IAEXPAVCGenerationPos@CTCSmokeGenerator@@ABV34@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00723110, 300 bytes, thiscall `ret 14h`.
// Identity (proven): STLport 4.6 _vector.h `_M_insert_overflow(pointer, const _Tp&, const __false_type&, size_type, bool)` line for
//   line; its caller is the landed vector<CGenerationPos>::push_back 0x00723270 (`_M_insert_overflow(_M_finish, x, _IsPODType(),
//   1, true)`), its callees the landed __uninitialized_copy 0x00722B70 (hand955) and __uninitialized_fill_n 0x00722BD0 over
//   the 20-byte Ego_r.pdb CTCSmokeGenerator::CGenerationPos {C3DVector Pos; CIntelligentPointer<CThing> PEmitter}. The catalog
//   label "std::vector_ReserveAndCopy" is descriptive.
// Callees: malloc @ 0x00BFEA0E, __uninitialized_copy @ 0x00722B70, __uninitialized_fill_n @ 0x00722BD0, CBaseIntelligentPointer
//   ctor / GetPItem / SetPItem / dtor @ 0x00A01B10 / 0x00A01B50 / 0x00A01B90 / 0x00A01C10 (the inline element copy and
//   destructor; CIntelligentPointer<CThing> vtable 0x01238C6C), free @ 0x00BFEA14.
// Shape: the one-element path is the inline _Construct (C3DVector's user copy, then the intelligent pointer's copy ctor);
//   _M_clear's _Destroy loop is inline (each element's implicit destructor runs ~CBaseIntelligentPointer on PEmitter).
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CBaseObject;
class CBaseObjectPointer;
class CThing;
class CBaseIntelligentPointer {                            // Ego_r.pdb: vptr + PData, 8 bytes
public:
    CBaseIntelligentPointer();                            // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                   // 0x00A01C10
    CBaseObject* GetPItem() const;                        // 0x00A01B50
    void SetPItem(CBaseObject* item);                     // 0x00A01B90
    CBaseObjectPointer* PData;                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class CTCSmokeGenerator {
public:
    class CGenerationPos {                                // Ego_r.pdb, 20 bytes
    public:
        C3DVector Pos;                                    // +0x00
        CIntelligentPointer<CThing> PEmitter;             // +0x0C
    };
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
__forceinline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> __forceinline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
// Same-TU out-of-line instantiations with the STLport 4.6 bodies (retail 0x00722B70 landed in hand955, 0x00722BD0): with
// them visible VC7.1 homes the empty tag temporaries and new_start in the dead argument slots as retail does.
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                                  const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                            size_type __fill_len, bool __atend = false);
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
template class vector<CTCSmokeGenerator::CGenerationPos>;
}   // namespace std

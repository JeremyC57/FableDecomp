// std::vector::_M_insert_overflow(CTCDivineWrath::PerEffectInfo* pos, const CTCDivineWrath::PerEffectInfo& x, const __false_type&, size_type fill_len, bool atend) -- STLport growth of the divine-wrath hit-effect list: move into a size + max(size, n) block with x inserted fill_len times at pos
// Grade: DIFFER(300v300) -- esi/edi swap only: retail keeps `this` in edi and new_finish in esi (push edi before `mov edi,ecx`); ours the reverse. Tried: implicit vs __forceinline record copy, STLport __destroy/__destroy_aux chain, declared-only helpers, base-less pointer copy-ctor init list, QFE-4035.; this sweep: the _M_clear/_M_set bodies written out (STLport 4.5 form) -- inert
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00793370, length 300 bytes (thiscall, `ret 14h`).
// Identity: the catalog label "Vector_InsertRange_CBaseIntelligentPointer" is descriptive. The only caller is the landed
//   vector<CTCDivineWrath::PerEffectInfo>::push_back 0x007935D0 (`_M_insert_overflow(_M_finish, x, __false_type(), 1,
//   true)`); the element is Ego_r.pdb CTCDivineWrath::PerEffectInfo {C3DVector Position; CIntelligentPointer<CThing>
//   Effect} (20 bytes, `imul 66666667h ; sar edx,3`). Byte twin: 0x007981B0 (CTCUnholyPower's TU).
// Callees: malloc 0x00BFEA0E (allocator::allocate), __uninitialized_copy 0x007924F0 (prefix and suffix),
//   CBaseIntelligentPointer() 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90 (the Effect copy; vtable
//   CIntelligentPointer<CThing> 0x01238C6C), __uninitialized_fill_n 0x00792550, ~CBaseIntelligentPointer 0x00A01C10
//   (the inlined _M_clear's _Destroy loop), free 0x00BFEA14.
// Shape: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)` verbatim (the landed 0x00569E30 recipe); the
//   copy/fill helpers are out of line (same-TU `__declspec(noinline)` bodies keep each `__false_type()` homed on the
//   `atend` slot); the record's and the pointer's copy constructors are `__forceinline` (the landed push_back model) and
//   `_M_clear()` inlines to the destroy loop (the pointer's compiler-generated destructor is just the out-of-line base
//   destructor) plus `if (_M_start) free(_M_start)`; the new block is spilled to the dead `fill_len` slot.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* m_pItem;                                   // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    virtual void v00();
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer()
    {
        SetPItem(other.GetPItem());
    }
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CTCDivineWrath {
public:
    struct PerEffectInfo {                                  // Ego_r.pdb, 20 bytes
        C3DVector                   Position;               // +0x00
        CIntelligentPointer<CThing> Effect;                 // +0x0c
        __forceinline PerEffectInfo(const PerEffectInfo& o) : Position(o.Position), Effect(o.Effect) { }
    };
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x007924F0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x00792550
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
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
    // handle insertion
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CTCDivineWrath::PerEffectInfo>::_M_insert_overflow(CTCDivineWrath::PerEffectInfo*,
                                                                        const CTCDivineWrath::PerEffectInfo&,
                                                                        const __false_type&, unsigned int, bool);

}

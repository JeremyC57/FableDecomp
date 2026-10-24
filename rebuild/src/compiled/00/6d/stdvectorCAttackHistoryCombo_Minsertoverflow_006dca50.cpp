// std::vector<CAttackHistoryCombo>::_M_insert_overflow(CAttackHistoryCombo* pos, const CAttackHistoryCombo& x, const __false_type&, size_type fill_len, bool atend) -- STLport vector growth for a 16-byte record: move into a size+max(size, n) block with x inserted fill_len times at pos
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DCA50, 197 bytes (208 with int3 padding), thiscall `ret 14h`. Only caller: the vector's
//   `_M_fill_insert(pos, n, x)` 0x006DCCC0 (call site 0x006DCDBE, `_M_insert_overflow(pos, x, __false_type(), n, false)`).
//   The catalog label "CFeatDef_Vector_Resize" is descriptive (CFeatDef is a propagated _Uninit_copy label: the PDB
//   CFeatDef is 100 bytes, this element is 16). The element is CHeroExperienceDef::AttackHistoryCombos' record: the
//   def's transfer (0x006D91AB/0x006D91B9) serialises its last vector (+0xA8) through this TU's 16-byte vector code.
// Callees: malloc @ 0x00BFEA0E (allocator::allocate), __uninitialized_copy @ 0x006DBCE0 (x2, /Gr `__fastcall`),
//   __uninitialized_fill_n @ 0x006DBD90 (/Gr `__fastcall`), the 8-byte-element vector copy constructor @ 0x004F9408
//   (the record's first member), vector<CAttackHistoryCombo>::_M_clear() @ 0x004F963D (destroy the old range, free the block;
//   an identical-code-folded copy shared with another 16-byte vector).
// Layout: STLport vector {_M_start, _M_finish, _M_end_of_storage}; PDB CAttackHistoryCombo = { vector<pair<
//   EAttackHistoryType, EDamageAttribute> > AttackCombo; float Multiplier; } (the implicit copy constructor calls the
//   8-byte-pair vector copy at +0 and moves the float at +0xC as a dword).
// Shape: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)` verbatim (landed 0x00569E30 model): `old_size =
//   size(); len = old_size + (max)(old_size, fill_len)` (max through references: the `lea ecx` pick between the
//   spilled size and the fill_len slot), allocate, copy the prefix, `_Copy_Construct` (null-tested placement new of the
//   implicit copy constructor) when fill_len == 1 else fill_n, copy the suffix unless atend, `_M_clear()`, `_M_set`.
//   The tag-taking callees carry same-TU noinline bodies that never touch the tag, so VC7.1 homes every
//   `__false_type()` temporary on the live `atend` slot (`lea ecx,[esp+28h]`).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
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

}

struct SPair8 { long first; long second; };              // pair<EAttackHistoryType, EDamageAttribute>

class CPairVector {                                         // vector<8-byte pair>, 12 bytes
public:
    SPair8* _M_start;
    SPair8* _M_finish;
    SPair8* _M_end_of_storage;
    CPairVector(const CPairVector& other);                  // 0x004F9408
};

class CAttackHistoryCombo {                                 // 16 bytes in retail (PDB 20: Dinkumware vector)
public:
    CPairVector AttackCombo;                                // +0x00 vector<pair<EAttackHistoryType, EDamageAttribute> >
    float Multiplier;                                       // +0x0C
};

namespace std {

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x006DBCE0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x006DBD90
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
    void _M_clear();                                                                           // 0x004F963D
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

template void vector<CAttackHistoryCombo>::_M_insert_overflow(CAttackHistoryCombo*, const CAttackHistoryCombo&, const __false_type&,
                                                         unsigned int, bool);

}
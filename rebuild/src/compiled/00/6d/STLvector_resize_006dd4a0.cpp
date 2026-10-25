// _STL::vector::resize(unsigned int new_size) -- STLport vector<CAttackHistoryCombo>::resize(n): erase the surplus [begin + n, end) when shrinking, else fill-insert n - size() default attack-history combos at the end
// Leaf: ?resize@?$vector@VCAttackHistoryCombo@@V?$allocator@VCAttackHistoryCombo@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DD4A0, 91 bytes, thiscall on the vector, `ret 4`.
// Identity: FableWin.pdb ?resize@?$vector@VCAttackHistoryCombo@@...@@QAEXI@Z; sole caller
//   GFSerialiseVectorBinaryIn<CAttackHistoryCombo> 0x006DD580 (landed; the catalog's "<CBrainUpdateZone>" is propagated).
// Callees: vector<CAttackHistoryCombo>::erase(first, last) 0x006DC9F0, vector<CAttackHistoryCombo>::_M_fill_insert(pos,
//   n, const T&) 0x006DCCC0 (the inline insert's body).
// Layout: Ego_r.pdb CAttackHistoryCombo: AttackCombo vector<pair<EAttackHistoryType, EDamageAttribute> > +0 (12 bytes in
//   retail STLport), Multiplier float +0xC (retail 16 bytes); FableWin.pdb lists its user default constructor.
//   The leaf names the exact one-argument symbol (the inline two-argument resize is also emitted as a COMDAT).
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }` with the two-argument
//   `resize(n, const _Tp& x) { if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x); }` inline:
//   CAttackHistoryCombo's default constructor zeroes only the combo vector's three words (Multiplier is left
//   unset), stored after the 16-byte size subtraction (`sar edi,4`; the erase address `shl eax,4 ; add eax,edx`).
//   The temporary's vector destructor (`if (_M_start) free(_M_start)`) folds away as in retail because erase and
//   _M_fill_insert have same-TU noinline stand-ins that never write through the value reference (the retail TU holds
//   their bodies); declaration-only callees make VC7.1 re-test the start word after the call (112 v 91 bytes).
// Label correction: catalogued as `vector<std::pair<CCharString...>::resize` (propagated); the element is
//   CAttackHistoryCombo.
enum EAttackHistoryType { ATTACK_HISTORY_TYPE_NONE = 0 };
enum EDamageAttribute { DAMAGE_ATTRIBUTE_NONE = 0 };

extern "C" void __cdecl free(void* block);                         // 0x00BFEA14 (_Vector_base destructor)

namespace _STL {
template <class _T1, class _T2> struct pair {                       // STLport pair
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base() { if (_M_start) free(_M_start); }
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

class CAttackHistoryCombo {                                         // 16 bytes in retail
public:
    _STL::vector<_STL::pair<EAttackHistoryType, EDamageAttribute> > AttackCombo;   // +0x00
    float Multiplier;                                               // +0x0C
    CAttackHistoryCombo() { }
};

// 0x006DC9F0 erase, 0x006DCCC0 _M_fill_insert (out of line in retail): same-TU noinline stand-ins that, like the
// retail bodies, never write through the value reference (not graded)
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) _Tp* _STL::vector<_Tp, _Alloc>::erase(_Tp* __first, _Tp* __last)
{
    this->_M_finish = __first + (this->_M_finish - __last);
    return __first;
}
template <class _Tp, class _Alloc>
__declspec(noinline) void _STL::vector<_Tp, _Alloc>::_M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x)
{
    for (; __n != 0; --__n, ++__pos)
        __pos->Multiplier = __x.Multiplier;
}
#pragma auto_inline(on)
template void _STL::vector<CAttackHistoryCombo>::resize(unsigned int);
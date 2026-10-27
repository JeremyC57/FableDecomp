// std::vector::_M_allocate_and_copy(unsigned int __n, const std::pair<EAttackHistoryType, EDamageAttribute>* __first, const std::pair<EAttackHistoryType, EDamageAttribute>* __last) -- (vector<std::pair<EAttackHistoryType, EDamageAttribute>>) a new block holding copies of [first, last)
// Leaf: ??$_M_allocate_and_copy@PBU?$pair@W4EAttackHistoryType@@W4EDamageAttribute@@@std@@@?$vector@U?$pair@W4EAttackHistoryType@@W4EDamageAttribute@@@std@@V?$allocator@U?$pair@W4EAttackHistoryType@@W4EDamageAttribute@@@std@@@2@@std@@IAEPAU?$pair@W4EAttackHistoryType@@W4EDamageAttribute@@@1@IPBU21@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F95E9, 70 bytes, thiscall (this unused), `ret 0Ch` (size-optimised module).
// Identity (proven): its only caller is the landed vector<pair<EAttackHistoryType, EDamageAttribute> >::operator= 0x004F9508, which copies Ego_r.pdb CAttackHistoryCombo::AttackCombo (that vector type) for CHeroExperienceDef::AttackHistoryCombos.
// Callees: malloc 0x00BFEA0E (STLport __malloc_alloc, skipped for n == 0).
// Shape (family M042, representative 0x0045D264): STLport `__result = allocate(__n); __uninitialized_copy(__first, __last,
//   __result, __false_type()); return __result;` with allocate/_Construct/__uninitialized_copy `__forceinline`; the pair's explicit STLport copy constructor (`pair(const pair<_T1,_T2>&)`) re-addresses the second member (`[esi+ecx+4]`).
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int);
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
enum EAttackHistoryType { ATTACK_HISTORY_TYPE_NONE = 0 };
enum EDamageAttribute { DAMAGE_ATTRIBUTE_NONE = 0 };
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
protected:
    template <class _ForwardIterator>
    _Tp* _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        _Tp* __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
public:
    void Instantiate(unsigned int n, const _Tp* f, const _Tp* l) { _M_allocate_and_copy(n, f, l); }
};
template class vector<std::pair<EAttackHistoryType, EDamageAttribute> >;
}
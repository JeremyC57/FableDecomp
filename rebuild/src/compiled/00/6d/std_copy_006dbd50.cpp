// std::__copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result, const random_access_iterator_tag&, _Distance*) -- STLport random-access copy: assign last - first elements forward, return the end of the output (CAttackHistoryCombo)
// Leaf: ??$__copy@PAVCAttackHistoryCombo@@PAV1@H@std@@YIPAVCAttackHistoryCombo@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006dbd50, 59 bytes, __fastcall (first in ecx, last in edx), `ret 0Ch`.
// Identity (proven): bound (reloc_bindings) as ??$__copy@PAVCAttackHistoryCombo@@PAV1@H@std@@YI...; its element is 16 bytes and its assignment is CAttackHistoryCombo's implicit operator= -- vector<pair<EAttackHistoryType, EDamageAttribute> >::operator= 0x004F9508 (bound) on AttackCombo, then Multiplier (+0x0C); callers the landed vector erase 0x006DC9F0 and GFSerialiseVectorBinaryOut 0x006DC550.
// Callee: the element's member assignment 0x004F9508 (thiscall).
// Shape (family L031): n = (last - first) >> 4; per element one thiscall assignment of the leading member, then the
//   trailing scalar copied through al/eax; returns the advanced result.
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                             // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // 0x004F9508
};
}
enum EAttackHistoryType { };
enum EDamageAttribute { };
class CAttackHistoryCombo {                                // Ego_r.pdb, 16 bytes in retail
public:
    std::vector<std::pair<EAttackHistoryType, EDamageAttribute> > AttackCombo;   // +0x00
    float Multiplier;                                     // +0x0C
};
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}
template CAttackHistoryCombo* __fastcall std::__copy(CAttackHistoryCombo*, CAttackHistoryCombo*, CAttackHistoryCombo*, const std::random_access_iterator_tag&, int*);
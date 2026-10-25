// std::__destroy_aux<CAttackHistoryCombo*>(first, last, const __false_type&) -- STLport: run each record's destructor
// over [first, last)
// Leaf: ??$__destroy_aux@PAVCAttackHistoryCombo@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E41C0, 29 bytes, `__fastcall` (ecx = first, edx = last, the empty tag reference
//   on the stack, `ret 4`): a 16-byte-stride loop of `push 0 ; mov ecx,esi ; call 0x004E1CFC`.
// Identity (callee of FINDINGS chunk 388's 0x4F963D, renamed in this pass to std::vector<CAttackHistoryCombo>::_M_clear;
//   was the propagated Dinkumware label `_Destroy_range<allocator<NCreatureHitNotification::CFlash> >` with a vptr
//   model): 0x004F963D passes its [_M_start, _M_finish) here, and that _M_clear is the one the landed
//   std::vector<CAttackHistoryCombo>::_M_insert_overflow 0x006DCA50 calls; the same vector's operator= 0x004F932C also
//   calls it. struct_layouts: CAttackHistoryCombo {vector<pair<EAttackHistoryType,EDamageAttribute> > AttackCombo +0,
//   float Multiplier} = 16 bytes in retail; the per-element callee 0x004E1CFC frees the +0 block and then deletes `this`
//   only when bit 0 of its flag is set -- CAttackHistoryCombo's scalar deleting destructor (no vptr: the freed word is
//   the AttackCombo buffer).
// Shape: STLport `for (; first != last; ++first) _Destroy(&*first);` with `_Destroy(p) { p->~_Tp(); }`: under
//   optimize("s") VC7.1 calls the non-inlined implicit destructor through `??_G` with flag 0 (as 0x005799DE /
//   0x0047BAF0).
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    pointer _M_start;                                                   // +0x00
    pointer _M_finish;                                                  // +0x04
    _STLP_alloc_proxy<pointer, _Tp, allocator<_Tp> > _M_end_of_storage; // +0x08
    ~vector() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }   // trivial elements
};
}

enum EAttackHistoryType { ATTACK_HISTORY_TYPE_0 };
enum EDamageAttribute { DAMAGE_ATTRIBUTE_0 };
struct SAttackHistoryEntry { EAttackHistoryType first; EDamageAttribute second; };   // pair<EAttackHistoryType, EDamageAttribute>

class CAttackHistoryCombo {                         // 16 bytes in retail (PDB 20: Dinkumware vector)
public:
    std::vector<SAttackHistoryEntry> AttackCombo;   // +0x00
    float Multiplier;                               // +0x0C
};

namespace std {
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template void __fastcall __destroy_aux(CAttackHistoryCombo*, CAttackHistoryCombo*, const __false_type&);
}

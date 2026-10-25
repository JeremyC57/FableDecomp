// std::vector<CAttackHistoryCombo>::_M_clear() -- STLport: destroy [start, finish), then release the block
// Leaf: ?_M_clear@?$vector@VCAttackHistoryCombo@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F963D, 35 bytes, thiscall `ret`: `__destroy_aux(start, finish, __false_type())`
//   (0x004E41C0, `__fastcall` -- ecx = first, edx = last, the empty tag by reference in the high byte of the `push ecx`
//   slot, `lea eax,[esp+7]`), then `if (start) free(start)` (0x00BFEA14).
// Identity (FINDINGS chunk 388 callee "a 16-byte-element vector _M_clear"; was the `__fastcall OnReadFinished(
//   AutoTinyActiveFile*)` stand-in catalogued as StdList_Dtor_004f963d): its callers are the landed
//   std::vector<CAttackHistoryCombo>::_M_insert_overflow 0x006DCA50 (which already declares 0x004F963D as this vector's
//   _M_clear) and the same vector's operator= 0x004F932C (`&x != this`, 16-byte `sar 4` sizes, _M_allocate_and_copy
//   0x004F9484); struct_layouts: CAttackHistoryCombo {vector<pair<EAttackHistoryType,EDamageAttribute> > AttackCombo +0,
//   float Multiplier} (16 bytes in retail with the 12-byte STLport vector) -- the callee's element destructor (the
//   `push 0 ; call ??_G` 0x004E1CFC, see the GOTCHAS-style note on `_Destroy` under optimize("s")) frees the +0 block.
// Shape: STLport 4.x `_M_clear() { _Destroy(_M_start, _M_finish); _M_end_of_storage.deallocate(_M_start,
//   _M_end_of_storage._M_data - _M_start); }` with _Destroy -> __destroy_aux(.., __false_type) and the null-tested
//   deallocate inline (`__forceinline` _Destroy: the size TU otherwise keeps it out of line). The tag has a user-declared
//   constructor (no zero-fill of the temporary).
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

namespace std {
struct __false_type { __false_type() { } };

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x004E41C0

template <class _ForwardIterator>
__forceinline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

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
    void _M_clear();
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
template <class _Tp>
void vector<_Tp>::_M_clear()
{
    _Destroy(_M_start, _M_finish);
    _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
}

template void vector<CAttackHistoryCombo>::_M_clear();
}

// std::__copy(const SRecord16* first, const SRecord16* last, SRecord16* result, const random_access_iterator_tag&, int*) -- STLport counted forward assignment of 16-byte POD records, returns the advanced result
// Retail: Fable.exe (TLC, VC7.1) @ 0x004936BA, 41 bytes, `ret 0Ch`.
// Callees: none (each 16-byte record is assigned as a `movsd` x4 block copy through esi/edi).
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)`: the signed
// count `(last - first)` (`sub ; sar 4`) is computed once, then `*result = *first; ++first; ++result;` per
// element; the advanced result is returned (the untouched result for an empty range). /Gr algorithm TU:
// `__fastcall` with first in ecx, last in edx, result + the tag reference + the null distance pointer on the
// stack (both callers push `0`, `lea eax,[ebp+0Bh]`, result). The result pointer lives in its argument slot
// (`mov edi,[esp+0Ch] ; add dword ptr [esp+0Ch],10h`) because esi/edi are taken by the block copy.
// Callers: 0x004939AA (a `copy` forwarder) and 0x00493E2C (vector::erase(first, last): `__copy(last, finish,
// first)` then `_M_finish = result`).
// Label correction: "_Move<CMusicEntry*,CMusicEntry*>" is a propagated Dinkumware-era label; the body is the
// STLport `__copy` instantiation for a 16-byte POD record (COMDAT-folded across every such element type, so
// the record is named by its size).
// Pragma: optimize("s") -- the size TU copies the record with `movsd` x4 and keeps result in its slot.
// Leaf: __copy
#pragma optimize("s", on)

struct SRecord16 {                       // 16-byte POD record (assigned as a block)
    unsigned char _body[16];
};

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SRecord16* __fastcall __copy(const SRecord16* __first, const SRecord16* __last, SRecord16* __result,
                             const random_access_iterator_tag&, int*)
{
    for (int __n = int(__last - __first); __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

}
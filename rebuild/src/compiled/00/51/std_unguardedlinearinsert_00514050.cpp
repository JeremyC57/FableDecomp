// std::__unguarded_linear_insert(CThingBuilding** last, CThingBuilding* val, CIsHouseLargerThan comp) -- STLport insertion-sort step: shift the elements that sort after val up by one, then drop val into the hole
// Flags: /Gr
// Leaf: ??$__unguarded_linear_insert@PAPAVCThingBuilding@@PAV1@VCIsHouseLargerThan@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00514050, 67 bytes, __fastcall (last ecx, val edx, the 4-byte comparator by value on the stack), `ret 4`.
// Identity (proven): reloc_bindings ??$__unguarded_linear_insert@PAPAVCThingBuilding@@PAV1@VCIsHouseLargerThan@@@std@@YI...; its comparator 0x00512410 is bound ??RCIsHouseLargerThan@@QBE_NPAVCThingBuilding@@0@Z; caller the insertion sort 0x00517500 / 0x00515420.
// Callee: CIsHouseLargerThan::operator() 0x00512410 (thiscall on the comparator's argument slot).
// Shape (family K128, representative 0x00514050): STLport 4.6 `__next = __last; --__next; while (__comp(__val, *__next)) {
//   *__last = *__next; __last = __next; --__next; } *__last = __val;` with the comparator out of line.
class CThingBuilding;
class CIsHouseLargerThan {                               // empty (passed as one stack word)
public:
    bool operator()(CThingBuilding* a, CThingBuilding* b) const;   // 0x00512410
};
namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
template void __fastcall __unguarded_linear_insert<CThingBuilding**, CThingBuilding*, CIsHouseLargerThan>(CThingBuilding**, CThingBuilding*, CIsHouseLargerThan);
}
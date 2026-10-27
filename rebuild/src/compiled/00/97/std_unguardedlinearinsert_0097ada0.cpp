// std::__unguarded_linear_insert(CThing** last, CThing* val, CThingCompare_InDirectionOfAndNearestToHeuristicBitch comp) -- STLport insertion-sort step: shift the elements that sort after val up by one, then drop val into the hole
// Flags: /Gr
// Leaf: ??$__unguarded_linear_insert@PAPAVCThing@@PAV1@VCThingCompare_InDirectionOfAndNearestToHeuristicBitch@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097ADA0, 67 bytes, __fastcall (last ecx, val edx, the 16-byte comparator by value on the stack), `ret 16`.
// Identity (proven): reloc_bindings ??$__unguarded_linear_insert@PAPAVCThing@@PAV1@VCThingCompare_InDirectionOfAndNearestToHeuristicBitch@?A0x4ba69f9e@@@std@@YI... (a second TU's anonymous-namespace comparator of the same name); caller the landed __linear_insert 0x0097B030.
// Callee: CThingCompare_InDirectionOfAndNearestToHeuristicBitch::operator() 0x0097ABF0 (thiscall on the comparator's argument slot).
// Shape (family K128, representative 0x00514050): STLport 4.6 `__next = __last; --__next; while (__comp(__val, *__next)) {
//   *__last = *__next; __last = __next; --__next; } *__last = __val;` with the comparator out of line.
class CThing;
namespace {
class CThingCompare_InDirectionOfAndNearestToHeuristicBitch {   // Ego_r.pdb 16 bytes (anonymous namespace of its TU)
public:
    float PosX, PosY;                                    // +0x00 Pos (C2DVector)
    float DirX, DirY;                                    // +0x08 Dir (C2DVector)
    bool operator()(CThing* a, CThing* b) const;         // 0x0097ABF0
};
}
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
template void __fastcall __unguarded_linear_insert<CThing**, CThing*, CThingCompare_InDirectionOfAndNearestToHeuristicBitch>(CThing**, CThing*, CThingCompare_InDirectionOfAndNearestToHeuristicBitch);
}
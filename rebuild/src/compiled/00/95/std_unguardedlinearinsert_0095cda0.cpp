// std::__unguarded_linear_insert(CThing** last, CThing* val, CThingCompare_Nearest comp) -- STLport insertion-sort step: shift the elements that sort after val up by one, then drop val into the hole
// Flags: /Gr
// Leaf: ??$__unguarded_linear_insert@PAPAVCThing@@PAV1@VCThingCompare_Nearest@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095CDA0, 67 bytes, __fastcall (last ecx, val edx, the 12-byte comparator by value on the stack), `ret 12`.
// Identity (probable): its comparator 0x004A8D80 is bound ??$?RVCThing@@@CThingCompare_Nearest@@QBEPAVCThing@@PAV1@0@Z (the 12-byte comparator by value; the pointer result tested as the bool); callers the sort helpers 0x0095D920 / 0x0095DA90; caller 0x0095D920 / 0x0095DA90.
// Callee: CThingCompare_Nearest::operator() 0x004A8D80 (thiscall on the comparator's argument slot).
// Shape (family K128, representative 0x00514050): STLport 4.6 `__next = __last; --__next; while (__comp(__val, *__next)) {
//   *__last = *__next; __last = __next; --__next; } *__last = __val;` with the comparator out of line.
class CThing;
class CThingCompare_Nearest {                            // Ego_r.pdb 12 bytes
public:
    float PosX, PosY, PosZ;                              // +0x00 Pos (C3DVector)
    template <class T> T* operator()(T* a, T* b) const;  // 0x004A8D80 for CThing (returns the nearer, or 0)
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
template void __fastcall __unguarded_linear_insert<CThing**, CThing*, CThingCompare_Nearest>(CThing**, CThing*, CThingCompare_Nearest);
}
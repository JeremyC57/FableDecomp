// std::pop_heap(CThing** first, CThing** last, CThingCompare_BestForTargeting2 comp) -- STLport forwarder: pass the range, the value-type tag and a copy of the comparator to __pop_heap_aux
// Leaf: ??$pop_heap@PAPAVCThing@@VCThingCompare_BestForTargeting2@@@std@@YIXPAPAVCThing@@0VCThingCompare_BestForTargeting2@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007fa910, 111 bytes, /Gr __fastcall (first in ecx, last in edx, the 52-byte comparator on the stack), `ret 34h`.
// Identity (probable): its callee 0x007FA7B0 is bound ?__pop_heap_aux@@YIXPAPAVCThing@@00VCThingCompare_BestForTargeting2@@@Z (the CThing* / CThingCompare_BestForTargeting2 heap code); no caller (unreferenced).
// Callee: __pop_heap_aux 0x007FA7B0 (__fastcall).
// Shape (family L067): `sub esp,34h` then the comparator's user copy constructor member by member (Hero, the inner
//   compare's two C3DVectors and four floats, the bool as a byte, EnemyPreferenceFactor), `push 0` for the (T*)0 tag.
class CThing;
class C3DVector {                                          // user copy constructor (FableWin/Ego_d ??0C3DVector@@QAE@ABV0@@Z)
public:
    float X, Y, Z;                                        // +0x00
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class CVectorCompare_BestForTargeting {                    // Ego_r.pdb, 44 bytes
public:
    C3DVector AnchorPos;                                  // +0x00
    C3DVector PreferredLine;                              // +0x0C
    float AngleCompare;                                   // +0x18
    float SqrDistCompare;                                 // +0x1C
    float OverrideShortDistanceFactor;                    // +0x20
    float OverrideShortDistanceFalloff;                   // +0x24
    bool CompareInXYPlane;                                // +0x28
};
class CThingCompare_BestForTargeting2 {                    // Ego_r.pdb, 52 bytes
public:
    CThing* Hero;                                         // +0x00
    CVectorCompare_BestForTargeting VectorCompare;        // +0x04
    float EnemyPreferenceFactor;                          // +0x30
    CThingCompare_BestForTargeting2(const CThingCompare_BestForTargeting2& other)   // Ego_r.pdb ??0..@@QAE@ABV0@@Z
        : Hero(other.Hero), VectorCompare(other.VectorCompare), EnemyPreferenceFactor(other.EnemyPreferenceFactor) { }
};
namespace std {
template <class _Tp> inline _Tp* __value_type(_Tp*) { return (_Tp*)0; }
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __pop_heap_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x007FA7B0
template <class _RandomAccessIter, class _Compare>
void __fastcall pop_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}
}
template void __fastcall std::pop_heap(CThing**, CThing**, CThingCompare_BestForTargeting2);
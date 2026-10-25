// std::fill(CNavigationAreaUpdate::CNavigationLayerUpdate* first, CNavigationAreaUpdate::CNavigationLayerUpdate* last, const CNavigationAreaUpdate::CNavigationLayerUpdate& value) -- assign one navigation-layer update (line list and switchable blockages) to every element of a range
// Leaf: ??$fill@PAVCNavigationLayerUpdate@CNavigationAreaUpdate@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051CAA0, 50 bytes, `__fastcall` (first ecx, last edx, value pointer
//   on the stack), `ret 4`.
// Identity: 24-byte stride = two 12-byte vectors, each over 16-byte elements (`sar 4` in both callees), assigned
//   through 0x00515320 (+0) and 0x0051BCD0 (+0xc): Ego_r.pdb CNavigationAreaUpdate::CNavigationLayerUpdate
//   {vector<C2DLineF> LineList; vector<CNavigationAreaUpdate::CSwitchableBlockage> SwitchableBlockages} (C2DLineF
//   16 bytes; CSwitchableBlockage = unsigned __int64 + 4-byte STLport list, 16 with alignment). The manifest's
//   "pair<CCharString, CCharString>" is propagated. One caller, 0x0051D732.
// Callees: vector<C2DLineF>::operator= @ 0x00515320, vector<CSwitchableBlockage>::operator= @ 0x0051BCD0.
// Shape: STLport fill with the implicit operator= inline; &val.SwitchableBlockages hoisted.

template <class T> class CVector {                       // STLport vector, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CVector& operator=(const CVector& other);            // out of line
};
class C2DVector { public: float X, Y; };
class C2DLineF { public: C2DVector Start, End; };        // 16 bytes

class CNavigationAreaUpdate {
public:
    class CSwitchableBlockage {                          // 16 bytes
    public:
        unsigned __int64 UniqueID;                       // +0x00
        void*            LineList;                       // +0x08 STLport list<C2DLineF> (node pointer)
    };
    class CNavigationLayerUpdate {                       // 24 bytes
    public:
        CVector<C2DLineF>            LineList;           // +0x00
        CVector<CSwitchableBlockage> SwitchableBlockages;   // +0x0c
    };
};
typedef CNavigationAreaUpdate::CNavigationLayerUpdate CNavigationLayerUpdate;

namespace std {
// STLport 4.6 fill; the algorithm templates are __fastcall here (first/last in ecx/edx, callee pops the value pointer).
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}

template void __fastcall std::fill<CNavigationLayerUpdate*, CNavigationLayerUpdate>(CNavigationLayerUpdate*, CNavigationLayerUpdate*, const CNavigationLayerUpdate&);
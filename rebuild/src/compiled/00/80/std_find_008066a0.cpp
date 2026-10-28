// std::__find(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, const NPlayerTargeting::CTargetPoint& val, const random_access_iterator_tag&) -- STLport random-access find over the player-targeting points: four CTargetPoint::operator== tests per pass (same thing and same index), then the 3/2/1 remainder; returns the match or last
// Leaf: ??$__find@PAVCTargetPoint@NPlayerTargeting@@V12@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008066A0, 352 bytes, /Gr `__fastcall` (first in ecx, last in edx; &val and the
//   tag reference on the stack), `ret 8`.
// Identity (proven): the 28-byte element (`imul 92492493h ; sar 4`) is compared as {CIntelligentPointer<CThing> GetPItem()
//   equality, then the dword at +8} = Ego_r.pdb NPlayerTargeting::CTargetPoint {PThing +0, Index +8, TargetingPosition +0xC,
//   FrameRejected +0x18} with its ??8CTargetPoint@NPlayerTargeting@@QAE_NABV01@@Z (inline here); Ego_r.pdb instantiates
//   std::find<vector<CTargetPoint>::iterator, CTargetPoint> in this player-targeting TU (the landed __find_if 0x008060C0,
//   __uninitialized_copy 0x00806C00 and vector<CTargetPoint>::operator= 0x008085C0 sit beside it); eight callers
//   0x00804C30 .. 0x0080795E. The catalog's "find<CBaseIntelligentPointer*,CBaseIntelligentPointer>" is behavioural.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (twice per comparison: the element's, then val's).
// Shape: STLport 4.x `__find` for random-access iterators (as the landed __find_if 0x008060C0 of this TU): trip_count =
//   (last - first) >> 2, four inline equality tests per pass, then `switch (last - first)` over 3/2/1, default last.
class CThing;
class CBaseObject;
class C3DVector { public: float X, Y, Z; };

#pragma pack(push, 1)
class CBaseIntelligentPointer {                             // 8 bytes, 1-aligned in retail
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void* PData;                                            // +0x04 CBaseObjectPointer*
};
#pragma pack(pop)
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

namespace NPlayerTargeting {
class CTargetPoint {                                        // Ego_r.pdb, 0x1C bytes
public:
    CIntelligentPointer<CThing> PThing;                     // +0x00
    long                        Index;                      // +0x08
    C3DVector                   TargetingPosition;          // +0x0C
    long                        FrameRejected;              // +0x18
    bool operator==(const CTargetPoint& other)
    {
        return PThing.GetPItem() == other.PThing.GetPItem() && Index == other.Index;
    }
};
}

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for (; __trip_count > 0; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
    case 0:
    default:
        return __last;
    }
}

template NPlayerTargeting::CTargetPoint* __fastcall __find(NPlayerTargeting::CTargetPoint*, NPlayerTargeting::CTargetPoint*,
                                                           const NPlayerTargeting::CTargetPoint&,
                                                           const random_access_iterator_tag&);
}
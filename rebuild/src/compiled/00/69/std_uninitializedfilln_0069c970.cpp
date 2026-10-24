// std::__uninitialized_fill_n(CQuake* first, unsigned int n, const CQuake& x, const __false_type&) -- STLport placement-copy of one camera quake into n raw slots; returns the end of the run
// Leaf: ??$__uninitialized_fill_n@PAVCQuake@CGameCameraManager@@
//
// Retail 0x0069C970, 104 bytes (+ int3 padding to 112), `ret 8` (/Gr: first in ecx -> eax as `cur`, n in edx moved to
//   edi and counted down; &x and the empty __false_type tag on the stack).
// Caller: 0x0069D386 in vector<CGameCameraManager::CQuake>::_M_insert_overflow 0x0069D300 (the n > 1 fill arm).
// Shape: STLport _uninitialized.h -- `cur = first; for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;`
//   (unsigned size_t: `test edx,edx ; jbe`), the placement-new null test and the memberwise copy inline (x's dwords
//   re-read every pass; the destination through a second +0x14-biased induction register).  The retail TU is /Gr,
//   written as explicit __fastcall.
// Model: CQuake / C3DVector as the landed std::_Construct 0x0069C860 (Ego_r.pdb CGameCameraManager::CQuake, 0x30).
// Label: catalog "fill<C3DAnimationStateAlpha*,C3DAnimationStateAlpha>" is a propagated label.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }  // memberwise copy: fixed-offset dword moves
};

class CGameCameraManager {
public:
    class CQuake {                                          // PDB sizeof 0x30
    public:
        C3DVector Position;                                 // +0x00
        C3DVector Axis;                                     // +0x0C
        long      Frame;                                    // +0x18
        float     Amplitude;                                // +0x1C
        float     Duration;                                 // +0x20
        float     Frequency;                                // +0x24
        float     Phase;                                    // +0x28
        bool      Global;                                   // +0x2C
    };
};

namespace std {
struct __false_type { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
}

namespace std {
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

template CGameCameraManager::CQuake* __fastcall std::__uninitialized_fill_n<CGameCameraManager::CQuake*, unsigned int,
    CGameCameraManager::CQuake>(CGameCameraManager::CQuake*, unsigned int, const CGameCameraManager::CQuake&,
                                const std::__false_type&);
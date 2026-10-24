// std::__uninitialized_copy(CQuake* first, CQuake* last, CQuake* result, const __false_type&) -- STLport placement-copy of a camera-quake range into raw storage; returns the end of the copy
// Leaf: ??$__uninitialized_copy@PAVCQuake@CGameCameraManager@@
//
// Retail 0x0069C8F0, 113 bytes (+ int3 padding to 128), `ret 8` (/Gr: first in ecx, last in edx; result and the
//   empty __false_type tag on the stack).
// Callers: 0x0069D35A and 0x0069D3A2 in vector<CGameCameraManager::CQuake>::_M_insert_overflow 0x0069D300 (the two
//   halves around the inserted run).
// Shape: STLport _uninitialized.h -- `cur = result; for (; first != last; ++first, ++cur) _Construct(&*cur, *first);
//   return cur;` with the placement-new null test and the memberwise copy inline (dword +0, then +4..+0x28 through a
//   second +0x14-biased induction pair, the Global byte last).  The retail TU is /Gr, written as explicit __fastcall.
// Model: CQuake / C3DVector as the landed std::_Construct 0x0069C860 (Ego_r.pdb CGameCameraManager::CQuake, 0x30).
// Label: catalog "_Move<Coin*,Coin*>" is a propagated label.
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
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}

template CGameCameraManager::CQuake* __fastcall std::__uninitialized_copy<CGameCameraManager::CQuake*,
    CGameCameraManager::CQuake*>(CGameCameraManager::CQuake*, CGameCameraManager::CQuake*,
                                 CGameCameraManager::CQuake*, const std::__false_type&);
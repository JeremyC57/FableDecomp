// std::swap(pair<long, CCompositeSurfaceInfo>& a, pair<long, CCompositeSurfaceInfo>& b) -- STLport swap through a temporary: copy a (add-ref the surface cache), assign b to a and the temporary to b (counted-pointer release / add-ref), then destroy the temporary
// Flags: /Gr
// Leaf: ??$swap@U?$pair@JVCCompositeSurfaceInfo@@@std@@@std@@YIXAAU?$pair@JVCCompositeSurfaceInfo@@@0@0@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members DeleteFunction->DeleteFunc are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BED690, 250 bytes, __fastcall (a ecx, b edx), plain `ret`.
// Identity (probable): the element is the CVectorMap<long, CCompositeSurfaceInfo> entry of the landed sort TU (vptr
//   0x012A8154 = RTTI .?AVCCompositeSurfaceInfo@@ stored into the temporary; the 16-byte pair layout of landed
//   __adjust_heap 0x00BEDD70); the body is STLport 4.6 _algobase.h swap verbatim. Catalog "swap<CCompositeSurfaceInfo>"
//   names the second member, not the swapped type.
// Callees: operator delete 0x00BFE9BC (count blocks), ~CBaseClass 0x0099A2E0 (the temporary's base).
#include <new>

// CBaseClass: the polymorphic engine root (out-of-line virtual destructor 0x0099A2E0, vtable 0x01231710).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

#include "engine/CCountedPointer.h"


class CCompositeSurfaceCache;
// Ego_r.pdb CCompositeSurfaceInfo (vtable 0x012A8154, 12 bytes): implicit copy, assignment and (virtual) destructor.
class CCompositeSurfaceInfo : public CBaseClass {
public:
    CCountedPointer<CCompositeSurfaceCache> Cache;  // +0x04
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
};
}   // namespace std

typedef std::pair<long, CCompositeSurfaceInfo> CSurfaceEntry;   // 0x10 bytes

namespace std {
template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
}   // namespace std

template void __fastcall std::swap(CSurfaceEntry&, CSurfaceEntry&);
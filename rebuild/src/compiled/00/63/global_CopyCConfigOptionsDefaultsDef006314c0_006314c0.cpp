// std::__copy<const NUISystem::CUIStateDef*, NUISystem::CUIStateDef*, int>(first, last, result, const random_access_iterator_tag&, int*) -- STLport __copy: counted forward assignment of 124-byte CUIStateDef elements, returns the advanced result
// Leaf: ??$__copy@PBVCUIStateDef@NUISystem@@PAV12@H@std@@YIPAVCUIStateDef@NUISystem@@PBV12@0PAV12@ABUrandom_access_iterator_tag@0@PAH@Z
// Name fix (NF382, unit with self 0x00625350; NF381 QUEUE p2): was the stand-in _global::Copy_CConfigOptionsDefaultsDef_006314c0 calling
//   ??4CConfigOptionsDefaultsDef. The element is NUISystem::CUIStateDef (0x7c = the vector<CUIStateDef> stride, `imul 84210843h ; sar 6`
//   = /124, engine/NUISystem_CUIStateDef.h); the landed vector<CUIStateDef> row 0x006315E0 binds this address under this spelling,
//   and its one callee 0x00625350 is NUISystem::CUIStateDef::operator= (FableWin ui_def1.obj copy helpers call
//   ??4CUIStateDef@NUISystem@@QAEAAV01@ABV01@@Z; landed binders 0x00625340 / 0x00632F80 / 0x00632FF0 bind it at 0x00625350).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006314C0, length 71 bytes (+ int3 padding), `ret 0Ch`.
// Callees: NUISystem::CUIStateDef::operator= @ 0x00625350.
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` from the shared
// stlport/fable_containers.h -- the signed count `last - first` once (imul 0x84210843 ; add ; sar 6), then one out-of-line
// assignment per element (`push esi ; mov ecx,edi ; call`), both pointers stepped by 0x7c; __fastcall (first in ecx, last in edx).
// Layout: only operator= is needed, and engine/NUISystem_CUIStateDef.h declares none (it is implicit there), so a sized local model.
#include "stlport/fable_containers.h"

namespace NUISystem {
class CUIStateDef { public:                             // 124 bytes (0x7c); its own assignment copies the body
    unsigned char _body[0x7c];
    CUIStateDef& operator=(const CUIStateDef& other);   // 0x00625350
};
}

template NUISystem::CUIStateDef* __fastcall std::__copy<const NUISystem::CUIStateDef*, NUISystem::CUIStateDef*, int>(const NUISystem::CUIStateDef*, const NUISystem::CUIStateDef*, NUISystem::CUIStateDef*, const std::random_access_iterator_tag&, int*);

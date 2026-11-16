// CCountedPointer<NBBBMaths::CLinearSpline>::Reset(T* data) -- drop the current share (destroying the spline on the last reference), then
// adopt data with a fresh count block (DeleteData 0x007F3FB0)
// Leaf: ?Reset@?$CCountedPointer@VCLinearSpline@NBBBMaths@@@@QAEXPAVCLinearSpline@NBBBMaths@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F4780, thiscall, `ret 4`.
// NF39 (2026-11-16): moved off its address-suffixed stand-in (CCountedPointer_007f3fb0::Reset with a local count block) onto the shared
//   engine/CCountedPointer.h Reset (the same source as the landed CCountedPointer<CAIBrain>::Reset 0x00834E40): LABELS.tsv / the queue
//   claim ?Reset@?$CCountedPointer@VCLinearSpline@NBBBMaths@@@@QAEXPAVCLinearSpline@NBBBMaths@@@Z (CTCLightningOrb's
//   CCountedPointer<NBBBMaths::CLinearSpline> +0x2C, engine/CTCLightningOrb.h); its landed caller binds this symbol.
#include "engine/CCountedPointer.h"
namespace NBBBMaths { class CLinearSpline; }
template void CCountedPointer<NBBBMaths::CLinearSpline>::Reset(NBBBMaths::CLinearSpline*);

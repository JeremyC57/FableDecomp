#include "rebuild_abi.h"
struct M { bool Check(); };
struct A { char p[0x64]; M* f64; };
struct B { char p[0x18]; A* f18; };
struct C { char p[4]; B* f4; char Is(); };
// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x004BD200 ?WasRenderedLastFrame@CTCGraphicAppearance@NGraphicAppearance@@QBE_NXZ (was ?Check@M@@QAE_NXZ)
namespace NGraphicAppearance { class CTCGraphicAppearance { public: bool WasRenderedLastFrame() const; }; }
char C::Is(){ bool r=reinterpret_cast<const NGraphicAppearance::CTCGraphicAppearance*>(this->f4->f18->f64)->WasRenderedLastFrame(); return (char)(r!=false); }
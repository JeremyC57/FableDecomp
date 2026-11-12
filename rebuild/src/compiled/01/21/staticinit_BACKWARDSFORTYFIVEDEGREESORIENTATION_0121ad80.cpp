// _global::$E_InventoryOrientationRHSet9() -- dynamic initializer of the anonymous-namespace const CRightHandedSet BACKWARDS_FORTY_FIVE_DEGREES_ORIENTATION @ 0x013B8BFC: Up = (sqrt(0.5), -sqrt(0.5), 0), Forward = (sqrt(0.5), sqrt(0.5), 0)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121AD80, 53 bytes (+ padding), plain `ret`, reached from the .CRT$XCU table (entry
//   0x013743C4, between the UPSIDE_DOWN_LEFT_ initializer 0x0121AD50 and 0x0121ADC0); catalog "sub_0121ad80" was a gap-scan placeholder.
// Identity: CTCInventoryItem::GetRHSetFromOrientation(EInventoryObjectOrientation) @ 0x005D8EF0 (landed) copies the object at
//   0x013B8BFC for case 9 and binds it as ?BACKWARDS_FORTY_FIVE_DEGREES_ORIENTATION@?A0x825ce476@@3VCRightHandedSet@@B; the FableWin.pdb
//   / Ego_r.pdb dynamic-initializer list of the CTCInventoryItem TU runs FORTY_FIVE, RIGHT_, LEFT_, UPSIDE_DOWN_, UPSIDE_DOWN_RIGHT_,
//   UPSIDE_DOWN_LEFT_, BACKWARDS_, BACKWARDS_RIGHT_, BACKWARDS_LEFT_ (..._FORTY_FIVE_DEGREES_ORIENTATION), the same order as retail's
//   .CRT$XCU entries 0x0121AC30 .. 0x0121ADF0: this is `dynamic initializer for 'BACKWARDS_FORTY_FIVE_DEGREES_ORIENTATION''.
// Shape: as the landed siblings 0x0121ACD0 / 0x0121AD50: VC7.1 folds the zero Z words into the object's static image (.bss) and keeps
//   only the sqrt-derived words dynamic (C3DVector's memberwise user copy constructor makes that possible). sqrt is the x87
//   intrinsic on the 0.5 double at 0x012316F0, evaluated once; every component is the float overload `sqrt(0.5f)` (<math.h>:
//   `(float)sqrt((double)x)`, as the sibling 0x0121ACD0): VC7.1 rounds it through the frame slot (`fst [esp]`, three reloads, one
//   `fchs` for Up.Y) and stores the still-loaded fsqrt result itself for Forward.X (a `sqrt(0.5)` double there compiles identically).
#include <math.h>
#pragma intrinsic(sqrt)

#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"     // retail branch: Ego_r.pdb CRightHandedSet {Up +0, Forward +0xc}, 24 bytes; the (up, forward)
                                        // constructor (Ego_r ??0CRightHandedSet@@QAE@ABVC3DVector@@0@Z) expanded in place

namespace {
const CRightHandedSet BACKWARDS_FORTY_FIVE_DEGREES_ORIENTATION(C3DVector(sqrt(0.5f), -sqrt(0.5f), 0.0f), C3DVector(sqrt(0.5f), sqrt(0.5f), 0.0f));
}
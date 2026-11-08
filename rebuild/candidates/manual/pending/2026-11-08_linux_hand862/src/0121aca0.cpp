// _global::$E_InventoryOrientationRHSet3() -- dynamic initializer of the anonymous-namespace const CRightHandedSet LEFT_FORTY_FIVE_DEGREES_ORIENTATION @ 0x0137CFA4: Up = (-sqrt(0.5), -sqrt(0.5), 0), Forward = (0, 0, -1)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121ACA0, 35 bytes (+ padding), plain `ret`, reached from the .CRT$XCU table; catalog
//   "sub_0121aca0" was a gap-scan placeholder.
// Identity: CTCInventoryItem::GetRHSetFromOrientation(EInventoryObjectOrientation) @ 0x005D8EF0 (landed) copies the object at 0x0137CFA4 for
//   case 3 and binds it as ?LEFT_FORTY_FIVE_DEGREES_ORIENTATION@?A0x825ce476@@3VCRightHandedSet@@B (reloc_bindings.tsv); the FableWin.pdb / Ego_r.pdb
//   dynamic-initializer list of the CTCInventoryItem TU runs FORTY_FIVE, RIGHT_, LEFT_, UPSIDE_DOWN_, UPSIDE_DOWN_RIGHT_, UPSIDE_DOWN_LEFT_,
//   BACKWARDS_, BACKWARDS_RIGHT_, BACKWARDS_LEFT_ (..._FORTY_FIVE_DEGREES_ORIENTATION), the same order as retail's .CRT$XCU entries
//   0x0121AC30 .. 0x0121ADF0: this is `dynamic initializer for 'LEFT_FORTY_FIVE_DEGREES_ORIENTATION''. The retail .data image holds the static words
//   (Up.Z = 0, Forward = (0, 0, -1)), matching the case-3 set.
// Shape: as the landed sibling 0x0121AD80: VC7.1 folds every constant word of the inlined constructors into the object's static image
//   and keeps only the two sqrt-derived Up words dynamic (C3DVector's memberwise user copy constructor makes that possible). Each
//   component is the float overload `sqrt(0.5f)` from <math.h> (`(float)sqrt((double)x)` over the x87 intrinsic on the 0.5 double at
//   0x012316F0): the fsqrt is shared, VC7.1 rounds it through the frame slot (`fst [esp]`, one reload) and each negation is its own
//   inline expansion (two `fchs`, one per position).
#include <math.h>
#pragma intrinsic(sqrt)

#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"     // retail branch: Ego_r.pdb CRightHandedSet {Up +0, Forward +0xc}, 24 bytes; the (up, forward)
                                        // constructor (Ego_r ??0CRightHandedSet@@QAE@ABVC3DVector@@0@Z) expanded in place

namespace {
const CRightHandedSet LEFT_FORTY_FIVE_DEGREES_ORIENTATION(C3DVector(-sqrt(0.5f), -sqrt(0.5f), 0.0f), C3DVector(0.0f, 0.0f, -1.0f));
}

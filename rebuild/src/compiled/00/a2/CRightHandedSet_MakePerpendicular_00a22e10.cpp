// CRightHandedSet::MakePerpendicular -- normalize the basis and remove its projection.
// Flags:
// Retail: 00a22e10,358 bytes,thiscall,no arguments.
// Leaf: ?MakePerpendicular@CRightHandedSet@@QAEXXZ
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_DOT_GROUPED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#include "engine/CRightHandedSet.h"
void CRightHandedSet::MakePerpendicular()
{
 Forward.Normalise();
 Up-=Forward*Up.Dot(Forward);
 Up.Normalise();
}
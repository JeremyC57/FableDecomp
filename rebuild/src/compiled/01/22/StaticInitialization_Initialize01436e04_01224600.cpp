// StaticInitialization::Initialize_01436e04() -- Initialize the scaled cosine threshold.
// Leaf: _$E1
// Identity: descriptive global and initializer names, original identity unproven. Own body computes intrinsic cosine of double012a1140 then multiplies float012a1138 and stores float01436e04. Only middle result has an observed reader00b61de1; other two are unreferenced outside initialization.
// Layout: scalar globals only; exact double angle0.4712389409542084 and float scale13000.0 observed in own read-only data. No owner class or shared header.
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(cos)
#endif
extern const double g_CUnprovenAngle_012a1140;extern const float g_CUnprovenScale_012a1138;float g_CUnprovenCosThreshold_01436e04=(float)(cos(g_CUnprovenAngle_012a1140)*g_CUnprovenScale_012a1138);
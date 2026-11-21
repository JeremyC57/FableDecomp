#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#include <math.h>
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
struct B { char pad[0x34]; CCharString BoneName;
 const CCharString GB0() const { return BoneName; }
 CCharString GB1() const { return BoneName; }
 const CCharString GB2() const { return CCharString(BoneName); }
 const CCharString GB3() const { CCharString r = BoneName; return r; }
 const CCharString GB4() const { return BoneName.GetAsCharArray(); }
};
struct K { char pad[0x20]; C4DVector Scaling;
 void SS0(const C3DVector& s) { Scaling.Initialise(s, 0.0f); }
 void SS1(const C3DVector& s) { Scaling.Initialise(s, 0.0); }
 void SS2(const C3DVector& s) { Scaling.Initialise(s, 0); }
 void SS3(const C3DVector& s) { Scaling = C4DVector(s, 0.0f); }
 void SS4(const C3DVector& s) { Scaling.X = s.X; Scaling.Y = s.Y; Scaling.Z = s.Z; Scaling.W = 0.0f; }
 void SS5(const C3DVector& s) { float w = 0.0f; Scaling.Initialise(s, w); }
};
struct W { short X,Y,Z;
 bool E0(const W& o) const { return X==o.X && Y==o.Y && Z==o.Z; }
 bool E1(const W& o) const { if (X==o.X && Y==o.Y && Z==o.Z) return true; return false; }
 bool E2(const W& o) const { return !(X!=o.X || Y!=o.Y || Z!=o.Z); }
 bool E3(const W& o) const { return (X==o.X) && (Y==o.Y) && (Z==o.Z) ? true : false; }
};
struct G {
 float G0(const C3DVector& v) { return ((float)fabs(v.X) > (float)fabs(v.Y) ? (float)fabs(v.X) : (float)fabs(v.Y)) > (float)fabs(v.Z) ? ((float)fabs(v.X) > (float)fabs(v.Y) ? (float)fabs(v.X) : (float)fabs(v.Y)) : (float)fabs(v.Z); }
 float G1(const C3DVector& v) { return (fabs(v.X) > fabs(v.Y) ? fabs(v.X) : fabs(v.Y)) > fabs(v.Z) ? (fabs(v.X) > fabs(v.Y) ? fabs(v.X) : fabs(v.Y)) : fabs(v.Z); }
 float G2(const C3DVector& v) { return (float)((fabs(v.X) > fabs(v.Y) ? fabs(v.X) : fabs(v.Y)) > fabs(v.Z) ? (fabs(v.X) > fabs(v.Y) ? fabs(v.X) : fabs(v.Y)) : fabs(v.Z)); }
 float G3(const C3DVector& v) { return (fabsf(v.X) > fabsf(v.Y) ? fabsf(v.X) : fabsf(v.Y)) > fabsf(v.Z) ? (fabsf(v.X) > fabsf(v.Y) ? fabsf(v.X) : fabsf(v.Y)) : fabsf(v.Z); }
};
void drv(B& b, K& k, W& w, G& g, const C3DVector& v) {
 b.GB0(); b.GB1(); b.GB2(); b.GB3(); b.GB4();
 k.SS0(v); k.SS1(v); k.SS2(v); k.SS3(v); k.SS4(v); k.SS5(v);
 w.E0(w); w.E1(w); w.E2(w); w.E3(w);
 g.G0(v); g.G1(v); g.G2(v); g.G3(v);
}

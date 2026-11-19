// Behaviour test for 0x00AEDE60 NParticleEngine::CParticleEmitter::SetOrientation(const C3DOrientationInterpolator&): the row's
// defines, includes and body verbatim over the shared headers (+ the hand4d overlay opt-in CMATRIX3X4_INVERSE_NO_TRANSLATE_FORCEINLINE),
// run on a zeroed emitter image with garbage in the three matrices. Covers: the interpolator copied (the caller's object untouched,
// the emitter's vptr slot untouched), MakePerpendicular called on the emitter's start then end set (in that order) before the matrices
// are built, Transform = (Forward x Up, Forward, Up) of the start set with its own old translation kept, InverseTransform the
// rotation-only inverse of the new Transform (cofactors / determinant, translation zeroed), EndTransform from the end set with its own
// old translation kept. Doubles: CRightHandedSet::MakePerpendicular (records the call, scales Up by 2 so the use of the perpendicular
// set is visible), CBaseClass ctor / dtor, the C3DOrientationInterpolator default constructor.
#include <stdio.h>
#include <string.h>
#include <math.h>
#define private public
#define protected public
// ---- row body ----
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_CREATE2D
#define FABLE_RETAIL_CPARTICLEEMITTER_STARTFRAME_PASS23
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CPARTICLE_SPRITE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_BASE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SET_RHSET_INLINE
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_PASS23
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_FORCEINLINE
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CVECTORMAP_INSERT
#include "engine/CParticleEmitter.h"
#include "engine/C3DOrientationInterpolator.h"

void NParticleEngine::CParticleEmitter::SetOrientation(const C3DOrientationInterpolator& orient_interp)
{
    OrientationInterpolator = orient_interp;
    OrientationInterpolator.StartOrientation.MakePerpendicular();
    OrientationInterpolator.EndOrientation.MakePerpendicular();
    Transform.Set(OrientationInterpolator.StartOrientation, Transform.GetTranslation());
    InverseTransform.SetInverseNoTranslate(Transform);
    EndTransform.Set(OrientationInterpolator.EndOrientation, EndTransform.GetTranslation());
}

// ---- doubles ----
static int g_mp_calls = 0;
static const void* g_mp_this[4];
void CRightHandedSet::MakePerpendicular() { if (g_mp_calls < 4) g_mp_this[g_mp_calls] = this; ++g_mp_calls; Up.X *= 2.0f; Up.Y *= 2.0f; Up.Z *= 2.0f; }
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
C3DOrientationInterpolator::C3DOrientationInterpolator() { }

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static int feq(float a, float b) { return fabs(a - b) <= 1e-4f * (1.0f + fabs(b)); }
static void setv(C3DVector& v, float x, float y, float z) { v.X = x; v.Y = y; v.Z = z; }
static int veq(const C3DVector& v, float x, float y, float z) { return feq(v.X, x) && feq(v.Y, y) && feq(v.Z, z); }
static void cross(const C3DVector& a, const C3DVector& b, float* r) { r[0] = a.Y*b.Z - a.Z*b.Y; r[1] = a.Z*b.X - a.X*b.Z; r[2] = a.X*b.Y - a.Y*b.X; }

int main()
{
    static double store[(sizeof(NParticleEngine::CParticleEmitter) + 7) / 8];
    memset(store, 0, sizeof store);
    NParticleEngine::CParticleEmitter& em = *(NParticleEngine::CParticleEmitter*)store;
    float* tf = &em.Transform.E11; float* itf = &em.InverseTransform.E11; float* etf = &em.EndTransform.E11;
    for (int i = 0; i < 12; ++i) { tf[i] = 100.0f + i; itf[i] = 200.0f + i; etf[i] = 300.0f + i; }
    *(void**)&em.OrientationInterpolator = (void*)0x1234;   // the emitter's vptr slot: operator= must leave it

    C3DOrientationInterpolator src;
    setv(src.StartOrientation.Up, 0.0f, 0.0f, 1.0f);   setv(src.StartOrientation.Forward, 0.6f, 0.8f, 0.0f);
    setv(src.EndOrientation.Up, 0.0f, 1.0f, 0.5f);     setv(src.EndOrientation.Forward, 1.0f, 0.0f, 0.0f);
    void* src_vptr = *(void**)&src;

    em.SetOrientation(src);

    check(g_mp_calls == 2, "MakePerpendicular called twice");
    check(g_mp_this[0] == &em.OrientationInterpolator.StartOrientation, "start set made perpendicular first, on the emitter copy");
    check(g_mp_this[1] == &em.OrientationInterpolator.EndOrientation, "end set made perpendicular second, on the emitter copy");
    check(*(void**)&em.OrientationInterpolator == (void*)0x1234, "emitter vptr slot untouched by operator=");
    check(*(void**)&src == src_vptr, "source vptr untouched");
    check(veq(src.StartOrientation.Up, 0.0f, 0.0f, 1.0f) && veq(src.EndOrientation.Up, 0.0f, 1.0f, 0.5f), "source sets untouched");
    check(veq(em.OrientationInterpolator.StartOrientation.Up, 0.0f, 0.0f, 2.0f), "start set copied then made perpendicular");
    check(veq(em.OrientationInterpolator.StartOrientation.Forward, 0.6f, 0.8f, 0.0f), "start forward copied");
    check(veq(em.OrientationInterpolator.EndOrientation.Up, 0.0f, 2.0f, 1.0f), "end set copied then made perpendicular");
    check(veq(em.OrientationInterpolator.EndOrientation.Forward, 1.0f, 0.0f, 0.0f), "end forward copied");

    // Transform: Right = Forward x Up, Forward, Up (the perpendicular start set), old translation 109/110/111 kept.
    float r[3]; cross(em.OrientationInterpolator.StartOrientation.Forward, em.OrientationInterpolator.StartOrientation.Up, r);
    check(feq(tf[0], r[0]) && feq(tf[1], r[1]) && feq(tf[2], r[2]), "Transform row 1 = right");
    check(feq(tf[3], 0.6f) && feq(tf[4], 0.8f) && feq(tf[5], 0.0f), "Transform row 2 = forward");
    check(feq(tf[6], 0.0f) && feq(tf[7], 0.0f) && feq(tf[8], 2.0f), "Transform row 3 = up");
    check(tf[9] == 109.0f && tf[10] == 110.0f && tf[11] == 111.0f, "Transform translation kept");

    // InverseTransform: rotation part times Transform's rotation part = identity, translation zero.
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) {
        float s = 0.0f; for (int k = 0; k < 3; ++k) s += tf[i*3+k] * itf[k*3+j];
        check(feq(s, i == j ? 1.0f : 0.0f), "Transform * InverseTransform = I");
    }
    check(itf[9] == 0.0f && itf[10] == 0.0f && itf[11] == 0.0f, "InverseTransform translation zeroed");

    // EndTransform from the perpendicular end set, its own old translation 309/310/311 kept.
    cross(em.OrientationInterpolator.EndOrientation.Forward, em.OrientationInterpolator.EndOrientation.Up, r);
    check(feq(etf[0], r[0]) && feq(etf[1], r[1]) && feq(etf[2], r[2]), "EndTransform row 1 = right");
    check(feq(etf[3], 1.0f) && feq(etf[4], 0.0f) && feq(etf[5], 0.0f), "EndTransform row 2 = forward");
    check(feq(etf[6], 0.0f) && feq(etf[7], 2.0f) && feq(etf[8], 1.0f), "EndTransform row 3 = up");
    check(etf[9] == 309.0f && etf[10] == 310.0f && etf[11] == 311.0f, "EndTransform translation kept");

    if (!g_fail) printf("00aede60_TEST PASS\n");
    return g_fail;
}

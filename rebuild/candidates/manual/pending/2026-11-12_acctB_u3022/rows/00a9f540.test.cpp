// Behaviour test for 0x00A9F540 C3DAnimationManager::CalcMeshTransformations (CMatrix4x4 overload).
// A host copy of the row's logic (the four static Apply* helpers, the inline keyframe / quaternion helpers with the engine's
// table-driven GFFastArcCos / GFFastSin, and the combination-blend loop) over stub animation data; the out-of-line callees
// (Allocate, CombineBaseAnimations, PeekAnimation / GetAnimation, ConvertTimeToFrames, GetKeyframeInterpolate,
// CQuaternion::operator*=, ApplyBoneMorph*, ApplyParentTransformations, FreeAll) are recorded stubs. Plain C++98.
#include <cstdio>
#include <cmath>

#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
static int g_fail = 0;
static void check(bool c, const char* what) { if (!c) { std::printf("check failed: %s\n", what); g_fail = 1; } }
static bool near(float a, float b, float eps) { return std::fabs(a - b) <= eps; }

static float g_ArcCosTable[1026];
static float g_CosTable[1025];
static void InitTables()
{
    for (int i = 0; i < 1026; ++i) { double c = i / 512.5 - 1.0; if (c > 1.0) c = 1.0; g_ArcCosTable[i] = (float)std::acos(c); }
    for (int j = 0; j < 1025; ++j) g_CosTable[j] = (float)std::cos(6.283185307179586 * j / 1024.0);
}
static long FloatToLongNear(float v) { return (long)std::floor(v + 0.5f); }   // round to nearest (ties are not hit here)
static float GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = FloatToLongNear(cosine - 0.5f);
    float frac = cosine - (float)i;
    float a = g_ArcCosTable[i];
    float b = g_ArcCosTable[i + 1];
    return ((1.0f - frac) * a + frac * b) * 0.15915494f;
}
static float GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = FloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i -= 256;
    i &= 0x3ff;
    float a = g_CosTable[i];
    float b = g_CosTable[i + 1];
    return (1.0f - frac) * a + frac * b;
}
static float GFGetInterpolatedValue(float old_val, float new_val, float interp) { float delta = (new_val - old_val) * interp; return old_val + delta; }

struct Quat {
    float X, Y, Z, W;
    float Dot(const Quat& q) const { return X * q.X + Y * q.Y + Z * q.Z + W * q.W; }
    Quat operator*(const Quat& q) const {          // Hamilton product (stub of the out-of-line 0x00A88B60)
        Quat r;
        r.W = W * q.W - X * q.X - Y * q.Y - Z * q.Z;
        r.X = W * q.X + X * q.W + Y * q.Z - Z * q.Y;
        r.Y = W * q.Y - X * q.Z + Y * q.W + Z * q.X;
        r.Z = W * q.Z + X * q.Y - Y * q.X + Z * q.W;
        return r;
    }
    void operator*=(const Quat& q) { *this = *this * q; }
    void SetSlerpFromIdentity(const Quat& q, float t)
    {
        float cosom = q.W; float sign = 1.0f;
        if (cosom < 0.0f) { sign = -1.0f; cosom = -cosom; }
        float scale0, scale1;
        if (1.0f - cosom > 0.05f) {
            float omega = GFFastArcCos(cosom);
            float sinom = (float)std::sqrt(1.0f - cosom * cosom);
            scale0 = GFFastSin((1.0f - t) * omega) * (1.0f / sinom);
            scale1 = GFFastSin(omega * t) * (1.0f / sinom);
        } else { scale0 = 1.0f - t; scale1 = t; }
        scale1 *= sign;
        X = scale1 * q.X; Y = scale1 * q.Y; Z = scale1 * q.Z; W = scale1 * q.W + scale0;
    }
};
struct Vec4 { float X, Y, Z, W; };
struct Keyframe {
    Quat Rotation; Vec4 Position; Vec4 Scaling;
    void PostApplyRelativeAnimation(const Keyframe& o)
    { Rotation *= o.Rotation; Position.X += o.Position.X; Position.Y += o.Position.Y; Position.Z += o.Position.Z; Position.W += o.Position.W; }
    void PostApplyRelativeAnimationAlpha(const Keyframe& o, float alpha)
    {
        Quat r; r.SetSlerpFromIdentity(o.Rotation, alpha); Rotation *= r;
        Position.X += o.Position.X * alpha; Position.Y += o.Position.Y * alpha; Position.Z += o.Position.Z * alpha; Position.W += o.Position.W * alpha;
    }
    void BlendWith(const Keyframe& o, float t)
    {
        float cosom = o.Rotation.Dot(Rotation); float sign = 1.0f;
        if (cosom < 0.0f) { sign = -1.0f; cosom = -cosom; }
        float scale0, scale1;
        if (1.0f - cosom > 0.05f) {
            float omega = GFFastArcCos(cosom);
            float sinom = (float)std::sqrt(1.0f - cosom * cosom);
            scale0 = GFFastSin((1.0f - t) * omega) * (1.0f / sinom);
            scale1 = GFFastSin(omega * t) * (1.0f / sinom);
        } else { scale0 = 1.0f - t; scale1 = t; }
        scale1 *= sign;
        Quat q;
        q.X = scale1 * o.Rotation.X + scale0 * Rotation.X; q.Y = scale1 * o.Rotation.Y + scale0 * Rotation.Y;
        q.Z = scale1 * o.Rotation.Z + scale0 * Rotation.Z; q.W = scale0 * Rotation.W + scale1 * o.Rotation.W;
        Rotation = q;
        Position.X += (o.Position.X - Position.X) * t; Position.Y += (o.Position.Y - Position.Y) * t;
        Position.Z += (o.Position.Z - Position.Z) * t; Position.W += (o.Position.W - Position.W) * t;
    }
};

// stub animation data: each bone's sequence yields a keyframe that depends on (bone, frame, fraction)
struct Sequence {
    int Bone; bool Identity;
    bool IsIdentity() const { return Identity; }
    void GetKeyframeInterpolate(Keyframe& k, unsigned long frame, float fraction) const
    {
        float a = 0.5f + 0.1f * (Bone + 1) + 0.01f * frame + fraction * 0.05f;
        k.Rotation.X = (float)std::sin(a) * 0.6f; k.Rotation.Y = 0.0f; k.Rotation.Z = (float)std::sin(a) * 0.8f; k.Rotation.W = (float)std::cos(a);
        k.Position.X = 1.0f + Bone; k.Position.Y = 2.0f * frame; k.Position.Z = fraction; k.Position.W = 0.0f;
        k.Scaling.X = k.Scaling.Y = k.Scaling.Z = 1.0f; k.Scaling.W = 0.0f;
    }
};
struct Animation {
    unsigned long AnimType; Sequence Sequences[4]; bool* BoneMask; float SamplesPerSecond; unsigned long FrameCount;
    bool IsRelative() const { return (AnimType & 4) != 0; }
    bool IsPartial() const { return (AnimType & 2) != 0; }
    const Sequence* PeekSequence(unsigned long i) const { return &Sequences[i]; }
    bool IsBoneEnabled(unsigned long b) const { return BoneMask == 0 || BoneMask[b]; }
    void ConvertTimeToFrames(float time, long& frame, float& fraction) const
    { float f = time * SamplesPerSecond; long w = (long)std::floor(f); frame = w; fraction = f - w; frame = (long)((unsigned long)frame % FrameCount); }
};
struct StateAlpha { long AnimationIndex; float AnimationTime; float AlphaWeight; float GetAlphaWeight() const { return AlphaWeight; } };

static void ApplyRelativeAnimation(Keyframe& k, const Sequence& s, long frame, float fraction)
{ if (!s.IsIdentity()) { Keyframe r; s.GetKeyframeInterpolate(r, frame, fraction); k.PostApplyRelativeAnimation(r); } }
static void ApplyRelativeAnimationAlpha(Keyframe& k, const Sequence& s, long frame, float fraction, float alpha)
{ if (!s.IsIdentity()) { Keyframe r; s.GetKeyframeInterpolate(r, frame, fraction); k.PostApplyRelativeAnimationAlpha(r, alpha); } }
static void ApplyPartialAnimation(Keyframe& k, const Sequence& s, bool enabled, long frame, float fraction)
{ if (enabled) s.GetKeyframeInterpolate(k, frame, fraction); }
static void ApplyPartialAnimationAlpha(Keyframe& k, const Sequence& s, bool enabled, long frame, float fraction, float alpha)
{ if (enabled) { Keyframe p; s.GetKeyframeInterpolate(p, frame, fraction); k.BlendWith(p, alpha); } }

// recorded out-of-line callees
static Keyframe g_pool[64]; static unsigned long g_alloc = 0; static int g_combine = 0, g_disp = 0, g_parent = 0, g_scale = 0, g_free = 0;
static Animation g_anims[3];
static void* Allocate(unsigned long bytes) { g_alloc = bytes; return g_pool; }
static void CombineBlended(Keyframe* k, unsigned long n)
{ ++g_combine; for (unsigned long i = 0; i < n; ++i) { Keyframe& f = k[i]; f.Rotation.X = f.Rotation.Y = f.Rotation.Z = 0.0f; f.Rotation.W = 1.0f; f.Position.X = f.Position.Y = f.Position.Z = f.Position.W = 0.0f; } }

// the row, over the host model
static void CalcMesh(const StateAlpha* states, unsigned long blend_count, const float* bone_morph, unsigned long bone_count, bool* returned_early)
{
    *returned_early = false;
    i64 keyframe_count = (i64)(bone_count * 5);
    if (keyframe_count < 0 || keyframe_count > 0x7fffffff) { *returned_early = true; return; }
    Keyframe* keyframes = keyframe_count != 0 ? (Keyframe*)Allocate((unsigned long)keyframe_count * sizeof(Keyframe)) : 0;
    CombineBlended(keyframes, bone_count);
    for (unsigned long i = 0; i < blend_count; ++i) {
        const StateAlpha& state = states[i];
        const Animation* animation = &g_anims[state.AnimationIndex];
        long frame; float fraction;
        animation->ConvertTimeToFrames(state.AnimationTime, frame, fraction);
        float alpha = state.GetAlphaWeight();
        if (animation->IsRelative()) {
            if (alpha == 1.0f) { for (unsigned long b = 0; b < bone_count; ++b) ApplyRelativeAnimation(keyframes[b], *animation->PeekSequence(b), frame, fraction); }
            else { for (unsigned long b = 0; b < bone_count; ++b) ApplyRelativeAnimationAlpha(keyframes[b], *animation->PeekSequence(b), frame, fraction, alpha); }
        } else if (animation->IsPartial()) {
            if (alpha == 1.0f) { for (unsigned long b = 0; b < bone_count; ++b) ApplyPartialAnimation(keyframes[b], *animation->PeekSequence(b), animation->IsBoneEnabled(b), frame, fraction); }
            else { for (unsigned long b = 0; b < bone_count; ++b) ApplyPartialAnimationAlpha(keyframes[b], *animation->PeekSequence(b), animation->IsBoneEnabled(b), frame, fraction, alpha); }
        }
    }
    if (bone_morph) ++g_disp;
    ++g_parent;
    if (bone_morph) ++g_scale;
    ++g_free;
}

static float QuatLen(const Quat& q) { return (float)std::sqrt(q.Dot(q)); }

int main()
{
    InitTables();
    // fast trig against the C library
    check(near(GFFastArcCos(0.5f), (float)(std::acos(0.5) / 6.283185307179586), 2e-3f), "GFFastArcCos");
    check(near(GFFastSin(0.125f), (float)std::sin(6.283185307179586 * 0.125), 2e-3f), "GFFastSin");
    bool mask[4] = { true, false, true, false };
    for (int a = 0; a < 3; ++a) {
        Animation& an = g_anims[a];
        an.AnimType = a == 0 ? 4 : (a == 1 ? 3 : 0); an.BoneMask = a == 1 ? mask : 0; an.SamplesPerSecond = 10.0f; an.FrameCount = 100;
        for (int b = 0; b < 4; ++b) { an.Sequences[b].Bone = b; an.Sequences[b].Identity = (a == 0 && b == 2); }
    }
    // 1. relative animation at full weight: the identity bone (2) untouched, the others multiplied in
    StateAlpha s1[1] = { { 0, 1.0f, 1.0f } };
    bool early = false; float morph[3] = { 1, 1, 1 };
    CalcMesh(s1, 1, morph, 4, &early);
    check(!early && g_alloc == 4 * 5 * sizeof(Keyframe) && g_combine == 1 && g_disp == 1 && g_scale == 1 && g_parent == 1 && g_free == 1, "calls (relative)");
    check(g_pool[2].Rotation.W == 1.0f && g_pool[2].Position.X == 0.0f, "identity bone skipped");
    Keyframe ref; g_anims[0].Sequences[0].GetKeyframeInterpolate(ref, 10, 0.0f);
    check(near(g_pool[0].Rotation.W, ref.Rotation.W, 1e-5f) && near(g_pool[0].Position.X, 1.0f, 1e-6f) && near(g_pool[0].Position.Y, 20.0f, 1e-5f), "relative applied");
    // 2. relative at alpha 0.5: the slerp from identity halves the angle, positions scaled by alpha
    StateAlpha s2[1] = { { 0, 1.0f, 0.5f } };
    CalcMesh(s2, 1, 0, 4, &early);
    check(g_disp == 1 && g_scale == 1 && g_parent == 2, "no morph calls without a morph");
    float half = (float)std::cos(0.5 * std::acos(ref.Rotation.W));
    check(near(g_pool[0].Rotation.W, half, 1e-2f) && near(QuatLen(g_pool[0].Rotation), 1.0f, 1e-2f) && near(g_pool[0].Position.X, 0.5f, 1e-5f), "relative alpha slerp");
    // 3. partial at full weight: enabled bones overwritten, disabled ones keep the base pose
    StateAlpha s3[1] = { { 1, 2.0f, 1.0f } };
    CalcMesh(s3, 1, 0, 4, &early);
    Keyframe r1; g_anims[1].Sequences[0].GetKeyframeInterpolate(r1, 20, 0.0f);
    check(near(g_pool[0].Rotation.X, r1.Rotation.X, 1e-6f) && g_pool[1].Rotation.W == 1.0f && g_pool[1].Position.Y == 0.0f, "partial overwrite / mask");
    // 4. partial at alpha 0.5 blends towards the sampled pose; a base-type animation (2) changes nothing
    StateAlpha s4[2] = { { 1, 2.0f, 0.5f }, { 2, 2.0f, 1.0f } };
    CalcMesh(s4, 2, 0, 4, &early);
    float hw = (float)std::cos(0.5 * std::acos(r1.Rotation.W));
    check(near(g_pool[0].Rotation.W, hw, 1e-2f) && near(g_pool[0].Position.Y, 20.0f, 1e-4f) && g_pool[3].Rotation.W == 1.0f, "partial blend");
    // 5. an over-long keyframe count returns before allocating or freeing
    unsigned long frees = g_free;
    CalcMesh(s1, 1, 0, 0x20000000UL, &early);
    check(early && g_free == frees, "range check returns early");
    if (g_fail) { std::printf("00a9f540_TEST FAIL\n"); return 1; }
    std::printf("00a9f540_TEST PASS\n");
    return 0;
}

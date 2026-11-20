// Behaviour test for 0x00a53860 C3DAnimationSequence::InlineGetKeyframeInterpolate (retail's out-of-line copy) (links the REAL row object; the slerp, the table helpers and the
// word-position blend are expanded in it). Raw native-layout storage; every offset is one the row reads: C3DAnimationSequenceData
// FrameCount +0x08 (ushort), track modes byte +0x0B (rotation bits 0..1, position bits 2..3), PositionFactor +0x0C, RotationTrack
// +0x14 (16-byte quaternions), PositionTrack +0x20 (6-byte word vectors), PalettedRotationTrack +0x24, PalettedPositionTrack +0x28;
// C3DKeyframe Rotation +0x00, Position +0x10 (W = 0 written at +0x1C), Scaling +0x20 (never written); C3DAnimationSequence Data +0x00 (0x00A53863 `mov ecx,[ecx]`).
// The two engine tables the row reads are built here: arc-cosine (1026 entries, acos(k * 2 / 1025 - 1)) and cosine (1025 entries,
// cos(2 * pi * k / 1024)). Expected values come from an independent double-precision model of the same table interpolation
// (round-to-nearest index, linear interpolation, the quarter-turn shift for sine), cross-checked against the exact slerp.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

float GFastArcCosTable[1026];
float GFastCosTable[1025];

class C3DKeyframe { public: float b[12]; };
class C3DAnimationSequenceData {
public:
    void GetKeyframeInterpolate(C3DKeyframe& dest, unsigned long frame, float fraction) const;
    void InlineGetKeyframeInterpolate(C3DKeyframe& dest, unsigned long frame, float fraction) const;
    unsigned char raw[0x2C];
};
class C3DAnimationSequence {
public:
    void InlineGetKeyframeInterpolate(C3DKeyframe& dest, unsigned long frame, float fraction) const;
    C3DAnimationSequenceData* Data; short ParentIndex; bool Shared;
};

static int failures, cases;
static void fail(const char* what, int id) { ++failures; if (failures < 30) printf("FAIL %s (case %d)\n", what, id); }
template<class T> static void put(void* base, int off, T v) { memcpy((unsigned char*)base + off, &v, sizeof v); }

enum { MAXF = 16 };
static float rot[MAXF][4];
static short pos[MAXF][3];
static unsigned char prot[MAXF], ppos[MAXF];
static C3DAnimationSequenceData seq;
static C3DAnimationSequence wrapper;

static double Rint(double x) { double f = floor(x); double d = x - f; if (d > 0.5) return f + 1; if (d < 0.5) return f; return (fmod(f, 2.0) == 0.0) ? f : f + 1; }
static double ModelArcCos(double value) {
    double p = (value + 1.0) * 512.5; long i = (long)Rint(p - 0.5); double t = p - i;
    return ((1.0 - t) * GFastArcCosTable[i] + t * GFastArcCosTable[i + 1]) * 0.15915494309189535; }
static double ModelSin(double angle) {
    double p = angle * 1024.0; long i = (long)Rint(p - 0.5); double t = p - i; i = (i - 256) & 1023;
    return (1.0 - t) * GFastCosTable[i] + t * GFastCosTable[i + 1]; }
static void ModelSlerp(const float* a, const float* b, double w, double* out, bool* lerp) {
    double c = (double)a[0] * b[0] + (double)a[1] * b[1] + (double)a[2] * b[2] + (double)a[3] * b[3];
    double sign = 1.0; if (c < 0.0) { c = -c; sign = -1.0; }
    double s1, s2;
    if (1.0 - c > 0.05) { double angle = ModelArcCos(c); double sina = sqrt(1.0 - c * c); s1 = ModelSin((1.0 - w) * angle) / sina; s2 = ModelSin(w * angle) / sina; *lerp = false; }
    else { s1 = 1.0 - w; s2 = w; *lerp = true; }
    s2 *= sign;
    for (int k = 0; k < 4; ++k) out[k] = s1 * a[k] + s2 * b[k];
}
static void ExactSlerp(const float* a, const float* b, double w, double* out) {
    double c = 0; for (int k = 0; k < 4; ++k) c += (double)a[k] * b[k];
    double sign = 1.0; if (c < 0) { c = -c; sign = -1.0; }
    double om = acos(c > 1.0 ? 1.0 : c), so = sin(om);
    double s1 = sin((1.0 - w) * om) / so, s2 = sign * sin(w * om) / so;
    for (int k = 0; k < 4; ++k) out[k] = s1 * a[k] + s2 * b[k];
}
static bool Near(double a, double b, double tol) { double d = a - b; if (d < 0) d = -d; return d <= tol; }

static void Call(C3DKeyframe& dest, unsigned long frame, float fraction) { wrapper.InlineGetKeyframeInterpolate(dest, frame, fraction); }

static void RunCase(int id, int frame_count, int rot_mode, int pos_mode, float position_factor, unsigned long frame, float fraction) {
    memset(&seq, 0xEE, sizeof seq);
    put<unsigned short>(seq.raw, 0x08, (unsigned short)frame_count);
    put<unsigned char>(seq.raw, 0x0B, (unsigned char)(rot_mode | (pos_mode << 2) | 0xC0));
    put<float>(seq.raw, 0x0C, position_factor);
    put<void*>(seq.raw, 0x14, rot); put<void*>(seq.raw, 0x20, pos); put<void*>(seq.raw, 0x24, prot); put<void*>(seq.raw, 0x28, ppos);
    C3DKeyframe dest; for (int k = 0; k < 12; ++k) dest.b[k] = 777.0f + k;
    Call(dest, frame, fraction);
    ++cases;
    unsigned long next = (frame + 1 < (unsigned long)frame_count) ? frame + 1 : 0;
    // rotation
    if (rot_mode & 2) {
        unsigned long i0 = frame, i1 = next;
        if (rot_mode == 3) { i0 = prot[frame]; i1 = prot[next]; }
        if (rot_mode == 3 && i0 == i1) { if (memcmp(dest.b, rot[i0], 16) != 0) fail("equal palette entries: rotation copied", id); }
        else {
            double m[4], x[4]; bool lerp; ModelSlerp(rot[i0], rot[i1], fraction, m, &lerp); ExactSlerp(rot[i0], rot[i1], fraction, x);
            for (int k = 0; k < 4; ++k) {
                if (!Near(dest.b[k], m[k], 3e-5)) fail("slerp against the table model", id);
                if (!lerp && !Near(dest.b[k], x[k], 4e-3)) fail("slerp against the exact slerp", id);
            }
        }
    } else if (rot_mode == 1) { if (memcmp(dest.b, rot[0], 16) != 0) fail("constant rotation", id); }
    else { if (dest.b[0] != 0.0f || dest.b[1] != 0.0f || dest.b[2] != 0.0f || dest.b[3] != 1.0f) fail("identity rotation", id); }
    // position
    double ex[3], mag[3] = { 0, 0, 0 };
    if (pos_mode & 2) {
        unsigned long i0 = frame, i1 = next;
        if (pos_mode == 3) { i0 = ppos[frame]; i1 = ppos[next]; }
        float w1 = 1.0f - fraction, w2 = fraction;
        for (int k = 0; k < 3; ++k) { double a = (double)pos[i0][k] * ((double)w1 * position_factor), b = (double)pos[i1][k] * ((double)w2 * position_factor); ex[k] = a + b; mag[k] = fabs(a) + fabs(b); }
    } else if (pos_mode == 1) { for (int k = 0; k < 3; ++k) { ex[k] = (double)pos[0][k] * position_factor; mag[k] = fabs(ex[k]); } }
    else { ex[0] = ex[1] = ex[2] = 0.0; }
    for (int k = 0; k < 3; ++k) { double tol = 1e-7 + 4e-7 * mag[k]; if (!Near(dest.b[4 + k], ex[k], tol)) fail("position", id); }
    if (dest.b[7] != 0.0f) fail("position W cleared", id);
    for (int s = 8; s < 12; ++s) if (dest.b[s] != 777.0f + s) fail("scaling untouched", id);
}

static unsigned long g_seed = 2463534242UL;
static unsigned long Rnd() { g_seed ^= g_seed << 13; g_seed ^= g_seed >> 17; g_seed ^= g_seed << 5; return g_seed; }
static double Uni() { return (Rnd() & 0xFFFFFF) / 16777216.0; }
static void Normalise(float* q) { double n = sqrt((double)q[0] * q[0] + (double)q[1] * q[1] + (double)q[2] * q[2] + (double)q[3] * q[3]); for (int k = 0; k < 4; ++k) q[k] = (float)(q[k] / n); }
static void RandomTracks(double spread) {
    float base[4]; for (int k = 0; k < 4; ++k) base[k] = (float)(Uni() * 2 - 1); Normalise(base);
    for (int f = 0; f < MAXF; ++f) {
        for (int k = 0; k < 4; ++k) rot[f][k] = (float)(base[k] + spread * (Uni() * 2 - 1));
        Normalise(rot[f]);
        if (Rnd() & 1) for (int k2 = 0; k2 < 4; ++k2) rot[f][k2] = -rot[f][k2];        // both hemispheres: the sign flip
        for (int c = 0; c < 3; ++c) pos[f][c] = (short)((long)(Rnd() % 60001) - 30000);
        prot[f] = (unsigned char)(Rnd() % MAXF); ppos[f] = (unsigned char)(Rnd() % MAXF);
    }
}

int main() {
    const double PI = 3.14159265358979323846;
    for (int k = 0; k < 1026; ++k) { double v = k * 2.0 / 1025.0 - 1.0; GFastArcCosTable[k] = (float)acos(v > 1.0 ? 1.0 : v); }
    for (int j = 0; j < 1025; ++j) GFastCosTable[j] = (float)cos(2.0 * PI * j / 1024.0);
    wrapper.Data = &seq; wrapper.ParentIndex = -1; wrapper.Shared = false;
    int id = 0;
    // hand-made tracks: every mode, the wrap to frame 0, equal / different palette entries
    RandomTracks(1.0);
    for (int fc = 1; fc <= 5; ++fc)
        for (int rm = 0; rm < 4; ++rm)
            for (int pm = 0; pm < 4; ++pm)
                for (int fr = 0; fr < fc; ++fr) {
                    prot[fr] = (unsigned char)(fr & 1 ? 3 : 7);                           // neighbours differ
                    RunCase(++id, fc, rm, pm, 0.01f, fr, 0.25f);
                    RunCase(++id, fc, rm, pm, 0.5f, fr, 0.0f);
                    RunCase(++id, fc, rm, pm, 2.0f, fr, 0.75f);
                }
    for (int e = 0; e < MAXF; ++e) prot[e] = 5;                                           // all palette entries equal: copy path
    for (int f2 = 0; f2 < 6; ++f2) { RunCase(++id, 6, 3, 3, 1.0f, f2, 0.4f); RunCase(++id, 6, 2, 2, 1.0f, f2, 0.4f); }
    // equal palette entries with NON-unit quaternions: the copy path must not run the slerp (which would rescale them)
    for (int e2 = 0; e2 < MAXF; ++e2) { prot[e2] = 9; for (int k3 = 0; k3 < 4; ++k3) rot[e2][k3] *= 0.5f; }
    for (int f3 = 0; f3 < 6; ++f3) { RunCase(++id, 6, 3, 1, 1.0f, f3, 0.4f); RunCase(++id, 6, 3, 0, 1.0f, f3, 0.9f); }
    // near-parallel neighbours (the plain-lerp branch) and wide ones, both hemispheres
    for (int r = 0; r < 3000; ++r) {
        RandomTracks(r % 3 == 0 ? 0.02 : (r % 3 == 1 ? 0.3 : 1.5));
        int fc2 = 1 + (int)(Rnd() % MAXF); unsigned long fr2 = Rnd() % fc2;
        RunCase(++id, fc2, (int)(Rnd() & 3), (int)(Rnd() & 3), (float)(0.001 + Uni() * 0.1), fr2, (float)Uni());
        RunCase(++id, fc2, 2 + (int)(Rnd() & 1), 2 + (int)(Rnd() & 1), (float)(0.001 + Uni()), fr2, (float)Uni());
    }
    if (failures) { printf("00a53860_TEST FAIL %d of %d\n", failures, cases); return 1; }
    printf("00a53860_TEST PASS (%d cases)\n", cases);
    return 0;
}
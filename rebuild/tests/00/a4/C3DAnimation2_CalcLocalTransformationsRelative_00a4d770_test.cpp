// Behaviour test for 0x00A4D770 C3DAnimation2::CalcLocalTransformationsRelative (links the REAL row object: the expanded
// GetKeyframeInterpolateFixPos / InlineGetKeyframeInterpolate / SetSlerp / GFFastArcCos / GFFastSin and the out-of-line
// ConvertTimeToFrames COMDAT run as compiled).
// What retail does: every bone's interpolated keyframe goes to the local kf_rel, which nothing reads, so the only memory the row
// writes outside its frame is dest[c] = the mesh's rest keyframe c, for c < SequenceCount. The compiler dropped the stores to
// kf_rel but kept what it could not prove dead: the next-frame choice, the palette reads, the quaternion dot product with its two
// compares and the three float-to-long conversions (the arc-cosine index and the two sine indices). Those survive only in the
// row's own frame, so the test reads the frame after the return (the bytes just under the caller's stack pointer; nothing runs
// between the return and the copy) and compares every slot with an independent model.
// Frame slots, E = stack pointer at entry (0x00A4D770..0x00A4D911): E+0x0C frame (the dead slot of t, written by
//   ConvertTimeToFrames), E-0x14 fraction, E-0x10 this, E-0x1C the angle in turns (float), E-0x18 the arc-cosine table index,
//   E-0x0C the last float handed to a conversion, E-0x08 / E-0x04 the two sine positions. E-0x1C / -0x18 / -0x0C / -0x08 / -0x04
//   keep the pre-filled 0xA5 bytes when no bone of the call reached the spherical interpolation, else the last such bone's values.
// Fixtures (raw native layout, every offset read by an instruction of the row):
//   C3DAnimation2 (0x7C): SamplesPerSecond +0x50, FrameCount +0x54, SequenceCount +0x58, Sequences +0x5C (8-byte counted pointers,
//     the data pointer first), RigidBoneMask +0x6C, RigidBonePositions +0x70.
//   C3DAnimationSequenceData (0x2C): FrameCount (word) +8, track modes +0x0B (rotation bits 0-1, position bits 2-3),
//     PositionFactor +0x0C, RotationTrack +0x14 (16-byte quaternions X Y Z W), PositionTrack +0x20, PalettedRotationTrack +0x24,
//     PalettedPositionTrack +0x28.
//   C3DMesh2: BoneKeyframes +0xA0 (0x30-byte C3DKeyframe). Globals GFastArcCosTable 0x013CB530 / GFastCosTable 0x013CD550.
// Reference model (independent, the row's x87 operations one statement each so it rounds the same under either precision
//   setting): frames = t * rate; whole = floor(frames) (nearest of frames - 0.5, plus one when frames is the next integer);
//   fraction = frames - whole; frame = unsigned whole mod FrameCount (+ FrameCount when negative as a long). Per bone:
//   next = frame + 1 < the sequence's FrameCount ? frame + 1 : 0; rotation mode 2 or 3 only: the two quaternions are frame / next
//   (mode 3: through the palette; equal palette entries stop here); cosa = |dot|; only when 1 - cosa > 0.05:
//   pos = (cosa + 1) * 512.5, index = nearest(pos - 0.5), angle = ((1 - (pos - index)) * table[index] + (pos - index) *
//   table[index + 1]) * 0.15915494, sine positions nearest((1 - fraction) * angle * 1024 - 0.5) and nearest(fraction * angle *
//   1024 - 0.5).
// Checks: (1) hand-written anchors (exact frame / index values, the 0.05 limit hit exactly, wrap of the next frame, palette
//   equality, zero bones, dest aliasing the rest pose, a FrameCount above 2^31); (2) 6000 seeded random cases under 53-bit and
//   again under 24-bit precision (the game's Direct3D setting); in each: dest (with guard keyframes either side), every input
//   image unchanged, and the frame slots above.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <float.h>

class C3DKeyframe;
class C3DMesh2;
class C3DAnimation2
{
public:
    void CalcLocalTransformationsRelative(C3DKeyframe* dest, unsigned long dest_size, float t, const C3DMesh2& mesh) const;
};
float GFastArcCosTable[1026];
float GFastCosTable[1025];

enum { MAXSEQ = 6, NQ = 24, RES = 0x100 };
struct RawSeq { unsigned char* data; void* info; };
static unsigned char g_anim[0x7C], g_anim0[0x7C];
static RawSeq g_seqs[MAXSEQ + 1], g_seqs0[MAXSEQ + 1];
static unsigned char g_sd[MAXSEQ + 1][0x2C], g_sd0[MAXSEQ + 1][0x2C];
static float g_rot[MAXSEQ + 1][NQ][4], g_rot0[MAXSEQ + 1][NQ][4];
static short g_pos[MAXSEQ + 1][NQ][3], g_pos0[MAXSEQ + 1][NQ][3];
static unsigned char g_palr[MAXSEQ + 1][NQ], g_palr0[MAXSEQ + 1][NQ], g_palp[MAXSEQ + 1][NQ], g_palp0[MAXSEQ + 1][NQ];
static unsigned char g_mask[MAXSEQ + 1], g_mask0[MAXSEQ + 1];
static float g_rigid[MAXSEQ + 1][3], g_rigid0[MAXSEQ + 1][3];
static unsigned char g_mesh[0xB0], g_mesh0[0xB0];
static unsigned char g_kf[MAXSEQ + 2][0x30], g_kf0[MAXSEQ + 2][0x30];
static unsigned char g_dest[MAXSEQ + 4][0x30], g_dest0[MAXSEQ + 4][0x30];
static unsigned char g_res[RES];
static unsigned char* g_sp;
static int g_fail, g_cases, g_slerps, g_skips;
static unsigned long g_seed = 77123;
static unsigned long Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }
static float RndF(long range, float div) { return (float)((long)(Rnd() % (unsigned long)(2 * range + 1)) - range) / div; }

static void Fail(const char* what, double got, double want)
{
    if (g_fail++ < 20) printf("FAIL case %d: %s got %.9g want %.9g\n", g_cases, what, got, want);
}
static void FailX(const char* what, unsigned long got, unsigned long want)
{
    if (g_fail++ < 20) printf("FAIL case %d: %s got %08lX want %08lX\n", g_cases, what, got, want);
}
static unsigned long Bits(float f) { unsigned long u; memcpy(&u, &f, 4); return u; }

// the x87 store-as-integer under round-to-nearest (ties to even), without arithmetic that could round under 24-bit precision
static long Nearest(double v)
{
    long i = (long)v;
    double r = v - (double)i;
    if (r > 0.5) return i + 1;
    if (r < -0.5) return i - 1;
    if (r == 0.5) return i + (i & 1);
    if (r == -0.5) return i - (i & 1);
    return i;
}

static void Put32(unsigned char* p, unsigned long v) { memcpy(p, &v, 4); }
static void PutF(unsigned char* p, float v) { memcpy(p, &v, 4); }
static unsigned long Res32(int k) { unsigned long u; memcpy(&u, g_res + (RES - 0x14) + k, 4); return u; }   // the dword at E + k

// One call: nothing between the return and the copy of the bytes under the stack pointer.
static void CallRow(const C3DAnimation2* anim, C3DKeyframe* dest, unsigned long dest_size, float t, const C3DMesh2* mesh)
{
    unsigned char* sp = (unsigned char*)_alloca(4);
    int i;
    for (i = 1; i <= RES; ++i) sp[-i] = 0xA5;
    anim->CalcLocalTransformationsRelative(dest, dest_size, t, *mesh);
    for (i = 0; i < RES; ++i) g_res[i] = sp[i - RES];
    g_sp = sp;
}

struct Scene
{
    int nseq; unsigned long frame_count; float rate; float t; unsigned long dest_size; bool alias;
    unsigned long seq_fc[MAXSEQ]; unsigned char modes[MAXSEQ]; bool rigid_mask;
};

static void FillRandom(void* p, int n) { unsigned char* b = (unsigned char*)p; for (int i = 0; i < n; ++i) b[i] = (unsigned char)Rnd(); }

// the fixtures' random content (tracks, palettes, keyframes); the scene fields are applied by Run
static void Randomise(int quat_kind)
{
    int s, i, k;
    FillRandom(g_anim, sizeof g_anim); FillRandom(g_mesh, sizeof g_mesh); FillRandom(g_kf, sizeof g_kf);
    FillRandom(g_sd, sizeof g_sd); FillRandom(g_pos, sizeof g_pos); FillRandom(g_mask, sizeof g_mask);
    for (s = 0; s <= MAXSEQ; ++s)
    {
        for (i = 0; i < NQ; ++i)
        {
            g_palr[s][i] = (unsigned char)(Rnd() % NQ); g_palp[s][i] = (unsigned char)(Rnd() % NQ);
            int kind = quat_kind == 3 ? (int)(Rnd() % 3) : quat_kind;
            float q[4];
            for (k = 0; k < 4; ++k) q[k] = kind == 2 ? RndF(8, 8.0f) : RndF(10000, 10000.0f);
            if (kind == 1)
            {
                if (i > 0 && (Rnd() & 1))                               // a neighbour of the previous quaternion: dots near 1
                    for (k = 0; k < 4; ++k) q[k] = g_rot[s][i - 1][k] + RndF(10000, 30000.0f);
                float len = (float)sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
                if (len < 0.001f) { q[3] = 1.0f; len = 1.0f; }
                for (k = 0; k < 4; ++k) q[k] /= len;
                if (Rnd() % 4 == 0) for (k = 0; k < 4; ++k) q[k] = -q[k];
            }
            for (k = 0; k < 4; ++k) g_rot[s][i][k] = q[k];
        }
        for (k = 0; k < 3; ++k) g_rigid[s][k] = RndF(1000, 10.0f);
    }
}

static void Run(const Scene& sc)
{
    ++g_cases;
    int s, i;
    PutF(g_anim + 0x50, sc.rate); Put32(g_anim + 0x54, sc.frame_count); Put32(g_anim + 0x58, (unsigned long)sc.nseq);
    Put32(g_anim + 0x5C, (unsigned long)g_seqs);
    Put32(g_anim + 0x6C, sc.rigid_mask ? (unsigned long)g_mask : 0); Put32(g_anim + 0x70, (unsigned long)g_rigid);
    for (s = 0; s <= MAXSEQ; ++s)
    {
        g_seqs[s].data = g_sd[s]; g_seqs[s].info = (void*)(0x51000000 + s);
        if (s < MAXSEQ)
        {
            unsigned short fc = (unsigned short)sc.seq_fc[s];
            memcpy(g_sd[s] + 8, &fc, 2); g_sd[s][0x0B] = sc.modes[s];
        }
        PutF(g_sd[s] + 0x0C, 0.03125f);
        Put32(g_sd[s] + 0x14, (unsigned long)g_rot[s]); Put32(g_sd[s] + 0x20, (unsigned long)g_pos[s]);
        Put32(g_sd[s] + 0x24, (unsigned long)g_palr[s]); Put32(g_sd[s] + 0x28, (unsigned long)g_palp[s]);
    }
    Put32(g_mesh + 0xA0, (unsigned long)g_kf);
    for (s = 0; s < MAXSEQ + 4; ++s) for (i = 0; i < 0x30; ++i) g_dest[s][i] = (unsigned char)(0xC0 + s * 7 + i);
    unsigned char* dest = sc.alias ? g_kf[0] : g_dest[1];

    memcpy(g_anim0, g_anim, sizeof g_anim); memcpy(g_seqs0, g_seqs, sizeof g_seqs); memcpy(g_sd0, g_sd, sizeof g_sd);
    memcpy(g_rot0, g_rot, sizeof g_rot); memcpy(g_pos0, g_pos, sizeof g_pos); memcpy(g_palr0, g_palr, sizeof g_palr);
    memcpy(g_palp0, g_palp, sizeof g_palp); memcpy(g_mask0, g_mask, sizeof g_mask); memcpy(g_rigid0, g_rigid, sizeof g_rigid);
    memcpy(g_mesh0, g_mesh, sizeof g_mesh); memcpy(g_kf0, g_kf, sizeof g_kf); memcpy(g_dest0, g_dest, sizeof g_dest);

    // ---- model ----
    double fr = (double)sc.t * (double)sc.rate;
    float frames = (float)fr;
    double h = (double)frames - 0.5;
    long whole = Nearest(h);
    float up = (float)((double)whole + 1.0);
    if (Bits(frames) == Bits(up)) ++whole;
    float fraction = (float)((double)frames - (double)whole);
    long frame = (long)((unsigned long)whole % sc.frame_count);
    if (frame < 0) frame = (long)((unsigned long)frame + sc.frame_count);

    bool any = false; float w_angle = 0.0f, w_last = 0.0f; long w_index = 0, w_s1 = 0, w_s2 = 0;
    for (s = 0; s < sc.nseq; ++s)
    {
        unsigned long next = ((unsigned long)frame + 1 < (unsigned long)(unsigned short)sc.seq_fc[s]) ? (unsigned long)frame + 1 : 0;
        int mode = sc.modes[s] & 3;
        if (mode < 2) continue;
        unsigned long a = (unsigned long)frame, b = next;
        if (mode == 3)
        {
            a = g_palr[s][frame]; b = g_palr[s][next];
            if (a == b) { ++g_skips; continue; }
        }
        const float* q1 = g_rot[s][a]; const float* q2 = g_rot[s][b];
        double cosa = (double)q1[3] * (double)q2[3];
        double term = (double)q1[2] * (double)q2[2];
        cosa = cosa + term;
        term = (double)q1[1] * (double)q2[1];
        cosa = cosa + term;
        term = (double)q1[0] * (double)q2[0];
        cosa = cosa + term;
        if (cosa < 0.0) cosa = -cosa;
        double rest = 1.0 - cosa;
        if (!(rest > (double)0.05f)) { ++g_skips; continue; }
        double pos = cosa + 1.0;
        pos = pos * 512.5;
        float posf = (float)pos;
        double pm = pos - 0.5;
        float idxf = (float)pm;
        long index = Nearest((double)idxf);
        double interp = (double)posf - (double)index;
        double inv = 1.0 - interp;
        double p1 = inv * (double)GFastArcCosTable[index];
        double p2 = interp * (double)GFastArcCosTable[index + 1];
        double sum = p1 + p2;
        sum = sum * (double)0.15915494f;
        float angle = (float)sum;
        double x = 1.0 - (double)fraction;
        x = x * (double)angle;
        x = x * 1024.0;
        x = x - 0.5;
        float xf = (float)x;
        double y = (double)angle * (double)fraction;
        y = y * 1024.0;
        y = y - 0.5;
        float yf = (float)y;
        any = true; w_angle = angle; w_index = index; w_s1 = Nearest((double)xf); w_s2 = Nearest((double)yf); w_last = yf;
        ++g_slerps;
    }

    CallRow((const C3DAnimation2*)g_anim, (C3DKeyframe*)dest, sc.dest_size, sc.t, (const C3DMesh2*)g_mesh);

    // ---- the row's frame ----
    if (Res32(0x04) != (unsigned long)dest || Res32(0x08) != sc.dest_size || Res32(0x10) != (unsigned long)g_mesh)
        FailX("frame not where expected (argument slots)", Res32(0x04), (unsigned long)dest);
    if (Res32(-0x10) != (unsigned long)g_anim) FailX("saved this", Res32(-0x10), (unsigned long)g_anim);
    if (Res32(0x0C) != (unsigned long)frame) FailX("frame", Res32(0x0C), (unsigned long)frame);
    if (Res32(-0x14) != Bits(fraction)) FailX("fraction bits", Res32(-0x14), Bits(fraction));
    if (any)
    {
        if (Res32(-0x18) != (unsigned long)w_index) FailX("arc-cosine index", Res32(-0x18), (unsigned long)w_index);
        if (Res32(-0x1C) != Bits(w_angle)) FailX("angle bits", Res32(-0x1C), Bits(w_angle));
        if (Res32(-0x08) != (unsigned long)w_s1) FailX("sine position 1", Res32(-0x08), (unsigned long)w_s1);
        if (Res32(-0x04) != (unsigned long)w_s2) FailX("sine position 2", Res32(-0x04), (unsigned long)w_s2);
        if (Res32(-0x0C) != Bits(w_last)) FailX("last converted float", Res32(-0x0C), Bits(w_last));
    }
    else
    {
        static const int slots[5] = { -0x1C, -0x18, -0x0C, -0x08, -0x04 };
        for (i = 0; i < 5; ++i) if (Res32(slots[i]) != 0xA5A5A5A5) FailX("slot written without a spherical interpolation", Res32(slots[i]), 0xA5A5A5A5);
    }

    // ---- dest and the inputs ----
    if (sc.alias)
    {
        if (memcmp(g_kf, g_kf0, sizeof g_kf)) Fail("aliased rest pose changed", 1, 0);
        if (memcmp(g_dest, g_dest0, sizeof g_dest)) Fail("dest buffer written in an aliased call", 1, 0);
    }
    else
    {
        for (s = 0; s < MAXSEQ + 4; ++s)
        {
            int c = s - 1;
            const unsigned char* want = (c >= 0 && c < sc.nseq) ? g_kf0[c] : g_dest0[s];
            if (memcmp(g_dest[s], want, 0x30)) Fail("dest keyframe", s, c);
        }
        if (memcmp(g_kf, g_kf0, sizeof g_kf)) Fail("rest pose changed", 1, 0);
    }
    if (memcmp(g_anim, g_anim0, sizeof g_anim)) Fail("animation changed", 1, 0);
    if (memcmp(g_seqs, g_seqs0, sizeof g_seqs)) Fail("sequence table changed", 1, 0);
    if (memcmp(g_sd, g_sd0, sizeof g_sd)) Fail("sequence data changed", 1, 0);
    if (memcmp(g_rot, g_rot0, sizeof g_rot)) Fail("rotation track changed", 1, 0);
    if (memcmp(g_pos, g_pos0, sizeof g_pos)) Fail("position track changed", 1, 0);
    if (memcmp(g_palr, g_palr0, sizeof g_palr) || memcmp(g_palp, g_palp0, sizeof g_palp)) Fail("palette changed", 1, 0);
    if (memcmp(g_mask, g_mask0, sizeof g_mask) || memcmp(g_rigid, g_rigid0, sizeof g_rigid)) Fail("rigid bone data changed", 1, 0);
    if (memcmp(g_mesh, g_mesh0, sizeof g_mesh)) Fail("mesh changed", 1, 0);
}

static Scene Base(int nseq, unsigned long frame_count, float rate, float t)
{
    Scene sc; memset(&sc, 0, sizeof sc);
    sc.nseq = nseq; sc.frame_count = frame_count; sc.rate = rate; sc.t = t; sc.dest_size = (unsigned long)nseq; sc.alias = false;
    sc.rigid_mask = true;
    for (int s = 0; s < MAXSEQ; ++s) { sc.seq_fc[s] = frame_count; sc.modes[s] = 0x0A; }
    return sc;
}
static void SetQ(int s, int i, float x, float y, float z, float w) { g_rot[s][i][0] = x; g_rot[s][i][1] = y; g_rot[s][i][2] = z; g_rot[s][i][3] = w; }

static void Anchors()
{
    Scene sc;
    // t = 1 s at 30 frames a second, 12 frames: floor(30) = 30 (the nearest of 29.5 is 30), frame 6, fraction 0
    Randomise(1); sc = Base(1, 12, 30.0f, 1.0f);
    SetQ(0, 6, 0.0f, 0.0f, 0.0f, 1.0f); SetQ(0, 7, 0.0f, 0.0f, 0.0f, 0.5f);          // dot 0.5: pos 768.75, index 768
    Run(sc);
    if (Res32(0x0C) != 6 || Res32(-0x14) != 0) FailX("anchor frame 6 / fraction 0", Res32(0x0C), 6);
    if (Res32(-0x18) != 768) FailX("anchor index 768", Res32(-0x18), 768);
    if (Res32(-0x04) != 0) FailX("anchor second sine position (fraction 0)", Res32(-0x04), 0);
    // 31 frames in: the nearest of 30.5 is 30, and 31 is the next integer, so floor = 31; frame 7; next = 8
    sc.t = 31.0f / 30.0f; sc.rate = 30.0f;
    Run(sc);
    // a quarter past a frame: fraction 0.25 exactly
    sc = Base(2, 10, 4.0f, 0.8125f);                                                 // frames 3.25: frame 3, fraction 0.25
    SetQ(1, 3, 0.5f, 0.5f, 0.5f, 0.5f); SetQ(1, 4, 0.5f, -0.5f, 0.5f, 0.5f);         // dot 0.5
    Run(sc);
    if (Res32(0x0C) != 3 || Res32(-0x14) != Bits(0.25f)) FailX("anchor frame 3 / fraction 0.25", Res32(0x0C), 3);
    // negative dot: the absolute value is used (dot -0.5 gives the same index as +0.5)
    SetQ(1, 4, -0.5f, 0.5f, -0.5f, -0.5f);
    Run(sc);
    if (Res32(-0x18) != 768) FailX("anchor index 768 (negative dot)", Res32(-0x18), 768);
    // negative time: -9 frames, unsigned remainder by 10 is 7
    sc = Base(1, 10, 30.0f, -0.3f); Run(sc);
    sc = Base(1, 10, 2.0f, -4.5f); Run(sc);
    if (Res32(0x0C) != 7) FailX("anchor frame 7 (negative time)", Res32(0x0C), 7);
    // the last frame of a sequence: next wraps to 0 (sequence FrameCount frame + 1), and one frame short of / past it
    for (int d = -1; d <= 2; ++d)
    {
        sc = Base(3, 12, 1.0f, 5.5f);
        sc.seq_fc[0] = sc.seq_fc[1] = sc.seq_fc[2] = (unsigned long)(5 + d);
        sc.modes[1] = 0x0F; sc.modes[2] = 0x02;
        Run(sc);
    }
    // 1 - cosa equal to the limit 0.05f exactly (the dot is the sum of two floats that add up to 1 - 0.05f): no interpolation
    {
        float lim = 0.05f; double c = 1.0 - (double)lim;
        float hi = (float)(floor(c * 16384.0) / 16384.0); float lo = (float)(c - (double)hi);
        sc = Base(1, 12, 1.0f, 2.5f);
        SetQ(0, 2, 1.0f, 1.0f, 0.0f, 0.0f); SetQ(0, 3, hi, lo, 0.0f, 0.0f);
        Run(sc);
        SetQ(0, 3, hi, lo * 0.5f, 0.0f, 0.0f);                                       // just under the limit: interpolates
        Run(sc);
        SetQ(0, 3, 1.0f, 0.0f, 0.0f, 0.0f);                                          // identical direction: dot 1
        Run(sc);
    }
    // palettes: equal entries skip; different entries pick the palette's quaternions, not the frame's
    Randomise(1); sc = Base(2, 12, 1.0f, 4.25f); sc.modes[0] = 0x03; sc.modes[1] = 0x0F;
    g_palr[0][4] = 9; g_palr[0][5] = 9; g_palr[1][4] = 2; g_palr[1][5] = 17;
    Run(sc);
    g_palr[0][5] = 10; Run(sc);
    // rotation modes 0 and 1 never read a track
    sc = Base(4, 12, 1.0f, 4.25f); sc.modes[0] = 0x00; sc.modes[1] = 0x01; sc.modes[2] = 0x04; sc.modes[3] = 0xFD; Run(sc);
    // no bones; dest aliasing the rest pose; no rigid-bone mask
    sc = Base(0, 12, 1.0f, 4.25f); Run(sc);
    sc = Base(MAXSEQ, 12, 1.0f, 4.25f); sc.alias = true; Run(sc);
    sc = Base(MAXSEQ, 12, 1.0f, 4.25f); sc.rigid_mask = false; sc.dest_size = 0; Run(sc);
    // a FrameCount above 2^31: the unsigned remainder is negative as a long and FrameCount is added back (no track is read)
    sc = Base(3, 0x90000000, 1.0f, -1.0f);
    for (int s = 0; s < MAXSEQ; ++s) sc.modes[s] = (unsigned char)(s & 1);
    Run(sc);
    sc.t = -1879048192.0f; Run(sc);                                                  // whole = 0x90000000: remainder 0
    sc.t = -1879048064.0f; Run(sc);                                                  // remainder 0x80: positive
    sc.frame_count = 0xFFFFFFF0; sc.t = -1000.0f; Run(sc);                           // remainder 0xFFFFFC18: negative as a long
    if ((long)Res32(0x0C) != (long)(0xFFFFFC18u + 0xFFFFFFF0u)) FailX("anchor wrapped frame", Res32(0x0C), 0xFFFFFC18u + 0xFFFFFFF0u);
}

static void RandomCases(int n)
{
    static const float rates[6] = { 1.0f, 15.0f, 30.0f, 29.97f, 0.5f, 7.3f };
    for (int c = 0; c < n; ++c)
    {
        if (c % 16 == 0) Randomise((int)(Rnd() % 4));
        Scene sc; memset(&sc, 0, sizeof sc);
        sc.nseq = (int)(Rnd() % (MAXSEQ + 1));
        sc.frame_count = 1 + Rnd() % 14;
        sc.rate = rates[Rnd() % 6];
        sc.t = (Rnd() & 1) ? RndF(40, 8.0f) / sc.rate * 4.0f : RndF(30000, 3000.0f);
        sc.dest_size = Rnd() % 9; sc.alias = Rnd() % 13 == 0; sc.rigid_mask = (Rnd() & 1) != 0;
        // the frame the row will use, to size the sequences around it
        double fr = (double)sc.t * (double)sc.rate; long fl = (long)floor((double)(float)fr);
        long frame = (long)((unsigned long)fl % sc.frame_count);
        for (int s = 0; s < MAXSEQ; ++s)
        {
            switch (Rnd() % 6)
            {
            case 0: sc.seq_fc[s] = (unsigned long)frame + 1; break;
            case 1: sc.seq_fc[s] = (unsigned long)frame + 2; break;
            case 2: sc.seq_fc[s] = (unsigned long)frame; break;
            case 3: sc.seq_fc[s] = Rnd() % 3; break;
            default: sc.seq_fc[s] = sc.frame_count; break;
            }
            sc.modes[s] = (unsigned char)Rnd();
            if (Rnd() % 3) sc.modes[s] |= 2;
            if (Rnd() % 5 == 0 && frame + 1 < NQ) g_palr[s][frame + 1] = g_palr[s][frame];
        }
        Run(sc);
    }
}

int main()
{
    int i;
    for (i = 0; i < 1026; ++i)
    {
        double v = (double)i / 512.5 - 1.0; if (v > 1.0) v = 1.0; if (v < -1.0) v = -1.0;
        GFastArcCosTable[i] = (float)acos(v);
    }
    for (i = 0; i < 1025; ++i) GFastCosTable[i] = (float)cos((double)i * (6.283185307179586 / 1024.0));
    _controlfp(_RC_NEAR, _MCW_RC);
    _controlfp(_PC_53, _MCW_PC);
    Anchors();
    RandomCases(6000);
    int slerps53 = g_slerps;
    _controlfp(_PC_24, _MCW_PC);
    Anchors();
    RandomCases(6000);
    _controlfp(_PC_53, _MCW_PC);
    if (g_sp == 0) Fail("no call made", 0, 1);
    if (slerps53 < 2000 || g_slerps - slerps53 < 2000 || g_skips < 500) Fail("too few interpolating bones in the random cases", g_slerps, g_skips);
    if (g_fail) { printf("00a4d770_TEST FAIL (%d failures in %d cases)\n", g_fail, g_cases); return 1; }
    printf("00a4d770_TEST PASS (%d cases, %d interpolating bones, %d bones stopped early)\n", g_cases, g_slerps, g_skips);
    return 0;
}
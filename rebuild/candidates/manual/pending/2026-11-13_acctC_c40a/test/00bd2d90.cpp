// Behaviour test for 0x00BD2D90 CEngineInternalPrimitiveAnimatedMesh::GetBoneTransformations: the first request of a frame allocates one
// 4x4 matrix per bone from the frame pool (null for a zero bone count), has the animation manager fill them with the blend of the start
// and end states at the primitive's interpolation set's interpolate (with the cached bone scalings), then replaces every matrix M by
// M * BoneSpace[i] (row-vector product, terms of one element summed in one expression), adds the bone count to the frame counter and
// caches the array; later requests return the cached array untouched.
#include <cstdio>
#include <cstring>
struct M4 { float d[4][4]; };
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
static M4 pool[8]; static int allocs = 0, blends = 0; static unsigned long counter = 0;
static float seenBlend; static const float* seenScalings; static bool seenFlag; static unsigned long seenCount;
static float sets[3] = { 0.25f, 0.5f, 0.75f };
static float scalings[3];
struct Mesh { unsigned long boneCount; M4* boneSpace; };
static M4* Allocate(unsigned long n) { ++allocs; return n ? pool : 0; }
static void Blend(float blend, const float* sc, const Mesh& mesh, M4* out, unsigned long n, bool flag)
{
    ++blends; seenBlend = blend; seenScalings = sc; seenFlag = flag; seenCount = n;
    for (unsigned long i = 0; i < n; ++i) for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) out[i].d[r][c] = (float)(i * 16 + r * 4 + c + 1);
}
struct Prim {
    long set; const Mesh* mesh; M4* transforms;
    const M4* Get() {
        if (transforms == 0) {
            const Mesh& m = *mesh; unsigned long n = m.boneCount;
            transforms = Allocate(n);
            float interpolate = sets[set];
            Blend(interpolate, scalings, m, transforms, n, true);
            for (unsigned long i = 0; i < n; ++i) {
                M4 copy = transforms[i]; const M4& b = m.boneSpace[i];
                for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c)
                    transforms[i].d[r][c] = copy.d[r][0] * b.d[0][c] + copy.d[r][1] * b.d[1][c] + copy.d[r][2] * b.d[2][c] + copy.d[r][3] * b.d[3][c];
            }
            counter += n;
        }
        return transforms;
    }
};
int main() {
    M4 space[2]; std::memset(space, 0, sizeof(space));
    for (int k = 0; k < 4; ++k) space[0].d[k][k] = 2.0f;                 // bone 0: uniform scale by 2
    space[1].d[0][1] = 1.0f; space[1].d[1][0] = 1.0f; space[1].d[2][2] = 1.0f; space[1].d[3][3] = 1.0f;   // bone 1: swap columns 0 and 1
    Mesh mesh = { 2, space };
    Prim p = { 1, &mesh, 0 };
    const M4* t = p.Get();
    check(t == pool && allocs == 1 && blends == 1, "first request allocates and blends once");
    check(seenBlend == 0.5f && seenScalings == scalings && seenFlag && seenCount == 2, "blend uses the set's interpolate, the scalings, the count and true");
    check(t[0].d[0][0] == 2.0f && t[0].d[3][3] == 32.0f && t[0].d[1][2] == 14.0f, "bone 0 is post-multiplied by its bone-space matrix (scale)");
    check(t[1].d[0][0] == 18.0f && t[1].d[0][1] == 17.0f && t[1].d[2][3] == 28.0f && t[1].d[3][0] == 30.0f, "bone 1: M * B, not B * M (columns swapped)");
    check(counter == 2, "bone count added to the frame counter");
    const M4* again = p.Get();
    check(again == t && allocs == 1 && blends == 1 && counter == 2 && t[0].d[0][0] == 2.0f, "second request returns the cache untouched");
    Mesh empty = { 0, space }; Prim q = { 2, &empty, 0 };
    check(q.Get() == 0 && allocs == 2 && blends == 2 && seenBlend == 0.75f && seenCount == 0 && counter == 2, "no bones: null array, blend still asked, nothing counted");
    check(q.Get() == 0 && allocs == 3, "a null cache is rebuilt on every request");
    if (!fails) std::printf("00bd2d90_TEST PASS\n");
    return fails;
}

// C3DSkeletalMorphTarget::GetBoneScaling(C3DVector& scaling, unsigned long boneNameCRC) const -- binary-search the target's bone morphs (sorted by name CRC) and write that bone's scaling into `scaling`, or (1,1,1) when the bone is not morphed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A44CD0, length 175 bytes, `ret 8`.
// Signature: Ego_r.pdb ?GetBoneScaling@C3DSkeletalMorphTarget@@QBEXAAVC3DVector@@K@Z -- an OUT-REFERENCE, not a by-value
// return (the stack layout is the same, so the earlier by-value model graded close but copied through `mov ecx,eax`).
// Class: C3DSkeletalMorphTarget (animation/mesh layer; Ego_r.pdb names). No callees.
// Retail offsets: BoneMorphs +0x04 (12-byte CArray {first, last, end}); C3DSkeletalBoneMorph is 0x14 bytes = PDB
// (BoneName +0x00, BoneNameCRC +0x04, Scaling +0x08).
// Shape: `lo` declared before `hi` (first pointer straight into ebp); `hi = size - 1` from the 20-byte element count
// (`imul 66666667h ; sar 3`), `mid = (lo + hi) / 2` signed, unsigned key compares through a morph pointer; the found arm
// assigns `scaling = morph->Scaling` and returns, the miss assigns a (1,1,1) temporary (three frame words copied out).
struct C3DVector
{
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CCharString { void* PStringData; };

struct C3DSkeletalBoneMorph                         // 0x14 bytes
{
    CCharString BoneName;                           // +0x00
    unsigned long BoneNameCRC;                      // +0x04
    C3DVector Scaling;                              // +0x08
};

struct CBoneMorphArray                              // CArray<C3DSkeletalBoneMorph>
{
    C3DSkeletalBoneMorph* first;
    C3DSkeletalBoneMorph* last;
    C3DSkeletalBoneMorph* end_of_storage;
    long size() const { return (long)(last - first); }
};

struct C3DSkeletalMorphTarget
{
    CCharString Name;                               // +0x00
    CBoneMorphArray BoneMorphs;                     // +0x04
    void GetBoneScaling(C3DVector& scaling, unsigned long boneNameCRC) const;
};

void C3DSkeletalMorphTarget::GetBoneScaling(C3DVector& scaling, unsigned long boneNameCRC) const
{
    long lo = 0;
    long hi = BoneMorphs.size() - 1;
    while (lo <= hi)
    {
        long mid = (lo + hi) / 2;
        const C3DSkeletalBoneMorph* morph = &BoneMorphs.first[mid];
        if (boneNameCRC == morph->BoneNameCRC)
        {
            scaling = morph->Scaling;
            return;
        }
        if (boneNameCRC < morph->BoneNameCRC)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    scaling = C3DVector(1.0f, 1.0f, 1.0f);
}

// ---- test harness ----
#include <stdio.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

static C3DSkeletalBoneMorph g_morphs[5] = {
    { { 0 }, 10, C3DVector(1.5f, 1.0f, 1.0f) },
    { { 0 }, 20, C3DVector(2.0f, 2.5f, 1.0f) },
    { { 0 }, 30, C3DVector(3.0f, 1.0f, 3.5f) },
    { { 0 }, 40, C3DVector(4.0f, 4.0f, 4.5f) },
    { { 0 }, 0xF0000000UL, C3DVector(5.0f, 5.0f, 5.0f) },
};

int main()
{
    C3DSkeletalMorphTarget target;
    target.Name.PStringData = 0;
    target.BoneMorphs.first = g_morphs; target.BoneMorphs.last = g_morphs + 5; target.BoneMorphs.end_of_storage = g_morphs + 5;
    C3DVector v(0.0f, 0.0f, 0.0f);
    unsigned long keys[5] = { 10, 20, 30, 40, 0xF0000000UL };
    for (int k = 0; k < 5; ++k) {
        v = C3DVector(-1.0f, -1.0f, -1.0f);
        target.GetBoneScaling(v, keys[k]);
        CHECK(v.X == g_morphs[k].Scaling.X && v.Y == g_morphs[k].Scaling.Y && v.Z == g_morphs[k].Scaling.Z);
    }
    unsigned long misses[4] = { 5, 25, 45, 0xFFFFFFFFUL };
    for (int m = 0; m < 4; ++m) {
        v = C3DVector(-1.0f, -1.0f, -1.0f);
        target.GetBoneScaling(v, misses[m]);
        CHECK(v.X == 1.0f && v.Y == 1.0f && v.Z == 1.0f);
    }
    target.BoneMorphs.last = g_morphs;                  // empty target
    v = C3DVector(-1.0f, -1.0f, -1.0f);
    target.GetBoneScaling(v, 20);
    CHECK(v.X == 1.0f && v.Y == 1.0f && v.Z == 1.0f);
    if (fails == 0) printf("00a44cd0_TEST PASS\n");
    return fails;
}
// Behaviour test for 0x00A50510 C3DAnimation2::InitialiseAnimationSequences(C3DAnimation2& shared_animation, const C3DMesh2& mesh,
// bool create_rigid_bones, unsigned long& build_result): host model. The relative flag is bit 2 of the animation type; the builder
// (stubbed here: C3DAnimationBuilder::BuildMeshAnimation 0x00AB2520) runs only for a mesh with bones and its result goes to
// build_result; the output's sequence count and generated-bone count size the sequence array and the private data buffer; each
// sequence takes the bone's parent index, then either shares the source sequence's data (shared, parent -1) or gets the next
// private data block initialised as a constant (frame count and rate of the shared animation, the generated transform); a bone
// mask is allocated when the shared animation has one, rigid-bone mask and positions only for a non-relative rigid request; then
// per bone: the inverse of its localization matrix, post-multiplied by the parent's matrix when it has a parent (rigid when the
// parent is rigid or the parent had more than one child), the rigid flag and the translation stored, and the bone mask copied
// through the original sequence index (false for generated bones).
#include <cstdio>
#include <cmath>

struct Matrix { float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43; };
struct Vec { float X, Y, Z; };
struct Bone { unsigned long NameCRC; long Parent; long OriginalNoChildren; Matrix LocalizationMatrix; };
struct Mesh { unsigned long BoneCount; Bone* Bones; };
struct SeqData { int Constant; unsigned long FrameCount; float SamplesPerSecond; Matrix Transform; };
struct Seq { SeqData* Data; short ParentIndex; bool Shared; };
struct SeqInfo { long OriginalIndex; Matrix GeneratedTransform; };
struct Output { SeqInfo Sequences[8]; unsigned long Count; unsigned long GeneratedBoneCount; };
struct Anim {
    int AnimType; float SamplesPerSecond; unsigned long FrameCount; unsigned long SequenceCount; Seq* Sequences;
    unsigned long SequenceDataBufferSize; SeqData* SequenceDataBuffer; bool* BoneMask; bool* RigidBoneMask; Vec* RigidBonePositions;
};

static Seq g_seqs[8]; static SeqData g_data[8]; static bool g_masks[3][8]; static Vec g_pos[8];
static int g_builds = 0;

static unsigned long Build(Output& o)          // the builder stub: three bones, the middle one generated
{
    ++g_builds;
    o.Count = 3; o.GeneratedBoneCount = 1;
    o.Sequences[0].OriginalIndex = 1; o.Sequences[1].OriginalIndex = -1; o.Sequences[2].OriginalIndex = 0;
    Matrix t = { 1,0,0, 0,1,0, 0,0,1, 7,8,9 };
    o.Sequences[1].GeneratedTransform = t;
    return 0x1234;
}
static void SetInverse(Matrix& r, const Matrix& m)
{
    float det = m.E11 * (m.E22 * m.E33 - m.E23 * m.E32) + m.E21 * (m.E32 * m.E13 - m.E33 * m.E12) + m.E31 * (m.E23 * m.E12 - m.E22 * m.E13);
    float inv = 1.0f / det;
    r.E11 = (m.E22 * m.E33 - m.E23 * m.E32) * inv; r.E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv; r.E31 = (m.E21 * m.E32 - m.E22 * m.E31) * inv;
    r.E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv; r.E22 = (m.E33 * m.E11 - m.E13 * m.E31) * inv; r.E32 = (m.E31 * m.E12 - m.E32 * m.E11) * inv;
    r.E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv; r.E23 = (m.E21 * m.E13 - m.E23 * m.E11) * inv; r.E33 = (m.E22 * m.E11 - m.E21 * m.E12) * inv;
    r.E41 = -(r.E11 * m.E41 + r.E21 * m.E42 + r.E31 * m.E43);
    r.E42 = -(r.E12 * m.E41 + r.E22 * m.E42 + r.E32 * m.E43);
    r.E43 = -(r.E13 * m.E41 + r.E23 * m.E42 + r.E33 * m.E43);
}
static void PostMultiply(Matrix& r, const Matrix& m)
{
    Matrix a = r;
    r.E11 = a.E11 * m.E11 + a.E12 * m.E21 + a.E13 * m.E31; r.E12 = a.E11 * m.E12 + a.E12 * m.E22 + a.E13 * m.E32;
    r.E13 = a.E11 * m.E13 + a.E12 * m.E23 + a.E13 * m.E33; r.E21 = a.E21 * m.E11 + a.E22 * m.E21 + a.E23 * m.E31;
    r.E22 = a.E21 * m.E12 + a.E22 * m.E22 + a.E23 * m.E32; r.E23 = a.E21 * m.E13 + a.E22 * m.E23 + a.E23 * m.E33;
    r.E31 = a.E31 * m.E11 + a.E32 * m.E21 + a.E33 * m.E31; r.E32 = a.E31 * m.E12 + a.E32 * m.E22 + a.E33 * m.E32;
    r.E33 = a.E31 * m.E13 + a.E32 * m.E23 + a.E33 * m.E33;
    r.E41 = a.E41 * m.E11 + a.E42 * m.E21 + a.E43 * m.E31 + m.E41;
    r.E42 = a.E41 * m.E12 + a.E42 * m.E22 + a.E43 * m.E32 + m.E42;
    r.E43 = a.E41 * m.E13 + a.E42 * m.E23 + a.E43 * m.E33 + m.E43;
}
static bool InitialiseAnimationSequences(Anim& self, Anim& shared, const Mesh& mesh, bool create_rigid_bones, unsigned long& build_result)
{
    bool relative = (self.AnimType & 4) != 0;
    Output output; output.Count = 0; output.GeneratedBoneCount = 0;
    if (mesh.BoneCount > 0)
        build_result = Build(output);
    self.SequenceCount = output.Count;
    self.Sequences = g_seqs;
    for (unsigned long k = 0; k < self.SequenceCount; ++k) { g_seqs[k].Data = 0; g_seqs[k].ParentIndex = -1; g_seqs[k].Shared = false; }
    self.SequenceDataBufferSize = output.GeneratedBoneCount;
    self.SequenceDataBuffer = g_data;
    unsigned long data_index = 0;
    for (unsigned long i = 0; i < self.SequenceCount; ++i) {
        const SeqInfo& info = output.Sequences[i];
        Seq& sequence = self.Sequences[i];
        sequence.ParentIndex = (short)mesh.Bones[i].Parent;
        if (info.OriginalIndex != -1) {
            sequence.Data = shared.Sequences[info.OriginalIndex].Data; sequence.Shared = true; sequence.ParentIndex = -1;
        } else {
            SeqData* d = &self.SequenceDataBuffer[data_index];
            sequence.Data = d; sequence.Shared = false;
            d->Constant = 1; d->FrameCount = shared.FrameCount; d->SamplesPerSecond = shared.SamplesPerSecond; d->Transform = info.GeneratedTransform;
            ++data_index;
        }
    }
    if (shared.BoneMask) self.BoneMask = self.SequenceCount ? g_masks[0] : 0;
    if (create_rigid_bones && !relative) {
        self.RigidBoneMask = self.SequenceCount ? g_masks[1] : 0;
        self.RigidBonePositions = self.SequenceCount ? g_pos : 0;
    }
    bool rigid[8];
    for (unsigned long r = 0; r < self.SequenceCount; ++r) rigid[r] = false;
    for (unsigned long i = 0; i < self.SequenceCount; ++i) {
        const Bone* bone = &mesh.Bones[i];
        Matrix local;
        SetInverse(local, bone->LocalizationMatrix);
        if (bone->Parent != -1) {
            const Bone* parent = &mesh.Bones[bone->Parent];
            PostMultiply(local, parent->LocalizationMatrix);
            rigid[i] = rigid[bone->Parent] || parent->OriginalNoChildren > 1;
        }
        if (self.RigidBoneMask) {
            self.RigidBoneMask[i] = rigid[i];
            Vec t = { local.E41, local.E42, local.E43 };
            self.RigidBonePositions[i] = t;
        }
        if (self.BoneMask) {
            long original_index = output.Sequences[i].OriginalIndex;
            if (original_index != -1) self.BoneMask[i] = shared.BoneMask[original_index];
            else self.BoneMask[i] = false;
        }
    }
    return true;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    SeqData sd0 = { 0, 0, 0.0f }, sd1 = { 0, 0, 0.0f };
    Seq ss[2]; ss[0].Data = &sd0; ss[0].ParentIndex = 0; ss[0].Shared = false; ss[1].Data = &sd1; ss[1].ParentIndex = 0; ss[1].Shared = false;
    bool smask[2] = { true, false };
    Anim shared = { 0, 30.0f, 12, 2, ss, 0, 0, smask, 0, 0 };
    Anim self = { 0, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0 };
    Bone bones[3];
    Matrix id = { 1,0,0, 0,1,0, 0,0,1, 0,0,0 };
    for (int b = 0; b < 3; ++b) { bones[b].NameCRC = b; bones[b].LocalizationMatrix = id; }
    bones[0].Parent = -1; bones[0].OriginalNoChildren = 2; bones[0].LocalizationMatrix.E41 = 1; bones[0].LocalizationMatrix.E42 = 2; bones[0].LocalizationMatrix.E43 = 3;
    bones[1].Parent = 0;  bones[1].OriginalNoChildren = 1; bones[1].LocalizationMatrix.E41 = 4; bones[1].LocalizationMatrix.E42 = 6; bones[1].LocalizationMatrix.E43 = 8;
    bones[2].Parent = 1;  bones[2].OriginalNoChildren = 0; bones[2].LocalizationMatrix.E22 = 2.0f; bones[2].LocalizationMatrix.E41 = 10;
    Mesh mesh = { 3, bones };
    unsigned long result = 0;
    bool ok = InitialiseAnimationSequences(self, shared, mesh, true, result);
    bool pass = ok && result == 0x1234 && g_builds == 1 && self.SequenceCount == 3 && self.SequenceDataBufferSize == 1;
    // sequence 0 shares source 1, sequence 2 shares source 0, sequence 1 is the generated constant one
    pass = pass && self.Sequences[0].Data == &sd1 && self.Sequences[0].Shared && self.Sequences[0].ParentIndex == -1;
    pass = pass && self.Sequences[2].Data == &sd0 && self.Sequences[2].Shared && self.Sequences[2].ParentIndex == -1;
    pass = pass && self.Sequences[1].Data == &g_data[0] && !self.Sequences[1].Shared && self.Sequences[1].ParentIndex == 0;
    pass = pass && g_data[0].Constant == 1 && g_data[0].FrameCount == 12 && Near(g_data[0].SamplesPerSecond, 30.0f) && Near(g_data[0].Transform.E43, 9.0f);
    // bone mask through the original index
    pass = pass && self.BoneMask && self.BoneMask[0] == false && self.BoneMask[1] == false && self.BoneMask[2] == true;
    // rigid flags: bone 1's parent had two children, bone 2's parent is rigid
    pass = pass && self.RigidBoneMask && !self.RigidBoneMask[0] && self.RigidBoneMask[1] && self.RigidBoneMask[2];
    // translations: root = -(1,2,3); bone 1 = -(4,6,8) + (1,2,3); bone 2 = -(10,0,0) + (4,6,8)
    pass = pass && Near(g_pos[0].X, -1) && Near(g_pos[0].Y, -2) && Near(g_pos[0].Z, -3);
    pass = pass && Near(g_pos[1].X, -3) && Near(g_pos[1].Y, -4) && Near(g_pos[1].Z, -5);
    pass = pass && Near(g_pos[2].X, -6) && Near(g_pos[2].Y, 6) && Near(g_pos[2].Z, 8);
    // a relative animation gets no rigid data; a bone-less mesh skips the builder
    Anim rel = { 4, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0 };
    Mesh empty = { 0, bones };
    unsigned long untouched = 77;
    InitialiseAnimationSequences(rel, shared, empty, true, untouched);
    pass = pass && untouched == 77 && g_builds == 1 && rel.SequenceCount == 0 && rel.RigidBoneMask == 0;
    if (!pass) { std::printf("00a50510_TEST FAIL\n"); return 1; }
    std::printf("00a50510_TEST PASS\n");
    return 0;
}
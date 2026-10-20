// Behaviour test for CRandomAppearanceMorph::TransferBinaryOut (0x0086B6D0): the output stream receives the part
// count 3, each part's mesh count and (id, weight, map) records, the three part maps, the skeletal morph count and
// (key, bone, amount, flag) records, the general map and the max texture group id -- in exactly that order.
#include <cstdio>
#include <cstring>

static char g_log[1024];
static void Log(const char* f, long v) { char b[32]; std::sprintf(b, "%s%ld;", f, v); std::strcat(g_log, b); }
struct CDataOutputStream {
    void WriteBool(bool v) { Log("b", v ? 1 : 0); }
    void WriteULong(unsigned long v) { Log("u", (long)v); }
    void WriteLong(long v) { Log("l", v); }
};
template <class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CTextureMorphMap { long id; };
static void GFWriteVectorMapMemDump(CDataOutputStream*, const CTextureMorphMap* map) { Log("m", map->id); }
struct CBodyPartMesh { long MeshID; long Weight; CTextureMorphMap TextureMorphs; };
struct CSkeletalMorph { long Bone; long Amount; bool Enabled; };
struct CSkeletalMorphEntry { long Key; CSkeletalMorph Morph; };
struct CSkeletalMorphMap { CArray<CSkeletalMorphEntry> Entries; };
struct CRandomAppearanceMorph {
    CArray<CArray<CBodyPartMesh> > BodyParts;
    CArray<CTextureMorphMap> BodyPartTextureMorphs;
    CSkeletalMorphMap SkeletalMorphs;
    CTextureMorphMap GeneralTextureMorphs;
    unsigned long MaxTextureGroupID;
    void TransferBinaryOut(CDataOutputStream* out);
};
void CRandomAppearanceMorph::TransferBinaryOut(CDataOutputStream* out)
{
    out->WriteULong(3);
    for (unsigned long part = 0; part < 3; ++part) {
        CArray<CBodyPartMesh>& meshes = BodyParts[part];
        unsigned long count = meshes.size();
        out->WriteULong(count);
        for (unsigned long i = 0; i < count; ++i) {
            CBodyPartMesh& mesh = meshes[i];
            out->WriteLong(mesh.MeshID);
            out->WriteLong(mesh.Weight);
            GFWriteVectorMapMemDump(out, &mesh.TextureMorphs);
        }
    }
    for (unsigned long part = 0; part < 3; ++part)
        GFWriteVectorMapMemDump(out, &BodyPartTextureMorphs[part]);
    unsigned long count = SkeletalMorphs.Entries.size();
    out->WriteULong(count);
    for (unsigned long i = 0; i < count; ++i) {
        out->WriteLong(SkeletalMorphs.Entries[i].Key);
        CSkeletalMorph& morph = SkeletalMorphs.Entries[i].Morph;
        out->WriteLong(morph.Bone);
        out->WriteLong(morph.Amount);
        out->WriteBool(morph.Enabled);
    }
    GFWriteVectorMapMemDump(out, &GeneralTextureMorphs);
    out->WriteULong(MaxTextureGroupID);
}
int main()
{
    CBodyPartMesh head[2] = { { 1, 10, { 100 } }, { 2, 20, { 200 } } };
    CBodyPartMesh legs[1] = { { 3, 30, { 300 } } };
    CArray<CBodyPartMesh> parts[3];
    parts[0]._M_start = head; parts[0]._M_finish = head + 2;
    parts[1]._M_start = 0; parts[1]._M_finish = 0;
    parts[2]._M_start = legs; parts[2]._M_finish = legs + 1;
    CTextureMorphMap partMaps[3] = { { 7 }, { 8 }, { 9 } };
    CSkeletalMorphEntry sk[2] = { { 5, { 50, 51, true } }, { 6, { 60, 61, false } } };
    CRandomAppearanceMorph m;
    m.BodyParts._M_start = parts; m.BodyParts._M_finish = parts + 3;
    m.BodyPartTextureMorphs._M_start = partMaps; m.BodyPartTextureMorphs._M_finish = partMaps + 3;
    m.SkeletalMorphs.Entries._M_start = sk; m.SkeletalMorphs.Entries._M_finish = sk + 2;
    m.GeneralTextureMorphs.id = 77; m.MaxTextureGroupID = 12;
    CDataOutputStream out; g_log[0] = 0;
    m.TransferBinaryOut(&out);
    const char* want = "u3;u2;l1;l10;m100;l2;l20;m200;u0;u1;l3;l30;m300;m7;m8;m9;u2;l5;l50;l51;b1;l6;l60;l61;b0;m77;u12;";
    if (std::strcmp(g_log, want) != 0) { std::printf("0086b6d0_TEST FAIL\n"); return 1; }
    std::printf("0086b6d0_TEST PASS\n");
    return 0;
}
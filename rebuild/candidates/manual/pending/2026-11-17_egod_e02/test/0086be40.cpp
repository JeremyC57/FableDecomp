// Behaviour test for CRandomAppearanceMorph::GetRandomBodyParts (0x0086BE40): the texture group pick (0 when MaxTextureGroupID is 0,
// else GFRandom + 1), the per-type mesh pick (types without meshes are skipped and consume no random numbers), the two texture-morph
// passes in order, and the head skip.
#include <cstdio>
#include <vector>

static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed *= 0x24a1; *seed += 0x24df; *seed = ror13(*seed);
    if (range) return *seed % range;
    return 0;
}
struct MeshTextureSet { long MeshIndex; std::vector<long> TextureMorphs; MeshTextureSet() : MeshIndex(0) {} };
struct BodyPartMesh { long MeshIndex; long Tex; };
struct Morph {
    std::vector<BodyPartMesh> BodyParts[3];
    long TypeTex[3];
    unsigned long MaxTextureGroupID;
    mutable std::vector<unsigned long> calls;
    void GetRandomTextureMorphs(unsigned long& seed, unsigned long group, long tex, std::vector<long>& out) const
    { calls.push_back(group); out.push_back(tex * 100 + (long)GFRandom(7, &seed)); }
    void GetRandomBodyParts(unsigned long& seed, std::vector<MeshTextureSet>& morph_set, bool ignore_head) const
    {
        unsigned long texture_group_id = 0;
        if (MaxTextureGroupID)
            texture_group_id = GFRandom(MaxTextureGroupID, &seed) + 1;
        for (long cbody_part_type = 0; cbody_part_type != 3; ++cbody_part_type) {
            MeshTextureSet mesh_texture_set;
            if (!BodyParts[cbody_part_type].empty()) {
                long body_part_index = GFRandom(BodyParts[cbody_part_type].size(), &seed);
                const BodyPartMesh& body_part = BodyParts[cbody_part_type][body_part_index];
                mesh_texture_set.MeshIndex = body_part.MeshIndex;
                GetRandomTextureMorphs(seed, texture_group_id, body_part.Tex, mesh_texture_set.TextureMorphs);
                GetRandomTextureMorphs(seed, texture_group_id, TypeTex[cbody_part_type], mesh_texture_set.TextureMorphs);
                if (!(ignore_head && cbody_part_type == 0))
                    morph_set.push_back(mesh_texture_set);
            }
        }
    }
};
static int fail(int n) { std::printf("0086be40_TEST FAIL %d\n", n); return 1; }
int main()
{
    Morph m; m.TypeTex[0] = 7; m.TypeTex[1] = 8; m.TypeTex[2] = 9; m.MaxTextureGroupID = 0;
    BodyPartMesh a = { 11, 1 }, b = { 12, 2 }, c = { 21, 3 };
    m.BodyParts[0].push_back(a); m.BodyParts[0].push_back(b); m.BodyParts[2].push_back(c);
    unsigned long seed = 1234, s2 = 1234;
    std::vector<MeshTextureSet> out;
    m.GetRandomBodyParts(seed, out, false);
    // replay: type 0 pick, 2 morph calls, type 1 skipped, type 2 pick + 2 morph calls
    unsigned long i0 = GFRandom(2, &s2); unsigned long r1 = GFRandom(7, &s2), r2 = GFRandom(7, &s2);
    unsigned long i2 = GFRandom(1, &s2); unsigned long r3 = GFRandom(7, &s2), r4 = GFRandom(7, &s2);
    if (out.size() != 2 || seed != s2) return fail(1);
    if (out[0].MeshIndex != (i0 ? 12 : 11) || out[0].TextureMorphs.size() != 2) return fail(2);
    if (out[0].TextureMorphs[0] != (long)((i0 ? 2 : 1) * 100 + r1) || out[0].TextureMorphs[1] != (long)(700 + r2)) return fail(3);
    if (out[1].MeshIndex != 21 || i2 != 0 || out[1].TextureMorphs[1] != (long)(900 + r4) || out[1].TextureMorphs[0] != (long)(300 + r3)) return fail(4);
    for (size_t k = 0; k < m.calls.size(); ++k) if (m.calls[k] != 0) return fail(5);
    // group pick and head skip
    m.MaxTextureGroupID = 5; m.calls.clear(); out.clear(); seed = 99; s2 = 99;
    m.GetRandomBodyParts(seed, out, true);
    unsigned long g = GFRandom(5, &s2) + 1;
    if (out.size() != 1 || out[0].MeshIndex != 21 || m.calls.size() != 4 || m.calls[0] != g || m.calls[3] != g) return fail(6);
    std::printf("0086be40_TEST PASS\n");
    return 0;
}

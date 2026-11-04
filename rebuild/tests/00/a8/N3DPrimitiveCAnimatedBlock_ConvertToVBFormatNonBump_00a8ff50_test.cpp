// Behaviour test for N3DPrimitive::CAnimatedBlock::ConvertToVBFormatNonBump (0x00A8FF50), g++ model of the retail logic.
#include <cstdio>
#include <vector>
struct V3 { float X, Y, Z; };
struct Vertex2 { V3 Position, Normal; float U, V; };
struct Out { float X, Y, Z; unsigned char Idx[4]; unsigned char Wt[4]; float NX, NY, NZ, U1, V1; };
struct BoneWeight { unsigned long BoneIndex, Weight; };
static void Convert(Out* out, const std::vector<Vertex2>& mesh, const std::vector<unsigned long>& list,
                    const std::vector<BoneWeight>& weights, unsigned short bonesPerVertex)
{
    for (unsigned long i = 0; i < list.size(); ++i) {
        const Vertex2& src = mesh[list[i]];
        Out& v = out[i];
        v.X = src.Position.X; v.Y = src.Position.Y; v.Z = src.Position.Z;
        v.NX = src.Normal.X; v.NY = src.Normal.Y; v.NZ = src.Normal.Z;
        v.U1 = src.U; v.V1 = src.V;
        unsigned long j;
        for (j = 0; j < bonesPerVertex; ++j) {
            const BoneWeight& w = weights[i * bonesPerVertex + j];
            v.Idx[j] = (unsigned char)(w.BoneIndex * 3);
            v.Wt[j] = (unsigned char)w.Weight;
        }
        for (; j < 4; ++j) { v.Idx[j] = 0; v.Wt[j] = 0; }
    }
}
int main()
{
    std::vector<Vertex2> mesh(3);
    for (int k = 0; k < 3; ++k) { Vertex2 m = { { k + 1.f, k + 2.f, k + 3.f }, { -1.f * k, 0.5f, 2.f }, 0.25f * k, 0.75f }; mesh[k] = m; }
    std::vector<unsigned long> list; list.push_back(2); list.push_back(0);
    std::vector<BoneWeight> w; BoneWeight a = { 1, 200 }, b = { 90, 55 }, c = { 4, 255 }, d = { 0, 0 };
    w.push_back(a); w.push_back(b); w.push_back(c); w.push_back(d);
    Out out[2];
    for (int k = 0; k < 2; ++k) for (int n = 0; n < 4; ++n) { out[k].Idx[n] = 0xEE; out[k].Wt[n] = 0xEE; }
    Convert(out, mesh, list, w, 2);
    int ok = out[0].X == 3.f && out[0].Z == 5.f && out[0].NX == -2.f && out[0].U1 == 0.5f && out[1].X == 1.f && out[1].V1 == 0.75f
          && out[0].Idx[0] == 3 && out[0].Wt[0] == 200 && out[0].Idx[1] == (unsigned char)270 && out[0].Wt[1] == 55
          && out[0].Idx[2] == 0 && out[0].Wt[3] == 0 && out[1].Idx[0] == 12 && out[1].Wt[0] == 255 && out[1].Idx[3] == 0;
    std::printf(ok ? "00a8ff50_TEST PASS\n" : "00a8ff50_TEST FAIL\n");
    return ok ? 0 : 1;
}
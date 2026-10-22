// Behaviour test for QuadricOptimiserInternals::COptimisedMesh::CopyTo (0x00ACD5A0): each SUBM chunk, paired in order
// with SubMeshes[i], loses its PRIM children and gains one new primitive chunk per primitive that still has a live face;
// the walk stops at the last sub-mesh chunk and every counted-pointer block is released again.
#include <cstdio>
#include <vector>
static int g_Blocks = 0;
struct Info { unsigned long RefCount; void* Data; void (*Del)(void*); };
template <class T> struct CP {
    T* Data; Info* I;
    static void DeleteData(void* d) { delete (T*)d; }
    explicit CP(T* d) : Data(d) { if (d) { I = new Info; I->RefCount = 1; I->Data = d; I->Del = DeleteData; ++g_Blocks; } else I = 0; }
    CP(const CP& o) : Data(o.Data), I(o.I) { if (I) ++I->RefCount; }
    ~CP() { Rel(); Data = 0; I = 0; }
    void Rel() { if (I) { --I->RefCount; if (I->RefCount == 0) { I->Del(I->Data); delete I; --g_Blocks; } } }
    CP& operator=(const CP& o) { Info* i = o.I; T* d = o.Data; if (I != i) { Rel(); Data = d; I = i; if (I) ++I->RefCount; } return *this; }
};
struct Chunk { int Kind; int Tag; };
struct SubChunk { std::vector<Chunk> Children; };
struct MeshChunk {
    std::vector<CP<SubChunk> > Subs;
    CP<SubChunk> First() { return Subs.empty() ? CP<SubChunk>(0) : Subs[0]; }
    CP<SubChunk> Next(const CP<SubChunk>& after) {
        for (unsigned k = 0; k + 1 < Subs.size(); ++k) if (Subs[k].Data == after.Data) return Subs[k + 1];
        return CP<SubChunk>(0);
    }
};
struct Face { bool Live; };
struct Prim { int Tag; std::vector<Face> Faces; };
struct SubMesh { std::vector<Prim*> Primitives; };
static void CopyTo(std::vector<SubMesh>& subMeshes, MeshChunk* mesh)
{
    CP<SubChunk> subMesh = mesh->First();
    if (subMesh.Data != 0) {
        unsigned int i = 0;
        do {
            SubMesh& source = subMeshes[i];
            std::vector<Chunk>& kids = subMesh.Data->Children;
            for (unsigned k = 0; k < kids.size(); ) { if (kids[k].Kind == 1) kids.erase(kids.begin() + k); else ++k; }
            for (unsigned int p = 0; p < source.Primitives.size(); ++p) {
                Prim* primitive = source.Primitives[p];
                unsigned int live = 0;
                unsigned int faceCount = (unsigned int)primitive->Faces.size();
                for (unsigned int f = 0; f < faceCount; ++f) if (primitive->Faces[f].Live) ++live;
                if (live > 0) {
                    CP<Chunk> chunk(new Chunk);
                    chunk.Data->Kind = 1; chunk.Data->Tag = primitive->Tag;
                    kids.push_back(*chunk.Data);
                }
            }
            subMesh = mesh->Next(subMesh);
            ++i;
        } while (subMesh.Data != 0);
    }
}
int main()
{
    int bad = 0;
    MeshChunk mesh;
    {
        for (int s = 0; s < 2; ++s) {
            CP<SubChunk> c(new SubChunk);
            Chunk old; old.Kind = 1; old.Tag = 99; c.Data->Children.push_back(old);
            Chunk keep; keep.Kind = 2; keep.Tag = 7; c.Data->Children.push_back(keep);
            mesh.Subs.push_back(c);
        }
    }
    Prim a; a.Tag = 1; Face live; live.Live = true; Face dead; dead.Live = false;
    a.Faces.push_back(dead); a.Faces.push_back(live);
    Prim b; b.Tag = 2; b.Faces.push_back(dead);
    Prim c; c.Tag = 3; c.Faces.push_back(live);
    std::vector<SubMesh> subs(2);
    subs[0].Primitives.push_back(&a); subs[0].Primitives.push_back(&b);
    subs[1].Primitives.push_back(&c);
    CopyTo(subs, &mesh);
    std::vector<Chunk>& k0 = mesh.Subs[0].Data->Children;
    std::vector<Chunk>& k1 = mesh.Subs[1].Data->Children;
    if (k0.size() != 2 || k0[0].Kind != 2 || k0[1].Tag != 1) bad |= 1;
    if (k1.size() != 2 || k1[0].Kind != 2 || k1[1].Tag != 3) bad |= 2;
    if (g_Blocks != 2) bad |= 4;                                   // only the two SUBM chunks still counted
    MeshChunk empty;
    CopyTo(subs, &empty);                                          // no sub-mesh chunk: nothing happens
    if (g_Blocks != 2) bad |= 8;
    if (bad) { std::printf("00acd5a0_TEST FAIL %d\n", bad); return 1; }
    std::printf("00acd5a0_TEST PASS\n");
    return 0;
}
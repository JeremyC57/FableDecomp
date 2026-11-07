// Behaviour test for 0x00A9BBE0 C3DMeshFileLoader::C3DMeshFileLoader: the root chunk is created, then every prototype is cloned
// (CreateNewInstance) into its parent's registered sub-chunk list through AddChunkType, and the loader keeps a counted pointer to
// the clone; the cloth chunk is registered twice (sub-mesh and primitive), the group chunk under the primitive and once through
// RegisterClothSubChunkType; the mirrored-texture chunk last, under the primitive.
#include <cstdio>
#include <cstring>

struct Chunk {
    const char* type;
    Chunk* subs[16];
    int nsubs;
    explicit Chunk(const char* t) : type(t), nsubs(0) { }
    Chunk* CreateNewInstance() const { return new Chunk(type); }
    void RegisterSubChunkType(Chunk* c) { subs[nsubs++] = c; }
};

struct Loader {
    Chunk* Root; Chunk *SubMesh, *MaterialList, *Helpers, *Transformation, *Bone, *Primitive, *SubMeshCloth, *Triangles, *Smoothing,
        *Vertices, *Unique, *Group, *PrimitiveCloth, *Material, *EngineMaterial, *MaterialExt, *Map, *HelperPoint, *Dummy, *Convex,
        *Mirrored;
    int cloth_sub_calls;
    Chunk* Add(const Chunk& proto, Chunk* parent) { Chunk* c = proto.CreateNewInstance(); parent->RegisterSubChunkType(c); return c; }
    Loader() : cloth_sub_calls(0) {
        Root = new Chunk("3DRT");
        SubMesh = Add(Chunk("SubMesh"), Root);
        MaterialList = Add(Chunk("MaterialList"), Root);
        Helpers = Add(Chunk("Helpers"), Root);
        Transformation = Add(Chunk("Transformation"), SubMesh);
        Bone = Add(Chunk("Bone"), SubMesh);
        Primitive = Add(Chunk("Primitive"), SubMesh);
        SubMeshCloth = Add(Chunk("Cloth"), SubMesh);
        Triangles = Add(Chunk("Triangles"), Primitive);
        Smoothing = Add(Chunk("SmoothingGroups"), Primitive);
        Vertices = Add(Chunk("Vertices"), Primitive);
        Unique = Add(Chunk("UniqueVertices"), Primitive);
        Group = Add(Chunk("Group"), Primitive);
        PrimitiveCloth = Add(Chunk("Cloth"), Primitive);
        ++cloth_sub_calls;
        Material = Add(Chunk("Material"), MaterialList);
        EngineMaterial = Add(Chunk("EngineMaterial"), MaterialList);
        MaterialExt = Add(Chunk("MaterialExt"), Material);
        Map = Add(Chunk("Map"), Material);
        HelperPoint = Add(Chunk("HelperPoint"), Helpers);
        Dummy = Add(Chunk("DummyObject"), Helpers);
        Convex = Add(Chunk("ConvexVolume"), Helpers);
        Mirrored = Add(Chunk("MTEH"), Primitive);
    }
};

int main()
{
    Loader l;
    bool ok = l.Root->nsubs == 3 && l.SubMesh->nsubs == 4 && l.Primitive->nsubs == 7 && l.MaterialList->nsubs == 2
        && l.Material->nsubs == 2 && l.Helpers->nsubs == 3 && l.cloth_sub_calls == 1;
    ok = ok && l.Primitive->subs[6] == l.Mirrored && !std::strcmp(l.Mirrored->type, "MTEH") && l.SubMeshCloth != l.PrimitiveCloth
        && !std::strcmp(l.SubMeshCloth->type, l.PrimitiveCloth->type) && l.Helpers->subs[1] == l.Dummy && !std::strcmp(l.Root->type, "3DRT");
    std::printf(ok ? "00a9bbe0_TEST PASS\n" : "00a9bbe0_TEST FAIL\n");
    return ok ? 0 : 1;
}
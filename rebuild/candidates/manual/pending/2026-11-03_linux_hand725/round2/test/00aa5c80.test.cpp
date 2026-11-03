// Behaviour test for 0x00AA5C80 C3DMeshFileXLoader::C3DMeshFileXLoader: the constructor registers eleven X chunk prototypes with the
// base loader's Register*SubChunkType members (root, sub-mesh, primitive, triangles, map), keeps the XAPR prototype the primitive
// registration hands back, and chains XAPB -> triangles -> XTST sub-chunk registrations through GetAsSuperChunk. Every
// temporary counted pointer is released (reference counts back to the registry's own).
#include <cstdio>
#include <cstring>
static char g_log[64][16]; static int g_n = 0;
static void Log(const char* where, const char* fcc) { std::sprintf(g_log[g_n++], "%s:%s", where, fcc); }
struct Chunk {
    char Type[5]; Chunk* Sub[4]; int SubCount; int Refs;
    Chunk() : SubCount(0), Refs(0) { Type[0] = 0; }
    explicit Chunk(const char* t) : SubCount(0), Refs(0) { std::memcpy(Type, t, 5); }
    Chunk* GetAsSuperChunk() { return this; }
    void RegisterSubChunkType(Chunk* c) { Sub[SubCount++] = c; ++c->Refs; Log(Type, c->Type); }
};
struct Ptr {                                            // the CCountedPointer behaviour the test needs
    Chunk* Data;
    Ptr() : Data(0) { }
    explicit Ptr(Chunk* d) : Data(d) { if (d) ++d->Refs; }
    Ptr(const Ptr& o) : Data(o.Data) { if (Data) ++Data->Refs; }
    Ptr& operator=(const Ptr& o) { if (o.Data) ++o.Data->Refs; if (Data) --Data->Refs; Data = o.Data; return *this; }
    ~Ptr() { if (Data) --Data->Refs; }
    Chunk* operator->() const { return Data; }
};
static Chunk g_store[32]; static int g_used = 0;
static Chunk* Clone(const char* t) { Chunk* c = &g_store[g_used++]; std::memcpy(c->Type, t, 5); c->Refs = 1; return c; }
struct Loader {
    Ptr Register(const char* where, const Chunk& c) { Log(where, c.Type); return Ptr(Clone(c.Type)); }
    Ptr RegisterRootSubChunkType(const Chunk& c) { return Register("root", c); }
    Ptr RegisterSubMeshSubChunkType(const Chunk& c) { return Register("sub", c); }
    Ptr RegisterPrimitiveSubChunkType(const Chunk& c) { return Register("prim", c); }
    Ptr RegisterTrianglesSubChunkType(const Chunk& c) { return Register("tri", c); }
    Ptr RegisterMapSubChunkType(const Chunk& c) { return Register("map", c); }
    Loader()
    {
        RegisterRootSubChunkType(Chunk("XBDS")); RegisterRootSubChunkType(Chunk("XBDB"));
        RegisterSubMeshSubChunkType(Chunk("XBDS")); RegisterSubMeshSubChunkType(Chunk("XBDB"));
        RegisterPrimitiveSubChunkType(Chunk("XUVT")); RegisterPrimitiveSubChunkType(Chunk("XMIF"));
        RegisterPrimitiveSubChunkType(Chunk("XBDS")); RegisterPrimitiveSubChunkType(Chunk("XBDB"));
        RegisterPrimitiveSubChunkType(Chunk("XVBD"));
        RegisterTrianglesSubChunkType(Chunk("XTST"));
        RegisterMapSubChunkType(Chunk("XTID"));
        Ptr animated; animated = RegisterPrimitiveSubChunkType(Chunk("XAPR"));
        Ptr block(Clone("XAPB")); animated->GetAsSuperChunk()->RegisterSubChunkType(block.Data);
        Ptr tris(Clone("TRIS")); block->GetAsSuperChunk()->RegisterSubChunkType(tris.Data);
        Ptr strip(Clone("XTST")); tris->GetAsSuperChunk()->RegisterSubChunkType(strip.Data);
    }
};
int main()
{
    Loader l;
    static const char* expect[] = { "root:XBDS","root:XBDB","sub:XBDS","sub:XBDB","prim:XUVT","prim:XMIF","prim:XBDS","prim:XBDB",
        "prim:XVBD","tri:XTST","map:XTID","prim:XAPR","XAPR:XAPB","XAPB:TRIS","TRIS:XTST" };
    if (g_n != 15) { std::printf("00aa5c80_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 15; ++i) if (std::strcmp(g_log[i], expect[i]) != 0) { std::printf("00aa5c80_TEST FAIL\n"); return 1; }
    for (int i = 0; i < g_used; ++i) {                  // only the registry's own reference (and a parent's) survive
        int want = 1 + (std::strcmp(g_store[i].Type, "XAPB") == 0 || std::strcmp(g_store[i].Type, "TRIS") == 0 || (i == g_used - 1) ? 1 : 0);
        if (g_store[i].Refs != want) { std::printf("00aa5c80_TEST FAIL\n"); return 1; }
    }
    std::printf("00aa5c80_TEST PASS\n");
    return 0;
}

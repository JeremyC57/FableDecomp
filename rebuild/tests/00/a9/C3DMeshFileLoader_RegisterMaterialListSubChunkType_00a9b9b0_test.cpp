// behaviour test for 0x00A9B9B0 C3DMeshFileLoader::RegisterMaterialListSubChunkType: the chunk type and the MaterialListChunkType member's super chunk reach AddChunkType, whose counted result is returned unchanged
#include <cstdio>
struct Super { int id; };
struct Chunk { Super* super; int calls; Super* GetAsSuperChunk() { ++calls; return super; } };
struct Result { const void* type; Super* parent; long refs; };
static Result AddChunkType(const void* type, Super* parent) { Result r; r.type = type; r.parent = parent; r.refs = 1; return r; }
struct Loader { Chunk* member; };                            // stands in for MaterialListChunkType (+0x70)
static Result Register(Loader& l, const void* type)          // the function under test
{
    return AddChunkType(type, l.member->GetAsSuperChunk());
}
int main()
{
    int fail = 0;
    Super s; s.id = 7; Chunk c; c.super = &s; c.calls = 0; Loader l; l.member = &c;
    int type = 3;
    Result r = Register(l, &type);
    if (r.type != &type || r.parent != &s || r.refs != 1) fail |= 1;
    if (c.calls != 1) fail |= 2;
    if (fail) { std::printf("00a9b9b0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00a9b9b0_TEST PASS\n");
    return 0;
}
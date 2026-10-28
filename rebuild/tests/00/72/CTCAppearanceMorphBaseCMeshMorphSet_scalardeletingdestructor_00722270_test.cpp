extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x00722270 CTCAppearanceMorphBase::CMeshMorphSet scalar deleting destructor: members are torn down in
// reverse order (OriginalMeshTextureMorphs, then each MeshParts element's TextureMorphs, then the array block), and the
// object is freed only when bit 0 of the flags is set; the return value is `this`.
static int order[16]; static int n = 0; static int freed = 0;
class CEngineTextureMorphSet { public: int id; ~CEngineTextureMorphSet() { order[n++] = id; } };
struct CMeshMorphSetParts { unsigned long MeshIndex; CEngineTextureMorphSet TextureMorphs; };
struct CMeshMorphSet {
    CMeshMorphSetParts* first; CMeshMorphSetParts* last;
    CEngineTextureMorphSet OriginalMeshTextureMorphs;
};
static void* ScalarDeletingDtor(CMeshMorphSet* s, unsigned int flags, bool heap)
{
    s->OriginalMeshTextureMorphs.~CEngineTextureMorphSet();
    for (CMeshMorphSetParts* p = s->first; p != s->last; ++p) p->TextureMorphs.~CEngineTextureMorphSet();
    order[n++] = 99;                                   // the parts block is released
    if ((flags & 1) && heap) ++freed;
    return s;
}
int main()
{
    CMeshMorphSetParts parts[2];
    parts[0].TextureMorphs.id = 1; parts[1].TextureMorphs.id = 2;
    CMeshMorphSet s; s.first = parts; s.last = parts + 2; s.OriginalMeshTextureMorphs.id = 7;
    void* r = ScalarDeletingDtor(&s, 1, true);
    CHECK(r == &s);
    CHECK(n == 4 && order[0] == 7 && order[1] == 1 && order[2] == 2 && order[3] == 99);
    CHECK(freed == 1);
    n = 0; s.first = s.last = parts;
    ScalarDeletingDtor(&s, 0, true);
    CHECK(n == 2 && order[0] == 7 && order[1] == 99);
    CHECK(freed == 1);
    parts[0].TextureMorphs.id = 0; parts[1].TextureMorphs.id = 0; s.OriginalMeshTextureMorphs.id = 0;
    if (fails == 0) printf("00722270_TEST PASS\n"); else printf("00722270_TEST FAIL\n");
    return fails;
}
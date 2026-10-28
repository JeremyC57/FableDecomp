// Test: CLocalDetailPrimitiveMeshZSpriteBatch ctor -- base members (Next 0, Type, Map, zero bounds), then Objects, ZSpriteEntries 0 and HasMeshPrimitiveHandles false; ObjectCount untouched.
#include <cstdio>
#include <cstring>
#include <new>
struct CType {}; class CMap { public:};
struct CBase { CBase* Next; float Box[6]; float Sphere[4]; const CType* Type; CMap* Map;
    CBase(const CType* t, CMap* m) : Next(0), Type(t), Map(m) { for (int i = 0; i < 6; ++i) Box[i] = 0.0f; for (int j = 0; j < 4; ++j) Sphere[j] = 0.0f; } };
struct CBatch : CBase { void* Objects; void* ZSpriteEntries; int ObjectCount; bool HasMeshPrimitiveHandles;
    CBatch(const CType* t, CMap* m) : CBase(t, m) { Objects = 0; ZSpriteEntries = 0; HasMeshPrimitiveHandles = false; } };
int main()
{
    static unsigned char raw[sizeof(CBatch)]; std::memset(raw, 0xCD, sizeof raw);
    CType t; CMap m; CBatch* b = new (raw) CBatch(&t, &m);
    bool ok = b->Next == 0 && b->Type == &t && b->Map == &m && b->Objects == 0 && b->ZSpriteEntries == 0 &&
              !b->HasMeshPrimitiveHandles && b->Box[5] == 0.0f && b->Sphere[3] == 0.0f && b->ObjectCount == (int)0xCDCDCDCD;
    std::printf(ok ? "00bfac70_TEST PASS\n" : "00bfac70_TEST FAIL\n");
    return ok ? 0 : 1;
}
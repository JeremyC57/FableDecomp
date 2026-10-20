// Behaviour test for 00706c90: body copied from src/00706c90.cpp (CTCHeroAttachableAppearanceModifiers::GetMeshesUsed), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() {} };
namespace NGraphicAppearance {
struct CMeshInfo {
    long Index; float SizeX; unsigned char PrimaryMesh : 1;
    CMeshInfo(long index, float sizeX, bool primary) : Index(index), SizeX(sizeX) { PrimaryMesh = primary; }
};
}
static int g_overflow = 0;
struct CMeshInfoVector {
    NGraphicAppearance::CMeshInfo* _M_start; NGraphicAppearance::CMeshInfo* _M_finish; NGraphicAppearance::CMeshInfo* _M_end_of_storage;
    void _M_insert_overflow(NGraphicAppearance::CMeshInfo* pos, const NGraphicAppearance::CMeshInfo& x, const __true_type&, unsigned int fillLen, bool atEnd)
    { ++g_overflow; CHECK(pos == _M_finish && fillLen == 1 && atEnd); new (_M_finish) NGraphicAppearance::CMeshInfo(x); ++_M_finish; ++_M_end_of_storage; }
    void push_back(const NGraphicAppearance::CMeshInfo& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) NGraphicAppearance::CMeshInfo(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CDefPointerTex { void* Object; };
struct CModifierGraphic { CDefPointerTex PTextureReplacement; long BankIndex; float RenderSizeX; };
struct CModifierGraphicVector { CModifierGraphic* _M_start; CModifierGraphic* _M_finish; CModifierGraphic* _M_end_of_storage; };
struct CRetailContainer12 { void* a; void* b; void* c; };
class CThing;
class CTCBase { public: virtual ~CTCBase() { } CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; };
namespace NGraphicAppearance {
class CAGraphicAppearanceComponent { public: virtual void OnAppearanceClear() { } virtual void GetMeshesUsed(CMeshInfoVector& meshes) const = 0; };
}
class CTCHeroAttachableAppearanceModifiers : public CTCBase, public NGraphicAppearance::CAGraphicAppearanceComponent {
public:
    unsigned long FrameLastChanged; unsigned char AppearanceModifiersLoaded : 1; unsigned char GraphicsDirty : 1;
    CRetailContainer12 PrimHandles; CRetailContainer12 HiddenMaterialNames; CRetailContainer12 AppearanceModifiers;
    CModifierGraphicVector Graphics;
    virtual void OnAppearanceClear() { }
    virtual void GetMeshesUsed(CMeshInfoVector& meshes) const;
};
void CTCHeroAttachableAppearanceModifiers::GetMeshesUsed(CMeshInfoVector& meshes) const
{
    for (const CModifierGraphic* it = Graphics._M_start; it != Graphics._M_finish; ++it)
        meshes.push_back(NGraphicAppearance::CMeshInfo(it->BankIndex, it->RenderSizeX, false));
}
int main()
{
    CModifierGraphic graphics[3]; for (int i = 0; i < 3; ++i) { graphics[i].BankIndex = 10 + i; graphics[i].RenderSizeX = 1.5f * i; }
    CTCHeroAttachableAppearanceModifiers tc; tc.Graphics._M_start = graphics; tc.Graphics._M_finish = graphics + 3; tc.Graphics._M_end_of_storage = graphics + 3;
    unsigned char storage[sizeof(NGraphicAppearance::CMeshInfo) * 4]; memset(storage, 0xff, sizeof storage);
    CMeshInfoVector meshes; meshes._M_start = (NGraphicAppearance::CMeshInfo*)storage; meshes._M_finish = meshes._M_start; meshes._M_end_of_storage = meshes._M_start + 2;
    const NGraphicAppearance::CAGraphicAppearanceComponent& comp = tc;
    comp.GetMeshesUsed(meshes);                                   // two fit, the third grows
    CHECK(meshes._M_finish - meshes._M_start == 3 && g_overflow == 1);
    CHECK(meshes._M_start[0].Index == 10 && meshes._M_start[0].SizeX == 0.0f && meshes._M_start[0].PrimaryMesh == 0);
    CHECK(meshes._M_start[2].Index == 12 && meshes._M_start[2].SizeX == 3.0f && meshes._M_start[2].PrimaryMesh == 0);
    tc.Graphics._M_finish = graphics;                             // no graphics: nothing appended
    comp.GetMeshesUsed(meshes);
    CHECK(meshes._M_finish - meshes._M_start == 3 && g_overflow == 1);
    if (fails == 0) printf("00706c90_TEST PASS\n"); else printf("00706c90_TEST FAIL (%d)\n", fails);
    return fails;
}
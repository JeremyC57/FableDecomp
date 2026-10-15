// Behaviour test for CRegion::CRegion @ 0x006BC410
#include <cstdio>
#include <cstdlib>
static int g_strings = 0;
class CCharString { public: CCharString() : PStringData(0) { ++g_strings; } void* PStringData; };
template <class T> struct SRetailAllocProxy { T* m_data; SRetailAllocProxy(T* data) : m_data(data) {} };
template <class T> struct CRetailVector {
    T* m_start; T* m_finish; SRetailAllocProxy<T> m_endOfStorage;
    CRetailVector() : m_start(0), m_finish(0), m_endOfStorage(0) {}
};
struct CDefPointerRegionDef { void* Object; CDefPointerRegionDef() : Object(0) {} };
struct C2DVector { float X, Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} };
struct CExitTextOffsetMap {
    struct node { unsigned char colour; node* parent; node* left; node* right; unsigned char value[12]; };
    node* m_header; unsigned int m_nodeCount; unsigned char m_keyCompare;
    CExitTextOffsetMap() : m_header(0)
    {
        m_header = (node*)std::malloc(sizeof(node));
        m_nodeCount = 0; m_header->colour = 0; m_header->parent = 0; m_header->left = m_header; m_header->right = m_header;
    }
};
struct CRegion {
    CRetailVector<int> Maps, VisMaps;
    CCharString Name, DisplayName, RegionDefName;
    CDefPointerRegionDef PDef;
    CCharString MiniMapGraphic;
    float MiniMapScale;
    C2DVector MiniMapOffset;
    CExitTextOffsetMap MiniMapRegionExitTextOffsets;
    C2DVector WorldMapOffset, NameGraphicOffset;
    bool AppearOnWorldMap, CreatureGenerationEnabled, SoundThemesEnabled;
    CRegion() : MiniMapScale(1.0f), MiniMapOffset(0, 0), WorldMapOffset(0, 0), AppearOnWorldMap(false),
                CreatureGenerationEnabled(true), SoundThemesEnabled(true) {}
};
int main()
{
    CRegion r;
    CExitTextOffsetMap::node* h = r.MiniMapRegionExitTextOffsets.m_header;
    if (g_strings != 4 || r.Maps.m_start || r.VisMaps.m_endOfStorage.m_data || r.PDef.Object || r.MiniMapScale != 1.0f ||
        r.MiniMapOffset.X != 0 || r.MiniMapOffset.Y != 0 || r.WorldMapOffset.X != 0 || r.WorldMapOffset.Y != 0 ||
        !h || h->colour != 0 || h->parent || h->left != h || h->right != h || r.MiniMapRegionExitTextOffsets.m_nodeCount != 0 ||
        r.AppearOnWorldMap || !r.CreatureGenerationEnabled || !r.SoundThemesEnabled) {
        std::printf("006bc410_TEST FAIL\n"); return 1;
    }
    std::free(h);
    std::printf("006bc410_TEST PASS\n");
    return 0;
}
// Compile the complete production method and exercise the real map and renderer models.
#define FABLE_RETAIL_STLP_MAP
#include <cstdlib>
// CEngineMap::PeekHasWater — water-type and depth test for up to three landscape theme layers.
// Leaf: ?PeekHasWater@CEngineMap@@QBE_NJJ@Z
// Flags: /d2inlT1267
// Inline-budget: expand all three retail PeekThemeId sites while retaining
// the original self-recursive COMDAT for translated coordinates in other maps.
#define FABLE_RETAIL_PASS22_WATER_PATCH_LOAD
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CENGINEMAP_PEEKWATER
#define FABLE_RETAIL_CENGINEMAP_PEEK_BODIES
#define FABLE_RETAIL_CENGINEMAP_PEEKTHEMEID_ORIGINAL_PASS27
#define FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_CIDRAWMAP_DRAW
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAP_ORIGINAL_PASS26
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEFCLASSINDEX
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_PEEKTHEME
#define FABLE_RETAIL_CENGINEMAP_GPENGINEWORLDMAP
#include "engine/EWaterType.h"
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"
#include "engine/CDefinitionManager.h"
#include "engine/CEngineLandscapeRenderer.h"

namespace NEngineInternal {
extern const CDefinitionManager* GPDefinitionManager;
extern CEngineLandscapeRenderer* GPLandscapeRenderer;
}

bool CEngineMap::PeekHasWater(const long mapx, const long mapy) const
{
    long t0 = PeekThemeId(mapx, mapy, 0);
    if (t0 > 0) {
        t0 = NEngineInternal::GPDefinitionManager->GetDefClassIndexFromGlobalIndex(t0);
        bool has_water = NEngineInternal::GPLandscapeRenderer->PeekTheme(t0).WaterType != WATER_TYPE_NULL;
        long t1 = PeekThemeId(mapx, mapy, 1);
        if (t1 > 0) {
            t1 = NEngineInternal::GPDefinitionManager->GetDefClassIndexFromGlobalIndex(t1);
            has_water |= NEngineInternal::GPLandscapeRenderer->PeekTheme(t1).WaterType != WATER_TYPE_NULL;
            long t2 = PeekThemeId(mapx, mapy, 2);
            if (t2 > 0) {
                t2 = NEngineInternal::GPDefinitionManager->GetDefClassIndexFromGlobalIndex(t2);
                has_water |= NEngineInternal::GPLandscapeRenderer->PeekTheme(t2).WaterType != WATER_TYPE_NULL;
            }
        }
        return has_water && PeekWaterDepth(mapx, mapy) > 0.001f;
    }
    return false;
}

#include "engine/C2DExtentsI.h"
#include "engine/CEngineInit.h"
#include <cstdio>
#include <cstring>
#include <cmath>
// Deterministic dependency bodies; all classes/layouts come from selected shared headers.
CCharString::CCharString() {}
CCharString::CCharString(const CCharString&) {}
CCharString::~CCharString() {}
CWideString::CWideString() {}
CWideString::~CWideString() {}
CEngineMemoryInit::CEngineMemoryInit() {}
CEngineInit::CEngineInit() {}
CEngineInit::~CEngineInit() {}
CEngineComponent::CEngineComponent() {}
CEngineComponent::~CEngineComponent() {}
void CEngineComponent::FrameStart(const CEngineComponentParams&) {}
void CEngineComponent::FrameEnd(const CEngineComponentParams&) {}
bool CEngineComponent::LoadNewScene(const CEngineComponentLoadNewSceneParams&) {return false;}
void CEngineComponent::Render(const CEngineComponentRenderParams&) {}
void CEngineComponent::PreLayerRender(const CEngineComponentRenderParams&) {}
void CEngineComponent::PostLayerRender(const CEngineComponentRenderParams&) {}
void CEngineComponent::WorldFrameUpdate(bool) {}
void CEngineComponent::PausedWorldFrameUpdate() {}
CCharString CEngineComponent::GetConsoleEnableFunctionName() {return CCharString();}
void CEngineComponent::OnReleaseDefaultPoolResources() {}
bool CEngineComponent::OnRestoreDefaultPoolResources() {return true;}
CEngineWorldMap::CEngineWorldMap(const CEngineInit&) : Map(0),MapArrayWidth(0),MapArrayHeight(0),MapOffsetX(0),MapOffsetY(0) {}
CEngineWorldMap::~CEngineWorldMap() {}
unsigned long CEngineWorldMap::PeekSceneFilterFlags() const {return 0;}
CEngineStaticMapFileBlockManager::~CEngineStaticMapFileBlockManager() {}
CEngineMap::CEngineMap(const CIDrawMap* map,long w,long h,long x,long y,long,CCharString,bool)
 : LandscapeMap(0),LocalDetailCacheMap(0),GameMap(map),MapWidth((unsigned short)w),MapHeight((unsigned short)h),WorldPosX((unsigned short)x),WorldPosY((unsigned short)y),EdgeHeights(0) {}
CEngineMap::~CEngineMap() {}
CIDrawMap::~CIDrawMap() {}
long CIDrawMap::DrawGetEngineThemeAt(long,long,long) const {return 0;}
unsigned char CIDrawMap::DrawGetEngineBlendAt(long,long,long) const {return 0;}
bool CIDrawMap::DrawFindLastUnblockedPosition(const C3DVector*,const C3DVector*,C3DVector*) const {return false;}
namespace NEngineInternal { CEngineWorldMap* GPEngineWorldMap=0; }

// GPU services are outside this query. The fixture stores themes in the real
// renderer's CArray and uses real CEngineMap and CEngineWorldMap objects.
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CEngineLandscapeMeshBuilder::~CEngineLandscapeMeshBuilder() {}
CEngineLandscapeRenderer::CEngineLandscapeRenderer(const CEngineMemoryInit&) {}
CEngineLandscapeRenderer::~CEngineLandscapeRenderer() {}
unsigned long CEngineLandscapeRenderer::PeekSceneFilterFlags() const { return 0; }
CEngineLandscapeRenderer::CLandscapeSettings::CLandscapeSettings() {}
CEngineLandscapeRenderer::CFastForegroundTextureLoadInfo::CFastForegroundTextureLoadInfo() {}
CCharString CShaderBase::GetDebugName() const { return CCharString(); }
CVertexShader::CVertexShader() : Data(0) {}
CVertexShaderData::~CVertexShaderData() {}
CCharString CVertexShader::GetDebugName() const { return CCharString(); }
unsigned long CVertexShader::CountReferences() { return 0; }
CPixelShader::CPixelShader() : Data(0) {}
CPixelShaderData::~CPixelShaderData() {}
CCharString CPixelShader::GetDebugName() const { return CCharString(); }
unsigned long CPixelShader::CountReferences() { return 0; }
CLandscapeTesselatorBase::CLandscapeTesselatorBase() : Data(0) {}
unsigned long CMovableResource::Verify() const { return 0; }
bool CMovableResource::CanRelocateData(long) const { return false; }
CIndexBuffer::CIndexBuffer() {}
void CIndexBuffer::Uninitialise() {}
void CIndexBuffer::RelocateData(long, void*, void*, long) {}
void CTexture::Uninitialise() {}
void CLandscapePatchTesselator::TesselateEdge(CPatchTesselationEdgeStrip*, long, long, bool, bool, LANDSCAPE_EDGE_STRIP_SIDE, unsigned short*) {}
long CLandscapePatchTesselator::GetBridgeVertexBufferSize() { return 0; }
unsigned char* CLandscapePatchTesselator::GetBridgeVertexBufferAsCArray() { return 0; }
long CLandscapePatchTesselator::GetSizeOfVertex() { return 0; }
void CLandscapePatchTesselator::SetBridgeVertexBufferSize(long) {}

namespace NEngineInternal {
const CDefinitionManager* GPDefinitionManager = 0;
CEngineLandscapeRenderer* GPLandscapeRenderer = 0;
}

class TestDrawMap : public CIDrawMap {
public:
    long themes[3];
    mutable long queries[3], x, y;
    mutable unsigned int query_count;
    TestDrawMap() : x(-1), y(-1), query_count(0) { SetThemes(0, 0, 0); }
    void SetThemes(long t0, long t1, long t2) {
        themes[0] = t0;
        themes[1] = t1;
        themes[2] = t2;
        query_count = 0;
    }
    virtual long DrawGetHeightMapSizeX() const { return 16; }
    virtual long DrawGetHeightMapSizeY() const { return 16; }
    virtual C2DExtentsI DrawGetHeightMapExtents() const { return C2DExtentsI(16, 16); }
    virtual float DrawGetSizeZAt(const C3DVector*, float) const { return 0.0f; }
    virtual float DrawBlockGetSizeZAt(unsigned long, unsigned long, float) const { return 0.0f; }
    virtual float DrawGetSeaLevel(float) const { return 0.0f; }
    virtual long DrawGetEngineThemeAt(long mapx, long mapy, long layer) const {
        x = mapx;
        y = mapy;
        if (query_count >= 3 || layer < 0 || layer >= 3)
            std::abort();
        queries[query_count++] = layer;
        return themes[layer];
    }
};

static float water_depth;
static long last_depth_x, last_depth_y;
static unsigned int depth_calls, definition_count;
static long definitions[3];

long CDefinitionManager::GetDefClassIndexFromGlobalIndex(long theme) const
{
    if (definition_count >= 3 || (theme != 10 && theme != 20))
        std::abort();
    definitions[definition_count++] = theme;
    return theme == 20 ? 1 : 0;
}

float CEngineMap::PeekWaterDepth(long mapx, long mapy) const
{
    ++depth_calls;
    last_depth_x = mapx;
    last_depth_y = mapy;
    return water_depth;
}

static unsigned int failures;
#define CHECK(value) do { if (!(value)) { ++failures; std::printf("FAIL %d\n", __LINE__); } } while (0)

int main()
{
    CEngineInit init;
    CEngineWorldMap world(init);
    NEngineInternal::GPEngineWorldMap = &world;
    short grid[4] = {0, 0, 0, 0};
    world.Map = grid;
    world.MapArrayWidth = 2;
    world.MapArrayHeight = 2;
    world.MapOffsetX = 64;
    world.MapOffsetY = 96;

    CEngineMemoryInit memory;
    CEngineLandscapeRenderer renderer(memory);
    CDefinitionManager manager;
    NEngineInternal::GPDefinitionManager = &manager;
    NEngineInternal::GPLandscapeRenderer = &renderer;
    CLandscapeTheme dry = {};
    dry.WaterType = WATER_TYPE_NULL;
    CLandscapeTheme wet = {};
    wet.WaterType = WATER_TYPE_RIVER;
    renderer.LandscapeThemes.push_back(dry);
    renderer.LandscapeThemes.push_back(wet);

    TestDrawMap own, neighbour;
    CEngineMap here(&own, 16, 16, 64, 96, 0, CCharString(), false);
    CEngineMap there(&neighbour, 16, 16, 80, 96, 0, CCharString(), false);
    CEngineMap empty(0, 16, 16, 64, 96, 0, CCharString(), false);
    world.MapList.push_back(static_cast<CEngineMap*>(0));
    world.MapList.push_back(&there);
    world.MapList.push_back(&empty);

    struct Case {
        long t0, t1, t2;
        float depth;
        unsigned int queries, lookups, depth_queries;
        bool expected;
    };
    const Case cases[] = {
        {0, 20, 20, 0.002f, 1, 0, 0, false},
        {-1, 20, 20, 0.002f, 1, 0, 0, false},
        {10, 0, 20, 0.002f, 2, 1, 0, false},
        {10, -1, 20, 0.002f, 2, 1, 0, false},
        {10, 10, 0, 0.002f, 3, 2, 0, false},
        {10, 10, -1, 0.002f, 3, 2, 0, false},
        {10, 10, 10, 0.002f, 3, 3, 0, false},
        {20, 0, 0, 0.002f, 2, 1, 1, true},
        {20, 10, 10, 0.0f, 3, 3, 1, false},
        {10, 20, 0, 0.001f, 3, 2, 1, false},
        {10, 20, 10, 0.002f, 3, 3, 1, true},
        {10, 10, 20, 0.002f, 3, 3, 1, true},
        {20, 20, 20, 0.002f, 3, 3, 1, true}
    };
    for (unsigned int i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        const Case& c = cases[i];
        own.SetThemes(c.t0, c.t1, c.t2);
        water_depth = c.depth;
        depth_calls = definition_count = 0;
        CHECK(here.PeekHasWater(3, 4) == c.expected);
        CHECK(own.x == 3 && own.y == 4 && own.query_count == c.queries);
        CHECK(depth_calls == c.depth_queries && definition_count == c.lookups);
        for (unsigned int j = 0; j < own.query_count; ++j)
            CHECK(own.queries[j] == static_cast<long>(j));
        for (unsigned int j = 0; j < definition_count; ++j)
            CHECK(definitions[j] == own.themes[j]);
        if (depth_calls)
            CHECK(last_depth_x == 3 && last_depth_y == 4);
    }

    own.SetThemes(10, 0, 0);
    neighbour.SetThemes(20, 0, 0);
    grid[0] = 1;
    depth_calls = definition_count = 0;
    water_depth = 0.002f;
    CHECK(here.PeekHasWater(20, 3) && own.query_count == 0);
    CHECK(neighbour.query_count == 2 && neighbour.x == 4 && neighbour.y == 3);
    CHECK(depth_calls == 1 && last_depth_x == 20 && last_depth_y == 3);

    own.SetThemes(20, 0, 0);
    neighbour.SetThemes(20, 0, 0);
    depth_calls = definition_count = 0;
    CHECK(here.PeekHasWater(20, 17));
    CHECK(own.query_count == 2 && own.x == 15 && own.y == 15);
    CHECK(neighbour.query_count == 0);

    grid[0] = 2;
    own.SetThemes(20, 0, 0);
    depth_calls = definition_count = 0;
    CHECK(here.PeekHasWater(-1, 20));
    CHECK(own.query_count == 2 && own.x == 0 && own.y == 15);

    own.SetThemes(20, 0, 0);
    depth_calls = definition_count = 0;
    CHECK(here.PeekHasWater(20, 3));
    CHECK(own.query_count == 2 && own.x == 15 && own.y == 3);

    own.SetThemes(20, 0, 0);
    depth_calls = definition_count = 0;
    CHECK(here.PeekHasWater(3, -1));
    CHECK(own.query_count == 2 && own.x == 3 && own.y == 0);

    grid[0] = 0;
    depth_calls = definition_count = 0;
    CHECK(!empty.PeekHasWater(3, 4) && depth_calls == 0 && definition_count == 0);

    std::puts(failures ? "00b412a0_TEST FAIL" : "00b412a0_TEST PASS");
    return failures ? 1 : 0;
}
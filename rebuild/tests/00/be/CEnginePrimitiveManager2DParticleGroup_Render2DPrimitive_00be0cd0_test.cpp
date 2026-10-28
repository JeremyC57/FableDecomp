// Behaviour test for 00be0cd0: src/00be0cd0.cpp copied verbatim; the renderers, settings and the sub-primitive pool
// stubbed so the early-outs (disabled manager, disabled layer, displacement pass off) and the queued sub-primitives
// (list, flags, alpha, depth, manager) are observable.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// Retry 10b (B-12 sweep): the first sub-primitive is a const REFERENCE to the new-expression's pointer (`CEngineSubPrimitiveParticleGroup* const& subPrimitive = new ...`): the layer-enable byte load then encodes retail's `[eax+ecx+320h]` (layer as the SIB base); a value local gives `[ecx+eax+320h]`.
// CEnginePrimitiveManager2DParticleGroup::Render2DPrimitive(CEngineInternalPrimitiveBase&, CEngineSubPrimitiveChain&, ESubPrimitiveType) -- when the manager and the group's 2D layer are enabled, queue the group as a pool-allocated sub-primitive at its layer's depth: in the given sort list, or (for a screen-displacement group, when the displacement pass is available) in lists 5 and 6 with a second displacement sub-primitive
// Leaf: Render2DPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE0CD0, length 256 bytes (`ret 0Ch`, virtual).
// Callees: CEngineSubPrimitiveBase::operator new @ 0x00BEB540 (`__fastcall`, size 0x18), CBase::CBase @ 0x0099A2F0,
// CEngineSubPrimitiveParticleGroup::CEngineSubPrimitiveParticleGroup(CEngineInternalPrimitiveBase&) @ 0x00BBBDB0 (the second
// construction calls it out of line; the first is inlined), CEnginePrimitiveRenderer2D::Layer2DToSortDistance(EEngine2DLayer)
// @ 0x00B4A6A0 (2D renderer @ 0x01436E7C, layer-enable bytes at +0x320), CEngineSubPrimitiveRenderer::AddSubPrimitive(long,
// CEngineSubPrimitiveBase*, float, CEnginePrimitiveManager*) @ 0x00B84720 (renderer @ 0x01436E74); the displacement-pass flag is
// byte +0xee of the engine settings @ 0x01436E38. Vtable 0x012A5BB0 = CEngineSubPrimitiveParticleGroup.
// Layout: the sub-primitive classes as the landed CEnginePrimitiveManagerDecal::RenderPrimitive 0x00BBA340 (RenderFlags +4,
// Alpha +8, Parent +0xc, NextSubPrimitive +0x10); the group's ScreenDisplacement +0xa0 and Layer +0xac as the landed
// CEngineInternalPrimitive2DParticleGroup::Update 0x00BE0DE0; the manager's Enable +8.
// Shape: RenderFlags/Alpha are assigned after `new` (stored through the possibly-null pointer); the layer depth is a named
// float homed in the dead `primitive` argument slot; the chain argument is unused.
enum EEngine2DLayer { };                         // Ego_r.pdb layer enum (passed as a 32-bit value)
class CBase {
public:
    virtual ~CBase();
    CBase();                                       // 0x0099A2F0
};
class CEngineInternalPrimitiveBase;
class CEnginePrimitiveManager;
class __declspec(novtable) CEngineSubPrimitiveBase : public CBase {   // PDB, 0x18 bytes
public:
    unsigned long                 RenderFlags;      // +0x04
    unsigned char                 Alpha;            // +0x08
    CEngineInternalPrimitiveBase* Parent;           // +0x0c
    CEngineSubPrimitiveBase*      NextSubPrimitive; // +0x10
    CEnginePrimitiveManager*      Manager;          // +0x14
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase& parent) : Parent(&parent), NextSubPrimitive(0) { }
    static void* __fastcall operator new(unsigned int size);   // 0x00BEB540 (sub-primitive pool)
};
class CEngineSubPrimitiveParticleGroup : public CEngineSubPrimitiveBase {   // vtable 0x012A5BB0
public:
    CEngineSubPrimitiveParticleGroup(CEngineInternalPrimitiveBase& parent) : CEngineSubPrimitiveBase(parent) { }   // 0x00BBBDB0
    virtual ~CEngineSubPrimitiveParticleGroup();
};

class CEngineInternalPrimitive2DParticleGroup {    // retail layout (see the landed Update 0x00BE0DE0)
public:
    unsigned char _pad00[0xa0];
    bool          ScreenDisplacement;               // +0xa0
    unsigned char _pada1[0xb];
    long          Layer;                            // +0xac
};

class CEnginePrimitiveRenderer2D {
public:
    unsigned char _pad[0x320];
    bool          LayerEnabled[1];                  // +0x320 (indexed by layer)
    float Layer2DToSortDistance(EEngine2DLayer layer); // 0x00B4A6A0
};
extern CEnginePrimitiveRenderer2D* g_pPrimitiveRenderer2D;   // 0x01436E7C

struct CEngineSettings_01436e38 {
    unsigned char _pad[0xee];
    bool          DisplacementPassEnabled;          // +0xee (descriptive)
};
extern CEngineSettings_01436e38* g_pEngineSettings_01436e38;   // 0x01436E38

class CEngineSubPrimitiveRenderer {
public:
    void AddSubPrimitive(long list, CEngineSubPrimitiveBase* subPrimitive, float distance, CEnginePrimitiveManager* manager);   // 0x00B84720
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74

class CEngineSubPrimitiveChain;

class CEnginePrimitiveManager2DParticleGroup {
public:
    void*         SceneList;                        // +0x04
    bool          Enable;                           // +0x08
    virtual void Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain, long type);
};

void CEnginePrimitiveManager2DParticleGroup::Render2DPrimitive(CEngineInternalPrimitiveBase& primitive, CEngineSubPrimitiveChain& chain,
                                                               long type)
{
    if (!Enable)
        return;
    CEngineInternalPrimitive2DParticleGroup& group = (CEngineInternalPrimitive2DParticleGroup&)primitive;
    if (!g_pPrimitiveRenderer2D->LayerEnabled[group.Layer])
        return;
    CEngineSubPrimitiveParticleGroup* const& subPrimitive = new CEngineSubPrimitiveParticleGroup(primitive);
    subPrimitive->Alpha = 0xff;
    subPrimitive->RenderFlags = 1;
    float depth = g_pPrimitiveRenderer2D->Layer2DToSortDistance((EEngine2DLayer)group.Layer);
    if (group.ScreenDisplacement) {
        if (!g_pEngineSettings_01436e38->DisplacementPassEnabled)
            return;
        g_pEngineSubPrimitiveRenderer->AddSubPrimitive(5, subPrimitive, depth, (CEnginePrimitiveManager*)this);
        CEngineSubPrimitiveParticleGroup* displacement = new CEngineSubPrimitiveParticleGroup(primitive);
        displacement->Alpha = 0xff;
        displacement->RenderFlags = 0x80;
        g_pEngineSubPrimitiveRenderer->AddSubPrimitive(6, displacement, depth, (CEnginePrimitiveManager*)this);
        return;
    }
    g_pEngineSubPrimitiveRenderer->AddSubPrimitive(type, subPrimitive, depth, (CEnginePrimitiveManager*)this);
}
// ---- end source ----
CBase::CBase() {}
CBase::~CBase() {}
CEngineSubPrimitiveParticleGroup::~CEngineSubPrimitiveParticleGroup() {}
void* __fastcall CEngineSubPrimitiveBase::operator new(unsigned int size) { return malloc(size); }
static unsigned char g_rendererRaw[0x400];
CEnginePrimitiveRenderer2D* g_pPrimitiveRenderer2D = (CEnginePrimitiveRenderer2D*)g_rendererRaw;
float CEnginePrimitiveRenderer2D::Layer2DToSortDistance(EEngine2DLayer layer) { return 10.0f + layer; }
static CEngineSettings_01436e38 g_settings;
CEngineSettings_01436e38* g_pEngineSettings_01436e38 = &g_settings;
struct SAdd { long list; CEngineSubPrimitiveBase* sub; float distance; CEnginePrimitiveManager* manager; };
static SAdd g_adds[8]; static int g_nAdds = 0;
void CEngineSubPrimitiveRenderer::AddSubPrimitive(long list, CEngineSubPrimitiveBase* sub, float distance, CEnginePrimitiveManager* manager)
{ SAdd a = { list, sub, distance, manager }; g_adds[g_nAdds++] = a; }
static CEngineSubPrimitiveRenderer g_subRenderer;
CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer = &g_subRenderer;
int main()
{
    static unsigned char groupRaw[0xb0]; memset(groupRaw, 0, sizeof(groupRaw));
    CEngineInternalPrimitive2DParticleGroup* group = (CEngineInternalPrimitive2DParticleGroup*)groupRaw;
    CEngineInternalPrimitiveBase& prim = *(CEngineInternalPrimitiveBase*)groupRaw;
    CEngineSubPrimitiveChain& chain = *(CEngineSubPrimitiveChain*)0;
    CEnginePrimitiveManager2DParticleGroup mgr; mgr.Enable = false;
    group->Layer = 3;
    mgr.Render2DPrimitive(prim, chain, 2); CHECK(g_nAdds == 0);             // manager disabled
    mgr.Enable = true;
    mgr.Render2DPrimitive(prim, chain, 2); CHECK(g_nAdds == 0);             // layer 3 disabled
    g_pPrimitiveRenderer2D->LayerEnabled[3] = true;
    mgr.Render2DPrimitive(prim, chain, 2);
    CHECK(g_nAdds == 1 && g_adds[0].list == 2 && g_adds[0].distance == 13.0f && g_adds[0].manager == (CEnginePrimitiveManager*)&mgr);
    CHECK(g_adds[0].sub->RenderFlags == 1 && g_adds[0].sub->Alpha == 0xff && g_adds[0].sub->Parent == &prim && g_adds[0].sub->NextSubPrimitive == 0);
    group->ScreenDisplacement = true;
    mgr.Render2DPrimitive(prim, chain, 2); CHECK(g_nAdds == 1);             // displacement pass off
    g_settings.DisplacementPassEnabled = true;
    mgr.Render2DPrimitive(prim, chain, 2);
    CHECK(g_nAdds == 3 && g_adds[1].list == 5 && g_adds[1].sub->RenderFlags == 1 && g_adds[2].list == 6 && g_adds[2].sub->RenderFlags == 0x80);
    CHECK(g_adds[2].sub->Alpha == 0xff && g_adds[2].distance == 13.0f);
    if (fails == 0) printf("00be0cd0_TEST PASS\n"); else printf("00be0cd0_TEST FAIL\n");
    return fails;
}
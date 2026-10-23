#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
// CEnginePrimitiveManagerRepeatedZSprites::RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain) -- when enabled: take the camera distance; during an occlusion pass make sure the primitive has the pass's occlusion info and, unless it is too long occluded, queue an occlusion-test sub-primitive (result 1); otherwise store the sort depth and append the primitive to the batch list (result 2)
// Leaf: ?RenderPrimitive@CEnginePrimitiveManagerRepeatedZSprites
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BABF70, length 384 bytes (with int3 padding; body 0x17d), `ret 0Ch` (virtual).
// Callees: operator new @ 0x00BFEA1A, CEnginePrimitiveOcclusionInfo ctor @ 0x00B2FA90, CEnginePrimitiveOcclusionManager::
//   GetOccludedFrames(CEnginePrimitiveOcclusionInfo*) @ 0x00B2FAC0 (global @ 0x01436E58, threshold at +0xc), the sub-primitive
//   pool operator new @ 0x00BEB540 (`__fastcall`, size 0x18) and the occlusion sub-primitive constructor @ 0x00B98B10
//   (manifest "CEngineSubPrimitive2DClockSprite" is propagated), CEngineSubPrimitiveRenderer::AddSubPrimitive(long list,
//   CEngineSubPrimitiveBase*, float, CEnginePrimitiveManager*) @ 0x00B84720 (global @ 0x01436E74), the batch vector's
//   _M_insert_overflow @ 0x00BAC720.
// Globals: camera @ 0x01436EA0 (world position +0x54), render settings @ 0x01436E78 (occlusion pass flag +0xbd, current
//   occlusion layer +0x78).
// Layout: manager Enable +0x08 (PDB), batch list (STLport vector of primitive pointers) +0x80; primitive as this chunk's
//   occlusion models (flags +0x0c, bit 1 of +0x34, occlusion info +0x48), world position +0x64, depth bias +0x70, sort
//   depth +0x8c. Modelled locally.
// Shape: the distance is the camera difference summed Z*Z + Y*Y + X*X on the x87 stack and kept in the dead `primitive`
//   argument slot; push_back is the inline STLport form whose value temporary reuses that slot.
#include <math.h>

struct C3DVector { float X, Y, Z; };

class CEnginePrimitiveOcclusionInfo {
public:
    virtual void v000();
    virtual void Destroy();                        // slot 1
    long RefCount;                                 // +0x04
    unsigned char _pad08[0x18];
    CEnginePrimitiveOcclusionInfo();               // 0x00B2FA90
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CEnginePrimitiveOcclusionManager {
public:
    unsigned char _pad00[0xc];
    unsigned long MaxOccludedFrames;               // +0x0c
    unsigned long GetOccludedFrames(CEnginePrimitiveOcclusionInfo* info);   // 0x00B2FAC0
    __forceinline bool IsRecentlyVisible(CEnginePrimitiveOcclusionInfo* info) { return GetOccludedFrames(info) <= MaxOccludedFrames; }
};
extern CEnginePrimitiveOcclusionManager* g_pPrimitiveOcclusionManager;   // 0x01436E58

template <class T> class CIVCountedPointer {
public:
    T* Object;
    __forceinline void Assign(T* object)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = object;
    }
};

class CEngineInternalPrimitiveBase {
public:
    void*                         _vptr;            // +0x00
    long                          RefCount;         // +0x04
    long                          Type;             // +0x08
    unsigned long                 EnginePrimitiveFlags;   // +0x0c
    unsigned char                 _pad10[0x24];
    unsigned char                 Flags;            // +0x34
    unsigned char                 _pad35[0x13];
    CIVCountedPointer<CEnginePrimitiveOcclusionInfo> OcclusionInfo[2];   // +0x48
    unsigned char                 _pad50[0x14];
    C3DVector                     WorldPosition;    // +0x64
    float                         DepthBias;        // +0x70
    unsigned char                 _pad74[0x18];
    float                         SortDepth;        // +0x8c
};

class CEnginePrimitiveManager;
class CEngineSubPrimitiveBase {
public:
    static void* __fastcall operator new(size_t size);   // 0x00BEB540 (sub-primitive pool)
};
class CEngineSubPrimitiveOcclusionTest : public CEngineSubPrimitiveBase {
public:
    unsigned char _body[0x18];
    CEngineSubPrimitiveOcclusionTest(CEngineInternalPrimitiveBase& parent);   // 0x00B98B10
};
class CEngineSubPrimitiveRenderer {
public:
    void AddSubPrimitive(long list, CEngineSubPrimitiveBase* subPrimitive, float distance, CEnginePrimitiveManager* manager);   // 0x00B84720
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74

struct CEngineCamera {
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;                    // +0x54
};
extern CEngineCamera* g_pEngineCamera;              // 0x01436EA0

struct CEngineRenderSettings {
    unsigned char _pad00[0x78];
    long          CurrentOcclusionLayer;            // +0x78
    unsigned char _pad7c[0x41];
    bool          OcclusionPass;                    // +0xbd
};
extern CEngineRenderSettings* g_pEngineRenderSettings;   // 0x01436E78

struct __true_type { __true_type() {} };
template <class T> struct CVector {                 // STLport vector<T>
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int n, bool atEnd);   // 0x00BAC720
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

// Stand-in for the out-of-line STLport _M_insert_overflow @ 0x00BAC720, compiled above the graded function under
// auto_inline(off) so VC7.1 sees that the value is only read through its reference and the tag never touched (the
// GOTCHAS retry-10f rule): the push_back temporary then shares the dead `primitive` argument slot with the distance, as
// in retail. Only RenderPrimitive is graded.
void GrowPrimitiveList(CEngineInternalPrimitiveBase** pos, CEngineInternalPrimitiveBase* value);
#pragma auto_inline(off)
template <> void CVector<CEngineInternalPrimitiveBase*>::_M_insert_overflow(CEngineInternalPrimitiveBase** pos,
    CEngineInternalPrimitiveBase* const& x, const __true_type&, unsigned int n, bool atEnd)
{
    GrowPrimitiveList(pos, x);
}
#pragma auto_inline(on)

class CEngineSubPrimitiveChain;
enum ERendered { RENDERED_NOTHING = 0, RENDERED_OCCLUSION = 1, RENDERED_SUB_PRIMITIVES = 2 };

class CEnginePrimitiveManagerRepeatedZSprites {
public:
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& primitive, long layer, CEngineSubPrimitiveChain& chain);
    void*         SceneList;                        // +0x04
    bool          Enable;                           // +0x08
    unsigned char _pad09[0x80 - 0x09];
    CVector<CEngineInternalPrimitiveBase*> Primitives;   // +0x80
};

ERendered CEnginePrimitiveManagerRepeatedZSprites::RenderPrimitive(CEngineInternalPrimitiveBase& primitive, long layer,
                                                                  CEngineSubPrimitiveChain& chain)
{
    if (!Enable)
        return RENDERED_NOTHING;
    float dx = primitive.WorldPosition.X - g_pEngineCamera->WorldPosition.X;
    float dy = primitive.WorldPosition.Y - g_pEngineCamera->WorldPosition.Y;
    float dz = primitive.WorldPosition.Z - g_pEngineCamera->WorldPosition.Z;
    float distance = (float)sqrt(dx * dx + dy * dy + dz * dz);
    if (g_pEngineRenderSettings->OcclusionPass) {
        long occlusion = g_pEngineRenderSettings->CurrentOcclusionLayer;
        if (!(primitive.Flags & 2) && occlusion != -1) {
            if (primitive.EnginePrimitiveFlags & 0x4000)
                goto queue;
            if (!primitive.OcclusionInfo[occlusion].Object)
                primitive.OcclusionInfo[occlusion].Assign(new CEnginePrimitiveOcclusionInfo);
        }
        if (!(primitive.EnginePrimitiveFlags & 0x4000)) {
            if (occlusion != -1) {
                CEnginePrimitiveOcclusionInfo* info = primitive.OcclusionInfo[occlusion].Object;
                if (info) {
                    if (g_pPrimitiveOcclusionManager->IsRecentlyVisible(info))
                        goto queue;
                }
            }
            g_pEngineSubPrimitiveRenderer->AddSubPrimitive(7, new CEngineSubPrimitiveOcclusionTest(primitive), distance,
                                                           (CEnginePrimitiveManager*)this);
            return RENDERED_OCCLUSION;
        }
    }
queue:
    primitive.SortDepth = distance - primitive.DepthBias;
    Primitives.push_back(&primitive);
    return RENDERED_SUB_PRIMITIVES;
}
// ---- test harness ----
static int g_infoCreated, g_infoDestroyed, g_subCreated, g_added, g_grown;
static long g_addList; static float g_addDistance; static void* g_addManager;
static CEngineInternalPrimitiveBase* g_grownValue;
static unsigned long g_frames;
void CEnginePrimitiveOcclusionInfo::v000() {}
void CEnginePrimitiveOcclusionInfo::Destroy() { ++g_infoDestroyed; }
CEnginePrimitiveOcclusionInfo::CEnginePrimitiveOcclusionInfo() { RefCount = 1; ++g_infoCreated; }
unsigned long CEnginePrimitiveOcclusionManager::GetOccludedFrames(CEnginePrimitiveOcclusionInfo* info) { return info ? g_frames : 0; }
static CEnginePrimitiveOcclusionManager g_occ;
CEnginePrimitiveOcclusionManager* g_pPrimitiveOcclusionManager = &g_occ;
static unsigned char g_subPool[4][0x40]; static int g_subUsed;
void* __fastcall CEngineSubPrimitiveBase::operator new(size_t size) { (void)size; return g_subPool[g_subUsed++ & 3]; }
CEngineSubPrimitiveOcclusionTest::CEngineSubPrimitiveOcclusionTest(CEngineInternalPrimitiveBase& parent) { (void)parent; ++g_subCreated; }
void CEngineSubPrimitiveRenderer::AddSubPrimitive(long list, CEngineSubPrimitiveBase* sub, float distance, CEnginePrimitiveManager* manager)
{ (void)sub; ++g_added; g_addList = list; g_addDistance = distance; g_addManager = manager; }
static CEngineSubPrimitiveRenderer g_ren;
CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer = &g_ren;
static CEngineCamera g_cam;
CEngineCamera* g_pEngineCamera = &g_cam;
static CEngineRenderSettings g_rs;
CEngineRenderSettings* g_pEngineRenderSettings = &g_rs;
void GrowPrimitiveList(CEngineInternalPrimitiveBase** pos, CEngineInternalPrimitiveBase* value) { (void)pos; ++g_grown; g_grownValue = value; }
static int g_fail;
static void Check(int ok, const char* what) { if (!ok) { ++g_fail; printf("FAIL %s\n", what); } }
int main()
{
    static CEnginePrimitiveManagerRepeatedZSprites mgr;
    static CEngineInternalPrimitiveBase prim;
    CEngineInternalPrimitiveBase* store[4];
    CEngineSubPrimitiveChain* chain = 0;
    g_occ.MaxOccludedFrames = 5;
    g_cam.WorldPosition.X = 0; g_cam.WorldPosition.Y = 0; g_cam.WorldPosition.Z = 0;
    prim.WorldPosition.X = 3; prim.WorldPosition.Y = 4; prim.WorldPosition.Z = 0; prim.DepthBias = 1;
    mgr.Primitives._M_start = mgr.Primitives._M_finish = store; mgr.Primitives._M_end_of_storage = store + 4;
    mgr.Enable = false;
    Check(mgr.RenderPrimitive(prim, 0, *chain) == RENDERED_NOTHING, "disabled");
    mgr.Enable = true;
    g_rs.OcclusionPass = false;
    Check(mgr.RenderPrimitive(prim, 0, *chain) == RENDERED_SUB_PRIMITIVES, "normal queue");
    Check(prim.SortDepth == 4.0f && mgr.Primitives._M_finish == store + 1 && store[0] == &prim, "sort depth + push");
    // occlusion pass, layer 1, no info yet -> create info; frames 9 > 5 -> occlusion test queued
    g_rs.OcclusionPass = true; g_rs.CurrentOcclusionLayer = 1; g_frames = 9;
    Check(mgr.RenderPrimitive(prim, 0, *chain) == RENDERED_OCCLUSION, "occlusion test");
    Check(g_infoCreated == 1 && prim.OcclusionInfo[1].Object != 0, "info created");
    Check(g_subCreated == 1 && g_added == 1 && g_addList == 7 && g_addDistance == 5.0f && g_addManager == (void*)&mgr, "sub-primitive added");
    // frames 3 <= 5 -> queue
    g_frames = 3;
    Check(mgr.RenderPrimitive(prim, 0, *chain) == RENDERED_SUB_PRIMITIVES, "recently visible queues");
    Check(g_infoCreated == 1 && mgr.Primitives._M_finish == store + 2, "no new info, pushed");
    // flag 0x4000 -> queue directly
    prim.EnginePrimitiveFlags = 0x4000; prim.OcclusionInfo[0].Object = 0; g_rs.CurrentOcclusionLayer = 0;
    Check(mgr.RenderPrimitive(prim, 0, *chain) == RENDERED_SUB_PRIMITIVES && g_infoCreated == 1, "0x4000 skips");
    // layer -1 -> occlusion test without info
    prim.EnginePrimitiveFlags = 0; g_rs.CurrentOcclusionLayer = -1;
    Check(mgr.RenderPrimitive(prim, 0, *chain) == RENDERED_OCCLUSION && g_added == 2, "layer -1 tests");
    // full vector -> overflow path
    mgr.Primitives._M_finish = store + 4; g_rs.OcclusionPass = false;
    mgr.RenderPrimitive(prim, 0, *chain);
    Check(g_grown == 1 && g_grownValue == &prim, "overflow");
    printf(g_fail ? "00babf70_TEST FAIL\n" : "00babf70_TEST PASS\n");
    return g_fail;
}
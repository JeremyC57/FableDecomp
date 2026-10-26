// CEnginePrimitiveChangerManager::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& primitive) -- forwards the changer's current child to its own manager's shadow render, after handing the child the changer's mesh effect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC5A20, length 116 bytes.
// Callees: the child's virtual SetMeshEffect (slot 25, +0x64, takes the counted effect by const reference), the effect's
// virtual destroy (slot 1) at zero count, the child manager's virtual RenderPrimitiveToShadowBuffer (slot 8, +0x20) through the
// engine's manager table (@ 0x01436E84, +0x10 indexed by the child's type at +8, as in the landed CEngineScene::
// RenderPrimitiveList @ 0x00B917E0).
// Shape: `this` is unused (`ret 4`); no current index (-1), no child or a child without bit 0 of its +0x34 flag byte returns 0
// (one shared `xor eax,eax` tail: the single-exit nested-if form; early `return 0`s give the lane a shrink-wrapped
// return before `push esi`); the child is the current entry of the changer's 8-byte handle vector (+0x48, the STLport
// `*(begin() + n)` element, whose slot address VC forms with `lea`); the slot is a named pointer whose primitive is
// read twice in the test (`handle->m_pPrimitive && handle->m_pPrimitive->m_flags34 & 1`): the CSE'd second read leaves
// retail's dead `lea eax,[edx+eax*8]` beside the `mov esi,[edx+eax*8+4]` load (a single read through `child` drops it). The mesh effect goes through an inline accessor returning
// the counted pointer BY VALUE: the temporary is homed in the dead argument slot, add-ref'd with `inc [ecx+4]` and released
// after the call by the ref-count base's decrement / test accessors. The child manager's result is returned.
struct CRefCounted {                               // second base: the count sits at +4 behind the vptr
    long m_count;
    void AddRef() { ++m_count; }
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};

struct CCountedVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};

class CEngineMeshEffect : public CCountedVirtual, public CRefCounted { public:
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

struct CMeshEffectPointer {                        // CIVCountedPointer<CEngineMeshEffect>
    CEngineMeshEffect* m_p;
    CMeshEffectPointer(const CMeshEffectPointer& other) : m_p(other.m_p)
    {
        if (m_p)
            m_p->AddRef();
    }
    ~CMeshEffectPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
};

typedef long ERendered;

class CEngineInternalPrimitiveBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24();
    virtual void SetMeshEffect(const CMeshEffectPointer& effect);   // slot 25 (+0x64)
    unsigned char _pad04[0x4];
    long          m_type;                          // +0x08
    unsigned char _pad0c[0x28];
    unsigned char m_flags34;                       // +0x34 (bit 0: casts shadows)
};

class CEnginePrimitiveHandle { public:                    // 8 bytes: the handle id and the primitive
    long                          m_id;
    CEngineInternalPrimitiveBase* m_pPrimitive;
};

template <class T> struct CRetailVector {
    T* first;
    T* last;
    T* end;
    T* begin() { return first; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct CEngineInternalPrimitiveChanger : CEngineInternalPrimitiveBase {
    unsigned char                         _pad38[0x10];   // +0x38 (the base is padded to 0x38)
    CRetailVector<CEnginePrimitiveHandle> Primitives;   // +0x48
    CMeshEffectPointer                    m_meshEffect; // +0x54
    long                                  m_currentIndex;   // +0x58 (-1 = none)
    CMeshEffectPointer GetMeshEffect() const { return m_meshEffect; }
};

struct CEnginePrimitiveManagerBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07();
    virtual ERendered RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& primitive);   // slot 8 (+0x20)
};

struct CEngine_01436e84 {
    unsigned char                 _pad[0x10];
    CEnginePrimitiveManagerBase*  m_managers[1];   // +0x10, indexed by primitive type
};
extern CEngine_01436e84* g_pEngine_01436e84;       // 0x01436E84

class CEnginePrimitiveChangerManager { public:
    ERendered RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& primitive);
};

ERendered CEnginePrimitiveChangerManager::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& primitive)
{
    CEngineInternalPrimitiveChanger& changer = static_cast<CEngineInternalPrimitiveChanger&>(primitive);
    if (changer.m_currentIndex != -1) {
        CEnginePrimitiveHandle* handle = &changer.Primitives[changer.m_currentIndex];
        if (handle->m_pPrimitive && (handle->m_pPrimitive->m_flags34 & 1)) {
            CEngineInternalPrimitiveBase* child = handle->m_pPrimitive;
            child->SetMeshEffect(changer.GetMeshEffect());
            return g_pEngine_01436e84->m_managers[child->m_type]->RenderPrimitiveToShadowBuffer(*child);
        }
    }
    return 0;
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

#define V(n) void CEngineInternalPrimitiveBase::v##n() { }
V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
V(20) V(21) V(22) V(23) V(24)
#undef V
#define M(n) void CEnginePrimitiveManagerBase::v##n() { }
M(00) M(01) M(02) M(03) M(04) M(05) M(06) M(07)
#undef M
static int g_destroys = 0, g_sets = 0, g_renders = 0;
static CEngineInternalPrimitiveBase* g_setThis = 0;
static CEngineMeshEffect* g_setEffect = 0;
static long g_effectCountSeen = 0;
static CEngineInternalPrimitiveBase* g_rendered = 0;
static CEnginePrimitiveManagerBase* g_renderMgr = 0;
void CCountedVirtual::v00() { }
void CCountedVirtual::Destroy() { ++g_destroys; }
void CEngineInternalPrimitiveBase::SetMeshEffect(const CMeshEffectPointer& effect)
{
    ++g_sets; g_setThis = this; g_setEffect = effect.m_p; g_effectCountSeen = effect.m_p ? effect.m_p->m_count : -1;
}
ERendered CEnginePrimitiveManagerBase::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& primitive)
{
    ++g_renders; g_rendered = &primitive; g_renderMgr = this; return 7;
}
static unsigned char g_engineRaw[0x10 + 4 * 4];
CEngine_01436e84* g_pEngine_01436e84 = (CEngine_01436e84*)g_engineRaw;

int main()
{
    static CEnginePrimitiveManagerBase mgr0, mgr2;
    g_pEngine_01436e84->m_managers[0] = &mgr0;
    (&g_pEngine_01436e84->m_managers[0])[2] = &mgr2;
    static CEngineInternalPrimitiveBase childA, childB;
    childA.m_type = 2; childA.m_flags34 = 1;
    childB.m_type = 0; childB.m_flags34 = 2;          // bit 0 clear: no shadow
    static CEnginePrimitiveHandle handles[3] = { { 10, 0 }, { 11, &childA }, { 12, &childB } };
    static CEngineMeshEffect effect;
    effect.m_count = 1;
    static unsigned char raw[sizeof(CEngineInternalPrimitiveChanger)];
    CEngineInternalPrimitiveChanger* changer = (CEngineInternalPrimitiveChanger*)raw;
    changer->Primitives.first = handles; changer->Primitives.last = handles + 3; changer->Primitives.end = handles + 3;
    changer->m_meshEffect.m_p = &effect;
    CEnginePrimitiveChangerManager manager;
    changer->m_currentIndex = -1;
    CHECK(manager.RenderPrimitiveToShadowBuffer(*changer) == 0);
    changer->m_currentIndex = 0;                        // no child
    CHECK(manager.RenderPrimitiveToShadowBuffer(*changer) == 0);
    changer->m_currentIndex = 2;                        // child without the shadow bit
    CHECK(manager.RenderPrimitiveToShadowBuffer(*changer) == 0);
    CHECK(g_sets == 0 && g_renders == 0);
    changer->m_currentIndex = 1;
    CHECK(manager.RenderPrimitiveToShadowBuffer(*changer) == 7);
    CHECK(g_sets == 1 && g_setThis == &childA && g_setEffect == &effect && g_effectCountSeen == 2);
    CHECK(g_renders == 1 && g_rendered == &childA && g_renderMgr == &mgr2);
    CHECK(effect.m_count == 1 && g_destroys == 0);
    if (fails == 0) printf("00bc5a20_TEST PASS\n");
    return fails;
}
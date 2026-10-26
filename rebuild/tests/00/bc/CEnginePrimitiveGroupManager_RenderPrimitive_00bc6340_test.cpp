// behaviour test for 0x00BC6340 CEnginePrimitiveGroupManager::RenderPrimitive (body verbatim, callees stubbed)
#include <stdio.h>
// CEnginePrimitiveGroupManager::RenderPrimitive(CEngineInternalPrimitiveBase& group, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain) -- render a primitive group: during an occlusion pass make sure the group owns the pass's occlusion info and share it with each child's occlusion target, render every enabled child of the layer through its type's manager and return the best render result
// Grade: RELOCATION_MATCH -- the child is tested through the handle (`handle.Data && (handle.Data->RenderLayerMask & layer) && handle.Data->Enable`) and only then named, which leaves retail's dead `lea eax,[eax+ecx*8]` after the child load; `result` declared before `count` loads begin into ecx and schedules `xor eax,eax` before the `sar`
// Leaf: ?RenderPrimitive@CEnginePrimitiveGroupManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC6340, length 322 bytes, `ret 0Ch` (virtual, slot 7).
// Callees: operator new @ 0x00BFEA1A, CEnginePrimitiveOcclusionInfo::CEnginePrimitiveOcclusionInfo @ 0x00B2FA90 (0x20 bytes),
//   the child's virtual slot 27 (+0x6c, the primitive that carries its occlusion info; name unknown), the counted info's
//   virtual Destroy (slot 1), the child manager's RenderPrimitive (slot 7) on the type table @ 0x01436E84.
// Globals: the engine render settings @ 0x01436E78 (+0x78: the current occlusion layer, -1 outside occlusion passes;
//   the landed rows read its fade settings at +0x7c..).
// Layout: CEngineInternalPrimitiveBase as this chunk's CreateOcclusionGroup/RenderEntireScene models (RefCount +0x04,
//   Type +0x08, EnginePrimitiveFlags +0x0c, RenderLayerMask +0x30, Enable = bit 0 of +0x34, per-layer occlusion info
//   +0x48); the group's child handle vector (8-byte CEnginePrimitiveHandle, Data at +4) begins at +0x50.
// Shape: the best result lives in the dead `group` argument slot; the child vector is re-read every iteration; the info
//   copy is the counted pointer's operator= (identity test, release, add-ref).
class CEngineSubPrimitiveBase;
class CEngineSubPrimitiveChain { public:
    CEngineSubPrimitiveBase* LastOpaque;
    CEngineSubPrimitiveBase* LastAlpha;
};

class CEnginePrimitiveOcclusionInfo {
public:
    virtual void v000();
    virtual void Destroy();                        // slot 1
    long RefCount;                                 // +0x04
    unsigned char _pad08[0x18];
    CEnginePrimitiveOcclusionInfo();               // 0x00B2FA90
    void Release() { if (--RefCount == 0) Destroy(); }
};

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
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        T* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};

class CEngineInternalPrimitiveBase;
class CEnginePrimitiveHandle { public:
    void* _vptr;
    CEngineInternalPrimitiveBase* Data;            // +0x04
};
template <class T> class CArray { public:
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    unsigned long size() const { return m_pEnd - m_pBegin; }
    T& operator[](unsigned long i) { return m_pBegin[i]; }
};

class CEngineInternalPrimitiveBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26();
    virtual CEngineInternalPrimitiveBase* GetOcclusionPrimitive();   // slot 27 (+0x6c)
    long                          RefCount;         // +0x04
    long                          Type;             // +0x08
    unsigned long                 EnginePrimitiveFlags;   // +0x0c
    unsigned char                 _pad10[0x20];
    unsigned long                 RenderLayerMask;  // +0x30
    unsigned char                 Enable : 1;       // +0x34
    unsigned char                 _pad35[0x13];
    CIVCountedPointer<CEnginePrimitiveOcclusionInfo> OcclusionInfo[2];   // +0x48
    CArray<CEnginePrimitiveHandle> Children;        // +0x50 (groups)
};

class CEnginePrimitiveManager {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual long RenderPrimitive(CEngineInternalPrimitiveBase& primitive, long layer, CEngineSubPrimitiveChain& chain);   // slot 7
};
class CEnginePrimitiveTypeInfo { public:
    unsigned char            _pad[0x10];
    CEnginePrimitiveManager* PrimitiveManagers[47];   // +0x10
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84

struct CEngineRenderSettings {
    unsigned char _pad[0x78];
    long          CurrentOcclusionLayer;           // +0x78
};
extern CEngineRenderSettings* g_pEngineRenderSettings;   // 0x01436E78

class CEnginePrimitiveGroupManager : public CEnginePrimitiveManager {
public:
    virtual long RenderPrimitive(CEngineInternalPrimitiveBase& group, long layer, CEngineSubPrimitiveChain& chain);
};

long CEnginePrimitiveGroupManager::RenderPrimitive(CEngineInternalPrimitiveBase& group, long layer, CEngineSubPrimitiveChain& chain)
{
    long occlusion = g_pEngineRenderSettings->CurrentOcclusionLayer;
    if (occlusion != -1 && !(group.EnginePrimitiveFlags & 0x4000) && !group.OcclusionInfo[occlusion].Object)
        group.OcclusionInfo[occlusion].Assign(new CEnginePrimitiveOcclusionInfo);
    long result = 0;
    unsigned long count = group.Children.size();
    for (unsigned long i = 0; i < count; ++i) {
        CEnginePrimitiveHandle& handle = group.Children[i];
        if (handle.Data && (handle.Data->RenderLayerMask & layer) && handle.Data->Enable) {
            CEngineInternalPrimitiveBase* child = handle.Data;
            if (occlusion != -1) {
                CEngineInternalPrimitiveBase* target = child->GetOcclusionPrimitive();
                if (target)
                    target->OcclusionInfo[occlusion] = group.OcclusionInfo[occlusion];
            }
            long rendered = g_pEnginePrimitiveTypeInfo->PrimitiveManagers[child->Type]->RenderPrimitive(*child, layer, chain);
            if (rendered > result)
                result = rendered;
        }
    }
    return result;
}

// ---- stubs ----
void CEngineInternalPrimitiveBase::v00() { }
void CEngineInternalPrimitiveBase::v01() { }
void CEngineInternalPrimitiveBase::v02() { }
void CEngineInternalPrimitiveBase::v03() { }
void CEngineInternalPrimitiveBase::v04() { }
void CEngineInternalPrimitiveBase::v05() { }
void CEngineInternalPrimitiveBase::v06() { }
void CEngineInternalPrimitiveBase::v07() { }
void CEngineInternalPrimitiveBase::v08() { }
void CEngineInternalPrimitiveBase::v09() { }
void CEngineInternalPrimitiveBase::v10() { }
void CEngineInternalPrimitiveBase::v11() { }
void CEngineInternalPrimitiveBase::v12() { }
void CEngineInternalPrimitiveBase::v13() { }
void CEngineInternalPrimitiveBase::v14() { }
void CEngineInternalPrimitiveBase::v15() { }
void CEngineInternalPrimitiveBase::v16() { }
void CEngineInternalPrimitiveBase::v17() { }
void CEngineInternalPrimitiveBase::v18() { }
void CEngineInternalPrimitiveBase::v19() { }
void CEngineInternalPrimitiveBase::v20() { }
void CEngineInternalPrimitiveBase::v21() { }
void CEngineInternalPrimitiveBase::v22() { }
void CEngineInternalPrimitiveBase::v23() { }
void CEngineInternalPrimitiveBase::v24() { }
void CEngineInternalPrimitiveBase::v25() { }
void CEngineInternalPrimitiveBase::v26() { }
void CEnginePrimitiveManager::v00() { }
void CEnginePrimitiveManager::v01() { }
void CEnginePrimitiveManager::v02() { }
void CEnginePrimitiveManager::v03() { }
void CEnginePrimitiveManager::v04() { }
void CEnginePrimitiveManager::v05() { }
void CEnginePrimitiveManager::v06() { }

static CEngineInternalPrimitiveBase* g_target[8];
static int g_childIndex(CEngineInternalPrimitiveBase* p);
CEngineInternalPrimitiveBase* CEngineInternalPrimitiveBase::GetOcclusionPrimitive() { return g_target[g_childIndex(this)]; }
static int g_infoDestroyed; static int g_infoCreated;
CEnginePrimitiveOcclusionInfo::CEnginePrimitiveOcclusionInfo() { RefCount = 1; g_infoCreated++; }
void CEnginePrimitiveOcclusionInfo::v000() { }
void CEnginePrimitiveOcclusionInfo::Destroy() { g_infoDestroyed++; }
static long g_ret[8]; static int g_calls; static long g_lastLayer;
long CEnginePrimitiveManager::RenderPrimitive(CEngineInternalPrimitiveBase& p, long layer, CEngineSubPrimitiveChain&) { g_calls++; g_lastLayer = layer; return g_ret[g_childIndex(&p)]; }
static CEnginePrimitiveManager g_mgr;
static CEnginePrimitiveTypeInfo g_info;
CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo = &g_info;
static CEngineRenderSettings g_settings;
CEngineRenderSettings* g_pEngineRenderSettings = &g_settings;

static CEngineInternalPrimitiveBase g_child[4];
static CEngineInternalPrimitiveBase g_tgt[4];
static int g_childIndex(CEngineInternalPrimitiveBase* p) { for (int i = 0; i < 4; ++i) if (p == &g_child[i]) return i; return 7; }

static int fails;
static void check(int c, const char* w) { if (!c) { fails++; printf("FAIL %s\n", w); } }

int main()
{
    CEnginePrimitiveGroupManager mgr; CEngineSubPrimitiveChain chain;
    CEngineInternalPrimitiveBase group;
    CEnginePrimitiveHandle handles[5];
    g_info.PrimitiveManagers[2] = &g_mgr;
    for (int i = 0; i < 4; ++i) { g_child[i].Type = 2; g_child[i].RenderLayerMask = 1; g_child[i].Enable = 1; g_tgt[i].OcclusionInfo[0].Object = 0; g_tgt[i].OcclusionInfo[1].Object = 0; }
    g_child[1].RenderLayerMask = 2;      // filtered by layer
    g_child[2].Enable = 0;               // disabled
    handles[0].Data = &g_child[0]; handles[1].Data = 0; handles[2].Data = &g_child[1]; handles[3].Data = &g_child[2]; handles[4].Data = &g_child[3];
    group.Children.m_pBegin = handles; group.Children.m_pEnd = handles + 5; group.Children.m_pCapacity = handles + 5;
    group.EnginePrimitiveFlags = 0; group.OcclusionInfo[0].Object = 0; group.OcclusionInfo[1].Object = 0;
    g_ret[0] = 3; g_ret[3] = 7;
    // 1: no occlusion pass -> render children 0 and 3, max result
    g_settings.CurrentOcclusionLayer = -1; g_calls = 0;
    long r = mgr.RenderPrimitive(group, 1, chain);
    check(r == 7 && g_calls == 2 && g_lastLayer == 1 && g_infoCreated == 0, "no occlusion");
    // 2: occlusion layer 1, group gets info, child 0's target shares it, child 3 has no target
    g_settings.CurrentOcclusionLayer = 1; g_calls = 0; g_target[0] = &g_tgt[0]; g_target[3] = 0;
    g_ret[0] = -5; g_ret[3] = -2;
    r = mgr.RenderPrimitive(group, 1, chain);
    CEnginePrimitiveOcclusionInfo* info = group.OcclusionInfo[1].Object;
    check(r == 0 && g_calls == 2 && g_infoCreated == 1 && info && g_tgt[0].OcclusionInfo[1].Object == info && info->RefCount == 2, "occlusion share");
    // 3: again: info present (not recreated), target already shares it (same pointer, no change)
    g_calls = 0; g_ret[0] = 4; g_ret[3] = 1;
    r = mgr.RenderPrimitive(group, 1, chain);
    check(r == 4 && g_infoCreated == 1 && info->RefCount == 2, "occlusion same");
    check(g_calls == 2, "occlusion same calls");
    // 4: target holds a different info with count 1 -> released and destroyed, then shares group's
    CEnginePrimitiveOcclusionInfo other; other.RefCount = 1; g_tgt[3].OcclusionInfo[1].Object = &other; g_target[3] = &g_tgt[3];
    r = mgr.RenderPrimitive(group, 1, chain);
    check(g_infoDestroyed == 1 && g_tgt[3].OcclusionInfo[1].Object == info && info->RefCount == 3, "occlusion replace");
    // 5: target holds a shared info (count 2) -> released, not destroyed
    CEnginePrimitiveOcclusionInfo other2; other2.RefCount = 2; g_tgt[3].OcclusionInfo[1].Object = &other2;
    r = mgr.RenderPrimitive(group, 1, chain);
    check(g_infoDestroyed == 1 && other2.RefCount == 1 && g_tgt[3].OcclusionInfo[1].Object == info && info->RefCount == 4, "occlusion release shared");
    // 6: flag 0x4000 on a fresh layer -> no info created; target gets the null info, its old one released
    group.EnginePrimitiveFlags = 0x4000; g_settings.CurrentOcclusionLayer = 0; g_target[3] = 0;
    CEnginePrimitiveOcclusionInfo other3; other3.RefCount = 1; g_tgt[0].OcclusionInfo[0].Object = &other3;
    int createdBefore = g_infoCreated;
    r = mgr.RenderPrimitive(group, 1, chain);
    check(g_infoCreated == createdBefore && group.OcclusionInfo[0].Object == 0 && g_tgt[0].OcclusionInfo[0].Object == 0 && g_infoDestroyed == 2, "no-occlusion flag");
    // 7: empty group
    group.Children.m_pEnd = handles; g_calls = 0;
    r = mgr.RenderPrimitive(group, 1, chain);
    check(r == 0 && g_calls == 0, "empty");
    if (!fails) printf("00bc6340_TEST PASS\n");
    return fails;
}
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
// Behaviour test for 0x00B23C80 CEngine::SetAttachedPrimitive: the forwarder must call CEnginePrimitiveAttachmentManager::SetAttachedPrimitive on the published component
// pointer with every argument unchanged and return the callee's result.
#include <cstdio>
class CEnginePrimitive { public: int v; };
class CEnginePrimitiveHandle { public: int v; };
class CEnginePrimitiveAttachmentInfo { public: int v; };
static int gCalls = 0;
static const void* gThis = 0;
static const void* gPtr[8];
static double gVal[8];
class CEnginePrimitiveAttachmentManager { public:
    void SetAttachedPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, const CEnginePrimitiveAttachmentInfo& attachmentInfo, const CEnginePrimitiveHandle& parentHandle, long attachIndex, unsigned long flags)
    {
        ++gCalls; gThis = this;
        gPtr[0] = &handle;
        gPtr[1] = &primitive;
        gPtr[2] = &attachmentInfo;
        gPtr[3] = &parentHandle;
        gVal[4] = (double)attachIndex;
        gVal[5] = (double)flags;
    }
};
static CEnginePrimitiveAttachmentManager gComponent;
CEnginePrimitiveAttachmentManager* g_pPrimitiveAttachmentManager_01436e70 = &gComponent;
class CEngine { public:
    virtual void SetAttachedPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, const CEnginePrimitiveAttachmentInfo& attachmentInfo, const CEnginePrimitiveHandle& parentHandle, long attachIndex, unsigned long flags);
};
void CEngine::SetAttachedPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, const CEnginePrimitiveAttachmentInfo& attachmentInfo, const CEnginePrimitiveHandle& parentHandle, long attachIndex, unsigned long flags)
{
    g_pPrimitiveAttachmentManager_01436e70->SetAttachedPrimitive(handle, primitive, attachmentInfo, parentHandle, attachIndex, flags);
}
int main()
{
    CEnginePrimitiveHandle o0;
    CEnginePrimitive o1;
    CEnginePrimitiveAttachmentInfo o2;
    CEnginePrimitiveHandle o3;
    CEngine engine;
    engine.SetAttachedPrimitive(o0, o1, o2, o3, -11, 45u);
    if (gCalls != 1 || gThis != &gComponent || gPtr[0] != &o0 || gPtr[1] != &o1 || gPtr[2] != &o2 || gPtr[3] != &o3 || gVal[4] != -11.0 || gVal[5] != 45.0) {
        std::printf("00b23c80_TEST FAIL\n");
        return 1;
    }
    std::printf("00b23c80_TEST PASS\n");
    return 0;
}

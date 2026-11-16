// CEngine::Uninitialise() -- unregister the device-reset callback, clear the D3D stream/index/texture bindings, delete every engine component (popping from the back), the scratch Z buffers and the render layers, then chain to CInitBaseClass::Uninitialise and clear IsInitialised
// Leaf: Uninitialise
#include <string.h>

// NF45 (round 7): the render core 0x01436E18 is NEngineInternal::GPRenderManager (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderManagerCore at offset 0). The local
//   CRenderManagerCore / IDirect3DDevice9 / CRenderManager models are the shared ones: the stream / index / texture unbinds are the
//   Ego_r.pdb inline members SelectVertexBuffer(0, 0) (SetStreamSource slot 100), SelectIndexBuffer(0, 0) (PSelectedIndexBuffer
//   +0x3CF4, SetIndices slot 104) and AttachTextureToStage(stage, 0) (repo FORCEINLINE_HELPERS: CurrentlySelectedTextures +0x3D00,
//   TextureStagesInUse +0x3CB8, SetTexture slot 65). The 0x01436E78 store is NEngineInternal::GPPrimitiveRendererSharedData
//   (?GPPrimitiveRendererSharedData@NEngineInternal@@3PAVCEnginePrimitiveRendererSharedData@@A; was g_pEngineComponentGlobal).
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#include "fable_compiler.h"
#include "engine/CRenderManager.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
class CDeviceResetCallback { public:
    virtual void OnPreDeviceReset() = 0;
    virtual bool OnPostDeviceReset() = 0;
};
class CDisplayManager { public:
    void RemoveDeviceResetCallback(CDeviceResetCallback*a0);   // 0x009BF5A0 (Ego_r.pdb ?RemoveDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z)
};
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }   // 0x01436E1C ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A
class CEnginePrimitiveRendererSharedData;
namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }   // 0x01436E78
extern void* g_pEngineGlobal_01436eb0;              // 0x01436EB0

template <class T> class CArray { public:                  // STLport vector<T*> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    bool empty() const { return size() == 0; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    T& back() { return *(begin() + (size() - 1)); }
    void pop_back() { --_M_finish; }
    static T* copy(T* first, T* last, T* result)
    {
        if (first == last)
            return result;
        unsigned long n = (char*)last - (char*)first;
        return (T*)((char*)memmove(result, first, n) + n);
    }
    T* erase(T* first, T* last)
    {
        T* i = copy(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};

struct CDeletable { virtual ~CDeletable(); };
class CSurface : public CDeletable { public: };
class CEngineLayerRenderer : public CDeletable { public: };
class CEngineComponent : public CDeletable { public: };

class CInitBaseClass {
public:
    virtual ~CInitBaseClass();
    bool Valid;                                     // +0x04
    virtual void Uninitialise();                    // 0x0099A360
};
class CIEngine : public CInitBaseClass {
public:
    bool Active;                                    // +0x08
};
class CEngine : public CIEngine, public CDeviceResetCallback {   // CDeviceResetCallback at +0x0c
public:
    unsigned char _pad10[0x84 - 0x10];
    bool          IsInitialised;                    // +0x84
    unsigned char _pad85[0x14c - 0x85];
    CArray<CSurface*>             ScratchZBuffersD24S8;   // +0x14c
    long                          AnisotropicFilteringLevel;   // +0x158
    CArray<CEngineLayerRenderer*> RenderLayers;     // +0x15c
    CArray<CEngineComponent*>     EngineComponents; // +0x168
    virtual void Uninitialise();
};

void CEngine::Uninitialise()
{
    if (!IsInitialised)
        return;
    NEngineInternal::GPDisplayManager->RemoveDeviceResetCallback(this);
    NEngineInternal::GPRenderManager->SelectVertexBuffer(0, 0);
    NEngineInternal::GPRenderManager->SelectIndexBuffer(0, 0);
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, 0);
    NEngineInternal::GPRenderManager->AttachTextureToStage(1, 0);
    NEngineInternal::GPRenderManager->AttachTextureToStage(2, 0);
    NEngineInternal::GPRenderManager->AttachTextureToStage(3, 0);
    while (!EngineComponents.empty()) {
        unsigned long count = EngineComponents.size();
        CEngineComponent* component = EngineComponents.begin()[count - 1];
        EngineComponents.pop_back();
        if (component)
            delete component;
    }
    NEngineInternal::GPPrimitiveRendererSharedData = 0;
    for (unsigned long i = 0; i < ScratchZBuffersD24S8.size(); ++i)
        delete ScratchZBuffersD24S8[i];
    ScratchZBuffersD24S8.clear();
    for (CEngineLayerRenderer** it = RenderLayers.begin(); it != RenderLayers.end(); ++it) {
        delete *it;
        *it = 0;
    }
    RenderLayers.clear();
    g_pEngineGlobal_01436eb0 = 0;
    CInitBaseClass::Uninitialise();
    IsInitialised = false;
}
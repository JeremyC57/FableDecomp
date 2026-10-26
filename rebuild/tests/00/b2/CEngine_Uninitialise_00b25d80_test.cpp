// Behaviour test for 00b25d80: final.cpp copied verbatim, every out-of-line callee stubbed, a concrete D3D device and
// counting deletable objects so the shutdown order, the three delete loops and the flag/global resets are observable.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __stdcall
#define __forceinline inline
#define long int
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

// ---- final.cpp verbatim ----
// CEngine::Uninitialise() -- unregister the device-reset callback, clear the D3D stream/index/texture bindings, delete every engine component (popping from the back), the scratch Z buffers and the render layers, then chain to CInitBaseClass::Uninitialise and clear IsInitialised
// Leaf: Uninitialise
#include <string.h>

struct IDirect3DBaseTexture9;
struct IDirect3DDevice9 {
    virtual void __stdcall Slot000(); virtual void __stdcall Slot001(); virtual void __stdcall Slot002(); virtual void __stdcall Slot003();
    virtual void __stdcall Slot004(); virtual void __stdcall Slot005(); virtual void __stdcall Slot006(); virtual void __stdcall Slot007();
    virtual void __stdcall Slot008(); virtual void __stdcall Slot009(); virtual void __stdcall Slot010(); virtual void __stdcall Slot011();
    virtual void __stdcall Slot012(); virtual void __stdcall Slot013(); virtual void __stdcall Slot014(); virtual void __stdcall Slot015();
    virtual void __stdcall Slot016(); virtual void __stdcall Slot017(); virtual void __stdcall Slot018(); virtual void __stdcall Slot019();
    virtual void __stdcall Slot020(); virtual void __stdcall Slot021(); virtual void __stdcall Slot022(); virtual void __stdcall Slot023();
    virtual void __stdcall Slot024(); virtual void __stdcall Slot025(); virtual void __stdcall Slot026(); virtual void __stdcall Slot027();
    virtual void __stdcall Slot028(); virtual void __stdcall Slot029(); virtual void __stdcall Slot030(); virtual void __stdcall Slot031();
    virtual void __stdcall Slot032(); virtual void __stdcall Slot033(); virtual void __stdcall Slot034(); virtual void __stdcall Slot035();
    virtual void __stdcall Slot036(); virtual void __stdcall Slot037(); virtual void __stdcall Slot038(); virtual void __stdcall Slot039();
    virtual void __stdcall Slot040(); virtual void __stdcall Slot041(); virtual void __stdcall Slot042(); virtual void __stdcall Slot043();
    virtual void __stdcall Slot044(); virtual void __stdcall Slot045(); virtual void __stdcall Slot046(); virtual void __stdcall Slot047();
    virtual void __stdcall Slot048(); virtual void __stdcall Slot049(); virtual void __stdcall Slot050(); virtual void __stdcall Slot051();
    virtual void __stdcall Slot052(); virtual void __stdcall Slot053(); virtual void __stdcall Slot054(); virtual void __stdcall Slot055();
    virtual void __stdcall Slot056(); virtual void __stdcall Slot057(); virtual void __stdcall Slot058(); virtual void __stdcall Slot059();
    virtual void __stdcall Slot060(); virtual void __stdcall Slot061(); virtual void __stdcall Slot062(); virtual void __stdcall Slot063();
    virtual void __stdcall Slot064();
    virtual long __stdcall SetTexture(unsigned long stage, IDirect3DBaseTexture9* texture) = 0;   // slot 65 (+0x104)
    virtual void __stdcall Slot066(); virtual void __stdcall Slot067(); virtual void __stdcall Slot068(); virtual void __stdcall Slot069();
    virtual void __stdcall Slot070(); virtual void __stdcall Slot071(); virtual void __stdcall Slot072(); virtual void __stdcall Slot073();
    virtual void __stdcall Slot074(); virtual void __stdcall Slot075(); virtual void __stdcall Slot076(); virtual void __stdcall Slot077();
    virtual void __stdcall Slot078(); virtual void __stdcall Slot079(); virtual void __stdcall Slot080(); virtual void __stdcall Slot081();
    virtual void __stdcall Slot082(); virtual void __stdcall Slot083(); virtual void __stdcall Slot084(); virtual void __stdcall Slot085();
    virtual void __stdcall Slot086(); virtual void __stdcall Slot087(); virtual void __stdcall Slot088(); virtual void __stdcall Slot089();
    virtual void __stdcall Slot090(); virtual void __stdcall Slot091(); virtual void __stdcall Slot092(); virtual void __stdcall Slot093();
    virtual void __stdcall Slot094(); virtual void __stdcall Slot095(); virtual void __stdcall Slot096(); virtual void __stdcall Slot097();
    virtual void __stdcall Slot098(); virtual void __stdcall Slot099();
    virtual long __stdcall SetStreamSource(unsigned int stream, void* data, unsigned int offset, unsigned int stride) = 0;   // slot 100 (+0x190)
    virtual void __stdcall Slot101(); virtual void __stdcall Slot102(); virtual void __stdcall Slot103();
    virtual long __stdcall SetIndices(void* indexData) = 0;   // slot 104 (+0x1a0)
};

class CRenderManagerCore {
public:
    unsigned char          _pad0[0x3cb8];
    long                   NumTextureStagesUsed;     // +0x3cb8
    unsigned char          _pad3cbc[0x3cf0 - 0x3cbc];
    IDirect3DDevice9*      PD3DDevice;               // +0x3cf0
    void*                  CurrentIndices;           // +0x3cf4
    unsigned char          _pad3cf8[8];
    IDirect3DBaseTexture9* CurrentTextures[4];       // +0x3d00
    void SetIndices(void* indices)
    {
        CurrentIndices = indices;
        PD3DDevice->SetIndices(indices);
    }
    void SetTexture(long stage, IDirect3DBaseTexture9* texture)
    {
        if (CurrentTextures[stage] != texture) {
            CurrentTextures[stage] = texture;
            PD3DDevice->SetTexture(stage, texture);
        }
        if (NumTextureStagesUsed < stage + 1)
            NumTextureStagesUsed = stage + 1;
    }
};
extern CRenderManagerCore* g_pRenderManagerCore;   // 0x01436E18

class CDeviceResetCallback { public:
    virtual void OnPreDeviceReset() = 0;
    virtual bool OnPostDeviceReset() = 0;
};
class CRenderManager {
public:
    void RemoveDeviceResetCallback(CDeviceResetCallback* callback);   // 0x009BF5A0
};
extern CRenderManager* g_pRenderManager;            // 0x01436E1C
extern void* g_pEngineComponentGlobal;              // 0x01436E78
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
    g_pRenderManager->RemoveDeviceResetCallback(this);
    g_pRenderManagerCore->PD3DDevice->SetStreamSource(0, 0, 0, 0);
    g_pRenderManagerCore->SetIndices(0);
    g_pRenderManagerCore->SetTexture(0, 0);
    g_pRenderManagerCore->SetTexture(1, 0);
    g_pRenderManagerCore->SetTexture(2, 0);
    g_pRenderManagerCore->SetTexture(3, 0);
    while (!EngineComponents.empty()) {
        unsigned long count = EngineComponents.size();
        CEngineComponent* component = EngineComponents.begin()[count - 1];
        EngineComponents.pop_back();
        if (component)
            delete component;
    }
    g_pEngineComponentGlobal = 0;
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

// ---- stubs ----
void __stdcall IDirect3DDevice9::Slot000() {}
void __stdcall IDirect3DDevice9::Slot001() {}
void __stdcall IDirect3DDevice9::Slot002() {}
void __stdcall IDirect3DDevice9::Slot003() {}
void __stdcall IDirect3DDevice9::Slot004() {}
void __stdcall IDirect3DDevice9::Slot005() {}
void __stdcall IDirect3DDevice9::Slot006() {}
void __stdcall IDirect3DDevice9::Slot007() {}
void __stdcall IDirect3DDevice9::Slot008() {}
void __stdcall IDirect3DDevice9::Slot009() {}
void __stdcall IDirect3DDevice9::Slot010() {}
void __stdcall IDirect3DDevice9::Slot011() {}
void __stdcall IDirect3DDevice9::Slot012() {}
void __stdcall IDirect3DDevice9::Slot013() {}
void __stdcall IDirect3DDevice9::Slot014() {}
void __stdcall IDirect3DDevice9::Slot015() {}
void __stdcall IDirect3DDevice9::Slot016() {}
void __stdcall IDirect3DDevice9::Slot017() {}
void __stdcall IDirect3DDevice9::Slot018() {}
void __stdcall IDirect3DDevice9::Slot019() {}
void __stdcall IDirect3DDevice9::Slot020() {}
void __stdcall IDirect3DDevice9::Slot021() {}
void __stdcall IDirect3DDevice9::Slot022() {}
void __stdcall IDirect3DDevice9::Slot023() {}
void __stdcall IDirect3DDevice9::Slot024() {}
void __stdcall IDirect3DDevice9::Slot025() {}
void __stdcall IDirect3DDevice9::Slot026() {}
void __stdcall IDirect3DDevice9::Slot027() {}
void __stdcall IDirect3DDevice9::Slot028() {}
void __stdcall IDirect3DDevice9::Slot029() {}
void __stdcall IDirect3DDevice9::Slot030() {}
void __stdcall IDirect3DDevice9::Slot031() {}
void __stdcall IDirect3DDevice9::Slot032() {}
void __stdcall IDirect3DDevice9::Slot033() {}
void __stdcall IDirect3DDevice9::Slot034() {}
void __stdcall IDirect3DDevice9::Slot035() {}
void __stdcall IDirect3DDevice9::Slot036() {}
void __stdcall IDirect3DDevice9::Slot037() {}
void __stdcall IDirect3DDevice9::Slot038() {}
void __stdcall IDirect3DDevice9::Slot039() {}
void __stdcall IDirect3DDevice9::Slot040() {}
void __stdcall IDirect3DDevice9::Slot041() {}
void __stdcall IDirect3DDevice9::Slot042() {}
void __stdcall IDirect3DDevice9::Slot043() {}
void __stdcall IDirect3DDevice9::Slot044() {}
void __stdcall IDirect3DDevice9::Slot045() {}
void __stdcall IDirect3DDevice9::Slot046() {}
void __stdcall IDirect3DDevice9::Slot047() {}
void __stdcall IDirect3DDevice9::Slot048() {}
void __stdcall IDirect3DDevice9::Slot049() {}
void __stdcall IDirect3DDevice9::Slot050() {}
void __stdcall IDirect3DDevice9::Slot051() {}
void __stdcall IDirect3DDevice9::Slot052() {}
void __stdcall IDirect3DDevice9::Slot053() {}
void __stdcall IDirect3DDevice9::Slot054() {}
void __stdcall IDirect3DDevice9::Slot055() {}
void __stdcall IDirect3DDevice9::Slot056() {}
void __stdcall IDirect3DDevice9::Slot057() {}
void __stdcall IDirect3DDevice9::Slot058() {}
void __stdcall IDirect3DDevice9::Slot059() {}
void __stdcall IDirect3DDevice9::Slot060() {}
void __stdcall IDirect3DDevice9::Slot061() {}
void __stdcall IDirect3DDevice9::Slot062() {}
void __stdcall IDirect3DDevice9::Slot063() {}
void __stdcall IDirect3DDevice9::Slot064() {}
void __stdcall IDirect3DDevice9::Slot066() {}
void __stdcall IDirect3DDevice9::Slot067() {}
void __stdcall IDirect3DDevice9::Slot068() {}
void __stdcall IDirect3DDevice9::Slot069() {}
void __stdcall IDirect3DDevice9::Slot070() {}
void __stdcall IDirect3DDevice9::Slot071() {}
void __stdcall IDirect3DDevice9::Slot072() {}
void __stdcall IDirect3DDevice9::Slot073() {}
void __stdcall IDirect3DDevice9::Slot074() {}
void __stdcall IDirect3DDevice9::Slot075() {}
void __stdcall IDirect3DDevice9::Slot076() {}
void __stdcall IDirect3DDevice9::Slot077() {}
void __stdcall IDirect3DDevice9::Slot078() {}
void __stdcall IDirect3DDevice9::Slot079() {}
void __stdcall IDirect3DDevice9::Slot080() {}
void __stdcall IDirect3DDevice9::Slot081() {}
void __stdcall IDirect3DDevice9::Slot082() {}
void __stdcall IDirect3DDevice9::Slot083() {}
void __stdcall IDirect3DDevice9::Slot084() {}
void __stdcall IDirect3DDevice9::Slot085() {}
void __stdcall IDirect3DDevice9::Slot086() {}
void __stdcall IDirect3DDevice9::Slot087() {}
void __stdcall IDirect3DDevice9::Slot088() {}
void __stdcall IDirect3DDevice9::Slot089() {}
void __stdcall IDirect3DDevice9::Slot090() {}
void __stdcall IDirect3DDevice9::Slot091() {}
void __stdcall IDirect3DDevice9::Slot092() {}
void __stdcall IDirect3DDevice9::Slot093() {}
void __stdcall IDirect3DDevice9::Slot094() {}
void __stdcall IDirect3DDevice9::Slot095() {}
void __stdcall IDirect3DDevice9::Slot096() {}
void __stdcall IDirect3DDevice9::Slot097() {}
void __stdcall IDirect3DDevice9::Slot098() {}
void __stdcall IDirect3DDevice9::Slot099() {}
void __stdcall IDirect3DDevice9::Slot101() {}
void __stdcall IDirect3DDevice9::Slot102() {}
void __stdcall IDirect3DDevice9::Slot103() {}
static int g_setTexture = 0, g_setStream = 0, g_setIndices = 0, g_removeCallback = 0, g_baseUninit = 0, g_deleted = 0;
struct CTestDevice : IDirect3DDevice9 {
    long __stdcall SetTexture(unsigned long stage, IDirect3DBaseTexture9* texture) { ++g_setTexture; return 0; }
    long __stdcall SetStreamSource(unsigned int stream, void* data, unsigned int offset, unsigned int stride) { ++g_setStream; return 0; }
    long __stdcall SetIndices(void* indexData) { ++g_setIndices; return 0; }
};
void CRenderManager::RemoveDeviceResetCallback(CDeviceResetCallback* callback) { ++g_removeCallback; }
CDeletable::~CDeletable() { ++g_deleted; }
CInitBaseClass::~CInitBaseClass() {}
void CInitBaseClass::Uninitialise() { ++g_baseUninit; Valid = false; }
static CTestDevice g_device;
static CRenderManagerCore g_core;
static CRenderManager g_manager;
CRenderManagerCore* g_pRenderManagerCore = &g_core;
CRenderManager* g_pRenderManager = &g_manager;
void* g_pEngineComponentGlobal = 0;
void* g_pEngineGlobal_01436eb0 = 0;
struct CTestEngine : CEngine {
    void OnPreDeviceReset() {}
    bool OnPostDeviceReset() { return true; }
};
template <class T> static void Fill(CArray<T*>& a, T** storage, int n)
{
    a._M_start = storage; a._M_finish = storage + n; a._M_end_of_storage = storage + n;
}
int main()
{
    static CTestEngine engine;
    static CEngineComponent* components[4]; static CSurface* surfaces[3]; static CEngineLayerRenderer* layers[2];
    components[0] = new CEngineComponent; components[1] = 0; components[2] = new CEngineComponent; components[3] = new CEngineComponent;
    for (int i = 0; i < 3; ++i) surfaces[i] = new CSurface;
    for (int i = 0; i < 2; ++i) layers[i] = new CEngineLayerRenderer;
    Fill(engine.EngineComponents, components, 4); Fill(engine.ScratchZBuffersD24S8, surfaces, 3); Fill(engine.RenderLayers, layers, 2);
    g_core.PD3DDevice = &g_device; g_core.NumTextureStagesUsed = 0; g_core.CurrentIndices = (void*)1;
    for (int i = 0; i < 4; ++i) g_core.CurrentTextures[i] = (IDirect3DBaseTexture9*)(i == 2 ? 0 : (void*)(size_t)(i + 1));
    g_pEngineComponentGlobal = (void*)1; g_pEngineGlobal_01436eb0 = (void*)1;
    engine.IsInitialised = false; engine.Valid = true;
    engine.Uninitialise();                      // not initialised: nothing happens
    CHECK(g_removeCallback == 0 && g_deleted == 0 && g_pEngineComponentGlobal == (void*)1);
    engine.IsInitialised = true;
    engine.Uninitialise();
    CHECK(g_removeCallback == 1 && g_setStream == 1 && g_setIndices == 1 && g_core.CurrentIndices == 0);
    CHECK(g_setTexture == 3 && g_core.NumTextureStagesUsed == 4);     // stage 2 was already null, the other three are reset
    CHECK(g_deleted == 3 + 3 + 2);                                      // three components (one null skipped), three surfaces, two layers
    CHECK(engine.EngineComponents.empty() && engine.ScratchZBuffersD24S8.size() == 0 && engine.RenderLayers.size() == 0);
    CHECK(layers[0] == 0 && layers[1] == 0);                            // the layer slots are nulled before the clear
    CHECK(g_pEngineComponentGlobal == 0 && g_pEngineGlobal_01436eb0 == 0 && g_baseUninit == 1 && !engine.Valid && !engine.IsInitialised);
    if (fails) { printf("00b25d80_TEST FAIL (%d)\n", fails); return 1; }
    printf("00b25d80_TEST PASS\n");
    return 0;
}
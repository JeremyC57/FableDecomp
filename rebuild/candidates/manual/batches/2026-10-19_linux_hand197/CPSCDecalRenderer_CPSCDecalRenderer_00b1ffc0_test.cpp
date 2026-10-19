#include <cstdio>
#include <cstring>
// Behaviour test for 00b1ffc0 ~CPSCDecalRenderer: the mesh handle (declared last) dies first; a handle that holds the only
// reference tells its primitive (upper-case) before the release destroys it (lower-case); a shared primitive is only
// released; each handle's base and finally the component base are destroyed.
static char g_log[32]; static int g_n = 0;
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    int id;
    virtual void v000() {}
    virtual void Destroy() { g_log[g_n++] = (char)('a' + id); }
    virtual void v002() {}
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle) { g_log[g_n++] = (char)('A' + id); }
    long RefCount;                                                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {                          // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    void ReleaseRef() { if (Object) { Object->Release(); Object = 0; } }
    CIVCountedPointer& operator=(int) { ReleaseRef(); Object = 0; return *this; }
    ~CIVCountedPointer() { ReleaseRef(); }
};
class CBaseClass {                                 // vtable 0x01231710
public:
    virtual void v000() {}
    ~CBaseClass() { g_log[g_n++] = '#'; }
};
class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001() {}
    void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    ~CEnginePrimitiveHandle() { Clear(); }
};
class CBaseClassNonCopyable {
public:
    virtual void v000() {}
    ~CBaseClassNonCopyable() { g_log[g_n++] = 'N'; }
};
namespace NParticleEngine {
class CComponent : public CBaseClassNonCopyable {
public:
    unsigned char _pad04[8];
};
}
class CPSCDecalRenderer : public NParticleEngine::CComponent {
public:
    float                  DecalLifeSecs;          // +0x0c
    long                   DecalBankIndex;         // +0x10
    unsigned long          StartColour;            // +0x14
    unsigned long          MidColour;              // +0x18
    unsigned long          EndColour;              // +0x1c
    unsigned char          _bits20[0x14];          // bit-field words +0x20..+0x33
    unsigned char          _flags34[4];            // AlphaFadeEnable.. +0x34
    float                  MaxPoolSize;            // +0x38
    float                  MaxPoolAlpha;           // +0x3c
    float                  MaxPoolLife;            // +0x40
    float                  PoolFrameIncreaseRate;  // +0x44
    float                  StencilCubeWidth;       // +0x48
    CEnginePrimitiveHandle LandscapeDecalsPrimHandle;   // +0x4c
    long                   LandscapeDecalsChildPrimIndex;   // +0x54
    CEnginePrimitiveHandle MeshDecalsPrimHandle;   // +0x58
    long                   MeshDecalsChildPrimIndex;        // +0x60
    ~CPSCDecalRenderer();
};

CPSCDecalRenderer::~CPSCDecalRenderer()
{
}

int main()
{
    CEnginePersistentPrimitiveBase landscape, mesh;
    landscape.id = 0; landscape.RefCount = 2;      // shared: no notification, no destroy
    mesh.id = 1; mesh.RefCount = 1;                // unique: notified, then destroyed
    {
        CPSCDecalRenderer* r = new CPSCDecalRenderer;
        r->LandscapeDecalsPrimHandle.Data.Object = &landscape;
        r->MeshDecalsPrimHandle.Data.Object = &mesh;
        g_n = 0;
        r->~CPSCDecalRenderer();
        operator delete(r);
    }
    g_log[g_n] = 0;
    bool ok = std::strcmp(g_log, "Bb##N") == 0 && landscape.RefCount == 1 && mesh.RefCount == 0;
    if (!ok) { std::printf("00b1ffc0_TEST FAIL %s\n", g_log); return 1; }
    std::printf("00b1ffc0_TEST PASS\n");
    return 0;
}

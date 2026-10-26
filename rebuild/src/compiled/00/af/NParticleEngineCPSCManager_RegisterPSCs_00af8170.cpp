// NParticleEngine::CPSCManager::RegisterPSCs() -- registers every particle-system component class (key name, Alloc factory, component type, GUI name) in the manager's vector map
// Leaf: RegisterPSCs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF8170, length 1051 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCharString::CCharString(const CCharString&)
// @ 0x0099EC30, CCharString::~CCharString @ 0x0099EAE0, CVectorMap<CCharString, CRegisteredPSCInfo>::Add @ 0x00AF2910
// (label correction: manifest says CTCSkeletalMorph::AddSkeletalMorph, propagated; see the landed 0x00AF2910 source).
// Alloc factories: CPSCRenderSprite 0x00AF7D10, CPSCRenderMesh 0x00AF7D80, CPSCSingleSprite 0x00AF7DF0,
// CPSCUpdateNormal 0x00AF7E60, CPSCEmitterGeneric 0x00AF7ED0, CPSCOrbit 0x00AF7F40, CPSCAttractor 0x00AF7FB0,
// CPSCLight 0x00AF8020, CPSCSpline 0x00AF8090, CPSCDecalRenderer 0x00AF8100 (manifest labels them
// CEnginePrimitive2DViewportManager::CreateInternalPrimitive -- propagated, wrong).
// Layout (Ego_r.pdb): CRegisteredPSCInfo = { AllocFunc +0, EPSCType ComponentType +4, CCharString GUIName +8 } (12 bytes).
// Shape: one block per class with three NAMED locals -- the GUI-name string, the info (inline ctor: AllocFunc,
// ComponentType, then the GUIName copy), then the key string -- so both Add arguments are pushed with `lea`
// (temporaries pass the ctor's eax instead) and the destructors run key, info.GUIName, GUI-name. Temporaries in the
// call (explicit or implicit const char* conversions) build the strings in the wrong order. The repeated type
// constants 1 and 2 are cached in edi by the compiler.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

namespace NParticleEngine {

class CPSCBase;
typedef CPSCBase* (__cdecl *PSCAllocFunc)();

enum EPSCType { PSC_TYPE_0, PSC_TYPE_1, PSC_TYPE_2, PSC_TYPE_3, PSC_TYPE_4 };

class CPSCRenderSprite   { public: static CPSCBase* Alloc(); };   // 0x00AF7D10
class CPSCRenderMesh     { public: static CPSCBase* Alloc(); };   // 0x00AF7D80
class CPSCSingleSprite   { public: static CPSCBase* Alloc(); };   // 0x00AF7DF0
class CPSCUpdateNormal   { public: static CPSCBase* Alloc(); };   // 0x00AF7E60
class CPSCEmitterGeneric { public: static CPSCBase* Alloc(); };   // 0x00AF7ED0
class CPSCOrbit          { public: static CPSCBase* Alloc(); };   // 0x00AF7F40
class CPSCAttractor      { public: static CPSCBase* Alloc(); };   // 0x00AF7FB0
class CPSCLight          { public: static CPSCBase* Alloc(); };   // 0x00AF8020
class CPSCSpline         { public: static CPSCBase* Alloc(); };   // 0x00AF8090
class CPSCDecalRenderer  { public: static CPSCBase* Alloc(); };   // 0x00AF8100

class CRegisteredPSCInfo { public:
    PSCAllocFunc AllocFunc;        // +0x00
    EPSCType     ComponentType;    // +0x04
    CCharString  GUIName;          // +0x08
    CRegisteredPSCInfo(PSCAllocFunc alloc, EPSCType type, const CCharString& name)
        : AllocFunc(alloc), ComponentType(type), GUIName(name) { }
};

struct CRegisteredPSCMap {         // CVectorMap<CCharString, CRegisteredPSCInfo> (16 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    unsigned char Compare;
    bool Dirty;
    void Add(const CCharString& key, const CRegisteredPSCInfo& info);   // 0x00AF2910
};

class CPSCManager { public:
    CRegisteredPSCMap RegisteredPSCs;   // +0x00
    void RegisterPSCs();
};

void CPSCManager::RegisterPSCs()
{
    {
        CCharString guiName("Sprite Renderer");
        CRegisteredPSCInfo info(&CPSCRenderSprite::Alloc, PSC_TYPE_1, guiName);
        CCharString name("CPSCRenderSprite");
        RegisteredPSCs.Add(name, info);
    }
    {
        CCharString guiName("Mesh Renderer");
        CRegisteredPSCInfo info(&CPSCRenderMesh::Alloc, PSC_TYPE_1, guiName);
        CCharString name("CPSCRenderMesh");
        RegisteredPSCs.Add(name, info);
    }
    {
        CCharString guiName("Single Sprite");
        CRegisteredPSCInfo info(&CPSCSingleSprite::Alloc, PSC_TYPE_2, guiName);
        CCharString name("CPSCSingleSprite");
        RegisteredPSCs.Add(name, info);
    }
    {
        CCharString guiName("Generic Particle System");
        CRegisteredPSCInfo info(&CPSCUpdateNormal::Alloc, PSC_TYPE_4, guiName);
        CCharString name("CPSCUpdateNormal");
        RegisteredPSCs.Add(name, info);
    }
    {
        CCharString guiName("Generic Emitter");
        CRegisteredPSCInfo info(&CPSCEmitterGeneric::Alloc, PSC_TYPE_3, guiName);
        CCharString name("CPSCEmitterGeneric");
        RegisteredPSCs.Add(name, info);
    }
    {
        CCharString guiName("Orbit");
        CRegisteredPSCInfo info(&CPSCOrbit::Alloc, PSC_TYPE_2, guiName);
        CCharString name("CPSCOrbit");
        RegisteredPSCs.Add(name, info);
    }
    {
        CCharString guiName("Attractor");
        CRegisteredPSCInfo info(&CPSCAttractor::Alloc, PSC_TYPE_2, guiName);
        CCharString name("CPSCAttractor");
        RegisteredPSCs.Add(name, info);
    }
    {
        CCharString guiName("Light");
        CRegisteredPSCInfo info(&CPSCLight::Alloc, PSC_TYPE_2, guiName);
        CCharString name("CPSCLight");
        RegisteredPSCs.Add(name, info);
    }
    {
        CCharString guiName("Spline");
        CRegisteredPSCInfo info(&CPSCSpline::Alloc, PSC_TYPE_2, guiName);
        CCharString name("CPSCSpline");
        RegisteredPSCs.Add(name, info);
    }
    {
        CCharString guiName("Decal Renderer");
        CRegisteredPSCInfo info(&CPSCDecalRenderer::Alloc, PSC_TYPE_2, guiName);
        CCharString name("CPSCDecalRenderer");
        RegisteredPSCs.Add(name, info);
    }
}

}
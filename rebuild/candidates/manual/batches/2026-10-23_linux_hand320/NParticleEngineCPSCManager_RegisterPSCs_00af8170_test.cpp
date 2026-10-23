// Behaviour test for 0x00AF8170 NParticleEngine::CPSCManager::RegisterPSCs: ten registrations, each with the
// key name, Alloc factory, component type and GUI name; all string temporaries destroyed.
#include <cstdio>
#include <cstring>

static int g_live = 0;
struct CCharString {
    char m_text[32];
    CCharString(const char* text, long length = -1) { (void)length; std::strcpy(m_text, text); ++g_live; }
    CCharString(const CCharString& other) { std::strcpy(m_text, other.m_text); ++g_live; }
    ~CCharString() { --g_live; }
};

class CPSCBase;
typedef CPSCBase* (*PSCAllocFunc)();
static CPSCBase* AllocA() { return 0; }
static CPSCBase* AllocB() { return 0; }

struct CRegisteredPSCInfo {
    PSCAllocFunc AllocFunc;
    int ComponentType;
    CCharString GUIName;
    CRegisteredPSCInfo(PSCAllocFunc alloc, int type, const CCharString& name)
        : AllocFunc(alloc), ComponentType(type), GUIName(name) { }
};

struct Entry { char key[32]; char gui[32]; PSCAllocFunc alloc; int type; };
struct CRegisteredPSCMap {
    Entry e[16]; int n; bool Dirty;
    CRegisteredPSCMap() : n(0), Dirty(false) { }
    void Add(const CCharString& key, const CRegisteredPSCInfo& info)
    {
        std::strcpy(e[n].key, key.m_text); std::strcpy(e[n].gui, info.GUIName.m_text);
        e[n].alloc = info.AllocFunc; e[n].type = info.ComponentType; ++n; Dirty = true;
    }
};

struct CPSCManager {
    CRegisteredPSCMap RegisteredPSCs;
    void Reg(const char* key, PSCAllocFunc alloc, int type, const char* gui)
    {
        CCharString guiName(gui);
        CRegisteredPSCInfo info(alloc, type, guiName);
        CCharString name(key);
        RegisteredPSCs.Add(name, info);
    }
    void RegisterPSCs()
    {
        Reg("CPSCRenderSprite", AllocA, 1, "Sprite Renderer");
        Reg("CPSCRenderMesh", AllocB, 1, "Mesh Renderer");
        Reg("CPSCSingleSprite", AllocA, 2, "Single Sprite");
        Reg("CPSCUpdateNormal", AllocB, 4, "Generic Particle System");
        Reg("CPSCEmitterGeneric", AllocA, 3, "Generic Emitter");
        Reg("CPSCOrbit", AllocB, 2, "Orbit");
        Reg("CPSCAttractor", AllocA, 2, "Attractor");
        Reg("CPSCLight", AllocB, 2, "Light");
        Reg("CPSCSpline", AllocA, 2, "Spline");
        Reg("CPSCDecalRenderer", AllocB, 2, "Decal Renderer");
    }
};

int main()
{
    CPSCManager m;
    m.RegisterPSCs();
    const CRegisteredPSCMap& r = m.RegisteredPSCs;
    if (r.n != 10 || g_live != 0 || !r.Dirty
        || std::strcmp(r.e[0].key, "CPSCRenderSprite") != 0 || std::strcmp(r.e[0].gui, "Sprite Renderer") != 0
        || r.e[3].type != 4 || r.e[4].type != 3 || r.e[9].type != 2 || r.e[1].alloc != AllocB
        || std::strcmp(r.e[9].gui, "Decal Renderer") != 0) {
        std::printf("00af8170_TEST FAIL\n");
        return 1;
    }
    std::printf("00af8170_TEST PASS\n");
    return 0;
}

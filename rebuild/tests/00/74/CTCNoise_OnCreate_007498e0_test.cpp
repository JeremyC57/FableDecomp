// Behaviour test for 007498e0: src/007498e0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCNoise::OnCreate()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCNoise::OnCreate() -- copy the owning thing's noise definition into the component, take its hearing distance, and set the lifetime: a timed noise lasts LifeTime seconds (+5 frames), a single-shot one 5 frames
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007498E0, length 160 bytes. Vtable slot 4 of CTCNoise (0x01268A74; catalog "vfunc_4",
// the CTCBase OnCreate slot).
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef<CThingNoiseDef>(long, CDefPointer*) @ 0x00749AC0
// (catalog <CThingPhysicalSwitchDef> is a byte twin), CDefClassBase::operator= @ 0x00431F10 (catalog "CThingBuildingDef::operator="),
// the CTCBase frame-source accessor @ 0x00686D60 (called for a static frames-per-second read; result unused), __ftol2.
// Globals: frames per second (long) @ 0x01375550; 5.0f @ 0x012462A8.
// Layout: PDB CTCNoise (0x50) = retail: Definition +0x0C (CTCNoiseDef 0x34: CDefClassBase 0x28, Type +0x28, LifeTime +0x2C,
// DistanceCanBeHeardFrom +0x30), LifeTimer +0x40, DistanceCanBeHeardFrom +0x44; CThingNoiseDef Noise +0x70 (PDB +0x74); the thing's
// def index is the word at CThing+0x8C.
// Shape: a zeroed local CDefPointer filled by GetDef, its object taken into a pointer local and released by hand at the end
// (`test ebp ; dec [ebp+4] ; jne`, no clear); the implicit
// CTCNoiseDef operator= (base operator= then the three member copies); `switch` with case 0 / case 1.
class CThing { public:
    unsigned char _pad00[0x8c];
    unsigned short DefIndex;                    // +0x8c
};
class CDefClassBase { public:
    virtual void Destroy();
    virtual void Release();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x28 - 0x08];
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
class CTCNoiseDef : public CDefClassBase { public:            // 0x34
    long  Type;                                 // +0x28 ENoiseType
    float LifeTime;                             // +0x2c (seconds)
    float DistanceCanBeHeardFrom;               // +0x30
};
class CThingNoiseDef : public CDefClassBase { public:
    unsigned char _pad28[0x70 - 0x28];
    CTCNoiseDef Noise;                          // +0x70 (PDB +0x74)
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};
class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointer<CThingNoiseDef>* out) const;   // 0x00749AC0
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CThingNoiseDef>* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
extern long g_FramesPerSecond;                  // 0x01375550
struct CFrameSource {
    static long GetFramesPerSecond() { return g_FramesPerSecond; }
};
struct CIntelligentPointerConstThing {
    virtual void v00();
    const CThing* m_pItem;
};
class CTCNoise { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    CTCNoiseDef Definition;                     // +0x0c
    long  LifeTimer;                            // +0x40
    float DistanceCanBeHeardFrom;               // +0x44
    CIntelligentPointerConstThing Creator;      // +0x48
    const CDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    CFrameSource* GetFrameSource();             // 0x00686D60 (CTCBase)
    void OnCreate();
};

void CTCNoise::OnCreate()
{
    CDefPointer<CThingNoiseDef> def;
    GetDefinitionManager().GetDef(ParentThing->DefIndex, &def);
    CThingNoiseDef* noiseDef = def.Object;
    Definition = noiseDef->Noise;
    DistanceCanBeHeardFrom = Definition.DistanceCanBeHeardFrom;
    switch (Definition.Type) {
    case 0:
        LifeTimer = (long)(Definition.LifeTime * (float)GetFrameSource()->GetFramesPerSecond() + 5.0f);
        break;
    case 1:
        LifeTimer = 5;
        break;
    }
    if (noiseDef && --noiseDef->RefCount == 0)
        noiseDef->Release();
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_released = 0; static int g_assign = 0;
void* FindDef_CDefinitionManager_GetDef_0() { return g_def; }
void CDefClassBase::Destroy() {}
void CDefClassBase::Release() { ++g_released; }
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { ++g_assign; memcpy(_pad08, other._pad08, sizeof(_pad08)); return *this; }
long g_FramesPerSecond = 30;
static CDefinitionManager g_mgr; static CFrameSource g_fs;
const CDefinitionManager& CTCNoise::GetDefinitionManager() const { return g_mgr; }
CFrameSource* CTCNoise::GetFrameSource() { return &g_fs; }
void CTCNoise::v00() {}
void CIntelligentPointerConstThing::v00() {}
int main()
{
    CThingNoiseDef def; def.RefCount = 2; def.Noise.Type = 0; def.Noise.LifeTime = 2.0f; def.Noise.DistanceCanBeHeardFrom = 12.5f;
    memset(def.Noise._pad08, 0x5a, sizeof(def.Noise._pad08));
    g_def = &def;
    CThing thing; memset(&thing, 0, sizeof(thing)); thing.DefIndex = 3;
    CTCNoise n; n.ParentThing = &thing; n.LifeTimer = -1; n.DistanceCanBeHeardFrom = 0;
    n.OnCreate();
    CHECK(g_assign == 1); CHECK(n.Definition.Type == 0); CHECK(n.Definition.LifeTime == 2.0f); CHECK(n.DistanceCanBeHeardFrom == 12.5f);
    CHECK(n.Definition._pad08[0] == 0x5a); CHECK(n.LifeTimer == 65); CHECK(def.RefCount == 1); CHECK(g_released == 0);
    def.Noise.Type = 1; n.LifeTimer = -1; n.OnCreate(); CHECK(n.LifeTimer == 5); CHECK(def.RefCount == 0); CHECK(g_released == 1);
    def.RefCount = 4; def.Noise.Type = 2; n.LifeTimer = -1; n.OnCreate(); CHECK(n.LifeTimer == -1); CHECK(def.RefCount == 3);
    if (fails == 0) printf("007498e0_TEST PASS\n"); else printf("007498e0_TEST FAIL\n");
    return fails;
}
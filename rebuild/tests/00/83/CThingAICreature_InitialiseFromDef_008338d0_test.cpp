#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __declspec(x)
#endif
class CCharStringData { public:
    char* Text;                                 // +0
    long  Length;                               // +4
};
class CCharString { public:
    CCharStringData* PData;
    const CCharString& operator=(const char* text);   // 0x0099EFE0
    bool IsEmpty() const { return PData == 0 || PData->Length == 0; }
};
bool g_bForceStandAroundBrain = false;
class CBrainDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
};
class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointer<CBrainDef>* out);          // 0x0079BD80
    bool GetDef(const CCharString& name, CDefPointer<CBrainDef>* out);   // 0x006D3E80
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callees of the stand-in bodies below
void* FindDef_CDefinitionManager_GetDef_1();
// Stand-in bodies (the real ones are out of line in retail): they write the out slot only inline, so VC7.1 knows the
// def pointer's address does not escape the call (the landed 0x00848730 idiom).
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CBrainDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
__declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<CBrainDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}

class CThingCreatureDef { public:
    unsigned char _pad00[0x90];
    float MaxWalkingSpeed;                      // +0x90
    float MaxJoggingSpeed;                      // +0x94
    float MaxRunningSpeed;                      // +0x98
    float MaxPushingSpeed;                      // +0x9c
    float MaxFlyingSpeed;                       // +0xa0
    unsigned char _pad0a4[0xe8 - 0xa4];
    long  BrainDef;                             // +0xe8
    unsigned char _pad0ec[0x148 - 0xec];
    bool LeaveDeadCreature : 1;                 // +0x148
    bool AbleToStrafe : 1;
    bool CreateDummyOnDeath : 1;
    bool ImmuneToAttack : 1;                    // bit 3
};
class CThingAICreature { public:
    void*         __vftable;
    unsigned char _pad04[0xe0 - 0x04];
    CThingCreatureDef* PDef;                    // +0xe0 CDefPointer<CThingCreatureDef const>::Object
    const CThingCreatureDef* GetCreatureDef() const { return PDef; }
    unsigned char _pad0e4[0x190 - 0xe4];
    CCharString   OverridingBrainName;          // +0x190
    unsigned char _pad194[0x1b8 - 0x194];
    float MaxWalkingSpeed;                      // +0x1b8
    float MaxJoggingSpeed;                      // +0x1bc
    float MaxRunningSpeed;                      // +0x1c0
    float MaxFlyingSpeed;                       // +0x1c4
    bool PerformingVillageTaskFlag : 1;         // +0x1c8
    bool AISuspended : 1;
    bool HasInformation : 1;
    bool WanderWithInformation : 1;
    bool WaveWithInformation : 1;
    bool ContinueAIWithInformation : 1;
    bool ImmuneToAttack : 1;                    // bit 6
    bool EnableCreatureAutoPlacing : 1;
    CDefinitionManager* GetDefinitionManager(); // 0x004C7990 (CThing)
    void InitialiseMovementSpeeds();            // 0x00830DD0
    void SetNewBrain(CDefPointer<CBrainDef> brainDef);   // 0x00833010
    void InitialiseFromDef();
};

void CThingAICreature::InitialiseFromDef()
{
    MaxWalkingSpeed = PDef->MaxWalkingSpeed;
    MaxJoggingSpeed = PDef->MaxJoggingSpeed;
    MaxRunningSpeed = PDef->MaxRunningSpeed;
    MaxFlyingSpeed = PDef->MaxFlyingSpeed;
    ImmuneToAttack = PDef->ImmuneToAttack;
    InitialiseMovementSpeeds();
    if (g_bForceStandAroundBrain)
        OverridingBrainName = "BRAIN_STAND_AROUND_LIKE_A_MORON";
    if (OverridingBrainName.IsEmpty()) {
        if (PDef->BrainDef) {
            CDefPointer<CBrainDef> brainDef;
            if (GetDefinitionManager()->GetDef(GetCreatureDef()->BrainDef, &brainDef))
                SetNewBrain(brainDef);
        }
    } else {
        CDefPointer<CBrainDef> brainDef;
        GetDefinitionManager()->GetDef(OverridingBrainName, &brainDef);
        SetNewBrain(brainDef);
    }
}
// ---- test doubles ----
static CCharStringData g_moron = { (char*)"BRAIN_STAND_AROUND_LIKE_A_MORON", 31 };
const CCharString& CCharString::operator=(const char* text) { if (std::strcmp(text, g_moron.Text) == 0) PData = &g_moron; return *this; }
void CBrainDef::v00() {} void CBrainDef::Destroy() {}
struct CTestBrainDef : CBrainDef { int destroyed; virtual void Destroy() { ++destroyed; } };
static CTestBrainDef g_byIndex, g_byName; static bool g_indexOk = true;
void* FindDef_CDefinitionManager_GetDef_0() { if (!g_indexOk) return 0; ++g_byIndex.m_refCount; return &g_byIndex; }
void* FindDef_CDefinitionManager_GetDef_1() { ++g_byName.m_refCount; return &g_byName; }
static CDefinitionManager g_dm;
CDefinitionManager* CThingAICreature::GetDefinitionManager() { return &g_dm; }
static int g_speeds = 0, g_setBrain = 0; static CBrainDef* g_lastBrain = 0; static long g_countInCall = 0;
void CThingAICreature::InitialiseMovementSpeeds() { ++g_speeds; }
void CThingAICreature::SetNewBrain(CDefPointer<CBrainDef> d) { ++g_setBrain; g_lastBrain = d.Object; g_countInCall = d.Object ? d.Object->m_refCount : 0; }
int main()
{
    g_byIndex.m_refCount = 1; g_byIndex.destroyed = 0; g_byName.m_refCount = 1; g_byName.destroyed = 0;
    CThingCreatureDef def; std::memset(&def, 0, sizeof def);
    def.MaxWalkingSpeed = 1.5f; def.MaxJoggingSpeed = 3.0f; def.MaxRunningSpeed = 6.0f; def.MaxPushingSpeed = 9.0f;
    def.MaxFlyingSpeed = 12.0f; def.BrainDef = 4; def.ImmuneToAttack = 1;
    CThingAICreature c; c.PDef = &def; c.OverridingBrainName.PData = 0; c.ImmuneToAttack = 0; c.HasInformation = 1;
    c.InitialiseFromDef();
    bool ok = c.MaxWalkingSpeed == 1.5f && c.MaxJoggingSpeed == 3.0f && c.MaxRunningSpeed == 6.0f && c.MaxFlyingSpeed == 12.0f
        && c.ImmuneToAttack && c.HasInformation && g_speeds == 1 && g_setBrain == 1 && g_lastBrain == &g_byIndex
        && g_countInCall == 3 && g_byIndex.m_refCount == 1;
    g_indexOk = false;                                       // lookup fails: no brain set
    c.InitialiseFromDef();
    ok = ok && g_setBrain == 1;
    def.BrainDef = 0; g_indexOk = true; def.ImmuneToAttack = 0;
    c.InitialiseFromDef();
    ok = ok && g_setBrain == 1 && !c.ImmuneToAttack;
    g_bForceStandAroundBrain = true;                         // the debug switch overrides by name
    c.InitialiseFromDef();
    ok = ok && c.OverridingBrainName.PData == &g_moron && g_setBrain == 2 && g_lastBrain == &g_byName && g_byName.m_refCount == 1;
    if (!ok) { std::printf("008338d0_TEST FAIL\n"); return 1; }
    std::printf("008338d0_TEST PASS\n");
    return 0;
}
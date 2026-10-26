#include <cstdio>
#include <cstring>
// CThingPlayerCreature::InitialiseMovementSpeeds() -- copy the slow-walk/walk/jog/run/fly maximum speeds from the creature def (sprint starts at 0), then re-initialise each through its "ST_*" speed table name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A9A00, length 353 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CThingCreatureBase::InitialiseMovementSpeed
//   (float*, const CCharString&) @ 0x00663510, CCharString::~CCharString @ 0x0099EAE0; literals "ST_SLOW_WALK",
//   "ST_WALK", "ST_JOG", "ST_RUN", "ST_SPRINT", "STANDARD_FLY".
// Layout: the model of landed CThingPlayerCreature::ResetMaxWalkingSpeedToDefault (0x006A9B70): PDef +0xe0, and the
//   retail speed block MaxSlowWalkingSpeed +0x18c .. MaxFlyingSpeed +0x1a0 (Ego_r.pdb +0x1b4.. ; the retail player
//   creature is 0x28 narrower there); CThingCreatureDef speeds at PDB - 4 (+0x8c..+0x98, MaxFlyingSpeed +0xa0).
// Shape: each name is an implicit const char* -> CCharString conversion temporary (one shared frame slot, re-addressed
//   with `lea` for the push); the speed-member addresses of the first three calls live in edi/ebx/ebp.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};

class CThingCreatureDef { public:
    unsigned char _pad00[0x8c];
    float         MaxSlowWalkingSpeed;                   // +0x8c
    float         MaxWalkingSpeed;                       // +0x90
    float         MaxJoggingSpeed;                       // +0x94
    float         MaxRunningSpeed;                       // +0x98
    float         MaxPushingSpeed;                       // +0x9c
    float         MaxFlyingSpeed;                        // +0xa0
};

template <class T> class CDefPointer { public:
    T* m_pDef;
    T* operator->() const { return m_pDef; }
};

class CThingCreatureBase { public:
    unsigned char                   _pad00[0xe0];
    CDefPointer<CThingCreatureDef>  PDef;                // +0xe0
    void InitialiseMovementSpeed(float* speed, const CCharString& name);   // 0x00663510
};

class CThingPlayerCreature : public CThingCreatureBase { public:
    unsigned char _pade4[0x18c - 0xe4];
    float         MaxSlowWalkingSpeed;                   // +0x18c
    float         MaxWalkingSpeed;                       // +0x190
    float         MaxJoggingSpeed;                       // +0x194
    float         MaxRunningSpeed;                       // +0x198
    float         MaxSprintingSpeed;                     // +0x19c
    float         MaxFlyingSpeed;                        // +0x1a0
    void InitialiseMovementSpeeds();
};

void CThingPlayerCreature::InitialiseMovementSpeeds()
{
    MaxSlowWalkingSpeed = PDef->MaxSlowWalkingSpeed;
    MaxWalkingSpeed = PDef->MaxWalkingSpeed;
    MaxJoggingSpeed = PDef->MaxJoggingSpeed;
    MaxRunningSpeed = PDef->MaxRunningSpeed;
    MaxSprintingSpeed = 0;
    MaxFlyingSpeed = PDef->MaxFlyingSpeed;
    InitialiseMovementSpeed(&MaxSlowWalkingSpeed, "ST_SLOW_WALK");
    InitialiseMovementSpeed(&MaxWalkingSpeed, "ST_WALK");
    InitialiseMovementSpeed(&MaxJoggingSpeed, "ST_JOG");
    InitialiseMovementSpeed(&MaxRunningSpeed, "ST_RUN");
    InitialiseMovementSpeed(&MaxSprintingSpeed, "ST_SPRINT");
    InitialiseMovementSpeed(&MaxFlyingSpeed, "STANDARD_FLY");
}


static int g_ctors = 0, g_dtors = 0;
CCharString::CCharString(const char* text, long) { m_pData = (void*)text; ++g_ctors; }
CCharString::~CCharString() { ++g_dtors; }
static char g_names[6][16]; static float* g_ptrs[6]; static int g_n = 0;
void CThingCreatureBase::InitialiseMovementSpeed(float* speed, const CCharString& name)
{ if (g_n < 6) { std::strcpy(g_names[g_n], (const char*)name.m_pData); g_ptrs[g_n] = speed; ++g_n; } *speed += 100.0f; }

int main()
{
    static CThingCreatureDef def;
    static CThingPlayerCreature p;
    def.MaxSlowWalkingSpeed = 1; def.MaxWalkingSpeed = 2; def.MaxJoggingSpeed = 3; def.MaxRunningSpeed = 4;
    def.MaxPushingSpeed = 5; def.MaxFlyingSpeed = 6;
    p.PDef.m_pDef = &def; p.MaxSprintingSpeed = 55;
    p.InitialiseMovementSpeeds();
    if (g_n != 6 || g_ctors != 6 || g_dtors != 6 || p.MaxSlowWalkingSpeed != 101 || p.MaxWalkingSpeed != 102
        || p.MaxJoggingSpeed != 103 || p.MaxRunningSpeed != 104 || p.MaxSprintingSpeed != 100 || p.MaxFlyingSpeed != 106
        || std::strcmp(g_names[0], "ST_SLOW_WALK") || std::strcmp(g_names[4], "ST_SPRINT") || std::strcmp(g_names[5], "STANDARD_FLY")
        || g_ptrs[3] != &p.MaxRunningSpeed) { std::printf("006a9a00_TEST FAIL\n"); return 1; }
    std::printf("006a9a00_TEST PASS\n");
    return 0;
}
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
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};

struct CThingCreatureDef {
    unsigned char _pad00[0x8c];
    float         MaxSlowWalkingSpeed;                   // +0x8c
    float         MaxWalkingSpeed;                       // +0x90
    float         MaxJoggingSpeed;                       // +0x94
    float         MaxRunningSpeed;                       // +0x98
    float         MaxPushingSpeed;                       // +0x9c
    float         MaxFlyingSpeed;                        // +0xa0
};

template <class T> struct CDefPointer {
    T* m_pDef;
    T* operator->() const { return m_pDef; }
};

struct CThingCreatureBase {
    unsigned char                   _pad00[0xe0];
    CDefPointer<CThingCreatureDef>  PDef;                // +0xe0
    void InitialiseMovementSpeed(float* speed, const CCharString& name);   // 0x00663510
};

struct CThingPlayerCreature : CThingCreatureBase {
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
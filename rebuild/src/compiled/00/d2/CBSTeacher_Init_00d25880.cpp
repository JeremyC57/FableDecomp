// CBS_Teacher::Init() -- register the teacher's thread with the script base, then copy the master data's kid-intro thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D25880, length 59 bytes (+ int3 padding).
// Callees: CScriptBase_InitCheckThread @ 0x00D258C0 (manifest label; one zero argument) on the
// parent script at +0x14.
// Globals: the quest master-data pointer @ 0x0143E90C (its CScriptThing at +0xbb4..+0xbbc).
// Layout from Ego_r.pdb (CBS_Teacher, 40 bytes): ParentClass +0x14, PMasterData +0x18, KidIntro
// (CScriptThing, 12 bytes) +0x1c. The copy is memberwise through CScriptThing's inline operator=
// (three dword moves), and the global pointer is re-read before each move because the stores
// through `this` may alias it (no type-based aliasing in VC7.1).
struct CScriptThing {                           // 12 bytes
    long m_id;
    long m_uid;
    long m_type;
    CScriptThing& operator=(const CScriptThing& other)
    {
        m_id = other.m_id;
        m_uid = other.m_uid;
        m_type = other.m_type;
        return *this;
    }
};

struct CV_HeroDollsScript {
    void InitCheckThread(long thread);          // 0x00D258C0
};

struct CQ_SunnyvaleMasterData {
    unsigned char _pad[0xbb4];
    CScriptThing  KidIntro;                     // +0xbb4
};

extern CQ_SunnyvaleMasterData* g_pMasterData;   // 0x0143E90C

struct CBS_Teacher {
    unsigned char           _pad[0x14];         // +0x00..+0x13 (script base)
    CV_HeroDollsScript*     ParentClass;        // +0x14
    CQ_SunnyvaleMasterData* PMasterData;        // +0x18
    CScriptThing            KidIntro;           // +0x1c
    void Init();
};

void CBS_Teacher::Init()
{
    ParentClass->InitCheckThread(0);
    KidIntro.m_id = g_pMasterData->KidIntro.m_id;
    KidIntro.m_uid = g_pMasterData->KidIntro.m_uid;
    KidIntro.m_type = g_pMasterData->KidIntro.m_type;
}
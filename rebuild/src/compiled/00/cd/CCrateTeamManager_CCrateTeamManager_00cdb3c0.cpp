// CCrateTeamManager::~CCrateTeamManager() -- destroy the two CScriptThing members (release their counted PImp, CBase::~CBase) and give the reinforcements timer back to the thing manager
// Leaf: ??1CCrateTeamManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDB3C0, 141 bytes.
// Callees: CBase::~CBase 0x0099A2E0 (twice), operator delete 0x00BFE9BC, the thing manager global
//          @ 0x0143E8F8 vtable slot 88 (+0x160, release a timer index).
//
// Shape: the landed constructor (0x00CDB360) model: PDB CCrateTeamManager (64 bytes) with
// TeamReinforcementsTimer (CTimer) +0x1c, CrateDropPos +0x24 and TeamCrateCarrier +0x30 (CScriptThing,
// vtable 0x01238C8C). The class itself is not polymorphic (no vtable store); its empty destructor runs
// the member destructors in reverse order: each CScriptThing's inline destructor stores its vtable,
// releases PImp (the `--count; if (count == 0)` re-read form, then both words nulled) and calls the
// out-of-line CBase destructor; the CTimer destructor hands its index back through slot 88. One zero
// register (ebx) serves both releases.
struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};

struct CCountedPointer {
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct CBase {
    virtual ~CBase();                      // 0x0099A2E0
};

struct CScriptThing : CBase {              // vtable 0x01238C8C
    CCountedPointer PImp;                  // +0x04
    virtual ~CScriptThing() { }
};

struct CThingManager_Calls {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void ReleaseTimer(long timerIndex);   // slot 88 (+0x160)
};
extern CThingManager_Calls* g_pThingManager_0143e8f8;   // 0x0143E8F8

struct CTimer {
    long TimerIndex;
    ~CTimer() { g_pThingManager_0143e8f8->ReleaseTimer(TimerIndex); }
};

struct CCrateTeamManager {
    long               StateCounter[6];        // +0x00
    long               MemberCount;            // +0x18
    CTimer             TeamReinforcementsTimer;    // +0x1c
    long               MaxTeamMembers;         // +0x20
    CScriptThing       CrateDropPos;           // +0x24
    CScriptThing       TeamCrateCarrier;       // +0x30
    CCrateTeamManager* EnemyTeam;              // +0x3c
    ~CCrateTeamManager();
};

CCrateTeamManager::~CCrateTeamManager()
{
}
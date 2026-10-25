// Behaviour test for 0093c830: body copied from src/0093c830.cpp (CAIStateGroup_StealthSeekEnemy::Update), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase { };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x60 - 0x4c];
    void* m_pSub60;
    unsigned char _pad2[0x91 - 0x64];
    unsigned char m_flags91;
    unsigned char m_flags92;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceX(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIBrain { unsigned char _pad[0x18]; CThing* PCreature; };
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual void Cleanup() {}
    virtual void OnReplace() {}
    virtual bool IsRunnable() { return false; }
    virtual void Update() {}
    virtual void v13() {}
    virtual long GetTimeToInitiate() { return 0; }
    virtual void v15() {} virtual void v16() {} virtual void v17() {}
    CAIBrain* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    void Terminate();
};
struct C3DVector { float x, y, z; };
struct CTCPerceiveThing : CTCBase {
    unsigned char _pad[0x18];
    float m_awareness;
    unsigned char _pad1c[0x20 - 0x1c];
    C3DVector m_perceivedPos;
    unsigned char _pad2c[2];
    bool m_heardSound;
};
static int g_dangerCalls = 0; static CThing* g_dangerSource = 0; static long g_dangerStatus = 0;
struct CTCStealth : CTCBase {
    void SetAsInDanger(CThing* source, long status) { ++g_dangerCalls; g_dangerSource = source; g_dangerStatus = status; }
};
static bool g_free = false; static int g_finishCalls = 0;
struct CThingCreatureBase : CThing {
    bool IsFreeToFinishAction() { return g_free; }
    void FinishCurrentAction() { ++g_finishCalls; }
};
static int g_runnableLeft = 0; static int g_sound = 0, g_move = 0, g_anim = 0, g_terminate = 0; static const char* g_log[32]; static int g_logN = 0;
struct CAIStateGroup_StealthSeekEnemy : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;
    C3DVector DetectedPos;
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PBrain->PCreature; }
    virtual void Update();
private:
    bool IsStillRunnable(const CTCPerceiveThing& perceive) const { g_log[g_logN++] = "runnable"; return g_runnableLeft-- > 0; }
    void SetWhatsThatSound() { ++g_sound; g_log[g_logN++] = "sound"; }
    void MoveTowardsEnemy(const CTCPerceiveThing* perceive) { ++g_move; g_log[g_logN++] = "move"; }
    void SetWhatsThatAnimation(const CTCPerceiveThing* perceive) { ++g_anim; g_log[g_logN++] = "anim"; }
};
void CAIStateGroupBase::Terminate() { ++g_terminate; g_log[g_logN++] = "terminate"; }

void CAIStateGroup_StealthSeekEnemy::Update()
{
    CTCPerceiveThing* perceive = 0;
    PBrain->PCreature->GetInterface(&perceive, 0x34);
    if (IS_THING_ALIVE(PEnemy) && (perceive->m_heardSound || perceive->m_awareness != 0.0f)) {
        if (GetCreature()->IsFreeToFinishAction())
            GetCreature()->FinishCurrentAction();
        while (IsStillRunnable(*perceive)) {
            DetectedPos = perceive->m_perceivedPos;
            CTCStealth* stealth;
            if (PEnemy.GetPItem()->GetInterfaceX(&stealth, 0x36))
                stealth->SetAsInDanger(PBrain->PCreature, 1);
            SetWhatsThatSound();
            MoveTowardsEnemy(perceive);
            SetWhatsThatAnimation(perceive);
        }
    }
    Terminate();
}
static CThingCreatureBase g_creature; static CThing g_enemy; static CInterfaceMap::SEntry g_cEntries[2], g_eEntries[2];
static CTCPerceiveThing g_perceive; static CTCStealth g_stealth; static CAIBrain g_brain;
static void Reset() { g_dangerCalls = 0; g_finishCalls = 0; g_sound = g_move = g_anim = g_terminate = 0; g_logN = 0; }
int main()
{
    memset(&g_creature, 0, sizeof(g_creature)); memset(&g_enemy, 0, sizeof(g_enemy)); memset(&g_perceive, 0, sizeof(g_perceive));
    g_cEntries[0].type = 0x10; g_cEntries[1].type = 0x34; g_cEntries[1].component = &g_perceive;
    g_creature.m_interfaces.first = g_cEntries; g_creature.m_interfaces.last = g_cEntries + 2; g_creature.m_presentBits[1] |= 1UL << (0x34 & 31);
    g_eEntries[0].type = 0x36; g_eEntries[0].component = &g_stealth; g_eEntries[1].type = 0x40;
    g_enemy.m_interfaces.first = g_eEntries; g_enemy.m_interfaces.last = g_eEntries + 2; g_enemy.m_presentBits[1] |= 1UL << (0x36 & 31);
    g_brain.PCreature = &g_creature;
    CAIStateGroup_StealthSeekEnemy sg; sg.PBrain = &g_brain; sg.PEnemy.m_pItem = &g_enemy; sg.DetectedPos.x = sg.DetectedPos.y = sg.DetectedPos.z = -1.0f;
    g_perceive.m_perceivedPos.x = 1.0f; g_perceive.m_perceivedPos.y = 2.0f; g_perceive.m_perceivedPos.z = 3.0f;
    // 1: enemy alive, sound heard, creature free: finish, two loop rounds with the stealth component, terminate
    Reset(); g_perceive.m_heardSound = true; g_free = true; g_runnableLeft = 2;
    sg.Update();
    CHECK(g_finishCalls == 1); CHECK(g_sound == 2 && g_move == 2 && g_anim == 2); CHECK(g_dangerCalls == 2);
    CHECK(g_dangerSource == &g_creature && g_dangerStatus == 1); CHECK(g_terminate == 1);
    CHECK(sg.DetectedPos.x == 1.0f && sg.DetectedPos.y == 2.0f && sg.DetectedPos.z == 3.0f);
    CHECK(g_logN == 10); CHECK(strcmp(g_log[0], "runnable") == 0 && strcmp(g_log[1], "sound") == 0 && strcmp(g_log[2], "move") == 0 && strcmp(g_log[3], "anim") == 0);
    CHECK(strcmp(g_log[9], "terminate") == 0);
    // 2: awareness only, not free, no stealth component on the enemy
    Reset(); g_perceive.m_heardSound = false; g_perceive.m_awareness = 0.5f; g_free = false; g_runnableLeft = 1; g_enemy.m_presentBits[1] = 0;
    sg.Update();
    CHECK(g_finishCalls == 0); CHECK(g_sound == 1); CHECK(g_dangerCalls == 0); CHECK(g_terminate == 1);
    // 3: nothing perceived
    Reset(); g_perceive.m_awareness = 0.0f; g_runnableLeft = 5;
    sg.Update();
    CHECK(g_sound == 0 && g_logN == 1 && g_terminate == 1);
    // 4: dead enemy
    Reset(); g_perceive.m_heardSound = true; g_enemy.m_flags91 = 1;
    sg.Update();
    CHECK(g_sound == 0 && g_terminate == 1);
    // 5: no enemy
    Reset(); sg.PEnemy.m_pItem = 0;
    sg.Update();
    CHECK(g_sound == 0 && g_terminate == 1);
    if (fails == 0) printf("0093c830_TEST PASS\n"); else printf("0093c830_TEST FAIL\n");
    return fails;
}
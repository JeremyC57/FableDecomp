// CBanditLeader::Main() -- bandit-toll leader: register the alive filter, then every frame until terminated: once the bandits are angered and Me (alive) is aware of the hero, join FACTION_BANDITS and follow the hero for good; otherwise, whenever the talk timer has run out and the hero is within 10 units, face the hero, tell him to go away (four escalating lines, re-arming the timer with 15/12/10/8 seconds), boo him on the first talk and one time in five after, and count the talk
// Grade: DIFFER(1362v1356) -- block placement only: retail lays the inline handle-dtor loop exit right after the outer loop and the angered (follow-the-hero) block after it; ours puts the angered block first and the inline dtor exit last (+6 bytes alignment); tried for(;;)/do-while/while tails, pre-loop term check, if/else and continue forms
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E4AA00, length 1360 bytes, plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0, out-of-line dtor 0x00E241B0), the scripted-thing handle (vtable
//   0x0127094C; its generated dtor is inline at the loop exit, 0x007E74D0 elsewhere) IsScripting 0x00CD23B9 / Release
//   0x00CD2770 / FollowThing 0x007E7320 / PlayAnimation 0x007E73D0 / IsPerformingScriptTask 0x007E7450,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF, rand 0x00BFEB16. CGameScriptInterface slots 7, 8, 70, 89 SetTimer / 90
//   GetTimer (through g_pGameScriptInterface 0x0143E8F8), 364/365/366 conversation, 475
//   EntitySetFacingAngleTowardsThing, 601 EntitySetInFaction, 636 IsThingAwareOfOtherThingInAnyWay; CScriptThing slot 75.
// Literals: TEXT_QST_011_BANDIT_LEADER_GO_AWAY 0x012E4FE0 / _AGAIN 0x012E5004 / _AGAIN_AGAIN 0x012E5030 / _SHORT
//   0x012E5060, "ST_OPINION_DISAPPROVAL_BOOING" 0x01299A04, "FACTION_BANDITS" 0x0125FE68; the animation-default bool
//   0x01375748.
// Layout: Ego_r.pdb CBanditLeader (36 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CV_BanditTollScript, retail = PDB - 0x14: BanditsAngered +0x48), PMasterData +0x18, TalkedToHero +0x1c,
//   CTimer TalkFrequency +0x20.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
extern C3DVector g_ZeroVector;                         // 0x0143E8E0
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
int __cdecl rand();                                    // 0x00BFEB16
void operator delete(void* p);                         // 0x00BFE9BC
struct CCPPointerInfo {
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4
    void* Data;                                        // +8
};
struct CScriptThing;
struct CCountedPointer {                               // {Data, Info}
    CScriptThing*   Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
enum EHeroAbility { HERO_ABILITY_14 = 14 };
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
struct CScriptThing {                                  // 12 bytes, vtable 0x01238C8C
    VPAD2(p0x0) VPAD1(p0x1)   // slots 0..2
    virtual CCharString GetDataString() const;   // slot 3 (+0xc)
    VPAD2(p1x0)   // slots 4..5
    __forceinline virtual const C3DVector& GetPos() const { if (!PImp.Data) return g_ZeroVector; return PImp.Data->GetPos(); }   // slot 6 (+0x18)
    VPAD8(p2x0) VPAD4(p2x1) VPAD2(p2x2)   // slots 7..20
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    VPAD16(p3x0) VPAD2(p3x1) VPAD1(p3x2)   // slots 22..40
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    VPAD32(p4x0)   // slots 43..74
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
    CCountedPointer PImp;                              // +0x04
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
};

struct CIsThingAlive {                                 // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                                  // 0x00E241B0
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CBaseObject {                                   // {vptr, intelligent-pointer link}
    void* IntelligentPointer;                          // +0x04
    CBaseObject();                                     // 0x0099A380
    virtual ~CBaseObject();                            // 0x0099A430
};
struct CScriptGameResourceObjectBase : CBaseObject {   // vtable 0x0126008C
    ~CScriptGameResourceObjectBase() { }
};
struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase {   // vtable 0x0127094C, 16 bytes
    CCountedPointer PImp;                              // +0x08
    VPAD2(s0) VPAD1(s1)                                // slots 1..3
    virtual void MoveToPosition(const C3DVector& pos, float speed, EScriptEntityMoveType type, bool a, bool b);   // slot 4 (+0x10), 0x007E72F0
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    void FollowThing(const CScriptThing& thing, float distance, bool run);   // 0x007E7320
    void PlayAnimation(const CCharString& anim, bool a, bool b, bool c, bool d, bool e, bool f, bool g);   // 0x007E73D0
    bool IsPerformingScriptTask() const;               // 0x007E7450
};
struct CGameScriptInterface {
    VPAD4(p0x0) VPAD2(p0x1) VPAD1(p0x2)   // slots 0..6
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& handle, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(p1x0) VPAD16(p1x1) VPAD8(p1x2) VPAD4(p1x3) VPAD1(p1x4)   // slots 9..69
    virtual const CScriptThing& GetHero() const;   // slot 70 (+0x118)
    VPAD16(p2x0) VPAD2(p2x1)   // slots 71..88
    virtual void SetTimer(long handle, long time) const;   // slot 89 (+0x164)
    virtual long GetTimer(long handle) const;   // slot 90 (+0x168)
    VPAD256(p3x0) VPAD16(p3x1) VPAD1(p3x2)   // slots 91..363
    virtual long AddNewConversation(const CScriptThing& thing, bool a, bool b) const;   // slot 364 (+0x5b0)
    virtual void AddPersonToConversation(long conversation, const CScriptThing& person) const;   // slot 365 (+0x5b4)
    virtual void AddLineToConversation(long conversation, const CCharString& line, bool a, const CScriptThing& speaker, const CScriptThing& listener) const;   // slot 366 (+0x5b8)
    VPAD64(p4x0) VPAD32(p4x1) VPAD8(p4x2) VPAD4(p4x3)   // slots 367..474
    virtual void EntitySetFacingAngleTowardsThing(const CScriptThing& thing, const CScriptThing& target, bool a) const;   // slot 475 (+0x76c)
    VPAD64(p5x0) VPAD32(p5x1) VPAD16(p5x2) VPAD8(p5x3) VPAD4(p5x4) VPAD1(p5x5)   // slots 476..600
    virtual void EntitySetInFaction(const CScriptThing& thing, const CCharString& faction) const;   // slot 601 (+0x964)
    VPAD32(p6x0) VPAD2(p6x1)   // slots 602..635
    virtual bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing& thing, const CScriptThing& other) const;   // slot 636 (+0x9f0)
};

struct CV_BanditTollScript {
    unsigned char _pad[0x48];
    bool BanditsAngered;                               // +0x48
};
extern bool g_bScriptAnimationFlag;                    // 0x01375748
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
struct CTimer {
    long Handle;
    __forceinline void SetTime(long time) { g_pGameScriptInterface->SetTimer(Handle, time); }
    __forceinline long GetTime() const { return g_pGameScriptInterface->GetTimer(Handle); }
};
struct CQ_SunnyvaleMasterData;
class CBanditLeader {
public:
    void*                   __vftable;                 // +0x00
    CGameScriptInterface*   Game;                      // +0x04
    CScriptThing            Me;                        // +0x08
    CV_BanditTollScript*    ParentClass;               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;               // +0x18
    long                    TalkedToHero;              // +0x1c
    CTimer                  TalkFrequency;             // +0x20
    void RegisterFilter(CIsThingAlive& filter);        // 0x00F35B10
    bool IsActiveThreadTerminating();                  // 0x00F35B30
    void Main();
};

void CBanditLeader::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    while (!IsActiveThreadTerminating()) {
        if (ParentClass->BanditsAngered && Me.IsAlive() && Game->IsThingAwareOfOtherThingInAnyWay(Game->GetHero(), Me)) {
            if (IsActiveThreadTerminating())
                return;
            if (scripted.IsScripting())
                scripted.Release();
            while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)3)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetInFaction(Me, "FACTION_BANDITS");
            scripted.FollowThing(Game->GetHero(), 1.0f, true);
            for (;;) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        if (TalkFrequency.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 10.0f)) {
            if (IsActiveThreadTerminating())
                return;
            if (scripted.IsScripting())
                scripted.Release();
            while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            while (scripted.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            switch (TalkedToHero) {
            case 0: {
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY", false, Me, Game->GetHero());
                TalkFrequency.SetTime(15);
                break;
            }
            case 1: {
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY_AGAIN", false, Me, Game->GetHero());
                TalkFrequency.SetTime(12);
                break;
            }
            case 2: {
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY_AGAIN_AGAIN", false, Me, Game->GetHero());
                TalkFrequency.SetTime(10);
                break;
            }
            default: {
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_LEADER_GO_AWAY_SHORT", false, Me, Game->GetHero());
                TalkFrequency.SetTime(8);
                break;
            }
            }
            if (TalkedToHero == 0 || rand() % 5 == 0) {
                if (IsActiveThreadTerminating())
                    return;
                scripted.PlayAnimation("ST_OPINION_DISAPPROVAL_BOOING", false, false, false, true, g_bScriptAnimationFlag, false, false);
                while (scripted.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                TalkFrequency.SetTime(8);
            }
            scripted.Release();
            TalkedToHero++;
        }
        Game->NewScriptFrame();
    }
}

// CBlockingBandit::Main() -- bandit-toll blocking bandit: register the alive filter, take Me under scripted control, look up M_BanditEscapePoint, then every frame: once the bandits flee, walk Me back to the escape point and fade it out; when the bandits are angered and Me is alive and aware of the hero, join FACTION_BANDITS and follow the hero; once the toll is paid, walk Me (once) back to M_BanditCampCentrePos and release it
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E49980, length 1424 bytes, plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0, out-of-line dtor 0x00E241B0), the scripted-thing handle (vtable
//   0x0127094C) IsScripting 0x00CD23B9 / Release 0x00CD2770 / MoveToPosition 0x007E72F0 / FollowThing 0x007E7320 /
//   IsPerformingScriptTask 0x007E7450 / GetScriptThing 0x007E7490 / dtor 0x007E74D0, IsDistanceFromThingToPositionOver
//   0x00CBE45C and FadeOutAndKillEntity 0x00CBE716 (`__fastcall`). CGameScriptInterface slots 7, 8, 70, 72
//   GetThingWithScriptName, 601 EntitySetInFaction, 636 IsThingAwareOfOtherThingInAnyWay; CScriptThing slot 75 IsAlive.
// Literals: "M_BanditEscapePoint" 0x012E4DA4, "M_BanditCampCentrePos" 0x012E4DF0, "FACTION_BANDITS" 0x0125FE68.
// Layout: Ego_r.pdb CBlockingBandit (28 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CV_BanditTollScript, retail = PDB - 0x14: BanditsAngered +0x48, TollPaid +0x49, BanditsFlee +0x4f), PMasterData
//   +0x18. g_pScriptDef: the toll bandit's close-to-exit distance +0x43c and fade time +0x440 (PDB +0x470/+0x474
//   TollBanditCloseToExit / TollBanditTimeToFade; the retail def is 0x34 smaller ahead of them, as in the landed
//   CSafeTrader::Main 0x00E4A280).
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
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool kill, float seconds, bool removeOnFade);        // 0x00CBE716
struct CCountedPointerWords { void* Data; void* Info; };
struct CBaseObject {                                   // {vptr, intelligent-pointer link}
    void* IntelligentPointer;                          // +0x04
    CBaseObject();                                     // 0x0099A380
    virtual ~CBaseObject();                            // 0x0099A430
};
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C, 16 bytes
    CCountedPointerWords PImp;                         // +0x08
    CScriptGameResourceObjectScriptedThingBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    VPAD1(s0) VPAD1(s1)                                // slots 2..3
    virtual void MoveToPosition(const C3DVector& pos, float speed, EScriptEntityMoveType type, bool a, bool b);   // slot 4 (+0x10), 0x007E72F0
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    void FollowThing(const CScriptThing& thing, float distance, bool run);   // 0x007E7320
    bool IsPerformingScriptTask() const;               // 0x007E7450
    CScriptThing GetScriptThing() const;               // 0x007E7490
};
struct CGameScriptInterface {
    VPAD4(p0x0) VPAD2(p0x1) VPAD1(p0x2)   // slots 0..6
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& handle, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(p1x0) VPAD16(p1x1) VPAD8(p1x2) VPAD4(p1x3) VPAD1(p1x4)   // slots 9..69
    virtual const CScriptThing& GetHero() const;   // slot 70 (+0x118)
    VPAD1(p2x0)   // slots 71..71
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    VPAD256(p3x0) VPAD256(p3x1) VPAD16(p3x2)   // slots 73..600
    virtual void EntitySetInFaction(const CScriptThing& thing, const CCharString& faction) const;   // slot 601 (+0x964)
    VPAD32(p4x0) VPAD2(p4x1)   // slots 602..635
    virtual bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing& thing, const CScriptThing& other) const;   // slot 636 (+0x9f0)
};

struct CV_BanditTollScript {
    unsigned char _pad[0x48];
    bool BanditsAngered;                               // +0x48
    bool TollPaid;                                     // +0x49
    unsigned char _pad4a[0x4f - 0x4a];
    bool BanditsFlee;                                  // +0x4f
};
struct CScriptDef {
    unsigned char _pad[0x43c];
    float TollBanditCloseToExit;                       // +0x43c
    float TollBanditTimeToFade;                        // +0x440
};
extern CScriptDef* g_pScriptDef;                       // 0x0143E90C
struct CQ_SunnyvaleMasterData;
class CBlockingBandit {
public:
    void*                   __vftable;                 // +0x00
    CGameScriptInterface*   Game;                      // +0x04
    CScriptThing            Me;                        // +0x08
    CV_BanditTollScript*    ParentClass;               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;               // +0x18
    void RegisterFilter(CIsThingAlive& filter);        // 0x00F35B10
    bool IsActiveThreadTerminating();                  // 0x00F35B30
    void Main();
};

void CBlockingBandit::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    bool returnedToCamp = false;
    CScriptThing escapePoint = Game->GetThingWithScriptName("M_BanditEscapePoint");
    while (!IsActiveThreadTerminating()) {
        if (ParentClass->BanditsFlee) {
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
            float closeToExit = g_pScriptDef->TollBanditCloseToExit;
            C3DVector exitPos = escapePoint.GetPos();
            while (IsDistanceFromThingToPositionOver(scripted.GetScriptThing(), exitPos, closeToExit + 2.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                scripted.MoveToPosition(exitPos, closeToExit, (EScriptEntityMoveType)1, false, true);
                while (scripted.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            FadeOutAndKillEntity(Me, true, g_pScriptDef->TollBanditTimeToFade, true);
        }
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
        }
        if (ParentClass->TollPaid && !returnedToCamp) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing campCentre = Game->GetThingWithScriptName("M_BanditCampCentrePos");
            C3DVector campPos = campCentre.GetPos();
            while (IsDistanceFromThingToPositionOver(scripted.GetScriptThing(), campPos, 4.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                scripted.MoveToPosition(campPos, 2.0f, (EScriptEntityMoveType)0, false, true);
                while (scripted.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            scripted.Release();
            returnedToCamp = true;
        }
        Game->NewScriptFrame();
    }
}

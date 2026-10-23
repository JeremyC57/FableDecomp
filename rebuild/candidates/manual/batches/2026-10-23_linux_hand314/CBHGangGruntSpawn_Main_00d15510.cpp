// CBH_GangGruntSpawn::Main() -- bounty-hunt gang grunt spawn point: register the alive filter; if Me's data string holds a number, start the wait timer with it; wait until the hero hits Me (plainly or with a special ability other than 14), sees Me, or the timer runs out; set Me on the hero, wait for the quest to end, then walk Me to the nearest REGION_EXIT_POINT and fade it out once there (or once the region is loaded)
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D15510, length 1072 bytes, plain `ret`.
// Callees: CBaseObject ctor 0x0099A380 / dtor 0x0099A430, the scripted-thing handle (vtable 0x0127094C) IsScripting
//   0x00CD23B9 / Release 0x00CD2770 / MoveToPosition 0x007E72F0 / dtor 0x007E74D0 (the generated dtor is expanded at the
//   first exit), CScriptThing copy 0x004ABE90 / dtor 0x004AA840 / IsAlive 0x004AB130, the filter registration 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099E4B0 / ~ 0x0099EAE0 / operator!= 0x0099E960,
//   GFCharStringToInt 0x0099E7F0, IsDistanceBetweenThingsUnder 0x00CBE2FF, FadeOutAndKillEntity 0x00CBE716 (`__fastcall`),
//   operator delete 0x00BFE9BC. CGameScriptInterface slots 7, 8, 22 MsgOnRegionLoaded, 70, 81 GetNearestWithDefName,
//   89 SetTimer / 90 GetTimer (through g_pGameScriptInterface 0x0143E8F8), 451, 632 CanThingBeSeenByOtherThing.
// Literals: "" 0x0122D70E, "NULL" 0x012393E4, "SCRIPT_NAME_HERO" 0x0125D1C8, "REGION_EXIT_POINT" 0x012CA510.
// Layout: Ego_r.pdb CBH_GangGruntSpawn (32 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, CTimer WaitForTimer +0x1c; CQ_BountyHuntScript retail = PDB - 0x14 (QuestFailed +0x48,
//   QuestSucceeded +0x49).
// Shape: the CRandomAttacker 0x00DE21D0 hit-test chain (conditional string temporaries in ebx bits 1/2/4, the result in a
//   byte slot); the handle's compiler-generated destructor is inline once (the first exit) and out of line elsewhere.
struct C3DVector { float X, Y, Z; };
extern C3DVector g_ZeroVector;                         // 0x0143E8E0
struct CCharString {
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator!=(const char* text) const;           // 0x0099E960
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
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
    ~CIsThingAlive() { }
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool a, float time, bool b);                  // 0x00CBE716
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
};
struct CGameScriptInterface {
    VPAD4(p0x0) VPAD2(p0x1) VPAD1(p0x2)   // slots 0..6
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& handle, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD8(p1x0) VPAD4(p1x1) VPAD1(p1x2)   // slots 9..21
    virtual bool MsgOnRegionLoaded(CCharString& region) const;   // slot 22 (+0x58)
    VPAD32(p2x0) VPAD8(p2x1) VPAD4(p2x2) VPAD2(p2x3) VPAD1(p2x4)   // slots 23..69
    virtual const CScriptThing& GetHero() const;   // slot 70 (+0x118)
    VPAD8(p3x0) VPAD2(p3x1)   // slots 71..80
    virtual CScriptThing GetNearestWithDefName(const CScriptThing& thing, const CCharString& defName) const;   // slot 81 (+0x144)
    VPAD4(p4x0) VPAD2(p4x1) VPAD1(p4x2)   // slots 82..88
    virtual void SetTimer(long handle, long time) const;   // slot 89 (+0x164)
    virtual long GetTimer(long handle) const;   // slot 90 (+0x168)
    VPAD256(p5x0) VPAD64(p5x1) VPAD32(p5x2) VPAD8(p5x3)   // slots 91..450
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const;   // slot 451 (+0x70c)
    VPAD128(p6x0) VPAD32(p6x1) VPAD16(p6x2) VPAD4(p6x3)   // slots 452..631
    virtual bool CanThingBeSeenByOtherThing(const CScriptThing& viewer, const CScriptThing& thing) const;   // slot 632 (+0x9e0)
};

struct CQ_BountyHuntScript {
    unsigned char _pad[0x48];
    bool QuestFailed;                                  // +0x48
    bool QuestSucceeded;                               // +0x49
};
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
struct CTimer {
    long Handle;
    __forceinline void SetTime(long time) { g_pGameScriptInterface->SetTimer(Handle, time); }
    __forceinline long GetTime() const { return g_pGameScriptInterface->GetTimer(Handle); }
};
struct CQ_SunnyvaleMasterData;
class CBH_GangGruntSpawn {
public:
    void*                   __vftable;                 // +0x00
    CGameScriptInterface*   Game;                      // +0x04
    CScriptThing            Me;                        // +0x08
    CQ_BountyHuntScript*    ParentClass;               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;               // +0x18
    CTimer                  WaitForTimer;              // +0x1c
    void RegisterFilter(CIsThingAlive& filter);        // 0x00F35B10
    bool IsActiveThreadTerminating();                  // 0x00F35B30
    void Main();
};

void CBH_GangGruntSpawn::Main()
{
    CScriptGameResourceObjectScriptedThingBase scripted;
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CCharString data = Me.GetDataString();
    if (data != "" && data != "NULL") {
        if (IsActiveThreadTerminating())
            return;
        WaitForTimer.SetTime(GFCharStringToInt(data));
    }
    while (!Me.MsgIsHitBy("SCRIPT_NAME_HERO")
           && !(Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_14, "SCRIPT_NAME_HERO"))
           && !Game->CanThingBeSeenByOtherThing(Game->GetHero(), Me)
           && WaitForTimer.GetTime() != 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (!ParentClass->QuestSucceeded && !ParentClass->QuestFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing exitPoint = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
    CCharString region;
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!exitPoint.IsAlive())
        return;
    if (IsActiveThreadTerminating())
        return;
    if (scripted.IsScripting()) {
        if (IsActiveThreadTerminating())
            return;
        scripted.MoveToPosition(exitPoint.GetPos(), 1.0f, (EScriptEntityMoveType)1, false, true);
    }
    while (!IsDistanceBetweenThingsUnder(Me, exitPoint, 2.0f) && !Game->MsgOnRegionLoaded(region)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    FadeOutAndKillEntity(Me, true, 1.0f, true);
}

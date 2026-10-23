// CMinionTrollSummoner::Main() -- the minion troll summoner: alive filter on Me, take scripted control (priority 4); the first
//   time (parent flag clear) play the "CS_MINIONCHASE_TROLL" cut-scene with Me cast as MINION; set the flag, then run to
//   "M_SummonerRunOffPoint" until within 2 units, face the hero, wait until Me is hit or the hero comes within 10 units,
//   release scripted control and idle until the thread terminates
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DAA700, 1133 bytes, plain `ret`, no EH frame.
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, CBase dtor 0x0099A2E0, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseIntelligentPointer ctor 0x0099A380 / dtor 0x0099A430, scripted-thing handle
//   IsScripting 0x00CD23B9 / Release 0x00CD2770 / GetScriptThing 0x007E7490 / MoveToPosition 0x007E72F0 /
//   IsPerformingScriptTask 0x007E7450 / out-of-line dtor 0x007E74D0, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 /
//   operator[] 0x00CD3D2E, handle operator= 0x008ABD10, movie ctor 0x006E7B60 / dtor 0x006E7B80, RunCutsceneMacro 0x00CBFB7D,
//   IsDistanceFromThingToPositionOver 0x00CBE45C, IsDistanceBetweenThingsUnder 0x00CBE2FF (all three __fastcall), CCharString
//   0x0099EBF0 / 0x0099EAE0, operator delete 0x00BFE9BC; GameCalls slots 7, 8 StartScriptingEntity, 70 GetHero,
//   72 GetThingWithScriptName, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities, 475 EntitySetFacingAngleTowardsThing;
//   CScriptThing slots 6 GetPos (impl slot 6), 21 MsgIsHitBy.
// Data: "MINION" 0x012D7A08, "CS_MINIONCHASE_TROLL" 0x012D7A10, "M_SummonerRunOffPoint" 0x012D7704, "" 0x0122D70E,
//   zero vector 0x0143E8E0.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (cut-scene-shown flag +0x54, descriptive).
// Shape: the landed CDF_ShineLight::Main 0x00E6EA30 handle model (implicit handle dtor expanded only at one return,
//   `inline_depth(0)` at the others) plus the CMagicBarrier::Main 0x00DA4E30 predicate holder. The run-off section sits in
//   its own block: that is what lets VC7.1 overlay the cut-scene's cast/movie slots with the position and the temporary
//   (0x44-byte frame; without the block the movie gets a slot of its own, 0x50).
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    __forceinline ~CCountedPointer()
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
struct CScriptThingImp {
    VPAD4(i0) VPAD2(i1)                                 // slots 0..5
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
};
extern const C3DVector g_ZeroPos;                       // 0x0143E8E0
struct CBase {
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
struct CScriptThing : CBase {                           // vtable 0x01238C8C, 12 bytes
    VPAD4(t1) VPAD1(t2)                                                  // slots 1..5
    virtual const C3DVector& GetPos() const { return PImp.Data == 0 ? g_ZeroPos : PImp.Data->GetPos(); }   // slot 6
    VPAD8(t7) VPAD4(t8) VPAD2(t9)                                        // slots 7..20
    virtual bool MsgIsHitBy(const CCharString& name) const;              // slot 21 (+0x54)
    CCountedPointer<CScriptThingImp> PImp;              // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing() { }                                 // out of line 0x004AA840
};
namespace NScript {
struct CPredicateBase { virtual bool operator()() const = 0; virtual CPredicateBase* Clone() const = 0; };
struct __declspec(novtable) CPredicateThing : CScriptThing {   // the predicate's held thing: same object, out-of-line dtor
    __forceinline CPredicateThing(const CScriptThing& t) : CScriptThing(t) { }
    ~CPredicateThing();                                // = ~CScriptThing 0x004AA840
};
struct CIsThingAlive : CPredicateBase {                // vtable 0x012C32F0
    CPredicateThing Thing;
    __forceinline CIsThingAlive(const CScriptThing& t) : Thing(t) { }
    virtual bool operator()() const; virtual CPredicateBase* Clone() const;
    ~CIsThingAlive() { }
};
}
struct CBaseIntelligentPointer {                        // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                        // +0x04
    CBaseIntelligentPointer();                          // 0x0099A380
    virtual ~CBaseIntelligentPointer();                 // 0x0099A430
};
struct CScriptGameResourceObjectBase : CBaseIntelligentPointer {   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    // implicit destructor; out-of-line copy 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    CScriptThing GetScriptThing() const;                // 0x007E7490
    void MoveToPosition(const C3DVector& pos, float speed, long moveType, bool a, bool b);   // 0x007E72F0
    bool IsPerformingScriptTask() const;                // 0x007E7450
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
};
struct CScriptGameResourceObjectMovieBase : CScriptGameResourceObjectBase {   // vtable 0x01260EF4, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectMovieBase();               // 0x006E7B60
    ~CScriptGameResourceObjectMovieBase();              // 0x006E7B80
};
struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    CCutsceneCastMap();                                 // 0x00CDBF70
    ~CCutsceneCastMap();                                // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CGameScriptInterface {
    VPAD4(p0) VPAD2(p1) VPAD1(p2)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(q0) VPAD16(q1) VPAD8(q2) VPAD4(q3) VPAD1(q4)                              // slots 9..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD1(r0)                                                                        // slot 71
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;      // slot 72 (+0x120)
    VPAD256(s0) VPAD32(s1) VPAD8(s2) VPAD1(s3)                                       // slots 73..369
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    VPAD8(u0)                                                                        // slots 371..378
    virtual void PauseAllNonScriptedEntities(bool pause);                            // slot 379 (+0x5ec)
    VPAD64(w0) VPAD16(w1) VPAD8(w2) VPAD4(w3) VPAD2(w4) VPAD1(w5)                    // slots 380..474
    virtual void EntitySetFacingAngleTowardsThing(const CScriptThing& thing, const CScriptThing& target, bool b) const;   // slot 475 (+0x76c)
};
struct CMinionQuestScript {
    unsigned char _pad[0x54];
    bool          ShownTrollCutscene;                   // +0x54 (descriptive)
};
struct CMinionTrollSummoner {
    void*                 __vftable;                    // +0x00
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    CMinionQuestScript*   ParentClass;                  // +0x14
    void*                 PMasterData;                  // +0x18
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00F35B10
    bool IsActiveThreadTerminating();                   // 0x00F35B30
    void Main();
};

void CMinionTrollSummoner::Main()
{
    SetPredicate(NScript::CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase summoner;
    if (summoner.IsScripting())
        summoner.Release();
    while (!Game->StartScriptingEntity(Me, summoner, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
    }
    if (IsActiveThreadTerminating())
#pragma inline_depth(0)
        return;
#pragma inline_depth()
    if (!ParentClass->ShownTrollCutscene) {
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        CCutsceneCastMap cast;
        cast["MINION"] = summoner;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        RunCutsceneMacro("CS_MINIONCHASE_TROLL", cast, 0, 0, false, true);
        game->PauseAllNonScriptedEntities(false);
    }
    ParentClass->ShownTrollCutscene = true;
    {
    CScriptThing runOffPoint = Game->GetThingWithScriptName("M_SummonerRunOffPoint");
    C3DVector runOffPos = runOffPoint.GetPos();
    while (IsDistanceFromThingToPositionOver(summoner.GetScriptThing(), runOffPos, 2.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        summoner.MoveToPosition(runOffPos, 0.0f, 1, false, true);
        while (summoner.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
    }
    if (IsActiveThreadTerminating())
#pragma inline_depth(0)
        return;
#pragma inline_depth()
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
    while (!Me.MsgIsHitBy("") && !IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 10.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
    }
    if (IsActiveThreadTerminating())
#pragma inline_depth(0)
        return;
#pragma inline_depth()
    summoner.Release();
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
    }
    }
}

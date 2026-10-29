// CMinionFieldGenerator::Main() -- the minion field generator: alive filter on Me, take scripted control (priority 4); the
//   first time (own flag clear) play the "CS_MINIONCHASE_FARM" cut-scene with Me cast as MINION and have the guildmaster
//   explain the barrier; then run to "M_SummonerToWindmill" until within 2 units, retake scripted control at priority 3,
//   face the "MagicBarrier" and idle until the thread terminates
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DAA260, 1172 bytes, plain `ret`, no EH frame.
// Callees: as the sibling CMinionTrollSummoner::Main 0x00DAA700 (same TU model), plus GameCalls slot 274
//   HeroReceiveMessageFromGuildMaster.
// Data: "MINION" 0x012D7A08, "CS_MINIONCHASE_FARM" 0x012D79F4, "TEXT_QST_045_GUILDMASTER_BARRIER" 0x012D79D0,
//   "M_SummonerToWindmill" 0x012D79B8, "MagicBarrier" 0x012D72B4, "" 0x0122D70E, zero vector 0x0143E8E0.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, cut-scene-shown flag +0x1c
//   (descriptive).
// Shape: CMinionTrollSummoner::Main with the post-cut-scene section in its own block, the cut-scene locals in a nested block
//   (their dtors run before the guildmaster message) and the position + run loop in a nested block (the final
//   GetThingWithScriptName temporary reuses its slot).
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
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
class CBaseClass {
public:
    CBaseClass();                                            // 0x0099A2D0
    virtual ~CBaseClass();                                   // 0x0099A2E0
};
class CScriptThing : public CBaseClass { public:                           // vtable 0x01238C8C, 12 bytes
    VPAD4(t1) VPAD1(t2)                                                  // slots 1..5
    virtual const C3DVector& GetPos() const { return PImp.Data == 0 ? g_ZeroPos : PImp.Data->GetPos(); }   // slot 6
    VPAD8(t7) VPAD4(t8) VPAD2(t9)                                        // slots 7..20
    virtual bool MsgIsHitBy(const CCharString& name) const;              // slot 21 (+0x54)
    CCountedPointer<CScriptThingImp> PImp;              // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing() { }                                 // out of line 0x004AA840
};
namespace NScript {
class CPredicateBase { public: virtual bool operator()() const = 0; virtual CPredicateBase* Clone() const = 0; };
class CIsThingAlive : public CPredicateBase { public:                // vtable 0x012C32F0
    CScriptThing Thing;                                // +0x04 (copied inline, destroyed out of line)
    __forceinline CIsThingAlive(const CScriptThing& t) : Thing(t) { }
    virtual bool operator()() const; virtual CPredicateBase* Clone() const;
    virtual ~CIsThingAlive();
};
// The filter's dtor is inlined into Main, but its CScriptThing member's dtor stays a call (0x004AA840).
#pragma inline_depth(0)
inline CIsThingAlive::~CIsThingAlive() { }
#pragma inline_depth()
}
class CBaseObject { public:                        // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
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
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4, 0x10 bytes
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
class CGameScriptInterface { public:
    VPAD4(p0) VPAD2(p1) VPAD1(p2)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(q0) VPAD16(q1) VPAD8(q2) VPAD4(q3) VPAD1(q4)                              // slots 9..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD1(r0)                                                                        // slot 71
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;      // slot 72 (+0x120)
    VPAD128(s0) VPAD64(s1) VPAD8(s2) VPAD1(s3)                                       // slots 73..273
    virtual void HeroReceiveMessageFromGuildMaster(const CCharString& text, const CCharString& sound, bool important, bool blocking) const;   // slot 274 (+0x448)
    VPAD64(s4) VPAD16(s5) VPAD8(s6) VPAD4(s7) VPAD2(s8) VPAD1(s9)                    // slots 275..369
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    VPAD8(u0)                                                                        // slots 371..378
    virtual void PauseAllNonScriptedEntities(bool pause);                            // slot 379 (+0x5ec)
    VPAD64(w0) VPAD16(w1) VPAD8(w2) VPAD4(w3) VPAD2(w4) VPAD1(w5)                    // slots 380..474
    virtual void EntitySetFacingAngleTowardsThing(const CScriptThing& thing, const CScriptThing& target, bool b) const;   // slot 475 (+0x76c)
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

struct CMinionFieldGenerator : public CEntityScriptBase {
    void*                 __vftable;                    // +0x00
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    void*                 ParentClass;                  // +0x14
    void*                 PMasterData;                  // +0x18
    bool                  ShownCutscene;                // +0x1c (descriptive)
    void Main();
};

void CMinionFieldGenerator::Main()
{
    SetPredicate(NScript::CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase generator;
    if (generator.IsScripting())
        generator.Release();
    while (!Game->StartScriptingEntity(Me, generator, (EScriptAIPriority)4)) {
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
    if (!ShownCutscene) {
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        {
            CCutsceneCastMap cast;
            cast["MINION"] = generator;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            RunCutsceneMacro("CS_MINIONCHASE_FARM", cast, 0, 0, false, true);
            game->PauseAllNonScriptedEntities(false);
        }
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_045_GUILDMASTER_BARRIER", "", true, true);
        ShownCutscene = true;
    }
    {
    CScriptThing windmill = Game->GetThingWithScriptName("M_SummonerToWindmill");
    {
    C3DVector windmillPos = windmill.GetPos();
    while (IsDistanceFromThingToPositionOver(generator.GetScriptThing(), windmillPos, 2.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        generator.MoveToPosition(windmillPos, 0.0f, 1, false, true);
        while (generator.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
    }
    }
#pragma inline_depth(0)
    if (IsActiveThreadTerminating())
        return;
    if (generator.IsScripting())
        generator.Release();
    while (!Game->StartScriptingEntity(Me, generator, (EScriptAIPriority)3)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("MagicBarrier"), false);
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    }
}
#pragma inline_depth()
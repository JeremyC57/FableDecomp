// CSP_DeskGuard::Main() -- the secret-passage desk guard: register an alive-filter over Me, take Me under scripted control, and until the hero is seen or heard now and then mutter about working underground while idle; then shout the spotted line and set the hero as its best enemy target
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DEDF30, length 623 bytes, plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0) over CScriptThing copy ctor 0x004ABE90 / ~CScriptThing 0x004AA840,
// RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30; scripted-thing handle (vtable 0x0127094C) inline
// ctor over CBaseObject 0x0099A380, IsScripting 0x00CD23B9, Release 0x00CD2770,,
// IsPerformingScriptTask 0x007E7450, dtor 0x007E74D0; rand 0x00BFEB16; CCharString 0x0099EBF0 /
// 0x0099EAE0; CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 364 AddNewConversation,
// 366 AddLineToConversation, 451 GiveThingBestEnemyTarget, 632 CanThingBe_Seen_ByOtherThing, 635
// CanThingBe_Heard_ByOtherThing. Literals "TEXT_QST_055_GUARD_WORKING_UNDERGROUND" 0x012DDE44,
// "TEXT_QST_055_GUARD_SURPRISED_BY_HERO" 0x012DC3FC.
// Label correction: the manifest class CSP_SecretPassageGuard is propagated; the vtable 0x012DDE28 belongs to the factory
// 0x00DEDE40 registered under "SP_DeskGuard" (0x00DECC25), so this is CSP_DeskGuard::Main.
// Layout: Ego_r.pdb CSP_DeskGuard (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
// PMasterData +0x18 (neither used here).
// Shape: the siblings CSP_SecretPassageGuard::Main 0x00DED810 / CSP_InvestigatingGuard::Main 0x00DEDB80; the landed CHTE_Audience::Main 0x00D6B240 handle (inline ctor, out-of-line dtor at every exit) and the
// CRockTrollTrigger::Main 0x00E0AD40 scoped filter.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void t00(); virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05();
    virtual const C3DVector& GetPosition() const;      // slot 6 (+0x18)
    void* PImp[2];
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
struct CIsThingAlive : public NScript::CPredicateBase {                                 // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CCountedPointerWords { void* Data; void* Info; };
class CBaseObject { public:
    void* m_pIntelligentPointer;
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C
    CCountedPointerWords PImp;
    CScriptGameResourceObjectScriptedThingBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    void MoveToPosition(const C3DVector& pos, float speed, long moveType, bool a, bool b);   // 0x007E72F0
public:
    virtual bool IsPerformingScriptTask() const;   // 0x007E7450
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern "C" int __cdecl rand();                         // 0x00BFEB16
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CEntityScriptBase { public:
    virtual void e00();
    class CGameScriptInterface* Game;                 // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
class CGameScriptInterface { public:
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD1(qe)                              // slots 9..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD256(ra) VPAD32(rb) VPAD4(rc) VPAD1(rd)                                       // slots 71..363
    virtual long AddNewConversation(const CScriptThing& speaker, bool a, bool b) const;   // slot 364 (+0x5b0)
    VPAD1(sa)                                                                        // slot 365
    virtual void AddLineToConversation(long conversation, const CCharString& text, bool a, const CScriptThing& speaker, const CScriptThing& listener) const;   // slot 366 (+0x5b8)
    VPAD64(ta) VPAD16(tb) VPAD4(tc)                                                  // slots 367..450
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const;   // slot 451 (+0x70c)
    VPAD128(ua) VPAD32(ub) VPAD16(uc) VPAD4(ud)                                      // slots 452..631
    virtual bool CanThingBe_Seen_ByOtherThing(const CScriptThing& seen, const CScriptThing& looker) const;   // slot 632 (+0x9e0)
    VPAD2(va)                                                                        // slots 633..634
    virtual bool CanThingBe_Heard_ByOtherThing(const CScriptThing& heard, const CScriptThing& listener) const;   // slot 635 (+0x9ec)
};
struct CSP_DeskGuard : CEntityScriptBase {
    void* ParentClass;                                 // +0x14 CQ_SecretPassageScript*
    void* PMasterData;                                 // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};

void CSP_DeskGuard::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsScripting())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, (EScriptAIPriority)3)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(), Me) && !Game->CanThingBe_Heard_ByOtherThing(Game->GetHero(), Me)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!handle.IsPerformingScriptTask() && rand() % 500 == 0) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddLineToConversation(conversation, "TEXT_QST_055_GUARD_WORKING_UNDERGROUND", false, Me, Game->GetHero());
        }
    }
    if (IsActiveThreadTerminating())
        return;
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddLineToConversation(conversation, "TEXT_QST_055_GUARD_SURPRISED_BY_HERO", false, Me, Game->GetHero());
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    if (handle.IsScripting())
        handle.Release();
}
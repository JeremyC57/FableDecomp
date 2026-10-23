// CCorneredGuard::Main() -- the cornered guard: register the alive filter, find the nearest MinionAttackingGuard, set the two fighting each other, then (once) heal whichever of the two is hit while the attacker lives and the hero is still over 15 units away, show the guard's health bar, call for help if the attacker still lives, wait for it to die, reward morality, thank the hero and follow him; finally spawn the parent's GuardChatter on Me, wait for the mission to end and remove Me
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA7830, length 1280 bytes, plain `ret`. Sibling of
//   CBalverineFightingGuard::Main 0x00DA6D50 (same TU family, same model).
// Callees: as 0x00DA6D50, plus IsDistanceBetweenThingsOver @ 0x00CBE3EA (`__fastcall`), CScriptThing::IsAlive @ 0x004AB130
//   (inlined in the first wait, a direct call at the call-for-help test, virtual in the last wait), CScriptThing slot 21
//   MsgIsHitBy (inlined on the local attacker, virtual on Me) and CGameScriptInterface slot 265 ModifyThingHealth.
// Literals: "MinionAttackingGuard" 0x012D7274, "" 0x0122D70E, "CorneredGuard" 0x012D724C, "HUD_QUEST_ICON_GUARD"
//   0x012D2B54, "TEXT_QST_045_GUARD_CALL_FOR_HELP" 0x012D7814, "TEXT_QST_045_CORNERED_GUARD_THANKS" 0x012D77F0,
//   "ParentClass." 0x012C9934, "GuardChatter" 0x012D7738. g_pScriptDef 0x0143E90C (+0xd50 the rescue morality).
// Layout: Ego_r.pdb CCorneredGuard (36 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData
//   +0x18, AskedForHerosHelp +0x1c, HealthBarID +0x20; CQ_MinionClifftopChaseScript retail = PDB - 0x14.
// Shape: the 0x00DA6D50 model (inline copy of a non-const thing for the by-value spawn argument, out-of-line const copy
//   for the filter and the functor member; the last wait calls IsAlive through a reference so it stays virtual).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
struct CRGBColour {
    unsigned char B, G, R, A;
    __forceinline CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CCPPointerInfo { long References; };
struct CScriptThing;
struct CCPPointerImp {                                 // CCountedPointer<CScriptThingImp>: Data, Info
    CScriptThing*   Data;
    CCPPointerInfo* Info;
    __forceinline CCPPointerImp(const CCPPointerImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->References; }
};
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
    VPAD16(ta) VPAD4(tb) VPAD1(tc)                     // slots 0..20
    virtual bool MsgIsHitBy(const CCharString& name) const   // slot 21 (+0x54), 0x004AAB30
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitBy(name);
    }
    VPAD32(td) VPAD16(te) VPAD4(tf) VPAD1(tg)          // slots 22..74
    virtual bool IsAlive() const                       // slot 75 (+0x12c), 0x004AB130
    {
        if (PImp.Data && PImp.Data->IsAlive())
            return true;
        return false;
    }
    CCPPointerImp PImp;                                // +0x04
    CScriptThing();                                    // 0x006E7B40
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    __forceinline CScriptThing(CScriptThing& other) : PImp(other.PImp) { }   // the inline copy of a non-const thing
    ~CScriptThing();                                   // 0x004AA840
};
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE3EA
struct CScriptThingView {
    void* vptr; void* Data; void* Info;
    const CScriptThing& Thing() const { return *reinterpret_cast<const CScriptThing*>(this); }
};
struct CGameScriptInterface {
    VPAD4(p0x0) VPAD2(p0x1) VPAD1(p0x2)   // slots 0..6
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    VPAD32(p1x0) VPAD16(p1x1) VPAD8(p1x2) VPAD4(p1x3) VPAD2(p1x4)   // slots 8..69
    virtual const CScriptThing& GetHero() const;   // slot 70 (+0x118)
    VPAD4(p2x0) VPAD2(p2x1)   // slots 71..76
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing& thing, const CCharString& name) const;   // slot 77 (+0x134)
    VPAD16(p3x0) VPAD8(p3x1) VPAD4(p3x2) VPAD2(p3x3)   // slots 78..107
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
    VPAD32(p4x0) VPAD8(p4x1) VPAD4(p4x2) VPAD2(p4x3) VPAD1(p4x4)   // slots 109..155
    virtual void GiveHeroMorality(long amount) const;   // slot 156 (+0x270)
    VPAD64(p5x0) VPAD8(p5x1)   // slots 157..228
    virtual void EntityFollowThing(const CScriptThing& thing, const CScriptThing& target, float distance, bool a) const;   // slot 229 (+0x394)
    VPAD32(p6x0) VPAD2(p6x1) VPAD1(p6x2)   // slots 230..264
    virtual void ModifyThingHealth(const CScriptThing& thing, float amount, bool a) const;   // slot 265 (+0x424)
    VPAD32(p7x0) VPAD16(p7x1) VPAD4(p7x2) VPAD2(p7x3) VPAD1(p7x4)   // slots 266..320
    virtual void DisplayQuestInfo(bool show) const;   // slot 321 (+0x504)
    VPAD2(p8x0) VPAD1(p8x1)   // slots 322..324
    virtual long AddQuestInfoBarHealth(const CScriptThing& thing, const CRGBColour& colour, const CCharString& icon, float a) const;   // slot 325 (+0x514)
    VPAD32(p9x0) VPAD4(p9x1) VPAD2(p9x2)   // slots 326..363
    virtual long AddNewConversation(const CScriptThing& thing, bool a, bool b) const;   // slot 364 (+0x5b0)
    VPAD1(p10x0)   // slots 365..365
    virtual void AddLineToConversation(long conversation, const CCharString& line, bool a, const CScriptThing& speaker, const CScriptThing& listener) const;   // slot 366 (+0x5b8)
    VPAD64(p11x0) VPAD16(p11x1) VPAD4(p11x2)   // slots 367..450
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const;   // slot 451 (+0x70c)
    VPAD16(p12x0) VPAD8(p12x1) VPAD4(p12x2)   // slots 452..479
    virtual void SetEntityAsRegionFollowing(const CScriptThing& thing, const CScriptThing& leader, bool follow) const;   // slot 480 (+0x780)
};
struct CScriptDef {
    unsigned char _pad[0xd44];
    unsigned char _pad2[0xd50 - 0xd44];
    long          GuardRescueMorality;                 // +0xd50
};
extern CScriptDef* g_pScriptDef;                       // 0x0143E90C
struct CSpawnedFuncBase {                              // retail 0x34 bytes
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];                         // +0x04 .. +0x34
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};
template <class T, class P1> struct CSpawnedFunc1 : CSpawnedFuncBase {   // 0x48 bytes
    void (T::*Func)(P1);                               // +0x34
    T* PClass;                                         // +0x38
    P1 Par1;                                           // +0x3c
    __forceinline CSpawnedFunc1(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(P1), const P1 par1)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) {}
    virtual void SuspendableProcess() { (PClass->*Func)(Par1); }
};
struct CScriptBase {                                   // retail 0x40 bytes
    unsigned char _body[0x40];
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
};
struct CQ_MinionClifftopChaseScript : CScriptBase {
    unsigned char _pad40[0x49 - 0x40];
    bool MissionFailed;                                // +0x49
    bool MissionSucceeded;                             // +0x4a
    bool QuestStartScreened;                           // +0x4b
    void GuardChatter(CScriptThing guard);             // 0x00DA71E0
};
struct CIsThingAlive {                                 // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
struct CQ_SunnyvaleMasterData;
class CCorneredGuard {
public:
    void*                         __vftable;           // +0x00
    CGameScriptInterface*         Game;                // +0x04
    CScriptThing                  Me;                  // +0x08
    CQ_MinionClifftopChaseScript* ParentClass;         // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;         // +0x18
    bool                          AskedForHerosHelp;   // +0x1c
    long                          HealthBarID;         // +0x20
    void RegisterFilter(CIsThingAlive& filter);        // 0x00F35B10
    bool IsActiveThreadTerminating();                  // 0x00F35B30
    void Main();
};

void CCorneredGuard::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing attacker = Game->GetNearestWithScriptName(Game->GetHero(), "MinionAttackingGuard");
    if (!AskedForHerosHelp) {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveThingBestEnemyTarget(Me, attacker);
        Game->GiveThingBestEnemyTarget(attacker, Me);
        while (attacker.IsAlive() && IsDistanceBetweenThingsOver(Me, Game->GetHero(), 15.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (Me.MsgIsHitBy("")) {
                if (IsActiveThreadTerminating())
                    return;
                Game->ModifyThingHealth(Me, 100.0f, false);
            }
            if (attacker.MsgIsHitBy("CorneredGuard")) {
                if (IsActiveThreadTerminating())
                    return;
                Game->ModifyThingHealth(attacker, 100.0f, false);
            }
        }
        if (IsActiveThreadTerminating())
            return;
        Game->DisplayQuestInfo(true);
        HealthBarID = Game->AddQuestInfoBarHealth(Me, CRGBColour(0, 255, 0, 255), "HUD_QUEST_ICON_GUARD", 1.0f);
        if (attacker.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddLineToConversation(conversation, "TEXT_QST_045_GUARD_CALL_FOR_HELP", false, Me, CScriptThing());
        }
        AskedForHerosHelp = true;
        while (reinterpret_cast<const CScriptThingView&>(attacker).Thing().IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->GiveHeroMorality(g_pScriptDef->GuardRescueMorality);
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddLineToConversation(conversation, "TEXT_QST_045_CORNERED_GUARD_THANKS", false, Me, CScriptThing());
        Game->EntityFollowThing(Me, Game->GetHero(), 3.0f, true);
        Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
    }
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CQ_MinionClifftopChaseScript, CScriptThing>(CCharString("ParentClass.") + CCharString("GuardChatter"), 0, ParentClass, &CQ_MinionClifftopChaseScript::GuardChatter, Me), CCharString(""));
    while (!ParentClass->MissionFailed && !ParentClass->MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, false, true);
}
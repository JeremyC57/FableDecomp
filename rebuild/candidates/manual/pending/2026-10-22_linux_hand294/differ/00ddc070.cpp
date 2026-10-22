// Grade: DIFFER(611v597) -- one block-placement residue: retail enters the final idle loop (`while (!IsActiveThreadTerminating()) NewScriptFrame();`) through a `jmp` to its bottom test, ours inverts it with a duplicated top test (+14 bytes); everything else matches with the CBowerstoneIntroGuard explicit-destroy model; tried while/for(;;)/goto-entered do-while idle loops, a for(;;) IsPerformingScriptTask loop, and the earlier implicit-destructor + inline_depth model (600v597, HeroWonRace wait loop rotated)
// CPrisonRaceWinningPostGuard::Main() -- the guard at the prison race finish: take scripted control, sheathe, stand at GuardPosition facing its angle; once the hero wins the race salute and then idle until the script ends
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DDC070, length 597 bytes (padded to 608), plain ret.
// Callees: CBaseIntelligentPointer ctor 0x0099A380 / dtor 0x0099A430; the scripted-thing handle's IsScripting 0x00CD23B9,
// Release 0x00CD2770, PlayAnimation 0x007E73D0 (slot 18), IsPerformingScriptTask 0x007E7450 (slot 26), out-of-line
// destructor 0x007E74D0; ~CScriptThing 0x004AA840; CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; operator
// delete 0x00BFE9BC; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; the animation-default bool
// 0x01375748; CScriptThing slot 10 GetAngleXY; CGameScriptInterface slots 7, 8 StartScriptingEntity, 77
// GetNearestWithScriptName, 472 EntityTeleportToThing, 474 EntitySetFacingAngle, 506 EntitySheatheWeapons.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8} + ParentClass +0x14 (CQ_PrisonRaceScript, Ego_r.pdb - 0x14:
// HeroWonRace +0x48); the handle as the landed CMeleeThunder::Main 0x00D58080.
// Shape: the landed CBowerstoneIntroGuard::Main 0x00D246C0 model (destructor-less handle view destroyed by hand at every exit; the
// inline copy only where retail expands it); the HeroWonRace wait is `for (;;) { if (terminating) { inline destroy; return; } if (won) break;
// NewScriptFrame(); }`, so one inline copy serves the pre-loop and in-loop exits as in retail.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CCountedPointerWords {
    void*           Data;
    CCPPointerInfo* Info;
    CCountedPointerWords& operator=(const CCountedPointerWords& other);   // 0x008AB1E0
    __forceinline void ReleaseInline()
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
struct CScriptThing {                                  // vtable 0x01238C8C
    virtual ~CScriptThing();                           // slot 0, out-of-line 0x004AA840
    virtual void t001() const; virtual void t002() const; virtual void t003() const; virtual void t004() const;
    virtual void t005() const; virtual void t006() const; virtual void t007() const; virtual void t008() const; virtual void t009() const;
    virtual float GetAngleXY() const;                  // slot 10 (+0x28)
    CCountedPointerWords PImp;
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CBaseObject {
    virtual void b000();
    void* IntelligentPointer;
    CBaseObject();                                     // 0x0099A380
    CBaseObject& operator=(const CBaseObject& other);  // 0x0099A3B0
};
struct CBaseObjectDtor {
    void* IntelligentPointer;
    virtual ~CBaseObjectDtor();                        // 0x0099A430
};
struct CScriptGameResourceObjectBaseDtor : CBaseObjectDtor {   // vtable 0x0126008C
    __forceinline virtual ~CScriptGameResourceObjectBaseDtor() { }
};
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C
    CCountedPointerWords PImp;
    CScriptGameResourceObjectScriptedThingBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006();
    virtual void s007(); virtual void s008(); virtual void s009(); virtual void s010(); virtual void s011(); virtual void s012();
    virtual void s013(); virtual void s014(); virtual void s015(); virtual void s016(); virtual void s017();
    virtual void PlayAnimation(const CCharString& anim, bool a, bool b, bool c, bool d, bool e, bool f, bool g);   // slot 18 0x007E73D0
    virtual void s019(); virtual void s020(); virtual void s021(); virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025();
    virtual bool IsPerformingScriptTask() const;       // slot 26 0x007E7450
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    void Destroy();                                    // 0x007E74D0
    __forceinline void DestroyInline()
    {
        PImp.ReleaseInline();
        reinterpret_cast<CScriptGameResourceObjectBaseDtor*>(this)->CScriptGameResourceObjectBaseDtor::~CScriptGameResourceObjectBaseDtor();
    }
};
struct CScriptedThingStorage {                         // a scripted-thing handle built out of line (0x10 bytes)
    void* _body[4];
    void Construct();                                  // 0x007E72A0 (CScriptGameResourceObjectScriptedThingBase ctor, out of line)
    CScriptGameResourceObjectScriptedThingBase& Object() { return *reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(this); }
};
struct CScriptGameResourceObjectMovieBase {            // vtable 0x01260EF4, 0x10 bytes
    void* _body[4];
    void Construct();                                  // 0x006E7B60 (out-of-line ctor)
    void Destroy();                                    // 0x006E7B80 (out-of-line dtor)
};
struct CCutsceneCastMap {                              // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    void Construct();                                  // 0x00CDBF70
    void Destroy();                                    // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;                               // map<CCharString, bool>
struct CCutsceneArgMap;                                // map<CCharString, CCharString>
// PDB ?RunCutsceneMacro@@YIXABVCCharString@@AAV?$map@...@PAV?$map@VCCharString@@_N...@PAV?$map@VCCharString@@V1@...@_N4@Z
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
struct CGameScriptInterface {
    VPAD4(pa) VPAD2(pb) VPAD1(pc)                                                    // slots 0..6
    virtual void NewScriptFrame() const;                                             // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD1(qe)                              // slots 9..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD4(ra) VPAD2(rb)                                                              // slots 71..76
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing& thing, const CCharString& name) const;   // slot 77 (+0x134)
    VPAD256(sa) VPAD32(sb) VPAD4(sc)                                                 // slots 78..369
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    virtual void FixMovieSequenceCamera(bool fix) const;                             // slot 371 (+0x5cc)
    VPAD4(ta) VPAD2(tb) VPAD1(tc)                                                    // slots 372..378
    virtual void PauseAllNonScriptedEntities(bool pause);                            // slot 379 (+0x5ec)
    VPAD64(ua) VPAD16(ub) VPAD8(uc) VPAD4(ud)                                        // slots 380..471
    virtual void EntityTeleportToThing(const CScriptThing& thing, const CScriptThing& target, bool a) const;   // slot 472 (+0x760)
    VPAD1(va)                                                                        // slot 473
    virtual void EntitySetFacingAngle(const CScriptThing& thing, float angle, bool a) const;   // slot 474 (+0x768)
    VPAD16(wa) VPAD8(wb) VPAD1(wc)                                                   // slots 475..499
    virtual void EntityForceToLookAtNothing(const CScriptThing& thing) const;        // slot 500 (+0x7d0)
    VPAD4(xa) VPAD1(xb)                                                              // slots 501..505
    virtual void EntitySheatheWeapons(const CScriptThing& thing, bool sheathe) const;   // slot 506 (+0x7e8)
    VPAD128(ya) VPAD32(yb) VPAD16(yc) VPAD8(yd) VPAD4(ye) VPAD2(yf)                  // slots 507..696
    virtual void StopOverrideMusic(bool a) const;                                    // slot 697 (+0xae4)
};
struct CEntityScriptBase {
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CQ_PrisonRaceScript {
    unsigned char _pad[0x48];
    bool HeroWonRace;                                  // +0x48 (PDB +0x5c)
};
extern bool g_AnimationDefaultFlag;                    // 0x01375748
struct CPrisonRaceWinningPostGuard : CEntityScriptBase {
    CQ_PrisonRaceScript* ParentClass;                  // +0x14
    void*                PMasterData;                  // +0x18
    void Main();
};

void CPrisonRaceWinningPostGuard::Main()
{
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) {
            scripted.DestroyInline();
            return;
        }
    }
    if (IsActiveThreadTerminating()) {
        scripted.Destroy();
        return;
    }
    Game->EntitySheatheWeapons(Me, false);
    Game->EntityTeleportToThing(Me, Game->GetNearestWithScriptName(Me, "GuardPosition"), false);
    Game->EntitySetFacingAngle(Me, Game->GetNearestWithScriptName(Me, "GuardPosition").GetAngleXY(), true);
    for (;;) {
        if (IsActiveThreadTerminating()) {
            scripted.DestroyInline();
            return;
        }
        if (ParentClass->HeroWonRace)
            break;
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating()) {
        scripted.Destroy();
        return;
    }
    scripted.PlayAnimation("ST_SALUTE", false, false, false, true, g_AnimationDefaultFlag, false, false);
    while (scripted.IsPerformingScriptTask()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) {
            scripted.Destroy();
            return;
        }
    }
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
    scripted.Destroy();
}

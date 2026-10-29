// CPrisonRaceWarden::Main() -- the prison race warden: take Me under scripted control, stand at GuardPosition facing its angle, wait until the race is over and the hero is within 5 units, then mark the hero's win and play the race-end cut-scene (the repeat version once the race was ever won before) with the hero and guard cast inside a paused movie sequence, mark the race ended and idle until the script ends
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DDD030, length 1019 bytes (padded to 1024), plain `ret`.
// Callees: scripted-thing handle (vtable 0x0127094C) inline ctor over CBaseObject 0x0099A380, IsScripting 0x00CD23B9,
// Release 0x00CD2770, out-of-line ctor 0x007E72A0 / dtor 0x007E74D0 / operator= 0x008ABD10, inline-once dtor (vtable
// 0x0126008C, ~CBaseObject 0x0099A430, operator delete 0x00BFE9BC); IsActiveThreadTerminating 0x00F35B30; ~CScriptThing
// 0x004AA840; CCharString 0x0099EBF0 / 0x0099EAE0; cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E;
// movie handle ctor 0x006E7B60 / dtor 0x006E7B80; IsDistanceBetweenThingsUnder 0x00CBE2FF and RunCutsceneMacro 0x00CBFB7D
// (both __fastcall); CScriptThing slot 10 GetAngleXY; CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity,
// 70 GetHero, 77 GetNearestWithScriptName, 370 StartMovieSequence, 371 FixMovieSequenceCamera, 379
// PauseAllNonScriptedEntities, 472 EntityTeleportToThing, 474 EntitySetFacingAngle, 500 EntityForceToLookAtNothing, 506
// EntitySheatheWeapons, 697 StopOverrideMusic. Literals "GuardPosition" 0x012DC9D8, "HERO" 0x01255174, "GUARD"
// 0x012CB384, "" 0x0122D70E, "CS_PRISON_RACE_END" 0x012DCADC, "CS_PRISON_RACE_END_REPEAT" 0x012DCAC0.
// Layout: Ego_r.pdb CPrisonRaceWarden (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData
// +0x18. CQ_PrisonRaceScript HeroWonRace / RaceEnded PDB +0x5c / +0x5e = retail +0x48 / +0x4a (quest-script base 0x14
// smaller, as the parked sibling CPrisonRaceWinningPostGuard::Main 0x00DDC070); CQ_SunnyvaleMasterData
// WasPrisonRaceEverWonByHero +0xed (PDB, unshifted).
// Shape: the landed CBowerstoneIntroGuard::Main 0x00D246C0 model (destructor-less handle views destroyed by hand at every
// exit, the guard handle's destructor inline only at the first loop's return); the hero handle is built out of line, so it
// is a storage view with an out-of-line Construct; Game is cached in a local for the paired PauseAllNonScriptedEntities;
// the terminate test duplicated in both cut-scene arms is hoisted by VC; the final idle loop is a do-while.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
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
class CScriptThing { public:                                  // vtable 0x01238C8C
    virtual ~CScriptThing();                           // slot 0, out-of-line 0x004AA840
    virtual void t001() const; virtual void t002() const; virtual void t003() const; virtual void t004() const;
    virtual void t005() const; virtual void t006() const; virtual void t007() const; virtual void t008() const; virtual void t009() const;
    virtual float GetAngleXY() const;                  // slot 10 (+0x28)
    CCountedPointerWords PImp;
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
class CBaseObject { public:
    virtual void b000();
    void* IntelligentPointer;
protected:
    CBaseObject();                                     // 0x0099A380
    CBaseObject& operator=(const CBaseObject& other);  // 0x0099A3B0
public:
};
struct CBaseObjectDtor {
    void* IntelligentPointer;
    virtual ~CBaseObjectDtor();                        // 0x0099A430
};
struct CScriptGameResourceObjectBaseDtor : CBaseObjectDtor {   // vtable 0x0126008C
    __forceinline virtual ~CScriptGameResourceObjectBaseDtor() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C
    CCountedPointerWords PImp;
    CScriptGameResourceObjectScriptedThingBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void s001();
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
class CScriptGameResourceObjectMovieBase { public:            // vtable 0x01260EF4, 0x10 bytes
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
class CGameScriptInterface { public:
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
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CQ_PrisonRaceScript {
    unsigned char _pad[0x48];
    bool HeroWonRace;                                  // +0x48 (PDB +0x5c)
    bool RaceStarted;                                  // +0x49
    bool RaceEnded;                                    // +0x4a
};
struct CQ_SunnyvaleMasterData {
    unsigned char _pad[0xed];
    bool WasPrisonRaceEverWonByHero;                   // +0xed
};
struct CPrisonRaceWarden : CEntityScriptBase {
    CQ_PrisonRaceScript*    ParentClass;               // +0x14
    CQ_SunnyvaleMasterData* PMasterData;               // +0x18
    void Main();
};

void CPrisonRaceWarden::Main()
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
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) {
        scripted.Destroy();
        return;
    }
    Game->EntitySheatheWeapons(Me, false);
    Game->EntityTeleportToThing(Me, Game->GetNearestWithScriptName(Me, "GuardPosition"), false);
    Game->EntitySetFacingAngle(Me, Game->GetNearestWithScriptName(Me, "GuardPosition").GetAngleXY(), true);
    Game->EntityForceToLookAtNothing(Me);
    if (IsActiveThreadTerminating()) {
        scripted.Destroy();
        return;
    }
    while (ParentClass->RaceEnded || !IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 5.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) {
            scripted.Destroy();
            return;
        }
    }
    if (IsActiveThreadTerminating()) {
        scripted.Destroy();
        return;
    }
    bool firstTime = !PMasterData->WasPrisonRaceEverWonByHero;
    ParentClass->HeroWonRace = true;
    Game->StopOverrideMusic(false);
    {
        CScriptedThingStorage heroStorage;
        heroStorage.Construct();
        CScriptGameResourceObjectScriptedThingBase& hero = heroStorage.Object();
        Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
        CCutsceneCastMap cast;
        cast.Construct();
        cast["HERO"] = hero;
        cast["GUARD"] = scripted;
        CScriptGameResourceObjectMovieBase movie;
        movie.Construct();
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        if (firstTime) {
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                movie.Destroy();
                cast.Destroy();
                hero.Destroy();
                scripted.Destroy();
                return;
            }
            RunCutsceneMacro("CS_PRISON_RACE_END", cast, 0, 0, false, true);
        } else {
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                movie.Destroy();
                cast.Destroy();
                hero.Destroy();
                scripted.Destroy();
                return;
            }
            RunCutsceneMacro("CS_PRISON_RACE_END_REPEAT", cast, 0, 0, false, true);
        }
        Game->FixMovieSequenceCamera(false);
        game->PauseAllNonScriptedEntities(false);
        movie.Destroy();
        cast.Destroy();
        hero.Destroy();
    }
    ParentClass->RaceEnded = true;
    do
        Game->NewScriptFrame();
    while (!IsActiveThreadTerminating());
    scripted.Destroy();
}
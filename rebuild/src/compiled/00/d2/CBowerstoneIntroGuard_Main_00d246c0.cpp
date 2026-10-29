// CBowerstoneIntroGuard::Main() -- Bowerstone town-life intro guard: hold an alive filter over Me and take Me under scripted control (priority 4); unless the parent script already played the guard speech, take the hero too, cast both ("HERO", "Guard") in the "CS_BOWERSTONE_INTRO_TOWN" cut-scene inside a paused movie sequence with a fixed camera, add the law-and-order and pubs log-book tutorial entries and mark DoneGuardSpeech; finally release the guard from scripted control
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D246C0, 896 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 (CIsThingAlive filter vtable 0x012C32F0, inline dtor),
//   CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CBaseObject ctor 0x0099A380 /
//   operator= 0x0099A3B0 / dtor 0x0099A430, scripted-thing handle (vtable 0x0127094C) IsScripting 0x00CD23B9 / Release 0x00CD2770 /
//   out-of-line dtor 0x007E74D0, the handle's CCountedPointer operator= 0x008AB1E0, cast map (map<CCharString, handle>)
//   ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, movie handle (vtable 0x01260EF4) PImp dtor 0x006E7AB0,
//   CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0, RunCutsceneMacro 0x00CBFB7D (__fastcall),
//   AddLogbookTutorialEntry 0x00CBE9EE (__fastcall), operator delete 0x00BFE9BC; CGameScriptInterface slots 7 NewScriptFrame,
//   8 StartScriptingEntity, 70 GetHero, 370 StartMovieSequence, 371 FixMovieSequenceCamera, 379 PauseAllNonScriptedEntities.
//   Literals "HERO" 0x01255174, "Guard" 0x01266D4C, "" 0x0122D70E, "CS_BOWERSTONE_INTRO_TOWN" 0x012CB674,
//   "TEXT_QST_LOG_TOWNS_LAWANDORDER" 0x012CB654, "TEXT_QST_LOG_TOWNS_PUBS" 0x012CB63C.
// Layout: Ego_r.pdb CBowerstoneIntroGuard (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18;
//   CQ_BowerstoneTownLifeIntroScript::DoneGuardSpeech PDB +0x5d = retail +0x49 (script base 0x14 smaller, as the landed CQ_* rows).
// Shape: the inline-once destructor band (CBalvMarker::Main idiom): the guard handle's destructor is expanded only at the first
//   loop's return, out of line elsewhere, so all three handles and the cast map are destructor-less views destroyed by hand at every
//   exit; the movie handle's ctor must be __forceinline (VC7.1 otherwise calls it out of line); the handle's operator= is an
//   explicit __forceinline base-then-member copy (the implicit one is emitted out of line).
namespace NScript { class CPredicateBase; }
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
class CScriptThing { public:
    virtual void t000();
    CCountedPointerWords PImp;
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
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
    __forceinline CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other)
    {
        CBaseObject::operator=(other);
        PImp = other.PImp;
        return *this;
    }
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    void Destroy();                                    // 0x007E74D0
    __forceinline void DestroyInline()
    {
        PImp.ReleaseInline();
        reinterpret_cast<CScriptGameResourceObjectBaseDtor*>(this)->CScriptGameResourceObjectBaseDtor::~CScriptGameResourceObjectBaseDtor();
    }
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    ~CMovieImpPointer();                               // 0x006E7AB0
};
struct CMovieImpWords { void* Data; void* Info; };
class CScriptGameResourceObjectMovieBase : public CBaseObject { public:   // vtable 0x01260EF4
    CMovieImpWords PImp;
    __forceinline CScriptGameResourceObjectMovieBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void m001();
    __forceinline void DestroyInline()
    {
        reinterpret_cast<CMovieImpPointer*>(&PImp)->~CMovieImpPointer();
        reinterpret_cast<CScriptGameResourceObjectBaseDtor*>(this)->CScriptGameResourceObjectBaseDtor::~CScriptGameResourceObjectBaseDtor();
    }
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
void __fastcall AddLogbookTutorialEntry(const CCharString& tag);   // 0x00CBE9EE
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
    virtual void NewScriptFrame() const;                                             // slot 7
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    VPAD32(qa) VPAD16(qb) VPAD8(qc) VPAD4(qd) VPAD1(qe)                              // slots 9..69
    virtual CScriptThing& GetHero();                                                 // slot 70 (+0x118)
    VPAD256(ra) VPAD32(rb) VPAD8(rc) VPAD2(rd) VPAD1(re)                             // slots 71..369
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5C8)
    virtual void FixMovieSequenceCamera(bool fix) const;                             // slot 371 (+0x5CC)
    VPAD4(sa) VPAD2(sb) VPAD1(sc)                                                    // slots 372..378
    virtual void PauseAllNonScriptedEntities(bool pause);                            // slot 379 (+0x5EC)
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void f00();
};
}
struct CIsThingAlive : public NScript::CPredicateBase {                                 // vtable 0x012C32F0
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;
    CScriptThing          Me;
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CQ_BowerstoneTownLifeIntroScript {
    unsigned char _pad[0x49];
    bool DoneGuardSpeech;                              // +0x49 (PDB +0x5d)
};
struct CBowerstoneIntroGuard : CEntityScriptBase {
    CQ_BowerstoneTownLifeIntroScript* ParentClass;     // +0x14
    void* PMasterData;                                 // +0x18
    void Main();
};

void CBowerstoneIntroGuard::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
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
    if (!ParentClass->DoneGuardSpeech) {
        if (IsActiveThreadTerminating()) {
            scripted.Destroy();
            return;
        }
        CCutsceneCastMap cast;
        cast.Construct();
        CScriptGameResourceObjectScriptedThingBase hero;
        while (!Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) {
                hero.Destroy();
                cast.Destroy();
                scripted.Destroy();
                return;
            }
        }
        if (IsActiveThreadTerminating()) {
            hero.Destroy();
            cast.Destroy();
            scripted.Destroy();
            return;
        }
        cast["HERO"] = hero;
        cast["Guard"] = scripted;
        {
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_BOWERSTONE_INTRO_TOWN", cast, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            AddLogbookTutorialEntry("TEXT_QST_LOG_TOWNS_LAWANDORDER");
            AddLogbookTutorialEntry("TEXT_QST_LOG_TOWNS_PUBS");
            ParentClass->DoneGuardSpeech = true;
            game->PauseAllNonScriptedEntities(false);
            movie.DestroyInline();
        }
        hero.Destroy();
        cast.Destroy();
    }
    if (scripted.IsScripting())
        scripted.Release();
    scripted.Destroy();
}
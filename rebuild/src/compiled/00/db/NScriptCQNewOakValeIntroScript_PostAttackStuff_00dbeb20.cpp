// CQ_NewOakValeIntroScript::PostAttackStuff() -- wait for the post-attack start marker to appear, then teleport the hero to it, put Oakvale in limbo, take the teddy bear, log the story, reset the camera and music, fade in, wait for the hero to reach his dad's trigger and play the "he's dead, Jim" cut-scene, then lift the limbo, restart the clock and close the post-attack quest
// Leaf: PostAttackStuff
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DBEB20, 1095 bytes.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, ~CScriptThing 0x004AA840 (and its inline body: counted-pimpl release,
//   operator delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0), IsActiveThreadTerminating 0x00CB7940, AddLogbookStoryEntry
//   0x00CBE87F and IsDistanceBetweenThingsUnder 0x00CBE2FF and RunCutsceneMacro 0x00CBFB7D (all __fastcall); handle / cast
//   map / movie helpers as the landed CRockTrollTrigger::Main 0x00E0AD40; CGameScriptInterface slots 7, 8, 70, 72, 125, 280,
//   341, 370, 371, 374, 379, 416, 417, 440, 472, 648, 657, 697, 698.
// Layout: Game +0x40, TimeStoppedToken +0x4c, HesDeadJimStarted +0x51.
// Shape: the marker test's CScriptThing temporary is destroyed inline (VC7.1 inlines the dtor at that first site only).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct SCountBlock {
    long  m_references;                                 // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4
    void* m_pObject;                                    // +8
};
struct CScriptThingImp;
template <class T> class CCountedPointer { public:                               // CCountedPointer<CScriptThingImp>
    CScriptThingImp* Data;
    SCountBlock*     Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
class CScriptGameResourceObjectScriptedThingBase { public:    // vtable 0x0127094C, 0x10 bytes
    void* _body[4];
    void Construct();                                  // 0x007E72A0 (out-of-line ctor)
    void Destroy();                                    // 0x007E74D0 (out-of-line dtor)
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
};
class CScriptGameResourceObjectMovieBase { public:            // vtable 0x01260EF4, 0x10 bytes
    void* _body[4];
    void Construct();                                  // 0x006E7B60
    void Destroy();                                    // 0x006E7B80
};
struct CCutsceneCastMap {                              // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    void Construct();                                  // 0x00CDBF70
    void Destroy();                                    // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void __fastcall AddLogbookStoryEntry(long entry);       // 0x00CBE87F
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_NewOakValeIntroScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[4];
    long                  TimeStoppedToken;             // +0x4c
    bool                  IntroDone;                    // +0x50
    bool                  HesDeadJimStarted;            // +0x51
    void PostAttackStuff();
};
} // namespace NScript
using NScript::CQ_NewOakValeIntroScript;

void NScript::CQ_NewOakValeIntroScript::PostAttackStuff()
{
    while (!Game->GetThingWithScriptName("M_PostAttackStart").IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->CacheMusicSet((EMusicSetType)0x2d);
    Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("M_PostAttackStart"), false);
    Game->SetVillageLimbo(Game->GetThingWithScriptName("V_OakVale"), true);
    Game->DisplayMoneyBag(false);
    Game->TakeObjectFromHero("OBJECT_TEDDY_BEAR_UNGIVEABLE");
    AddLogbookStoryEntry(0x14);
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->CameraResetToViewBehindHero(0.0f);
    Game->CameraDefault();
    Game->CacheMusicSet((EMusicSetType)0x39);
    Game->FadeScreenIn();
    CScriptThing dadTrigger = Game->GetThingWithScriptName("MK_OVI_DADTRIGGER");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), dadTrigger, 5.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    HesDeadJimStarted = true;
    {
    CScriptGameResourceObjectScriptedThingBase hero;
    hero.Construct();
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    CCutsceneCastMap cast;
    cast.Construct();
    cast["HERO"] = hero;
    CScriptGameResourceObjectMovieBase movie;
    movie.Construct();
    Game->StartMovieSequence("", movie);
    CGameScriptInterface* game = Game;
    game->PauseAllNonScriptedEntities(true);
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro("CS_OAKVALEINTRO_HESDEADJIM", cast, 0, 0, false, true);
    Game->FixMovieSequenceCamera(false);
    game->PauseAllNonScriptedEntities(false);
    movie.Destroy();
    cast.Destroy();
    hero.Destroy();
    }
    Game->SetVillageLimbo(Game->GetThingWithScriptName("V_OakVale"), false);
    Game->SetTimeAsStopped(false, TimeStoppedToken);
    Game->DeactivateQuest("Q__OakValeIntro_PostAttack", 0);
    Game->ResetToDefaultTheme(0.0f);
    Game->StopOverrideMusic(false);
}
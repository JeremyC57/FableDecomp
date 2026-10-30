// CQ_GuildTrainingScript::RunArrivalCutscene() -- set the evening, create the rival hero at the maze marker, take him and the hero under scripted control, cast them as "MAZE" and "HERO", play CS_GUILD_ARRIVE in a paused movie sequence with the camera fixed, fade out without the region-change fade or region text, then remove the rival
// Layout: only Game (+0x40, right after the 0x40-byte retail CScriptBase) is touched; no rebuild/include/engine header exists for the class.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D44CB0, length 944 bytes; called from 0x00D3BE0B.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840 / ~CBaseClass
//   0x0099A2E0 / operator delete 0x00BFE9BC (the inline temporary copy), CBaseObject ctor 0x0099A380, scripted-thing handle
//   IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0 / operator= 0x008ABD10, cast map ctor 0x00CDBF70 / dtor
//   0x00CDBFB0 / operator[] 0x00CD3D2E, movie ctor 0x006E7B60 / dtor 0x006E7B80, RunCutsceneMacro 0x00CBFB7D (__fastcall),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; CScriptThing::GetPos (slot 6); through the interface (+0x40):
//   NewScriptFrame (7), StartScriptingEntity (8), GetHero (70), GetThingWithScriptName (72), CreateCreature (91),
//   RemoveThing (108), StartMovieSequence (370), FixMovieSequenceCamera (371), FadeScreenOut (373),
//   PauseAllNonScriptedEntities (379), SetAllowScreenFadingOnNextRegionChange (381), SetRegionTextDisplayAsActive (569),
//   SetTimeOfDay (646).
// Strings: "CutsceneMaze" 0x012CE598, "MK_GTA_MAZE1" 0x012CE588, "CREATURE_RIVAL_HERO_MAZE_CUTSCENE" 0x012CE564, "MAZE"
//   0x012CE55C, "HERO" 0x01255174, "" 0x0122D70E, "CS_GUILD_ARRIVE" 0x012CE54C.
// Shape: the twin model of CQ_GuildTrainingScript::RunSaveXPCutscene 0x00D496F0 (here every handle / movie / map
//   destructor is the out-of-line copy and the movie ctor is out of line too); CRGBColour by value as the landed
//   CQ_SecretPassageScript::EndMission 0x00DF0A50.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CRGBColour { public:                                         // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& o) { B = o.B; G = o.G; R = o.R; A = o.A; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
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
struct CScriptThingImpl;
class C3DVector { public: float X, Y, Z; };
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
struct CScriptThingImp;
struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                                    // 0x006E7AB0
};
struct CCutsceneCastMap {                                   // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    CCutsceneCastMap();                                     // 0x00CDBF70
    ~CCutsceneCastMap();                                    // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_GuildTrainingScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
    void RunArrivalCutscene();
};
void CQ_GuildTrainingScript::RunArrivalCutscene()
{
    Game->SetTimeOfDay(19.0f);
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing rival = Game->CreateCreature("CREATURE_RIVAL_HERO_MAZE_CUTSCENE", Game->GetThingWithScriptName("MK_GTA_MAZE1").GetPos(), "CutsceneMaze", false);
    {
        CScriptGameResourceObjectScriptedThingBase maze;
        if (maze.IsValid())
            maze.Release();
        while (!Game->StartScriptingEntity(rival, maze, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectScriptedThingBase hero;
        if (hero.IsValid())
            hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CCutsceneCastMap cast;
        cast["MAZE"] = maze;
        cast["HERO"] = hero;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_ARRIVE", cast, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        Game->FadeScreenOut(0.5f, 0.0f, CRGBColour(0, 0, 0, 255));
        Game->SetAllowScreenFadingOnNextRegionChange(false);
        Game->SetRegionTextDisplayAsActive(false);
        game->PauseAllNonScriptedEntities(false);
    }
    Game->RemoveThing(rival, false, true);
}
}
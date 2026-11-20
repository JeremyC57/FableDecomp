// CQ_SecretPassageScript::PrisonEscape() -- once the secret passage is loaded and the hero reaches the cutscene marker: stop the mother following, play the "Jack captures" cutscene (hero + mother cast) and fade out, remove every live follower, make the tortured mother follow the hero by region, remove her and hand the hero the prison escape quest card
// Leaf: PrisonEscape
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members PImp_Data->Data, PImp_Info->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF0B70, 1281 bytes (int3 padded to 1296).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940, IsDistanceBetweenThingsUnder
//   0x00CBE2FF, ~CScriptThing 0x004AA840, operator delete 0x00BFE9BC, CBaseObject 0x0099A380 / ~ 0x0099A430, scripted-thing
//   object ~ 0x007E74D0 / operator= 0x008ABD10, ~CMovieImpPointer 0x006E7AB0, cast map Construct 0x00CDBF70 / Destroy 0x00CDBFB0
//   / operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D, ~vector<CScriptThing> 0x008AC970; Game slots 7, 8, 11, 70, 72, 108,
//   230, 231, 295, 370, 373, 376, 379, 480, 697.
// Layout (retail = Ego_r.pdb - 0x14): GlobalMymap +0x54 (cast map), FlagMap +0x60. CQ_SunnyvaleMasterData (no shift):
//   HenchmanOverrideState +0x0c.
#include "fable_compiler.h"
// vector<CScriptThing> and the cast map: the shared STLport 4.6 containers (stlport/fable_containers.h / fable_map.h, retail
//   branch); ~vector<CScriptThing> 0x008AC970 called out of line (FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE, as the old local model).
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP
// The tree destructor, clear 0x00CD3699 and _M_erase 0x00CD344A are other rows' COMDATs: declaration only here (NF178b).
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#define FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY
#define FABLE_RETAIL_STLP_MAP_DTOR_DECL_ONLY
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
#include "stlport/fable_map.h"
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
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) { } };
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C
    CCountedPointerWords PImp;
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                                // 0x006E7AB0
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4
    CMovieImpPointer PImp;
};
// Cast map: the shared STLport 4.6 map (stlport/fable_map.h, retail branch), Ego_r.pdb spelling map<CCharString, CScriptGameResourceObjectScriptedThingBase>:
//   no-argument ctor 0x00CDBF70, ~map 0x00CDBFB0 and operator[] 0x00CD3D2E (the last two declared only, as 0x00D29880).
void __fastcall RunCutsceneMacro(const CCharString& name, std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>& cast, std::map<CCharString, bool>* flags,
                                 std::map<CCharString, CCharString>* args, bool a5, bool a6);   // 0x00CBFB7D
class CRGBColour { public:                                     // D3DCOLOR byte order
    unsigned char B, G, R, A;
    FABLE_FORCEINLINE CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    FABLE_FORCEINLINE CRGBColour(const CRGBColour& o) { B = o.B; G = o.G; R = o.R; A = o.A; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct SScriptThingInfo {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_SunnyvaleMasterData {
    unsigned char _pad[0x0c];
    long HenchmanOverrideState;                         // +0x0c
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQ_SecretPassageScript : CScriptBase {
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    unsigned char           _pad48[0x54 - 0x48];
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>        GlobalMymap;                // +0x54
    std::map<CCharString, bool>        FlagMap;                    // +0x60
    void PrisonEscape();
};

void CQ_SecretPassageScript::PrisonEscape()
{
    while (!Game->IsLevelLoaded("PrisonSecretPassage")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing marker = Game->GetThingWithScriptName("CUTSCENE_MARKER");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), marker, 2.5f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing mother = Game->GetThingWithScriptName("MotherTorture");
    Game->EntityStopFollowing(mother);
    Game->StopOverrideMusic(false);
    {
        CScriptGameResourceObjectScriptedThingBase hero;
        CScriptGameResourceObjectScriptedThingBase motherObject;
        Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
        Game->StartScriptingEntity(mother, motherObject, (EScriptAIPriority)4);
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
        GlobalMymap["Hero"] = hero;
        GlobalMymap["Mother"] = motherObject;
        {
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            RunCutsceneMacro("CS_JACK_CAPTURES", GlobalMymap, &FlagMap, 0, false, true);
            Game->FadeScreenOut(0.5f, 0.0f, CRGBColour(0, 0, 0, 255));
            game->PauseAllNonScriptedEntities(false);
        }
    }
    std::vector<CScriptThing> followers;
    Game->GetFollowingEntityList(Game->GetHero(), followers);
    PMasterData->HenchmanOverrideState = 2;
    Game->Pause(1.0f);
    for (unsigned int i = 0; i < followers.size(); ++i) {
        if (followers[i].IsAlive())
            Game->RemoveThing(followers[i], false, true);
    }
    mother = Game->GetThingWithScriptName("MotherTorture");
    Game->SetEntityAsRegionFollowing(Game->GetHero(), mother, false);
    Game->RemoveThing(mother, false, true);
    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_PRISON_ESCAPE", "Q_PrisonEscapeRescueMother", false);
}
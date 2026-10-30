// CQ_SecretPassageScript::PrisonEscape() -- once the secret passage is loaded and the hero reaches the cutscene marker: stop the mother following, play the "Jack captures" cutscene (hero + mother cast) and fade out, remove every live follower, make the tortured mother follow the hero by region, remove her and hand the hero the prison escape quest card
// Leaf: PrisonEscape
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF0B70, 1281 bytes (int3 padded to 1296).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940, IsDistanceBetweenThingsUnder
//   0x00CBE2FF, ~CScriptThing 0x004AA840, operator delete 0x00BFE9BC, CBaseObject 0x0099A380 / ~ 0x0099A430, scripted-thing
//   object ~ 0x007E74D0 / operator= 0x008ABD10, ~CMovieImpPointer 0x006E7AB0, cast map Construct 0x00CDBF70 / Destroy 0x00CDBFB0
//   / operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D, ~vector<CScriptThing> 0x008AC970; Game slots 7, 8, 11, 70, 72, 108,
//   230, 231, 295, 370, 373, 376, 379, 480, 697.
// Layout (retail = Ego_r.pdb - 0x14): GlobalMymap +0x54 (cast map), FlagMap +0x60. CQ_SunnyvaleMasterData (no shift):
//   HenchmanOverrideState +0x0c.
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
    bool IsScripting();                                 // 0x00CD23B9
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
struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    void Construct();                                   // 0x00CDBF70 (out-of-line ctor)
    void Destroy();                                     // 0x00CDBFB0 (out-of-line dtor)
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
class CRGBColour { public:                                     // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& o) { B = o.B; G = o.G; R = o.R; A = o.A; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct SScriptThingInfo {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
struct CScriptThingPointer {                            // CCountedPointer<CScriptThingImp>
    void* PImp_Data;
    SScriptThingInfo* PImp_Info;
    __forceinline CScriptThingPointer& operator=(const CScriptThingPointer& other)
    {
        SScriptThingInfo* info = other.PImp_Info;
        void* data = other.PImp_Data;
        if (PImp_Info != info) {
            if (PImp_Info) {
                --PImp_Info->m_references;
                if (PImp_Info->m_references == 0) {
                    PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                    delete PImp_Info;
                }
            }
            PImp_Data = data;
            PImp_Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C, 12 bytes
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010();
    virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020();
    virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030();
    virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040();
    virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048(); virtual void t049(); virtual void t050();
    virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060();
    virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070();
    virtual void t071(); virtual void t072(); virtual void t073(); virtual void t074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c) 0x004AB130
    CScriptThingPointer PImp;                           // +0x04
    virtual ~CScriptThing();                                    // 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
};
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                          // 0x008AC970
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CCutsceneFlagMap {                               // map<CCharString, bool>, 12 bytes
    void* _body[3];
};
struct CQ_SunnyvaleMasterData {
    unsigned char _pad[0x0c];
    long HenchmanOverrideState;                         // +0x0c
};
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
struct CQ_SecretPassageScript : CScriptBase {
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    unsigned char           _pad48[0x54 - 0x48];
    CCutsceneCastMap        GlobalMymap;                // +0x54
    CCutsceneFlagMap        FlagMap;                    // +0x60
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
        CCutsceneCastMap cast;
        cast.Construct();
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
        cast.Destroy();
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
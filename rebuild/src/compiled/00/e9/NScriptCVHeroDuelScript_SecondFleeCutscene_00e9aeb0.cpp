// CV_HeroDuelScript::SecondFleeCutscene() -- hide the health bar, drop Thunder's minimap marker, take Thunder and then the hero under scripted control, play CS_GIBBETHILL_HERODUEL_FLEE with both cast inside a paused movie sequence, then restore Thunder's cutscene behaviour and flag that he entered the cave first
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9AEB0, 0x387 bytes, plain `ret`.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CBaseObject 0x0099A380 / ~ 0x0099A430, the handle's IsValid 0x00CD23B9 /
//   Release 0x00CD2770 / operator= 0x008ABD10 / out-of-line dtor 0x007E74D0, the cast map's ctor 0x00CDBF70 / operator[]
//   0x00CD3D2E / dtor 0x00CDBFB0, the movie PImp dtor 0x006E7AB0, the __fastcall cut-scene runner 0x00CBFB7D, operator delete
//   0x00BFE9BC, ~CScriptThing 0x004AA840, IsActiveThreadTerminating 0x00CB7940; Game slots 7, 8 StartScriptingEntity, 70
//   GetHero, 72 GetThingWithScriptName, 352 MiniMapRemoveMarker, 370 StartMovieSequence, 371 FixMovieSequenceCamera, 379
//   PauseAllNonScriptedEntities, 512 EntitySetCutsceneBehaviour.
// Shape: the landed CQ_WizardBattleScript::CutsceneScreamers 0x00E1CD40 (cast map, held movie, cached `game`) plus the
//   CMeleeThunder::Main 0x00D58080 handle (IsValid/Release, implicit dtor); the handles live in an inner block and every
//   exit except the first wait loop's sits under inline_depth(0), so only that one expands the dtors.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, OkToShowHealthBar +0x4e, ThunderEnteredCaveFirst +0x4f.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CCPPointerInfo { public:                                 // counted-pointer block (Ego_r.pdb)
    long RefCount;                                      // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
inline bool CScriptThing::MsgIsKilledBy(const CCharString& killer) const { if (!PImp.Data) return false; return PImp.Data->MsgIsKilledBy(killer); }
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker); }
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

class CScriptGameResourceObjectBase : public CBaseObject { public:    // vtable 0x0126008C
    virtual __forceinline ~CScriptGameResourceObjectBase() {}
};

struct CScriptThingImp;
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // 16 bytes, vtable 0x0127094C
    CCountedPointer<CScriptThingImp> PImp;              // +0x08
    CScriptGameResourceObjectScriptedThingBase() {}
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    // implicit destructor (no vptr store of its own); out of line 0x007E74D0
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};

struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                                // 0x006E7AB0
};

class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // 16 bytes, vtable 0x01260EF4
    CMovieImpPointer PImp;                              // +0x08
    CScriptGameResourceObjectMovieBase() {}
    // implicit destructor
};

struct CHeldMovie {                                     // holder: its forceinline dtor expands the movie's implicit dtor at every exit
    CScriptGameResourceObjectMovieBase Obj;
    __forceinline CHeldMovie() {}
    __forceinline ~CHeldMovie() {}
};

struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase> (12 bytes)
    void* _body[3];
    CCutsceneCastMap();                                 // 0x00CDBF70
    ~CCutsceneCastMap();                                // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_HeroDuelScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[0x4e - 0x48];
    bool                  OkToShowHealthBar;            // +0x4e (PDB 98)
    bool                  ThunderEnteredCaveFirst;      // +0x4f (PDB 99)
    void SecondFleeCutscene();
};
} // namespace NScript
using NScript::CV_HeroDuelScript;

void NScript::CV_HeroDuelScript::SecondFleeCutscene()
{
    OkToShowHealthBar = false;
    CScriptThing thunder = Game->GetThingWithScriptName("Thunder");
    Game->MiniMapRemoveMarker(thunder);
    {
        CScriptGameResourceObjectScriptedThingBase thunderHandle;
        if (thunderHandle.IsValid())
            thunderHandle.Release();
        while (!Game->StartScriptingEntity(thunder, thunderHandle, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetCutsceneBehaviour(thunder, (ECutsceneBehaviour)2);
        CScriptGameResourceObjectScriptedThingBase heroHandle;
        if (heroHandle.IsValid())
            heroHandle.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), heroHandle, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCutsceneCastMap cast;
            cast["HERO"] = heroHandle;
            cast["THUNDER"] = thunderHandle;
            CHeldMovie movie;
            {
                CCharString sequence("");
                Game->StartMovieSequence(sequence, movie.Obj);
            }
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_FLEE", cast, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            game->PauseAllNonScriptedEntities(false);
        }
    }
    Game->EntitySetCutsceneBehaviour(thunder, (ECutsceneBehaviour)1);
    ThunderEnteredCaveFirst = true;
    OkToShowHealthBar = true;
    return;
}
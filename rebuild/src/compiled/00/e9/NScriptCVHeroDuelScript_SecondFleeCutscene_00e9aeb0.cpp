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
struct CScriptThingImpl {                                   // the game-side thing behind PImp.Data (CScriptThing vtable layout)
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005(); virtual void i006(); virtual void i007();
    virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015();
    virtual void i016(); virtual void i017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;             // slot 18 (+0x48)
    virtual void i019(); virtual void i020();
    virtual bool MsgIsHitBy(const CCharString& attacker) const;              // slot 21 (+0x54)
    virtual void i022(); virtual void i023(); virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029();
    virtual void i030(); virtual void i031(); virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037();
    virtual void i038(); virtual void i039(); virtual void i040(); virtual void i041(); virtual void i042();
    virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const; // slot 43 (+0xac)
    virtual void i044();
    virtual void i045();
    virtual void i046();
    virtual void i047();
    virtual void i048();
    virtual void i049();
    virtual void i050();
    virtual void i051();
    virtual void i052();
    virtual void i053();
    virtual void i054();
    virtual void i055();
    virtual void i056();
    virtual void i057();
    virtual void i058();
    virtual void i059();
    virtual void i060();
    virtual void i061();
    virtual void i062();
    virtual void i063();
    virtual void i064();
    virtual void i065();
    virtual void i066();
    virtual void i067();
    virtual void i068();
    virtual void i069();
    virtual void i070();
    virtual void i071();
    virtual void i072();
    virtual void i073();
    virtual void i074();
    virtual bool IsAlive() const;                                                      // slot 75 (+0x12c)
};
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    __forceinline ~CScriptThing() {}                                      // out of line 0x004AA840
    bool IsAlive() const                                    // 0x004AB130
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->IsAlive();
    }
    bool MsgIsKilledBy(const CCharString& killer) const     // 0x004AAAF0
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsKilledBy(killer);
    }
    bool MsgIsHitBy(const CCharString& attacker) const      // 0x004AAB30
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitBy(attacker);
    }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const   // 0x004AAD60
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker);
    }
};
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

class CScriptBase { public:                                    // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
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
#pragma inline_depth(0)
        if (IsActiveThreadTerminating())
            return;
#pragma inline_depth()
        Game->EntitySetCutsceneBehaviour(thunder, (ECutsceneBehaviour)2);
        CScriptGameResourceObjectScriptedThingBase heroHandle;
        if (heroHandle.IsValid())
            heroHandle.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), heroHandle, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
        }
#pragma inline_depth(0)
        if (IsActiveThreadTerminating())
            return;
#pragma inline_depth()
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
#pragma inline_depth(0)
    }
#pragma inline_depth()
    Game->EntitySetCutsceneBehaviour(thunder, (ECutsceneBehaviour)1);
    ThunderEnteredCaveFirst = true;
    OkToShowHealthBar = true;
#pragma inline_depth(0)
    return;
#pragma inline_depth()
}
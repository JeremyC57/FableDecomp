// NScript::CExpression_FollowScript::Main() -- the Expression_Follow script: guard the thread on the hero being conscious, take the hero under scripted control, open a movie sequence, fade the screen out, spawn the FollowThread thread, reset the camera and fade back in
// Leaf: ?Main@CExpression_FollowScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEA020, 543 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the RTTI-less level-script vtable 0x012F55B8 [dtor 0x00EEA410, RegisterMain 0x00EE9F80 (landed
//   NScript::CExpression_FollowScript::RegisterMain, whose "Main" thread is the slot-2 vcall), Main 0x00EEA020, Init 0x00EE9F70,
//   OnPersist 0x00CBD4E0 (CScriptBase's)], referenced only from that slot (0x012F55C0); Ego_r.pdb
//   ?Main@CExpression_FollowScript@NScript@@UAEXXZ. The spawned thread 0x00EEA240 is ?FollowThread@CExpression_FollowScript@NScript@@QAEXXZ
//   (Ego_r name set; its "FollowThread" literal 0x012F55E8 sits at the spawn site). Catalog label sub_00eea020.
// Callees: CScriptBase::SetPredicate 0x00CB7920 / IsActiveThreadTerminating 0x00CB7940 / AddSpawnedFunction 0x00CB7E50, CScriptThing
//   copy 0x004ABE90 / dtor 0x004AA840 (the CIsThingConscious temporary), CBaseObject ctor / dtor 0x0099A380 / 0x0099A430,
//   ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0 (out of line here), CCountedPointer<CScriptGameResourceObjectMovieBase>
//   dtor 0x006E7AB0, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, operator new 0x00BFEA1A (0x3C), CSpawnedFuncBase
//   ctor 0x00CDD450; vtables CIsThingConscious 0x012C2FE8, CSpawnedFunc<CExpression_FollowScript> 0x012F55CC; CGameScriptInterfaceBase
//   slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 370 StartMovieSequence, 373 FadeScreenOut, 374 FadeScreenIn,
//   376 Pause, 416 CameraDefault.
// Literals: "" 0x0122D70E (movie sequence and region name), "FollowThread" 0x012F55E8; floats 0.5f, 0.5f, 1.0f, 1.0f (immediates).
// Layout: retail NScript headers (FABLE_RETAIL_NSCRIPT: CScriptThing, CLevelScriptBase, CGameScriptInterfaceBase, CPredicateBase,
//   CSpawnedFuncBase through FABLE_RETAIL_SUSPENDABLE_PROCESS); Ego_r.pdb gives the class no members of its own.
// Local models (no shared header yet; NOTES.md "Header debt"): NScript::CIsThingConscious (CPredicateBase + Thing, vtable
//   0x012C2FE8, the landed 0x00CDDAB0 / 0x00CDDAE0 / 0x00CDDB10 model), CBaseObject / CScriptGameResourceObjectBase /
//   ScriptedThingBase / MovieBase (the landed 0x00D7AA30 model), CSpawnedFunc<T> (the template every landed spawning row defines).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
void operator delete(void* block);                      // 0x00BFE9BC
class CCPPointerInfo {
public:
    long RefCount;                                      // +0x0
    void (__fastcall* DeleteFunc)(void* data);          // +0x4
    void* Data;                                         // +0x8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x0
    CCPPointerInfo* Info;                               // +0x4
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer();
};
class CRGBColour {                                      // Ego_r.pdb union: B, G, R, A bytes (D3DCOLOR order)
public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) : B(other.B), G(other.G), R(other.R), A(other.A) { }
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CPredicateBase.h"
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"

class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject {   // vtable 0x0126008C
public:
    CScriptGameResourceObjectBase() { }
    virtual ~CScriptGameResourceObjectBase() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase {   // vtable 0x0127094C, 0x10 bytes
public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    virtual ~CScriptGameResourceObjectScriptedThingBase();               // 0x007E74D0 (out of line here)
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase {   // vtable 0x01260EF4, 0x10 bytes
public:
    CCountedPointer<CScriptGameResourceObjectMovieBase> PImp;   // +0x08
};
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)

#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

#include "engine/CIsThingConscious.h"                 // vtable 0x012C2FE8: operator() / Clone / ~ (slots 0-2)
namespace NScript {
inline CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }   // inline here (retail expands it)
inline CIsThingConscious::~CIsThingConscious() { }      // inline here; out of line 0x00EF7AB0
class CQ_SunnyvaleMasterData;
class CExpression_FollowScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Main();
    void FollowThread();                                // 0x00EEA240
    // Source-level spawn helper of the landed quest rows (TalentlessBard Main 0x00ED7830, EscortTrader DoMission 0x00E1F0C0); no
    // symbol: retail allocates the thread first and builds the region-name temporary after it, while both temporaries live to the
    // end of the statement, which a direct AddSpawnedFunction(new ..., CCharString("")) call evaluates the other way round.
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};

void CExpression_FollowScript::Main()
{
    SetPredicate(CIsThingConscious(Game->GetHero()));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase hero;
    while (!Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
    Game->Pause(1.0f);
    SpawnFunction(new CSpawnedFunc<CExpression_FollowScript>("FollowThread", 0, this, &CExpression_FollowScript::FollowThread));
    Game->CameraDefault();
    Game->Pause(1.0f);
    Game->FadeScreenIn();
}
}   // namespace NScript
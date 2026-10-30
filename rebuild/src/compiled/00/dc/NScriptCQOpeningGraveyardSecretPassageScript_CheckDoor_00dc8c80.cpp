// CQ_OpeningGraveyardSecretPassageScript::CheckDoor() -- trigger the path generator, make it persistent, lock the secret-passage gate; until the lieutenant shows up (and then while he lives) re-trigger the generator whenever the hero comes back to the graveyard path; then play the path-door cut-scene, set the objective, fade the gate out and swap in the new door
// Leaf: CheckDoor
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC8C80, 1904 bytes.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, ~CScriptThing 0x004AA840 (and its inline body), ~vector<CScriptThing>
//   0x008AC970 (and its inline body), handle 0x007E72A0 / 0x007E74D0 / operator= 0x008ABD10, cast map 0x00CDBF70 /
//   0x00CDBFB0 / operator[] 0x00CD3D2E, movie 0x006E7B60 / 0x006E7B80, RunCutsceneMacro 0x00CBFB7D,
//   FadeOutAndKillEntity 0x00CBE716, IsActiveThreadTerminating 0x00CB7940; CScriptThing::IsAlive (slot 75); through the
//   interface (+0x40) slots 7, 8, 12, 70, 72, 75, 108, 296, 370, 376, 379, 447, 477, 547, 655.
// Layout (retail = Ego_r.pdb - 0x14): DoorClosed +0x57, LieutenantAround +0x68.
// Shape: VC7.1 keeps the inline ~CScriptThing / ~vector bodies at only some sites; `#pragma inline_depth(0)` around the
//   other return statements and from the lieutenant loop body on reproduces which cleanups call 0x004AA840 / 0x008AC970.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#define THING_INLINE_COPY
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
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class C3DVector { public: float X, Y, Z; };
struct CScriptThingImpl;
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18) 0x004AA980
    virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); virtual void t072(); virtual void t073(); virtual void t074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c) 0x004AB130
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04 Data, +0x08 Info
#ifdef THING_INLINE_COPY
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) {}
#else
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
#endif
    ~CScriptThing() {}
};
namespace std {
template<class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
#ifndef VEC_DTOR
#define VEC_DTOR __forceinline
#endif
    VEC_DTOR ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    T* erase(T* first, T* last);                        // 0x008AC9D0 for CScriptThing
    void clear() { erase(_M_start, _M_finish); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
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
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, float time, bool kill, bool removeOnFade);   // 0x00CBE716
class CScriptBase { public:                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
namespace NScript {
class CQ_OpeningGraveyardSecretPassageScript : public CScriptBase {
public:
    CGameScriptInterface*       Game;                   // +0x40
    void*                       PMasterData;            // +0x44
    unsigned char               _pad48[0x57 - 0x48];
    bool                        DoorClosed;             // +0x57
    unsigned char               _pad58[0x68 - 0x58];
    bool                        LieutenantAround;       // +0x68
    void CheckDoor();
};
} // namespace NScript
using NScript::CQ_OpeningGraveyardSecretPassageScript;

void NScript::CQ_OpeningGraveyardSecretPassageScript::CheckDoor()
{
    std::vector<CScriptThing> gates;
    Game->CreatureGeneratorTrigger(Game->GetThingWithScriptName("GraveyardPathGenerator"));
    Game->SetThingPersistent(Game->GetThingWithScriptName("GraveyardPathGenerator"), true);
    Game->GetAllThingsWithScriptName("SecretPassageGate", gates);
    CScriptThing gate = gates[0];
    DoorClosed = true;
    Game->SetThingAsUsable(gate, false);
    while (!LieutenantAround) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        if (!Game->IsRegionLoaded("GraveyardPath")) {
            if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
            while (!Game->IsRegionLoaded("GraveyardPath")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
            Game->CreatureGeneratorTrigger(Game->GetThingWithScriptName("GraveyardPathGenerator"));
        }
    }
    if (IsActiveThreadTerminating())
    {
#pragma inline_depth(0)
        return;
#pragma inline_depth()
    }
    while (Game->GetThingWithScriptName("UndeadLieutenant").IsAlive()) {
#pragma inline_depth(0)
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!Game->IsRegionLoaded("GraveyardPath")) {
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsRegionLoaded("GraveyardPath")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->CreatureGeneratorTrigger(Game->GetThingWithScriptName("GraveyardPathGenerator"));
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(1.0f);
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
    RunCutsceneMacro("CS_OPENGRAVE_PATHDOOR", cast, 0, 0, false, true);
    game->PauseAllNonScriptedEntities(false);
    movie.Destroy();
    cast.Destroy();
    hero.Destroy();
    }
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_04B", "", "Graveyard");
    FadeOutAndKillEntity(gate, 0.2f, false, true);
    Game->RemoveThing(Game->GetThingWithScriptName("GraveyardPathGenerator"), false, true);
    Game->SetThingAsUsable(gate, true);
    Game->SetThingPersistent(Game->GetThingWithScriptName("GraveyardPathNewDoor"), true);
    DoorClosed = false;
}
#pragma inline_depth()
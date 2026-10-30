// CQ_HangingTreeGoodScript::RemoveParty(bool) -- re-collect the HTG_Guard things; with the exit camera, play a paused fade-out on HTG_ExitCam; remove every live guard (emptying the list), re-fetch and remove the prisoner, then restore the camera and fade back in
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D732F0, length 789 bytes (`ret 4`); called from 0x00D6FB6C, 0x00D6FBFA
//   and 0x00D74C03.
// Callees: vector<CScriptThing>::erase(iterator, iterator) 0x008AC9D0, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940, CBaseObject 0x0099A380 / ~ 0x0099A430,
//   movie PImp dtor 0x006E7AB0, operator delete 0x00BFE9BC / ~CBaseClass 0x0099A2E0 (inline CScriptThing copies),
//   ~CScriptThing 0x004AA840 (the early-exit copy); CScriptThing slot 0 (virtual destructor) and IsAlive (slot 75);
//   through the interface (+0x40): GetThingWithScriptName (72), GetAllThingsWithScriptName (75), RemoveThing (108),
//   StartMovieSequence (370), FadeScreenOut (373), FadeScreenIn (374), Pause (376), PauseAllNonScriptedEntities (379),
//   CameraUseCameraPoint (411), CameraDefault (416).
// Strings: "HTG_Guard" 0x012D26F8, "HTG_ExitCam" 0x012D29FC, "" 0x0122D70E, "HTG_Prisoner" 0x012D270C.
// Layout (retail = Ego_r.pdb - 0x14, 12-byte vectors): PartySuccessfullySetUp +0x49, ExitCutsceneStart +0x59, Prisoner
//   +0x74, CurrentGuardVector +0xac.
// Shape: CScriptThing model of the landed CQ_HangingTreeGoodScript::ReassignLeader 0x00D74F80; movie model of the
//   landed CMurderTwistSheriff::DoRemindingHeroToKillSheriff 0x00EB15C0 (cached interface pointer around the pause).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
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
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;          // the source's fields are read first
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
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008();
    virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016();
    virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024();
    virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032();
    virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040();
    virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048();
    virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056();
    virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064();
    virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); virtual void t072();
    virtual void t073(); virtual void t074();
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
    ~CScriptThing() {}                                  // out of line 0x004AA840
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
};
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    T& back() { return *(end() - 1); }
    void pop_back() { --_M_finish; _M_finish->~T(); }
    T* erase(T* first, T* last);                        // 0x008AC9D0 for CScriptThing
    void clear() { erase(begin(), end()); }
};
}
class C3DVector;
class CRightHandedSet;
class CRGBColour { public:                                     // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& o) { B = o.B; G = o.G; R = o.R; A = o.A; }
};
class CBaseObject { public:                                    // CBaseIntelligentPointer, vtable 0x0129A7D8
    virtual void b000();
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                      // 0x0099A380
public:
};
struct CBaseObjectDtor {                                // destructor-only view of CBaseObject
    void* m_pIntelligentPointer;
    virtual ~CBaseObjectDtor();                         // 0x0099A430
};
struct CScriptGameResourceObjectBaseDtor : CBaseObjectDtor {   // vtable 0x0126008C
    __forceinline virtual ~CScriptGameResourceObjectBaseDtor() { }
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    ~CMovieImpPointer();                                // 0x006E7AB0
};
struct CMovieImpWords { void* Data; void* Info; };
class CScriptGameResourceObjectMovieBase : public CBaseObject { public:   // vtable 0x01260EF4; destructor-less view, destroyed by hand
    CMovieImpWords PImp;                                // +0x08
    __forceinline CScriptGameResourceObjectMovieBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void m001();
    __forceinline void DestroyInline()
    {
        reinterpret_cast<CMovieImpPointer*>(&PImp)->~CMovieImpPointer();
        reinterpret_cast<CScriptGameResourceObjectBaseDtor*>(this)->CScriptGameResourceObjectBaseDtor::~CScriptGameResourceObjectBaseDtor();
    }
};
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
class CQ_HangingTreeGoodScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    bool LeaderSpokenTo;                                // +0x48
    bool PartySuccessfullySetUp;                        // +0x49
    unsigned char _pad4a[0x59 - 0x4a];
    bool ExitCutsceneStart;                             // +0x59
    unsigned char _pad5a[0x68 - 0x5a];
    CScriptThing Leader;                                // +0x68
    CScriptThing Prisoner;                              // +0x74
    unsigned char _pad80[0xac - 0x80];
    std::vector<CScriptThing> CurrentGuardVector;       // +0xac
    void RemoveParty(bool useExitCamera);
};
void CQ_HangingTreeGoodScript::RemoveParty(bool useExitCamera)
{
    CurrentGuardVector.clear();
    Game->GetAllThingsWithScriptName("HTG_Guard", CurrentGuardVector);
    CScriptThing exitCam = Game->GetThingWithScriptName("HTG_ExitCam");
    if (useExitCamera) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->CameraUseCameraPoint(exitCam, 0, 0, -1.0f, 0, -1);
        ExitCutsceneStart = true;
        Game->Pause(3.0f);
        Game->FadeScreenOut(1.0f, 1.0f, CRGBColour(0, 0, 0, 255));
        Game->Pause(1.0f);
        game->PauseAllNonScriptedEntities(false);
        movie.DestroyInline();
    }
    while (!CurrentGuardVector.empty()) {
        if (CurrentGuardVector.back().IsAlive())
            Game->RemoveThing(CurrentGuardVector.back(), false, true);
        CurrentGuardVector.pop_back();
    }
    Prisoner = Game->GetThingWithScriptName("HTG_Prisoner");
    if (Prisoner.IsAlive())
        Game->RemoveThing(Prisoner, false, true);
    ExitCutsceneStart = false;
    if (useExitCamera) {
        if (IsActiveThreadTerminating())
            return;
        Game->CameraDefault();
        Game->FadeScreenIn();
    }
    PartySuccessfullySetUp = false;
}
}
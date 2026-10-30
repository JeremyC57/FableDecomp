// NScript::CV_FishingCompetitionScript::CFishCompOrganiser::OnPredicateFail() -- when the organiser was killed by the hero, fail the competition and spawn the parent script's EndCompetition thread
// Leaf: OnPredicateFail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E7F030, length 266 bytes, plain `ret`.
// Identity: Ego_r.pdb ?OnPredicateFail@CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@EAEXXZ.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, operator+(const CCharString&, const
//   CCharString&) @ 0x0099F570 (__fastcall, hidden result in ecx, left in edx), operator new @ 0x00BFEA1A (0x3c bytes),
//   CSpawnedFuncBase::CSpawnedFuncBase(const CCharString&, unsigned long) @ 0x00CDD450, CScriptBase::AddSpawnedFunction(
//   CSpawnedFuncBase*, const CCharString&) @ 0x00CB7E50 (catalog "CGuiVarTransferStruct::Add" is propagated); Me's
//   CGameScriptThing slot 18 MsgIsKilledBy; CV_FishingCompetitionScript::EndCompetition @ 0x00E7F140 (the thread's
//   member-function pointer). Vtable: CSpawnedFunc0<CV_FishingCompetitionScript> 0x012EA48C.
// Strings: "SCRIPT_NAME_HERO" @ 0x0125D1C8, "EndCompetition" @ 0x012EA63C, "ParentClass." @ 0x012C9934, "" @ 0x0122D70E.
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14; the retail quest base (landed
//   CheckCompletion 0x00D3AD60): MissionFailed +0x4a; CSpawnedFunc0 0x3c bytes (base 0x34, Func +0x34, PClass +0x38).
// Shape: the killed test is an implicit conversion temporary whose result is parked in a frame byte across its
//   destructor; the thread name `CCharString("ParentClass.") + CCharString("EndCompetition")` exists only when the
//   new succeeded, so VC guards its three temporaries with the folded flag mask `mov ebx,7`; the empty region name is an
//   explicit temporary (the landed CheckCompletion shape).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A

class CScriptThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    void* PImp[2];                                      // +0x04
};

// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T> struct CSpawnedFunc0 : CSpawnedFuncBase {   // vtable 0x012EA48C for CV_FishingCompetitionScript
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    CSpawnedFunc0(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
namespace NScript {
class CV_FishingCompetitionScript : public CScriptBase { public:
    void* GSI;                                          // +0x40
    void* PMasterData;                                  // +0x44
    bool  QuestStartScreenShown;                        // +0x48
    bool  MissionSucceeded;                             // +0x49
    bool  MissionFailed;                                // +0x4a
    void EndCompetition();                              // 0x00E7F140
    class CFishCompOrganiser;
};
class CV_FishingCompetitionScript::CFishCompOrganiser {
public:
    void*                        __vftable;             // +0x00
    void*                        Game;                  // +0x04
    CScriptThing                 Me;                    // +0x08
    CV_FishingCompetitionScript* ParentClass;           // +0x14
    void OnPredicateFail();
};

void CV_FishingCompetitionScript::CFishCompOrganiser::OnPredicateFail()
{
    bool killed = Me.MsgIsKilledBy("SCRIPT_NAME_HERO");
    if (killed) {
        ParentClass->MissionFailed = true;
        ParentClass->AddSpawnedFunction(
            new CSpawnedFunc0<CV_FishingCompetitionScript>(CCharString("ParentClass.") + CCharString("EndCompetition"), 0,
                                                           ParentClass, &CV_FishingCompetitionScript::EndCompetition),
            CCharString("", -1));
    }
}
}
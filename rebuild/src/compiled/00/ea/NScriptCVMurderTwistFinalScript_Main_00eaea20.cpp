// NScript::CV_MurderTwistFinalScript::Main() -- the Murder Twist finale quest main: bind the MurderTwistSheriff / MurderTwistSheriffGuard / MurderTwistAssassin / MurderTwistAssassinGuard entity scripts, wait until the sheriff's state reaches 3 or 4, mark V_MurderTwistFinal completed, and in state 3 wait until the OakBay level is no longer loaded; finally deactivate the active quest (delay 1)
// Leaf: ?Main@CV_MurderTwistFinalScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EAEA20, 732 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012EF978 [dtor 0x00EB6A90, RegisterMain 0x00EAE980 (landed as
//   NScript::CV_MurderTwistFinalScript::RegisterMain, which spawns slot 2 as "Main"), Main 0x00EAEA20, Init 0x00EB6A00, OnPersist
//   0x00EB6520 (landed as ...::OnPersist)]; Ego_r.pdb 0x00F225C0 ?Main@CV_MurderTwistFinalScript@NScript@@UAEXXZ (right after
//   RegisterMain 0x00F22490, as in retail). The four binding factories are the class's PDB Alloc statics, each paired with its
//   binding-name string: CMurderTwistSheriff::Alloc 0x00EAED10 ("MurderTwistSheriff"; news 0x38 and calls the landed
//   CMurderTwistSheriff ctor 0x00EAED90), CMurderTwistSheriffGuard::Alloc 0x00EB1F80, CMurderTwistAssassin::Alloc 0x00EB3C60,
//   CMurderTwistAssassinGuard::Alloc 0x00EB5450 (the last three landed under these PDB names; Ego_r donor order 0x00F1A0B0 /
//   0x00F1CFC0 / 0x00F1F3E0 / 0x00F20F70 matches retail's). Catalog label: sub_00eaea20 (gap scan).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930;
//   vtable CEntityScriptBinding<CV_MurderTwistFinalScript> 0x012EF9F0; CGameScriptInterface slots 7 NewScriptFrame,
//   11 IsLevelLoaded, 64 PostAddScriptedEntities, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 655 GetActiveQuestName
//   (ghidra_out/cgsi_slots_aligned.tsv; argument pushes match each signature).
// Literals: "MurderTwistSheriff" 0x012EF9F8, "MurderTwistSheriffGuard" 0x012EF9D8, "MurderTwistAssassin" 0x012EF9C4,
//   "MurderTwistAssassinGuard" 0x012EF9A8, "V_MurderTwistFinal" 0x012C5260, "OakBay" 0x012D31E4.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, PMasterData +0x44, SheriffState +0x48 (Ego_r +0x5C), AssassinState +0x4C.
//   The ESheriffState enumerator names are not in the PDB dumps, so the states are written as their numbers.
// Shape: the OakBay wait is `while (!(!IsLevelLoaded(..)))` -- the double negation (a wait-until-not-loaded) gives retail's
//   `test al,al ; setne bl` (a plain `while (IsLevelLoaded(..))` gives `mov bl,al` and a different loop layout).
// Model: the landed NScript::CV_RockTrollFirstEncounterScript::Main 0x00EC3D40 / chunk-573 CV_TrophyDealerScript::Main
//   0x00EE6E90 (CEntityScriptBinding, polymorphic CScriptBase, CGameScriptInterfaceBase keeping its padding virtuals).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
class __declspec(novtable) CEntityScriptBindingBase {
public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> class CEntityScriptBinding : public CEntityScriptBindingBase {   // 0x1c bytes
public:
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) { }
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};
// CGameScriptInterfaceBase / CGameScriptInterface are the retail script interface from the shared headers
//   (FABLE_RETAIL_NSCRIPT opt-in: CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    virtual void RegisterMain();                        // slot 1
    virtual void Main() = 0;                            // slot 2
    virtual void Init();                                // slot 3
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
};
namespace NScript {
class CV_MurderTwistFinalScript : public CScriptBase {
public:
    enum ESheriffState { };
    enum EAssassinState { };
    class CMurderTwistSheriff {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MurderTwistFinalScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EAED10
    };
    class CMurderTwistSheriffGuard {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MurderTwistFinalScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EB1F80
    };
    class CMurderTwistAssassin {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MurderTwistFinalScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EB3C60
    };
    class CMurderTwistAssassinGuard {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MurderTwistFinalScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EB5450
    };
    CGameScriptInterface* Game;                         // +0x40
    void* PMasterData;                                  // +0x44
    ESheriffState SheriffState;                         // +0x48
    EAssassinState AssassinState;                       // +0x4c
    virtual void Main();
};

void CV_MurderTwistFinalScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MurderTwistFinalScript>("MurderTwistSheriff", this, &CMurderTwistSheriff::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MurderTwistFinalScript>("MurderTwistSheriffGuard", this, &CMurderTwistSheriffGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MurderTwistFinalScript>("MurderTwistAssassin", this, &CMurderTwistAssassin::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MurderTwistFinalScript>("MurderTwistAssassinGuard", this, &CMurderTwistAssassinGuard::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (SheriffState != (ESheriffState)4 && SheriffState != (ESheriffState)3) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestAsCompleted("V_MurderTwistFinal", false, false, false);
    if (SheriffState == (ESheriffState)3) {
        if (IsActiveThreadTerminating())
            return;
        while (!(!Game->IsLevelLoaded("OakBay"))) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 1);
}
}   // namespace NScript
// NScript::CQS_MeetSisterScript::Main() -- the Meet-Sister quest main: bind the MeetSisterMessenger (flags 1) and MeetSisterSister entity scripts, post-add the scripted entities, then set the quest card objective (the Theresa info summary, HauntedHouse / BarrowFields)
// Leaf: ?Main@CQS_MeetSisterScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E28DE0, 370 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the quest-script vtable 0x012E2194 [dtor 0x00E299D0, RegisterMain 0x00E28D40 (landed
//   NScript::CQS_MeetSisterScript::RegisterMain, which spawns slot 2 as "Main"), Main 0x00E28DE0, Init 0x00E28D30,
//   OnPersist 0x00CBD4E0]; the only reference to the row is that slot. Ego_r.pdb ?Main@CQS_MeetSisterScript@NScript@@UAEXXZ
//   (donor 0x00E75170, right after RegisterMain 0x00E75040 as here). The two binding factories are the landed
//   CMeetSisterMessenger::Alloc 0x00E28F70 and CMeetSisterSister::Alloc 0x00E29040 (Ego_r.pdb Alloc statics of the class).
//   Catalog label: sub_00e28de0.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930; CEntityScriptBinding<T> vtable
//   0x012E2214; CGameScriptInterface slots 64 PostAddScriptedEntities, 296 SetQuestCardObjective, 655 GetActiveQuestName
//   (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "MeetSisterMessenger" 0x012E221C, "MeetSisterSister" 0x012E2200, "BarrowFields" 0x012C5D94, "HauntedHouse"
//   0x012E21F0, "TEXT_QUEST_THERESA_MOTHER_INFO_2_SUMMARY" 0x012E21C4.
// Layout: CScriptBase retail 0x40 bytes (polymorphic), Game +0x40; CEntityScriptBinding 0x1c bytes (the first binding's
//   Flags is 1: retail stores the ebx flag register, which holds 1 there, into both ClassAllocValid and Flags).
// Model: the landed NScript::CV_IntroductionToTrophiesScript::Main 0x00E9C4B0 / CQR_EscortTraderScript::Main 0x00E1E110.
// Shape: every new-expression's name string is a conditional temporary with its own ebx flag bit; the SetQuestCardObjective
//   strings are built right to left, the GetActiveQuestName result last.
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
#include "engine/CCountedPointer.h"
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
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
};
namespace NScript {
class CQS_MeetSisterScript : public CScriptBase {
public:
    class CMeetSisterMessenger {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQS_MeetSisterScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E28F70
    };
    class CMeetSisterSister {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQS_MeetSisterScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E29040
    };
    CGameScriptInterface* Game;                         // +0x40
    virtual void Main();
};

void CQS_MeetSisterScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQS_MeetSisterScript>("MeetSisterMessenger", this, &CMeetSisterMessenger::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQS_MeetSisterScript>("MeetSisterSister", this, &CMeetSisterSister::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_THERESA_MOTHER_INFO_2_SUMMARY", "HauntedHouse", "BarrowFields");
}
}   // namespace NScript
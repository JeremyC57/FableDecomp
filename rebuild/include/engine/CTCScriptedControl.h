#ifndef FABLE_ENGINE_CTCSCRIPTEDCONTROL_H
#define FABLE_ENGINE_CTCSCRIPTEDCONTROL_H
#include "engine/CTCBase.h"
#ifdef FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONLIST
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#endif
// TPI0x21fc6 original base and fields; two donor list12 become STLport list4.
// Native004c70f0 reads Locked+18 before its continuation predicate.
// Retail opt-in (chunk 765, rows 0x007128F0 / 0x00712A70 / 0x007122B0, the scripted-control actions' OnActivate):
// FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE adds the nested action base CTCScriptedControl::CActionBase (RTTI
// .?AVCActionBase@CTCScriptedControl@@ : CBaseClassNonCopyable : CBaseClass; vtable 0x01265504). Retail slots: 0 scalar deleting
// dtor 0x007185B0, 1 Update _purecall, 2 OnActivate 0x00711FD0 (`mov byte [ecx+8],1`), 3 OnAdd 0x00711FE0 (ret), 4 GetActionName
// _purecall, 5 OnCleanup 0x00712000 (ret); slot names from the overriders' Ego_r.pdb / FableWin.pdb spellings
// (?Update / ?OnActivate / ?OnAdd / ?GetActionName@...@NTCScriptedControl@@, ?OnActivate / ?OnAdd / ?OnCleanup@CActionBase@CTCScriptedControl@@UAEXXZ).
// OnActivate is an inline virtual (out of line for the vtable, expanded in the overriders' qualified calls: 0x007128F3).
// Members (Ego_r.pdb, sizeof 0xc): PTCScript +0x04, Activated +0x08, Terminated +0x09. Default path unchanged.
#ifdef FABLE_RETAIL_CTCSCRIPTEDCONTROL_CREATURE
class CThingCreatureBase;
#endif
class CTCScriptedControl : public CTCBase {
private:
    unsigned char PScriptHandleImp[8]; // CIntelligentPointer<CScriptGameResourceObjectScriptedThing>.
#ifdef FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONLIST
    // Opt-in (agentNF5, 2026-11-10, row 0x007137D0 AddAction; needs FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE, FABLE_RETAIL_STLP_LIST
    // and engine/CCountedPointer.h): the member typed as Ego_r.pdb has it, STLport list<CCountedPointer<CActionBase> > (4 bytes,
    // +0x14). Same layout as the byte array below.
public:
    class CActionBase;
private:
    std::list<CCountedPointer<CActionBase> > PActionList;
#else
    unsigned char PActionList[4]; // std::list<CCountedPointer<CActionBase> >.
#endif
    bool Locked;unsigned char _pad_0x19[3];unsigned char ScriptAIPriority[4]; // EScriptAIPriority follows aligned at+1c.
    unsigned char AttitudeTags[4]; // std::list<std::pair<long,EConversationAttitude> >.
public:
#ifdef FABLE_RETAIL_CTC_SCRIPTED_CONTROL_DIE_PASS30
    // Original public override, FableWin and retail scripted control slot 13.
    virtual void OnDie();
#endif
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SCRIPTED_CONTROL; } bool IsActive() const;
#ifdef FABLE_RETAIL_CTCSCRIPTEDCONTROL_CLEANUPALLACTIONS
    // Retail opt-in (p25F, row 0x00714370): the landed 0x00713630 ?CleanupAllActions@CTCScriptedControl@@QAEXXZ.
    void CleanupAllActions();                                            // 0x00713630
#endif
#ifdef FABLE_RETAIL_CTCSCRIPTEDCONTROL_ISACTIONLISTEMPTY
    // Opt-in (NF108 2026-11-19, row 0x004C70F0 IsActive): Ego_r.pdb 0x007262B0 + FableWin.pdb 0x0255AA00
    // ?IsActionListEmpty@CTCScriptedControl@@QBE_NXZ, out of line in retail at 0x007122C0 (called from IsActive). Declaration only.
    bool IsActionListEmpty() const;                                      // 0x007122C0
#endif
#ifdef FABLE_RETAIL_CTCSCRIPTEDCONTROL_ISLOCKED
    // Retail opt-in (chunk 1010b, row 0x0089B5B0 CGameScriptInterface::StartScriptingEntity): Ego_r.pdb / FableWin.pdb
    // ?IsLocked@CTCScriptedControl@@QBE_NXZ, inline in retail (`mov al,[esi+18h] ; test al,al` re-read every loop pass,
    // 0x0089B65E / 0x0089B687). Body only: no layout change.
    bool IsLocked() const { return Locked; }
#endif
#ifdef FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
    class CActionBase;
#ifdef FABLE_RETAIL_CTCSCRIPTEDCONTROL_ADD_ACTION
    // Retail opt-in (Codex pass18): both PDBs; native007137d0 takes this nested base.
    void AddAction(CActionBase* action);
#endif
#ifdef FABLE_RETAIL_CTCSCRIPTEDCONTROL_CREATURE
    CThingCreatureBase& GetParentCreature();
#endif
#endif
};

FABLE_STATIC_ASSERT_AT(57, sizeof(CTCScriptedControl)==0x24);

#ifdef FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#include "engine/CBaseClassNonCopyable.h"
class CCharString;
class CThingCreatureBase;
class CTCScriptedControl::CActionBase : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE_INLINE_CTOR
    // NF79 opt-in (row 0x00713140 NTCScriptedControl::CActionDoCreatureAction ctor): the constructor expanded in a derived constructor's
    // TU (retail 0x00713140 / 0x00713180: the root ctor 0x0099A2F0, then PTCScript / Activated / Terminated zeroed from one register, no
    // CActionBase vtable store before the derived one); the same three stores as the out-of-line copy 0x00711FA0.
    CActionBase() : PTCScript(0), Activated(false), Terminated(false) { }
#else
    CActionBase();
#endif
    virtual ~CActionBase();                                              // slot 0  0x007185B0 (scalar deleting)
    virtual void Update() = 0;                                           //  1 _purecall
    virtual void OnActivate() { Activated = true; }                      //  2 0x00711FD0
    virtual void OnAdd();                                                //  3 0x00711FE0
    virtual CCharString GetActionName() const = 0;                       //  4 _purecall
    virtual void OnCleanup();                                            //  5 0x00712000
    bool IsActive() const;
    bool IsTerminated() const;
    void Terminate();
    void SetScript(CTCScriptedControl* script) { PTCScript = script; }
protected:
    CThing& GetParentThing() { return PTCScript->GetParentThing(); }
    const CThing& GetParentThing() const { return PTCScript->GetParentThing(); }
    CThingCreatureBase& GetParentCreature();
    void SetThingToNotAvoid(const CThing*);
    CTCScriptedControl* PTCScript;                                       // +0x04
    bool Activated;                                                      // +0x08
    bool Terminated;                                                     // +0x09
};
FABLE_STATIC_ASSERT_AT(85, sizeof(CTCScriptedControl::CActionBase) == 0xc);
#endif
#endif

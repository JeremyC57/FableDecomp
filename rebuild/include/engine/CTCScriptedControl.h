#ifndef FABLE_ENGINE_CTCSCRIPTEDCONTROL_H
#define FABLE_ENGINE_CTCSCRIPTEDCONTROL_H
#include "engine/CTCBase.h"
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
    unsigned char PActionList[4]; // std::list<CCountedPointer<CActionBase> >.
    bool Locked;unsigned char _pad_0x19[3];unsigned char ScriptAIPriority[4]; // EScriptAIPriority follows aligned at+1c.
    unsigned char AttitudeTags[4]; // std::list<std::pair<long,EConversationAttitude> >.
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SCRIPTED_CONTROL; } bool IsActive() const;
#ifdef FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
    class CActionBase;
#ifdef FABLE_RETAIL_CTCSCRIPTEDCONTROL_CREATURE
    CThingCreatureBase& GetParentCreature();
#endif
#endif
};

FABLE_STATIC_ASSERT(sizeof(CTCScriptedControl)==0x24);

#ifdef FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#include "engine/CBaseClassNonCopyable.h"
class CCharString;
class CThingCreatureBase;
class CTCScriptedControl::CActionBase : public CBaseClassNonCopyable {
public:
    CActionBase();
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
FABLE_STATIC_ASSERT(sizeof(CTCScriptedControl::CActionBase) == 0xc);
#endif
#endif

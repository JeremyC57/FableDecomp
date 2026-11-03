#ifndef FABLE_ENGINE_CTCSCRIPTEDCONTROL_H
#define FABLE_ENGINE_CTCSCRIPTEDCONTROL_H
#include "engine/CTCBase.h"
// TPI0x21fc6 original base and fields; two donor list12 become STLport list4.
// Native004c70f0 reads Locked+18 before its continuation predicate.
class CTCScriptedControl : public CTCBase {
private:
    unsigned char PScriptHandleImp[8]; // CIntelligentPointer<CScriptGameResourceObjectScriptedThing>.
    unsigned char PActionList[4]; // std::list<CCountedPointer<CActionBase> >.
    bool Locked;unsigned char _pad_0x19[3];unsigned char ScriptAIPriority[4]; // EScriptAIPriority follows aligned at+1c.
    unsigned char AttitudeTags[4]; // std::list<std::pair<long,EConversationAttitude> >.
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SCRIPTED_CONTROL; } bool IsActive() const;
};

FABLE_STATIC_ASSERT(sizeof(CTCScriptedControl)==0x24);
#endif

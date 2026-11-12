#ifndef FABLE_ENGINE_NTCSCRIPTEDCONTROL_CSCRIPTCONVERSATION_H
#define FABLE_ENGINE_NTCSCRIPTEDCONTROL_CSCRIPTCONVERSATION_H
// NTCScriptedControl::CScriptConversation (agentFAM11, 2026-11-12): the scripted-control conversation action object. A different
// class from the global ::CScriptConversation of engine/CScriptConversation.h (CScriptConversationManager's map value, no vptr).
// Polymorphic: RTTI .?AVCScriptConversation@NTCScriptedControl@@ vtable 0x01265520, slot 0 = 0x00718840; both PDBs name
// ??1CScriptConversation@NTCScriptedControl@@UAE@XZ (virtual) and ??_E... (vector deleting destructor), and
// CActionTalkBase::Conversation (Ego_r) is CCountedPointer<NTCScriptedControl::CScriptConversation>.
// Only the destructor is declared; the members are not modelled (no layout).
namespace NTCScriptedControl {
class CScriptConversation {
public:
    virtual ~CScriptConversation();
};
}
#endif

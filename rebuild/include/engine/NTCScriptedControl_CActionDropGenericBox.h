#ifndef FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONDROPGENERICBOX_H
#define FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONDROPGENERICBOX_H
// NTCScriptedControl::CActionDropGenericBox (new shared header, agentNF77, row 0x00715560 Update): the scripted-control action that makes
// a creature put down the generic box it carries. RTTI .?AVCActionDropGenericBox@NTCScriptedControl@@ : CTCScriptedControl::CActionBase
// (ghidra_out/rtti_bases.tsv); retail vtable 0x0128D7BC: 0 scalar deleting dtor 0x00905110, 1 Update 0x00715560, 2 base OnActivate,
// 3 base OnAdd, 4 GetActionName 0x009036D0, 5 base OnCleanup (rebuild/manifest/rtti-vtables.tsv). Ego_r.pdb members (struct_layouts_egor.tsv,
// sizeof 20): ActionID +0x0C, ActionPluggedIn +0x10 (no STLport member: the PDB offsets hold; Update reads [edi+0Ch] / [edi+10h]).
// Methods: FableWin.pdb ??0CActionDropGenericBox@NTCScriptedControl@@QAE@XZ (retail 0x009036A0)
// and ??1CActionDropGenericBox@NTCScriptedControl@@UAE@XZ (FableWin only); Ego_r + FableWin ?Update@CActionDropGenericBox@NTCScriptedControl@@UAEXXZ (Ego_r 0x00728FC0) and
// ?GetActionName@CActionDropGenericBox@NTCScriptedControl@@UBE?AVCCharString@@XZ. Declarations only.
// Includer contract: FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE, FABLE_RETAIL_CCHARSTRING and fable_string.h.
#include "engine/CTCScriptedControl.h"
namespace NTCScriptedControl {
class CActionDropGenericBox : public CTCScriptedControl::CActionBase {
public:
    CActionDropGenericBox();                                             // 0x009036A0
    virtual ~CActionDropGenericBox();
    virtual void Update();                                               //  1 0x00715560
    virtual CCharString GetActionName() const;                           //  4 0x009036D0
private:
    long ActionID;                                                       // +0x0c
    bool ActionPluggedIn;                                                // +0x10
};
}
FABLE_STATIC_ASSERT(sizeof(NTCScriptedControl::CActionDropGenericBox) == 0x14);
#endif

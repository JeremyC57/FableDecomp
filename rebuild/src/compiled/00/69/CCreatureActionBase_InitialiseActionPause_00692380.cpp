// Leaf: ?CCreatureActionBase_InitialiseActionPause@@YIXPAVCCreatureActionBase@@@Z
#include "engine/CCreatureActionBase.h"
// Data symbol (NF415, 2026-11-25): the registered description was the bare VA 0x13bac90; it is the const CCharString
//   ACTION_PAUSE_ON_THIS_FRAME @ 0x013BAC90 (Ego_r.pdb + FableWin.pdb `dynamic initializer for 'ACTION_PAUSE_ON_THIS_FRAME'';
//   reloc_bindings ?ACTION_PAUSE_ON_THIS_FRAME@@3VCCharString@@B, defined by the landed initializer row 0x0121E900).
class CCharString;
extern const CCharString ACTION_PAUSE_ON_THIS_FRAME;   // 0x013BAC90

struct ActionRegistrar { void RegisterAction(int actionId, void* description); };
extern "C" ActionRegistrar* __cdecl GetActionRegistrar();

struct CCreatureActionBaseVirtuals {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6();
    virtual int GetActionId();
};

void __fastcall CCreatureActionBase_InitialiseActionPause(CCreatureActionBase* action) {
    CCreatureActionBaseVirtuals* virtuals =
        reinterpret_cast<CCreatureActionBaseVirtuals*>(action);
    int actionId = virtuals->GetActionId();
    if (actionId > 0) {
        ActionRegistrar* registrar = GetActionRegistrar();
        registrar->RegisterAction(actionId, (void*)&ACTION_PAUSE_ON_THIS_FRAME);
    }
}

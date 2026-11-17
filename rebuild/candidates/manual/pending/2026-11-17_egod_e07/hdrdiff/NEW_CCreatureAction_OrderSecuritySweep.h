#ifndef FABLE_ENGINE_CCREATUREACTION_ORDERSECURITYSWEEP_H
#define FABLE_ENGINE_CCREATUREACTION_ORDERSECURITYSWEEP_H
// New header (egod e07, row 0x00911300): Ego_r.pdb ??0CCreatureAction_OrderSecuritySweep@@QAE@AAVCThingCreatureBase@@@Z (Ego_r 0x00935E90,
// right before ??0CCreatureAction_Curiosity 0x00935F00 = retail 0x00911180) = retail 0x009110F0 (CCharString "ALARM", the base
// ctor 0x00693B30, vtable 0x0128F74C; `ret 4`). A CCreatureActionBase with no own members (retail sizeof 0xA8). Declaration only.
#include "engine/CCreatureActionBase.h"
class CThingCreatureBase;
class CCreatureAction_OrderSecuritySweep : public CCreatureActionBase {
public:
    CCreatureAction_OrderSecuritySweep(CThingCreatureBase& creature);   // 0x009110F0
    virtual void PerformAction();                                        // Ego_r ?PerformAction@CCreatureAction_OrderSecuritySweep@@UAEXXZ
    virtual CCreatureActionBase* Clone() const;                          // Ego_r ?Clone@CCreatureAction_OrderSecuritySweep@@UBEPAVCCreatureActionBase@@XZ
};
#endif

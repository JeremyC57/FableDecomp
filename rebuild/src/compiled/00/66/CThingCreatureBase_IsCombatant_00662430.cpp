#include "rebuild_abi.h"
// CThingCreatureBase::IsCombatant @ 0x00662430
struct CThingCreatureBase;
struct Sub { void Add(CThingCreatureBase *self); };
struct Obj { char pad[0x4c]; Sub *m_sub; };
extern "C" Obj *FABLE_FASTCALL FableGet_662430(CThingCreatureBase *self);
struct CThingCreatureBase { char dummy; void IsCombatant(); };
void CThingCreatureBase::IsCombatant(){
    Sub *s = FableGet_662430(this)->m_sub;
    s->Add(this);
}
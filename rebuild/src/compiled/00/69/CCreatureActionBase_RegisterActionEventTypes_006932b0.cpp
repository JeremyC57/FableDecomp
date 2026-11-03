// CCreatureActionBase::RegisterActionEventTypes() -- install the seven original action-event prototypes.
// Leaf: ?RegisterActionEventTypes@CCreatureActionBase@@SIXXZ
// Flags: /Gr
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureActionBase.h"
#include "engine/NCreatureActionEvents.h"
void __fastcall CCreatureActionBase::RegisterActionEventTypes()
{
 {
  CCountedPointer<NCreatureAction::CActionEventBase> cp_event = new NCreatureAction::CActionEventTriggerParticle;
  RegisteredActionEvents.insert(std::make_pair(cp_event->GetHook(),cp_event));
 }
 {
  CCountedPointer<NCreatureAction::CActionEventBase> cp_event = new NCreatureAction::CActionEventCameraShake;
  RegisteredActionEvents.insert(std::make_pair(cp_event->GetHook(),cp_event));
 }
 {
  CCountedPointer<NCreatureAction::CActionEventBase> cp_event = new NCreatureAction::CActionEventSound;
  RegisteredActionEvents.insert(std::make_pair(cp_event->GetHook(),cp_event));
 }
 {
  CCountedPointer<NCreatureAction::CActionEventBase> cp_event = new NCreatureAction::CActionEventShot;
  RegisteredActionEvents.insert(std::make_pair(cp_event->GetHook(),cp_event));
 }
 {
  CCountedPointer<NCreatureAction::CActionEventBase> cp_event = new NCreatureAction::CActionEventExplosion;
  RegisteredActionEvents.insert(std::make_pair(cp_event->GetHook(),cp_event));
 }
 {
  CCountedPointer<NCreatureAction::CActionEventBase> cp_event = new NCreatureAction::CActionEventAddObject;
  RegisteredActionEvents.insert(std::make_pair(cp_event->GetHook(),cp_event));
 }
 {
  CCountedPointer<NCreatureAction::CActionEventBase> cp_event = new NCreatureAction::CActionEventRemoveObject;
  RegisteredActionEvents.insert(std::make_pair(cp_event->GetHook(),cp_event));
 }
 DebugEventTrace=false;
}
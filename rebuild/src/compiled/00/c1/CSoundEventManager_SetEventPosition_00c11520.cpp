// 00c11520 CSoundEventManager::SetEventPosition
// Descriptive libpk names; native slot27 and position-field behavior, no original PDB coverage.
// Flags: /EHsc
// Leaf: ?SetEventPosition@CSoundEventManager@@UAEXJABVC3DVector@@@Z
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_STREAMED_PASS23
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_EVENT_POSITION_PASS23
#define FABLE_RETAIL_CSOUNDEVENTGROUP
#include "libpk/CSoundEventManager.h"
#include "libpk/CSoundEventGroup.h"
#include "libpk/CSoundPosition.h"
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;
void CSoundEventManager::SetEventPosition(long key,const C3DVector& position)
{
 PKCProtectionInstance lock(&g_lock_0143bb14);
 CSoundEvent* const& event=m_events.FindInChildThenSelf(key);
 if(event){
  CSoundPosition data;
  data.Relative=event->GetPositionFlag(this);
  data.Position=position;
  event->Set3DPosition(this,data);
 }
}
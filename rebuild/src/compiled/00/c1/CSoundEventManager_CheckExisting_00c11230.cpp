// 00c11230 CSoundEventManager::CheckExisting
// Descriptive library names; original libpk names absent from all supplied PDB name sets.
// Flags: /EHsc
// Leaf: ?CheckExisting@CSoundEventManager@@QAEHPAVCSoundSample@@JJJJK@Z
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_CHECK_EXISTING_PASS23
#define FABLE_RETAIL_CSOUNDEVENTGROUP
#include "libpk/CSoundEventManager.h"
#include "libpk/CSoundEventGroup.h"
#include "libpk/CSampleRef.h"
int CSoundEventManager::CheckExisting(CSoundSample* sample,long a,long b,long c,long force,unsigned long priority)
{
 CSampleRef key;
 key.Bind(sample,a,b,c);
 CSoundEvent* event=ListeningEvents.FindByKey(&key);
 key.Release();
 if(!event)return 1;
 if(!event->IsAudible(this)){
  event->Stop(this);
  return 1;
 }
 if((event->Pending||force)&&priority>=event->Priority){
  event->RestartPlayback(this);
  return 1;
 }
 return 0;
}
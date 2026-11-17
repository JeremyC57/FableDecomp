// CSoundEventManager::StopEventsWithField24(void*) -- EH-framed (vtable 0x012B2748 slot 20): under the module's scoped lock over the critical section @ 0x0143BB14, every active event (@ 0x00C20840) of the manager's fixed event pool (event set +0x2C, pool +0x5C, count +0x60, accessor @ 0x00C13620) whose +0x24 key is the argument is stopped with the manager (@ 0x00C21620); then every pooled resource the event set still finds for the key (@ 0x00C21FA0, re-queried after each) is deactivated with the manager (@ 0x00C26580)
// Leaf: ?StopEventsWithField24@CSoundEventManager@@UAEXPAX@Z
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C12520. Built under /EHsc. Previous header line: CInventory_00c12520::UnbindKey(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, every bound entity of the array at +0x5c (count at +0x60, accessor @ 0x00C13620, bound test @ 0x00C20840) whose word at +0x24 equals the key is released with `this` (@ 0x00C21620); then every observer the observable at +0x2c yields for the key (@ 0x00C21FA0, re-queried after each) is detached from `this` (@ 0x00C26580)
// NF63 (round 7): moved onto libpk/CSoundEventManager.h (the header's virtual StopEventsWithField24, EVENT_POSITION_PASS23 slot 20 =
// 0x012B2748 + 0x50), CSoundEventSet.h (overlay _EVENTPOOL, _FINDWITHFIELD), CSoundEventGroup.h (CSoundEvent, overlay _KEYFIELDS /
// _ISACTIVE) and CPooledResource.h (overlay _DEACTIVATE); the callees bind ?IsActive@CSoundEvent@@QAEJXZ,
// ?Stop@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z, ?Get@?$PKCFrallocStatic@VCSoundEvent@@@@QAEPAVCSoundEvent@@K@Z,
// ?FindWithField24@CSoundEventSet@@QAEPAVCPooledResource@@PAX@Z and ?Deactivate@CPooledResource@@QAEXPAUCSoundEventManager@@@Z (were the
// address-named CInventory_ / CEntity_ / CEntityArray_ / CObservable_ / CObserver_00c12520 stand-ins). Names of libpk members are
// descriptive (no PDB covers libpk); the self was the propagated catalog label Save_WriteEquipmentState.
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_STREAMED_PASS23
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_EVENT_POSITION_PASS23
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_CHECK_EXISTING_PASS23
#define FABLE_RETAIL_CSOUNDEVENTSET_EVENTPOOL
#define FABLE_RETAIL_CSOUNDEVENTSET_FINDWITHFIELD
#define FABLE_RETAIL_CSOUNDEVENTGROUP
#define FABLE_RETAIL_CSOUNDEVENT_KEYFIELDS
#define FABLE_RETAIL_CSOUNDEVENT_ISACTIVE
#define FABLE_RETAIL_CPOOLEDRESOURCE_DEACTIVATE
#include "libpk/PKCProtectionInstance.h"
#include "libpk/CSoundEventManager.h"
#include "libpk/CSoundEventGroup.h"
#include "libpk/CPooledResource.h"
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

void CSoundEventManager::StopEventsWithField24(void* key)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    for (unsigned long i = 0; i < m_events.Events.m_count; ++i) {
        CSoundEvent* event = m_events.Events.Get(i);
        if (event->IsActive() && event->Field24 == key)
            event->Stop(this);
    }
    for (CPooledResource* resource = m_events.FindWithField24(key); resource; resource = m_events.FindWithField24(key))
        resource->Deactivate(this);
}

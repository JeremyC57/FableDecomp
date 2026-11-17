// CSoundEventManager::StopEventsWithField18(void*) -- EH-framed: under the module's scoped lock over the critical section @ 0x0143BB14, every active event (@ 0x00C20840) of the manager's fixed event pool (event set +0x2C, pool +0x5C, count +0x60, accessor @ 0x00C13620) whose +0x18 key is the argument is stopped with the manager (@ 0x00C21620); the count is re-read each iteration
// Leaf: ?StopEventsWithField18@CSoundEventManager@@QAEXPAX@Z
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C120A0, oracle length 148 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push ebx ; push edi ; mov ebx,ecx ; push offset g_lock ; lea ecx,[esp+0Ch] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[ebx+60h] ; xor edi,edi ; cmp eax,edi ; mov [esp+18h],edi ; jbe done ; push ebp ; push esi ; lea ebp,[ebx+5Ch] ; loop: push edi ; mov ecx,ebp ; call CArray::Get ; mov esi,eax ; mov ecx,esi ; call CEntity::IsBound ; test eax,eax ; je next ; mov eax,[key] ; cmp [esi+18h],eax ; jne next ; push ebx ; mov ecx,esi ; call CEntity::Process ; next: mov eax,[ebx+60h] ; inc edi ; cmp edi,eax ; jb loop ; pop esi ; pop ebp ; done: lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~PKCProtectionInstance ; ...
// The sibling of the manager's virtual StopEventsWithField10 0x00C12460 / StopEventsWithField24 0x00C12520 (vtable 0x012B2748 slots 21 /
// 20) without their listener walk; no caller or table reference in retail.
// NF63 (round 7): moved onto libpk/CSoundEventManager.h (STREAMED_PASS23 layout: the event set at +0x2C), CSoundEventSet.h (overlay
// FABLE_RETAIL_CSOUNDEVENTSET_EVENTPOOL: PKCFrallocStatic<CSoundEvent> at set+0x30) and CSoundEventGroup.h (CSoundEvent, overlay
// _KEYFIELDS / _ISACTIVE); the callees bind ?IsActive@CSoundEvent@@QAEJXZ, ?Stop@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z and
// ?Get@?$PKCFrallocStatic@VCSoundEvent@@@@QAEPAVCSoundEvent@@K@Z (were the address-named CInventory_ / CEntity_ / CEntityArray_00c120a0
// stand-ins). The self name is descriptive (libpk has no PDB), in the header's StopEventsWithField<offset> spelling; it was the
// propagated catalog label Inventory_ProcessKeyboardBinding.
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_STREAMED_PASS23
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_CHECK_EXISTING_PASS23
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_STOPFIELD18
#define FABLE_RETAIL_CSOUNDEVENTSET_EVENTPOOL
#define FABLE_RETAIL_CSOUNDEVENTGROUP
#define FABLE_RETAIL_CSOUNDEVENT_KEYFIELDS
#define FABLE_RETAIL_CSOUNDEVENT_ISACTIVE
#include "libpk/PKCProtectionInstance.h"
#include "libpk/CSoundEventManager.h"
#include "libpk/CSoundEventGroup.h"
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

void CSoundEventManager::StopEventsWithField18(void* key)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    for (unsigned long i = 0; i < m_events.Events.m_count; ++i) {
        CSoundEvent* event = m_events.Events.Get(i);
        if (event->IsActive() && event->Field18 == key)
            event->Stop(this);
    }
}

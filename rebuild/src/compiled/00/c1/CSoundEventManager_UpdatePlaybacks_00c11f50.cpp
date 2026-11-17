// CSoundEventManager::UpdatePlaybacks() -- EH-framed: under the module's scoped lock over the critical section @ 0x0143BB14, every playback slot of the event set's Playbacks pool (manager +0x224, count +0x228, accessor @ 0x00C13700) that holds an event (+4) is updated (@ 0x00C276B0) and its event, unless audible to the manager (@ 0x00C20C10), is stopped (@ 0x00C21620); then the manager's pending items are assigned (@ 0x00C10CD0)
// Leaf: ?UpdatePlaybacks@CSoundEventManager@@QAEXXZ
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C11F50, oracle length 167 bytes. Built under /EHsc. Previous header line: CEditControlCentre_00c11f50::ProcessThemeElements() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, every entry of the array at +0x224 (count at +0x228, accessor @ 0x00C13700) that has an element at +4 refreshes its display (@ 0x00C276B0) and, unless the element validates against `this` (@ 0x00C20C10), the element is processed with `this` (@ 0x00C21620); then the centre refreshes out of line (@ 0x00C10CD0)
// Shape: the event pointer is re-read after the slot update (the register is reused).
// NF63 (round 7): moved onto libpk/CSoundEventManager.h (STREAMED + LISTENER_SPACE layout: the event set at +0x2C, its Playbacks
// PKCFrallocStatic<CPlaybackSlot> at set+0x1F8), CSoundEventGroup.h (CSoundEvent IsAudible / Stop, CHECK_EXISTING_PASS23) and
// CPlaybackSlot.h (overlay _UPDATE); the callees bind ?IsAudible@CSoundEvent@@QAE_NPAUCSoundEventManager@@@Z (q10493),
// ?Stop@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z, ?Get@?$PKCFrallocStatic@UCPlaybackSlot@@@@QAEPAUCPlaybackSlot@@K@Z,
// ?Update@CPlaybackSlot@@QAEXXZ and ?AssignPendingItems@CSoundEventManager@@QAEXXZ (were the address-named CEditControlCentre_ /
// CElement_ / CEntry_ / CEntryArray_00c11f50 stand-ins). Self name descriptive (libpk has no PDB; the catalog's
// CAIStateGroup_OnMapTextureChanged / ProcessThemeElements are propagated labels).
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_STREAMED_PASS23
#define FABLE_RETAIL_LISTENER_SPACE_SOURCE_PASS23
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_CHECK_EXISTING_PASS23
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_UPDATEPLAYBACKS
#define FABLE_RETAIL_CSOUNDEVENTGROUP
#define FABLE_RETAIL_CPLAYBACKSLOT_UPDATE
#include "libpk/PKCProtectionInstance.h"
#include "libpk/CSoundEventManager.h"
#include "libpk/CSoundEventGroup.h"
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

void CSoundEventManager::UpdatePlaybacks()
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    for (unsigned long i = 0; i < m_events.Playbacks.m_count; ++i) {
        CPlaybackSlot* slot = m_events.Playbacks.Get(i);
        if (slot->PEvent) {
            slot->Update();
            CSoundEvent* event = slot->PEvent;
            if (!event->IsAudible(this))
                event->Stop(this);
        }
    }
    AssignPendingItems();
}

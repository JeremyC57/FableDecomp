// 00c11520 CSoundEventManager::SetEventPosition(long, const CSoundPositionVector&) -- updates XYZ while preserving the coordinate mode under the audio lock.
// Grade: DIFFER(163v163) -- one X store follows rather than precedes the packet-address push.
// Retail: Fable.exe 0x00c11520, 163 bytes, thiscall, ret 8; descriptive vendor spellings.
// Flags: /EHsc
// Leaf: ?SetEventPosition@CSoundEventManager@@UAEXJABUCSoundPositionVector@@@Z
#include "libpk/PKCProtectionInstance.h"
#include "libpk/LHAudioEventPosition.h"
extern PKCCriticalSection LHAudioCriticalSection;

void CSoundEventManager::SetEventPosition(long key, const CSoundPositionVector& position)
{
    PKCProtectionInstance lock(&LHAudioCriticalSection);
    CSoundEvent* entry = EventSet.FindInChildThenSelf(key);
    if (entry) {
        CSoundPosition record;
        record.Relative = entry->IsPositionRelative(this);
        record.Position = position;
        entry->SetPosition(this, &record);
    }
}

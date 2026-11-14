// CSoundManagerLH::SetMonitorAllowedPostStopTailDurationMS(unsigned long duration) -- forward the monitor's allowed post-stop tail duration to the sound system's event manager, when the system exists
// Leaf: ?SetMonitorAllowedPostStopTailDurationMS@CSoundManagerLH@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3E830, 24 bytes, `ret 4` (the forwarded call is a tail jump).
// Name: CASoundManager slot 35 (engine/CASoundManager.h: Anniversary 360 PDB interface + the NGlobalConsole::ConsoleAudio_ commands); vtable 0x0129D2C4.
#include "engine/CSoundManagerLH.h"

void CSoundManagerLH::SetMonitorAllowedPostStopTailDurationMS(unsigned long duration)
{
    if (SoundSystem)
        SoundSystem->GetEventManager()->SetMonitorAllowedPostStopTailDurationMS(duration);
}

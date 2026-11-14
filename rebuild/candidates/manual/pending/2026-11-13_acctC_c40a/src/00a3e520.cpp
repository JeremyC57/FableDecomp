// CSoundManagerLH::StopMusic(unsigned long fadeOutMS) -- stop the playing music stream with a fade-out, when the sound system exists
// Leaf: ?StopMusic@CSoundManagerLH@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3E520, 24 bytes, `ret 4` (the forwarded call is a tail jump).
// Name: CASoundManager slot 18 (engine/CASoundManager.h: Anniversary 360 PDB interface + the NGlobalConsole::ConsoleAudio_ commands); vtable 0x0129D2C4.
#include "engine/CSoundManagerLH.h"

void CSoundManagerLH::StopMusic(unsigned long fadeOutMS)
{
    if (SoundSystem)
        SoundSystem->GetPlayer()->Stop(fadeOutMS);
}

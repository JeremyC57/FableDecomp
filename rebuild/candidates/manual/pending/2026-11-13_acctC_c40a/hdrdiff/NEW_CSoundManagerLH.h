#ifndef FABLE_ENGINE_CSOUNDMANAGERLH_H
#define FABLE_ENGINE_CSOUNDMANAGERLH_H
// CSoundManagerLH (agentC40, rows 0x00A3E520 / 0x00A3E830): the libpk-backed sound manager (vtable 0x0129D2C4, RTTI CSoundManagerLH :
// CASoundManager, base 0x40 bytes in retail). Its first own member is the libpk sound system at +0x40 (assigned by the constructor
// 0x00A3EF00 from the factory 0x00C096E0; read by every forwarder: `mov eax,[ecx+40h]`). The member name is descriptive (the sound
// module is in no PDB as a class). Overrides: the CASoundManager slot names (engine/CASoundManager.h). Declarations only; the later
// members (+0x44 the 2-D emitter, +0x48 ..) are not modelled yet.
#ifndef FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CASoundManager.h"
#include "libpk/CSoundSystem.h"
class CSoundManagerLH : public CASoundManager {
public:
    virtual void StopMusic(unsigned long fadeOutMS);                                      // slot 18 0x00A3E520
    virtual void SetMonitorAllowedPostStopTailDurationMS(unsigned long duration);         // slot 35 0x00A3E830
    CSoundSystem* SoundSystem;                                           // +0x40 (descriptive)
};
FABLE_STATIC_ASSERT(offsetof(CSoundManagerLH, SoundSystem) == 0x40);
#endif

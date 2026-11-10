#ifndef FABLE_ENGINE_CTCSLEEP_H
#define FABLE_ENGINE_CTCSLEEP_H
#include "engine/CTCBase.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "fable_compiler.h"
// Retail opt-in (Codex pass 17): Ego_r complete 32-byte class and original
// public non-const getter. Native IsRunnable reads the stamp at +10h.
#ifdef FABLE_RETAIL_CTCSLEEP
class CTCSleep : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_SLEEP; }
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    long GetLastFrameWokeUp() { return LastFrameWokeUp; }
protected:
    bool IsReadyToSleep;
    bool FastTrack;
    long LastFrameWokeUp;
    C3DVector PosAtFastTrackSetting;
};
FABLE_STATIC_ASSERT(sizeof(CTCSleep)==32);
#endif
#endif

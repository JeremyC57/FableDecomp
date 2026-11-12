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
#ifdef FABLE_RETAIL_CTCSLEEP_FASTTRACK
    // chunk Q15 (row 0x0092E130): Ego_r.pdb ?GetFastTrack@CTCSleep@@QAE_NXZ (out of line, 0x0092C3B0) and
    // ?GetIsReadyToSleep@CTCSleep@@QAE_NXZ (inline in retail: CAIStateGroup_MinionSleep::IsRunnable reads +0Ch).
    bool GetFastTrack();
    bool GetIsReadyToSleep() { return IsReadyToSleep; }
#endif
protected:
    bool IsReadyToSleep;
    bool FastTrack;
    long LastFrameWokeUp;
    C3DVector PosAtFastTrackSetting;
};
FABLE_STATIC_ASSERT(sizeof(CTCSleep)==32);
#endif
#endif

#ifndef FABLE_ENGINE_CTCFLASHONHIT_H
#define FABLE_ENGINE_CTCFLASHONHIT_H
#include "engine/CTCBase.h"
// TPI0x323a5 original fields. Native00667e45/48 and007e33a0 prove offsets.
class CTCFlashOnHit : public CTCBase {
private: long FadeInTime,FadeOutTime,LifeTime,Timer;
public:
    void SetFadeInTime(long n) { FadeInTime=n; }
    void SetFadeOutTime(long n) { FadeOutTime=n; }
    void SetLifeTime(long);
};
FABLE_STATIC_ASSERT(sizeof(CTCFlashOnHit)==0x1c);

#endif

#ifndef FABLE_ENGINE_CTCEFFECTONDIE_H
#define FABLE_ENGINE_CTCEFFECTONDIE_H
#include "engine/CTCBase.h"
#include "fable_string.h"

class CTCEffectOnDie : public CTCBase {
public:
    void SetEffect(long effect) { Effect = effect; }
#ifdef FABLE_RETAIL_GHOST_SWORD_FIXTURE_PASS27
    CTCEffectOnDie(CThing&);
#endif
    void SetTimer(long timer) { Timer = timer; }
    void SetAlphaFadeOutTimer(long fade_out_timer) { FadeOutTimer = fade_out_timer; }
    void SetSoundEffect(const CCharString& sound) { DeathSoundCriteria = sound; }
private:
    bool StartedNonRecoverableFadeout;
    long Effect;
    long Timer;
    long FadeOutTimer;
    CCharString DeathSoundCriteria;
    bool Pausable;
    bool Paused;
};
FABLE_STATIC_ASSERT_AT(21, sizeof(CTCEffectOnDie) == 0x24);
#endif

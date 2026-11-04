// CTCEffectOnDie::OnDie() -- unless paused, play the death sound criteria through the parent's sound player and create the death effect thing at the parent's position
// Leaf: ?OnDie@CTCEffectOnDie@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0D00, 112 bytes; CTCBase vtable slot 13 (Ego_r.pdb ?OnDie@CTCEffectOnDie@@UAEXXZ).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inside CThing::GetTC<CTCSoundPlayer>),
//   CTCSoundPlayer::PlayAttachedCriteria(const CCharString&, bool, bool, bool, long) 0x0066BD00 (PDB
//   ?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z), CThing::GetPos 0x004C73D0, the static __fastcall
//   CTCDParticleEmitter::Create(long, const C3DVector&, bool) 0x006E0880.
// Layout: Ego_r.pdb CTCEffectOnDie (36 bytes, no STL): Effect +0x10, DeathSoundCriteria +0x1C, Paused +0x21.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCSOUNDPLAYER
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/C3DVector.h"
class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(long effect, const C3DVector& position, bool attached);   // 0x006E0880
};
class CTCEffectOnDie : public CTCBase {
public:
    virtual void OnDie();
private:
    bool        StartedNonRecoverableFadeout;      // +0x0c
    long        Effect;                            // +0x10
    long        Timer;                             // +0x14
    long        FadeOutTimer;                      // +0x18
    CCharString DeathSoundCriteria;                // +0x1c
    bool        Pausable;                          // +0x20
    bool        Paused;                            // +0x21
};

void CTCEffectOnDie::OnDie()
{
    if (!Paused) {
        CTCSoundPlayer* soundPlayer;
        if (ParentThing->GetTC(&soundPlayer))
            soundPlayer->PlayAttachedCriteria(DeathSoundCriteria, true, false, false, -1);
        if (Effect)
            CTCDParticleEmitter::Create(Effect, ParentThing->GetPos(), false);
    }
}
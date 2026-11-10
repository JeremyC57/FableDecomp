#ifndef FABLE_ENGINE_CTCPHYSICALSHIELDSPELL_H
#define FABLE_ENGINE_CTCPHYSICALSHIELDSPELL_H
// Retail opt-in (Codex pass 17): original PDB members, retail set12 instead of16; native size54.
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CRGBColour.h"
class CSpecialAbilitiesPhysicalShieldDef;
class CTCPhysicalShieldSpell:public CTCBase {public:
 void CancelMove();
 void SetAICasting(long level){SpellLevel=level;}
 void SetTimeLimit(long time){DurationTimer=time;}
 void SetShieldHealth(float health){ShieldHealth=health;}

protected:
 CDefPointer<const CSpecialAbilitiesPhysicalShieldDef> PDef;
 long SpellLevel,ShieldFadeOutTimer,CurShieldFadeOutTimer,DurationTimer;float ShieldHealth,CurWobbleTimer,WobbleDelta;
 CRGBColour ShieldColor;CIntelligentPointer<CThing> ShieldEffect,SwirlyEffect;unsigned long LoopingSound;bool Pausable,Paused,CutSceneCancelled;
 unsigned char AllowReactionAttackers[12];
};
#endif

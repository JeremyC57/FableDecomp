// CTCObjectAugmentations::FrameUpdate() -- maintain augmentation sounds and particle effects.
// Grade: DIFFER(1161v1067) -- The shared owner and STLport containers retain the native predicate calls and component names. Vector allocation, period scheduling, and iterator code generation still differ.
// Leaf: ?FrameUpdate@CTCObjectAugmentations@@UAEXXZ
// Retail: 0x00766DD0, 1067 bytes, thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_AUGMENTATION_PARTICLES
#define FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_FRAME_OUTLINE_HELPERS
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCObjectAugmentations.h"

void CTCObjectAugmentations::FrameUpdate()
{
    typedef std::map<EObjectAugmentationType, CObjectAugmentationSoundSet> SoundMap;
    for (SoundMap::iterator it_init_sounds = InitSounds.begin(); it_init_sounds != InitSounds.end(); ++it_init_sounds) {
        if (!it_init_sounds->second.Played) {
            CTCSoundPlayer* sound_player;
            if (GetParentThing().GetTC(&sound_player)) {
                it_init_sounds->second.Handle = sound_player->PlayAttachedCriteria(it_init_sounds->second.Sound, false, false, false, -1);
                it_init_sounds->second.Played = true;
            }
        }
    }
    for (SoundMap::iterator it_looping_sounds = LoopingSounds.begin(); it_looping_sounds != LoopingSounds.end(); ++it_looping_sounds) {
        if (!it_looping_sounds->second.Played) {
            CTCSoundPlayer* sound_player;
            if (GetParentThing().GetTC(&sound_player)) {
                it_looping_sounds->second.Handle = sound_player->PlayAttachedCriteria(it_looping_sounds->second.Sound, false, false, false, -1);
                it_looping_sounds->second.Played = true;
            }
        }
    }

    if (GetParentThing().HasTC(TCI_SHOT_DEFINITION) && !IS_THING_ALIVE(ProjectileTrailEffect)) {
        long effect = 0;
        if (HasCombinationOfAugmentations(static_cast<EObjectAugmentationType>(OBJECT_AUGMENTATION_TYPE_FLAME | OBJECT_AUGMENTATION_TYPE_LIGHTNING)))
            effect = PDef->FireAndLightningProjectileTrailEffect;
        else if (HasAugmentationOfType(OBJECT_AUGMENTATION_TYPE_FLAME))
            effect = PDef->FireProjectileTrailEffect;
        else if (HasAugmentationOfType(OBJECT_AUGMENTATION_TYPE_LIGHTNING))
            effect = PDef->LightningProjectileTrailEffect;
        if (effect) {
            ProjectileTrailEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, GetParentThing().GetPos(), false));
            if (IS_THING_ALIVE(ProjectileTrailEffect)) {
                CTCDParticleEmitter* particle_driver;
                ProjectileTrailEffect->GetTC(&particle_driver);
                particle_driver->AttachToThing(GetParentThing(), 0x28, PDef->WeaponTrailAttachmentPoint, 0, 0.0f);
            }
        }
    }

    if (!InStrikeAction) {
        bool all_dead = true;
        for (long i = 0; i < static_cast<long>(CurrentRunningAugEffect.size()); ++i) {
            if (IS_THING_ALIVE(CurrentRunningAugEffect[i]))
                all_dead = false;
        }
        if (all_dead) {
            if (CurPeriod > 0)
                --CurPeriod;
            else {
                long num_particles = static_cast<long>(ParticleEffects.size());
                if (num_particles > 0) {
                    CurrentRunningAugEffectIndex = (CurrentRunningAugEffectIndex + 1) % num_particles;
                    CurPeriod = ParticleEffects[CurrentRunningAugEffectIndex][0].PeriodicFrames;
                    CurrentRunningAugEffect.clear();
                    long num = static_cast<long>(ParticleEffects[CurrentRunningAugEffectIndex].size());
                    CurrentRunningAugEffect.resize(num);
                    for (long i = 0; i < num; ++i) {
                        ParticleAndEndPoints& particle = ParticleEffects[CurrentRunningAugEffectIndex][i];
                        AttemptToStartParticle(CurrentRunningAugEffect[i], particle.ParticleEffectID, particle.ParticleEffectIDToBlendTo,
                                               particle.StartPoint, particle.EndPoint);
                    }
                }
            }
        }
    }

    std::vector<std::vector<AttackParticleAndEndPoints> >::iterator bit = AttackParticleEffects.begin();
    const std::vector<std::vector<AttackParticleAndEndPoints> >::iterator bendit = AttackParticleEffects.end();
    for (; bit != bendit; ++bit) {
        std::vector<AttackParticleAndEndPoints>& apeps = *bit;
        for (long i = 0; i < static_cast<long>(apeps.size()); ++i) {
            AttackParticleAndEndPoints& apep = apeps[i];
            if (apep.ParticleEffect.Get())
                apep.ParticleEffect.Get();
        }
    }
}

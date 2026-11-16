// CTCWillLightning::CLightningBeam::FrameUpdate(CIntelligentPointer<CThingCreatureBase> player, unsigned long* seed) -- one frame of a will-lightning beam: advance the clock and frame count, run the beam state machine (delay, activating, active hits on the target, waiting for children, deactivating), then update the positions, feed LIGHTNING_START / LIGHTNING_END to the particle emitter and frame-update the children, erasing the dead ones
// Leaf: ?FrameUpdate@CLightningBeam@CTCWillLightning@@QAEXV?$CIntelligentPointer@VCThingCreatureBase@@@@PAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A1B20, 2840 bytes (+ the 5-entry state jump table 0x007A2624), thiscall, `ret 0Ch` (player by value 8 bytes + seed), frame 0xbc.
// Identity: Ego_r.pdb / FableWin.pdb ?FrameUpdate@CLightningBeam@CTCWillLightning@@QAEXV?$CIntelligentPointer@VCThingCreatureBase@@@@PAK@Z;
//   the body recurses into itself for each child (0x007A25BC).
// Inferred: 1 dead release assert (FABLE_RETAIL_ASSERT, overlay fable_assert.h, device d) after the decapitating hit's direction:
//   `FABLE_RETAIL_ASSERT(direction.X == direction.X && direction.Y == direction.Y && direction.Z == direction.Z)` (a NaN check). It emits
//   no bytes; its dead arm's IL reads the three components and re-keys c2's x87 stacking of that copy only: retail 0x007A1E68 stores X
//   with fstp and squares Z,Y,X (the plain-hit copy 0x007A206B keeps X with fst, as ours did without the assert); it also closes the
//   4-byte size gap and the end-loop head pad. Measured: `direction == direction`, `direction.IsNonZero()`, Y-only / Z-only NaN tests and
//   asserts without a direction read (any count 1..7 at function / case / clock / either copy) do not match; the X-only test and 1..4
//   copies of the three-component test match (1 kept).
// FITTED: the assert condition (chosen by grading among plausible validity checks of a freshly computed direction).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_UPDATEPARAMETERS
#define FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_SETCREATEDUST
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCWILLLIGHTNING_CLASS
#define FABLE_RETAIL_CTCWILLLIGHTNING_BEAM_FRAMEUPDATE
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_CVECTORMAP_INSERT_FORCEINLINE
#define FABLE_RETAIL_STLP_LIST_ERASE_NOINLINE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_XYZ
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#define FABLE_RETAIL_NCREATUREMODE_LIGHTNINGABLE
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_PLAYUNATTACHEDCRITERIA
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CCountedPointer.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
#include "engine/CHitParameters.h"
#include "engine/CTCElectrocutedEffect.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventHitBy.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/NCreatureMode.h"
#include "engine/CCRC.h"
#include "engine/CTCWillLightning.h"
#include "engine/CSpecialAbilitiesLightningSpellDef.h"

void CTCWillLightning::CLightningBeam::FrameUpdate(CIntelligentPointer<CThingCreatureBase> player, unsigned long* seed)
{
    m_fTime += 1.0f / (float)CMainGameComponent::GetConstantFPS();
    ++m_nFrameCount;
    switch (m_lsState) {
    case 1:
        if (m_fTime - m_fTimeBase >= m_fDelay) {
            m_fTimeBase = m_fTime;
            m_lsState = (ELightningState)2;
            if (IS_THING_ALIVE(m_tcpHome)) {
                CTCWillLightning* will;
                if (m_tcpHome->GetTC(TCI_WILL_LIGHTNING, &will))
                    will->StartLoopingSound();
            }
        }
        break;
    case 2:
        if (m_bTemporary || (IS_THING_ALIVE(m_tcpHome) && IS_THING_ALIVE(m_tcpTarget))) {
            if (m_fTime - m_fTimeBase >= m_fActivationTime) {
                m_fTimeBase = m_fTime;
                m_lsState = (ELightningState)3;
                m_nFrameCount = -1;
                for (std::list<CCountedPointer<CLightningBeam> >::iterator it = m_lChildren.begin(); it != m_lChildren.end(); ++it) {
                    CCountedPointer<CLightningBeam> child = *it;
                    child->Activate();
                }
            }
        } else
            Deactivate(player);
        break;
    case 3:
        if (!m_bTemporary && (!IS_THING_ALIVE(m_tcpHome) || !IS_THING_ALIVE(m_tcpTarget)
                || NCreatureMode::IsUnderground(*m_tcpTarget.Get()) || !NCreatureMode::IsLightningable(*m_tcpTarget.Get()))) {
            Deactivate(player);
        } else if (m_bTemporary) {
            if (m_fTime - m_fTimeBase >= m_fLife)
                Deactivate(player);
        } else if (IS_THING_ALIVE(player)) {
            if (SuccessfulHit) {
                SuccessfulHit = false;
                CTCElectrocutedEffect* effect;
                if (!m_tcpTarget->GetTC(TCI_ELECTROCUTED_EFFECT, &effect))
                    effect = (CTCElectrocutedEffect*)m_tcpTarget->AddTC("CTCElectrocutedEffect", true, 0);
                effect->Reset();
            }
            if (NCreatureMode::IsLightningable(*m_tcpTarget.Get())) {
                m_fDecapitationClock += 1.0f / (float)CMainGameComponent::GetConstantFPS();
                if (m_tcpTarget->HasTC(TCI_DECAPITATION) && m_tcpTarget->GetType() == 1
                        && m_fDecapitationClock >= m_fDecapitationTime && m_fDecapitationTime != -1.0f
                        && m_nFrameCount % m_OnhitEveryFrame == 0) {
                    C3DVector direction = m_tcpTarget->GetPos() - player->GetPos();
                    FABLE_RETAIL_ASSERT(direction.X == direction.X && direction.Y == direction.Y && direction.Z == direction.Z);
                    if (direction.IsNonZero())
                        direction.Normalise();
                    else
                        direction = C3DVector(1.0f, 0.0f, 0.0f);
                    CHitParameters hit(0, player.Get(), 0, m_tcpTarget->GetPos(), direction, false, false, false, false);
                    hit.SetDecapitate(true);
                    hit.AddDamageWithAttribute(DAMAGE_LIGHTNING, m_fDamagePerHit);
                    hit.SetMoveBackOnHit(false);
                    CThingGameObject* struck = static_cast<CThingGameObject*>(m_tcpTarget.Get());
                    hit.SetCreateDustParticleEffectOnHit(false);
                    struck->OnHit(hit);
                    SuccessfulHit = !hit.IsBlocked() && !hit.IsHitNegated();
                    if (IS_THING_ALIVE(m_tcpTarget)) {
                        CTCEntityEvents* events;
                        if (m_tcpTarget->GetTC(TCI_ENTITY_EVENTS, &events))
                            events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, HERO_ABILITY_LIGHTNING_SPELL));
                    }
                } else if ((m_tcpTarget->GetTypeFlags() & 4) && m_nFrameCount % m_OnhitEveryFrame == 0) {
                    C3DVector direction = m_tcpTarget->GetPos() - player->GetPos();
                    if (direction.IsNonZero())
                        direction.Normalise();
                    else
                        direction = C3DVector(1.0f, 0.0f, 0.0f);
                    CHitParameters hit(0, player.Get(), 0, m_tcpTarget->GetPos(), direction, false, false, false, false);
                    hit.AddDamageWithAttribute(DAMAGE_LIGHTNING, m_fDamagePerHit);
                    hit.SetMoveBackOnHit(false);
                    CThingGameObject* struck = static_cast<CThingGameObject*>(m_tcpTarget.Get());
                    hit.SetCreateDustParticleEffectOnHit(false);
                    struck->OnHit(hit);
                    SuccessfulHit = !hit.IsBlocked() && !hit.IsHitNegated();
                    if (IS_THING_ALIVE(m_tcpTarget)) {
                        CTCEntityEvents* events;
                        if (m_tcpTarget->GetTC(TCI_ENTITY_EVENTS, &events))
                            events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, HERO_ABILITY_LIGHTNING_SPELL));
                    }
                }
            }
        } else if (IS_THING_ALIVE(player)) {
            CTCEntityEvents* events;
            if (m_tcpTarget->GetTC(TCI_ENTITY_EVENTS, &events)) {
                CHitParameters hit;
                hit.SetThingHitting(*player.Get());
                events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, HERO_ABILITY_LIGHTNING_SPELL));
            }
        }
        break;
    case 4:
        if (AllChildrenInactive()) {
            m_fTimeBase = m_fTime;
            m_lsState = (ELightningState)5;
            if (IS_THING_ALIVE(m_tcpTarget)) {
                CTCSoundPlayer* sound;
                if (m_tcpTarget->GetTC(TCI_SOUND_PLAYER, &sound)) {
                    sound->PlayAttachedCriteria(PDef->EndAttackeeSound, true, false, false, -1);
                }
            } else if (IS_THING_ALIVE(player) && !m_bTemporary) {
                CTCSoundPlayer* sound;
                if (player->GetTC(TCI_SOUND_PLAYER, &sound)) {
                    sound->PlayUnattachedCriteria(PDef->EndAttackeeSound, m_vEndPos, false, false);
                }
            }
        }
        break;
    case 5:
        if (m_fTime - m_fTimeBase >= m_fDeActivationTime) {
            m_fTimeBase = m_fTime;
            m_lsState = (ELightningState)6;
        }
        break;
    }
    UpdatePositions();
    if (m_lsState != 0 && m_lsState != 6) {
        std::pair<unsigned long, C3DVector> start(CCRC::Calc(0, "LIGHTNING_START", 15), m_vStartPos);
        std::pair<unsigned long, C3DVector> end(CCRC::Calc(0, "LIGHTNING_END", 13), m_vEndPos);
        NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> params;
        params.VectorParams.Insert(start);
        params.VectorParams.Insert(end);
        params.Update();
        if (m_tpParticleHandle.Get() && !m_tpParticleHandle->IsDead()) {
            CTCDParticleEmitter* emitter = 0;
            m_tpParticleHandle->GetTC(TCI_PARTICLE_EMITTER, &emitter);
            emitter->SetEmitterUpdateParameters(params);
        }
        for (std::list<CCountedPointer<CLightningBeam> >::iterator it = m_lChildren.begin(); it != m_lChildren.end(); ) {
            CCountedPointer<CLightningBeam> child = *it;
            child->FrameUpdate(player, seed);
            if (child->m_lsState == 6)
                it = m_lChildren.erase(it);
            else
                ++it;
        }
    }
}
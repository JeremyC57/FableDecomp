// CTCWillLightning::CLightningBeam::FrameUpdate(CIntelligentPointer<CThingCreatureBase> player, unsigned long* seed) -- one frame of a will-lightning beam: count the frame and advance its clock, then by state: 1 waits m_fDelay (state 2, the home's will-lightning looping sound); 2 waits m_fActivationTime while home and target live (state 3, frame count -1, activate the children) or deactivates; 3 deactivates a dead / underground / unlightningable target (or a temporary beam after m_fLife), else re-arms the target's electrocuted effect after a successful hit and every m_OnhitEveryFrame frames hits a lightningable target (a decapitating hit once the decapitation clock passes m_fDecapitationTime on a decapitatable creature, otherwise a plain hit on a game object) with m_fDamagePerHit, records whether it landed and posts a hit-by-lightning event; 4 waits for every child (state 5, the def's end-attackee sound on the target or at the end point through the player); 5 waits m_fDeActivationTime (state 6). Then update the positions, feed LIGHTNING_START / LIGHTNING_END to the particle emitter, and frame-update the children, erasing the finished ones
// Grade: DIFFER(2712v2840) -- md=490: chunk 868 shared-header rebuild of the stand-in draft (engine/CThing.h retail chain + the new slot 17..65 virtual opt-ins so CThingGameObject::OnHit is slot 65 = +0x104, CTCWillLightning.h CLightningBeam, CHitParameters LIGHTNING, CTCEntityEvents AddEvent, CTCSoundPlayer PlayUnattachedCriteria, NCreatureMode queries, CIntelligentPointer / CCountedPointer / CDefPointer / STLport list and CVectorMap; local models only for CParticleUpdateParameters, CTCDParticleEmitter, CCRC, IS_THING_ALIVE); the time step written before the frame count (r3) gives retail's x87 head; residues: the register-held constants (`xor ebp,ebp` for the zero tests, `mov ebx,10h`), the frame (0xc4 v 0xbc), the GetTC presence tests, the Normalise x87 order, the child ~CCountedPointer out of line (0x007A3200)
// Next idea: find the 8 frame bytes (a CHitParameters / pair temporary) and what gives ebp = 0 and ebx = 0x10 register homes (constant records with >= 2 references), then iterate the per-state blocks
// Leaf: ?FrameUpdate@CLightningBeam@CTCWillLightning@@QAEXV?$CIntelligentPointer@VCThingCreatureBase@@@@PAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A1B20, 2840 bytes (+ the 5-entry state jump table 0x007A2624), thiscall, `ret 0Ch`, frame 0xbc.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?FrameUpdate@CLightningBeam@CTCWillLightning@@QAEXV?$CIntelligentPointer@VCThingCreatureBase@@@@PAK@Z
//   (the PDB 0x018273D2 / 0x026B0EF0 entries); the body recurses into itself for each child (0x007A25BC).
// Draft model (DIFFER oracle, not a landing candidate): no shared header carries CLightningBeam, CThing's slot-65 OnHit, the
//   9-argument CHitParameters ctor or CParticleUpdateParameters, so this draft keeps local models in the style of the landed siblings
//   (Deactivate 0x0079FE90, IsTargetValid 0x0079FD90, CTCElectrocuteOnTouch::OnHit 0x007E2600). Layout: Ego_r.pdb CLightningBeam with
//   the 4-byte STLport list (m_bTemporary +0x50 .. SuccessfulHit +0x78, sizeof 0x7c); the def's EndAttackeeSound is +0x11c (InitSound
//   +0x110 / LoopingSound +0x114 per the landed CTCWillLightning ctor / StartLoopingSound rows, Ego_r order).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHING_VIRTUALS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_VIRTUALS
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_VIRTUALS
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_LIGHTNING
#define FABLE_RETAIL_CTCWILLLIGHTNING_BEAM
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_UNATTACHED
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CVECTORMAP_INSERT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CHitParameters.h"
#include "engine/CTCWillLightning.h"
#include "engine/CSpecialAbilitiesLightningSpellDef.h"
#include "engine/NCreatureMode.h"
#include "engine/CTCElectrocutedEffect.h"
#include "engine/NEntityEvents_CEventHitBySpecialAbility.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CMainGameComponent.h"
#include "engine/CVectorMap.h"
template<class T> bool __fastcall IS_THING_ALIVE(const T& pointer);   // 0x004CC340 <CIntelligentPointer<CThing>> / 0x006961F0 <CIntelligentPointer<CThingCreatureBase>> (labels)
template<> std::list<CCountedPointer<CTCWillLightning::CLightningBeam> >::iterator std::list<CCountedPointer<CTCWillLightning::CLightningBeam> >::erase(iterator position);   // 0x007A38B0
template<> CCountedPointer<CTCWillLightning::CLightningBeam>::~CCountedPointer();   // 0x007A3200 (the child release, out of line in retail)
#include "engine/CBaseClass.h"
class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0 (the shared declaration lives inside CSpecialEffectsDef.h's GETEFFECT_BODY opt-in only)
namespace NParticleEngine {
    // no shared header: NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long> (labels 0x0076C750 Update /
    // 0x006E2500 dtor); VectorParams is the CVectorMap<unsigned long,C3DVector> the emitter reads (Insert = push_back + Dirty).
    template<class K, class P> class CParticleUpdateParameters : public CBaseClass {
    public:
        CParticleUpdateParameters() { }
        virtual ~CParticleUpdateParameters();                       // 0x006E2500
        void Update();                                              // 0x0076C750
        CVectorMap<K, C3DVector> VectorParams;                      // +0x04
        CVectorMap<K, P> UpdateParams;                              // +0x14
    };
}
class CTCDParticleEmitter { public:                                 // no shared header; SetEmitterUpdateParameters 0x006E11A0
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>& params);
};
enum { LIGHTNING_STATE_NONE = 0, LIGHTNING_STATE_DELAY = 1, LIGHTNING_STATE_ACTIVATING = 2, LIGHTNING_STATE_ACTIVE = 3,
       LIGHTNING_STATE_WAITING_FOR_CHILDREN = 4, LIGHTNING_STATE_DEACTIVATING = 5, LIGHTNING_STATE_DEAD = 6 };   // descriptive (ELightningState enumerators unproven)
enum { THING_TYPE_CREATURE = 1, THING_TYPE_FLAG_GAME_OBJECT = 4, DAMAGE_ATTRIBUTE_LIGHTNING = 2 };   // descriptive

void CTCWillLightning::CLightningBeam::FrameUpdate(CIntelligentPointer<CThingCreatureBase> player, unsigned long* seed)
{
    m_fTime += 1.0f / (float)CMainGameComponent::GetConstantFPS();
    ++m_nFrameCount;
    switch (m_lsState) {
    case LIGHTNING_STATE_DELAY:
        if (m_fTime - m_fTimeBase >= m_fDelay) {
            m_fTimeBase = m_fTime;
            m_lsState = LIGHTNING_STATE_ACTIVATING;
            if (IS_THING_ALIVE(m_tcpHome)) {
                CTCWillLightning* will;
                if (m_tcpHome->GetTC(TCI_WILL_LIGHTNING, &will))
                    will->StartLoopingSound();
            }
        }
        break;
    case LIGHTNING_STATE_ACTIVATING:
        if (m_bTemporary || (IS_THING_ALIVE(m_tcpHome) && IS_THING_ALIVE(m_tcpTarget))) {
            if (m_fTime - m_fTimeBase >= m_fActivationTime) {
                m_fTimeBase = m_fTime;
                m_lsState = LIGHTNING_STATE_ACTIVE;
                m_nFrameCount = -1;
                for (std::list<CCountedPointer<CLightningBeam> >::iterator it = m_lChildren.begin(); it != m_lChildren.end(); ++it) {
                    CCountedPointer<CLightningBeam> child = *it;
                    child->Activate();
                }
            }
        } else
            Deactivate(player);
        break;
    case LIGHTNING_STATE_ACTIVE:
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
                    effect = (CTCElectrocutedEffect*)m_tcpTarget->AddTC(CCharString("CTCElectrocutedEffect"), true, 0);
                effect->Reset();
            }
            if (NCreatureMode::IsLightningable(*m_tcpTarget.Get())) {
                m_fDecapitationClock += 1.0f / (float)CMainGameComponent::GetConstantFPS();
                if (m_tcpTarget->HasTC(TCI_DECAPITATION) && m_tcpTarget->GetType() == THING_TYPE_CREATURE
                        && m_fDecapitationClock >= m_fDecapitationTime && m_fDecapitationTime != -1.0f
                        && m_nFrameCount % m_OnhitEveryFrame == 0) {
                    CThing* target = m_tcpTarget.Get();
                    C3DVector direction = target->GetPos() - player->GetPos();
                    if (direction.IsNonZero()) direction.Normalise();
                    else direction = C3DVector(1.0f, 0.0f, 0.0f);
                    CHitParameters hit(0, player.Get(), 0, m_tcpTarget->GetPos(), direction, false, false, false, false);
                    hit.SetDecapitate(true);
                    hit.AddDamageWithAttribute((EDamageAttribute)DAMAGE_ATTRIBUTE_LIGHTNING, m_fDamagePerHit);
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
                } else if ((m_tcpTarget->GetTypeFlags() & THING_TYPE_FLAG_GAME_OBJECT) && m_nFrameCount % m_OnhitEveryFrame == 0) {
                    CThing* target = m_tcpTarget.Get();
                    C3DVector direction = target->GetPos() - player->GetPos();
                    if (direction.IsNonZero()) direction.Normalise();
                    else direction = C3DVector(1.0f, 0.0f, 0.0f);
                    CHitParameters hit(0, player.Get(), 0, m_tcpTarget->GetPos(), direction, false, false, false, false);
                    hit.AddDamageWithAttribute((EDamageAttribute)DAMAGE_ATTRIBUTE_LIGHTNING, m_fDamagePerHit);
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
    case LIGHTNING_STATE_WAITING_FOR_CHILDREN:
        if (AllBeamsInactive()) {
            m_fTimeBase = m_fTime;
            m_lsState = LIGHTNING_STATE_DEACTIVATING;
            if (IS_THING_ALIVE(m_tcpTarget)) {
                CTCSoundPlayer* sound;
                if (m_tcpTarget->GetTC(TCI_SOUND_PLAYER, &sound))
                    sound->PlayAttachedCriteria(PDef->EndAttackeeSound.GetString(), true, false, false, -1);
            } else if (IS_THING_ALIVE(player) && !m_bTemporary) {
                CTCSoundPlayer* sound;
                if (player->GetTC(TCI_SOUND_PLAYER, &sound))
                    sound->PlayUnattachedCriteria(PDef->EndAttackeeSound.GetString(), m_vEndPos, false, false);
            }
        }
        break;
    case LIGHTNING_STATE_DEACTIVATING:
        if (m_fTime - m_fTimeBase >= m_fDeActivationTime) {
            m_fTimeBase = m_fTime;
            m_lsState = LIGHTNING_STATE_DEAD;
        }
        break;
    }
    UpdatePositions();
    if (m_lsState != LIGHTNING_STATE_NONE && m_lsState != LIGHTNING_STATE_DEAD) {
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
            if (child->m_lsState == LIGHTNING_STATE_DEAD)
                it = m_lChildren.erase(it);
            else
                ++it;
        }
    }
}

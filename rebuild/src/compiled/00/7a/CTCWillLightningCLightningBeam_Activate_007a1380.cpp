// CTCWillLightning::CLightningBeam::Activate()
// Leaf: ?Activate@CLightningBeam@CTCWillLightning@@QAEXXZ
// Flags: (none)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_DIRECT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_UPDATEPARAMETERS
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_GETEMITTERPOSITION_PASS26
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT_FORCEINLINE
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_CTOR_NOINLINE
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_UPDATE_DECL_PASS26
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_VECTORKEY_PASS26
#define FABLE_RETAIL_CTCWILLLIGHTNING_CLASS
#define FABLE_RETAIL_CTCWILLLIGHTNING_BEAM_FRAMEUPDATE
#define FABLE_RETAIL_CTCWILLLIGHTNING_BEAM_ACTIVATE_PASS26
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_STLP_LIST
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCWillLightning.h"

extern const CCharString PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;
extern const CCharString SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;
extern const CCharString MAIN_FOCAL_POSITION_NAME;

void CTCWillLightning::CLightningBeam::Activate()
{
    if (m_lsState == INACTIVE || m_lsState == DEACTIVATING) {
        if (m_fDelay == 0.0f) {
            ChangeState(ACTIVATING);
            if (IS_THING_ALIVE(m_tcpHome)) {
                CTCWillLightning* pTCLightning;
                if (m_tcpHome->GetTC(&pTCLightning))
                    pTCLightning->StartLoopingSound();
            }
        }
        else {
            ChangeState(DELAYING);
        }

        if (IS_THING_ALIVE(m_tcpHome) && !IS_THING_ALIVE(m_tpParticleHandle)) {
            if (m_bMainBeam)
                m_tpParticleHandle = CIntelligentPointer<CThing>(
                    CTCSpecialAbilities::CreateParticleEffect(g_nLargeLightningEffect, m_tcpHome->GetPos()));
            else
                m_tpParticleHandle = CIntelligentPointer<CThing>(
                    CTCSpecialAbilities::CreateParticleEffect(g_nLightningEffect, m_tcpHome->GetPos()));

            if (IS_THING_ALIVE(m_tpParticleHandle)) {
                CTCDParticleEmitter* tcParticleEmitter = 0;
                m_tpParticleHandle->GetTC(&tcParticleEmitter);

                if (m_bMainBeam) {
                    if (m_bRightHand)
                        tcParticleEmitter->AttachToThing(*m_tcpHome, 10, PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME, 0, 0.0f);
                    else
                        tcParticleEmitter->AttachToThing(*m_tcpHome, 10, SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME, 0, 0.0f);
                }
                else {
                    tcParticleEmitter->AttachToThing(*m_tcpHome, 12, MAIN_FOCAL_POSITION_NAME, 0, 0.0f);
                }

                m_vStartPos = tcParticleEmitter->GetEmitterPosition();
                if (IS_THING_ALIVE(m_tcpTarget))
                    m_vEndPos = m_tcpTarget->GetPos();

                std::pair<unsigned long, C3DVector> vtStartPos(NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0, "LIGHTNING_START"), m_vStartPos);
                std::pair<unsigned long, C3DVector> vtEndPos(NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0, "LIGHTNING_END"), m_vStartPos);
                NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> upParams;
                upParams.VectorParams.Insert(vtStartPos);
                upParams.VectorParams.Insert(vtEndPos);
                upParams.Update();
                tcParticleEmitter->SetEmitterUpdateParameters(upParams);
            }
        }
    }
}
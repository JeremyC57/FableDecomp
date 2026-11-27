// 0079f970 CTCWillLightning::CLightningBeam::UpdatePositions
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CVECTORMAP_COMPONENT_LOWERBOUND_OWNER_PASS26
// Leaf: ?UpdatePositions@CLightningBeam@CTCWillLightning@@QAEXXZ
// Flags: /O2 /Oy
#define FABLE_RETAIL_LIGHTNING_BEAM_POSITIONS_PASS29
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCWILLLIGHTNING_CLASS
#define FABLE_RETAIL_CTCWILLLIGHTNING_BEAM_ACTIVATE_PASS26
#define FABLE_RETAIL_CTCWILLLIGHTNING_BEAM_FRAMEUPDATE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCWillLightning.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/ThingAlive.h"
extern const CCharString MAIN_FOCAL_POSITION_NAME;

void CTCWillLightning::CLightningBeam::UpdatePositions()
{
    C3DVector vAbsoluteStart(0, 0, 0);
    C3DVector vAbsoluteDestination(0, 0, 0);
    if (IS_THING_ALIVE(m_tpParticleHandle)) {
        CTCDParticleEmitter* tcParticleEmitter = 0;
        if (!m_tpParticleHandle->GetTC(&tcParticleEmitter)) {
            FABLE_RETAIL_ASSERT_ARM(false);
        }
        vAbsoluteStart = tcParticleEmitter->GetEmitterPosition();
    }
    if (IS_THING_ALIVE(m_tcpHome))
        m_vStartPos = vAbsoluteStart;
    switch (m_lsState) {
    case DELAYING:
        m_vEndPos = m_vStartPos;
        break;
    case ACTIVATING:
        if (!IsTemporary() && IS_THING_ALIVE(m_tcpHome) && IS_THING_ALIVE(m_tcpTarget)) {
            C3DVector focus = m_tcpTarget->GetPos();
            const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance = m_tcpTarget->GetTCGraphicAppearance();
            if (ptc_graphic_appearance && !ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 0, focus, 1.0f)) {
                FABLE_RETAIL_ASSERT_ARM(false);
            }
            m_vDestination = focus;
        }
        m_vEndPos = (m_vDestination - m_vStartPos) * (m_fTime - m_fTimeBase) / m_fActivationTime + m_vStartPos;
        break;
    case ACTIVE:
        if (!IsTemporary() && IS_THING_ALIVE(m_tcpHome) && IS_THING_ALIVE(m_tcpTarget)) {
            if (!IsTemporary() && IS_THING_ALIVE(m_tcpHome) && IS_THING_ALIVE(m_tcpTarget)) {
                C3DVector focus = m_tcpTarget->GetPos();
                const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance = m_tcpTarget->GetTCGraphicAppearance();
                if (ptc_graphic_appearance && !ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 0, focus, 1.0f)) {
                    FABLE_RETAIL_ASSERT_ARM(false);
                }
                m_vDestination = focus;
            }
        }
        m_vEndPos = m_vDestination;
        break;
    case DEACTIVATINGCHILDREN:
        if (!IsTemporary() && IS_THING_ALIVE(m_tcpTarget) && IS_THING_ALIVE(m_tcpHome)) {
            if (!IsTemporary() && IS_THING_ALIVE(m_tcpHome) && IS_THING_ALIVE(m_tcpTarget)) {
                C3DVector focus = m_tcpTarget->GetPos();
                const NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance = m_tcpTarget->GetTCGraphicAppearance();
                if (ptc_graphic_appearance && !ptc_graphic_appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 0, focus, 1.0f)) {
                    FABLE_RETAIL_ASSERT_ARM(false);
                }
                m_vDestination = focus;
            }
        }
        m_vEndPos = m_vDestination;
        break;
    case DEACTIVATING:
        m_vEndPos = (m_vDestination - m_vStartPos) * (m_fDeActivationTime - (m_fTime - m_fTimeBase)) / m_fDeActivationTime + m_vStartPos;
        break;
    }
}
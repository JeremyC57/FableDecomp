// CTCWillLightning::CLightningBeam::CLightningBeam(CThing* home, CThing* target) -- a new lightning beam: zero state, positions and timers, 0.2 s activation / deactivation, the home and target things held in intelligent pointers, right-handed, no decapitation (-1), no def
// Leaf: ??0CLightningBeam@CTCWillLightning@@QAE@PAVCThing@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A0630. Ego_r.pdb ??0CLightningBeam@CTCWillLightning@@QAE@PAVCThing@@0@Z (engine/CTCWillLightning.h, the
//   nested CLightningBeam with Ego_r member names at the retail offsets: the STLport list child list is one pointer, so the members after it
//   sit 8 bytes below Ego_r's).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 (x3 members + the two temporaries), SetPItem 0x00A01B90 / GetPItem 0x00A01B50 (the
//   CThing* conversions and assignments), ~CBaseIntelligentPointer 0x00A01C10, malloc (the list's header node).
// Shape: members default-constructed in declaration order, then the body: the intelligent pointers assigned from temporaries built
//   from the raw things (target first, then home), PDef reset.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFPOINTER_RESET
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCWILLLIGHTNING_CLASS
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CSpecialAbilitiesLightningSpellDef.h"
#include "engine/CTCWillLightning.h"

CTCWillLightning::CLightningBeam::CLightningBeam(CThing* home, CThing* target)
{
    m_lsState = (ELightningState)0;
    m_fDamagePerHit = 0;
    m_fTime = 0;
    m_fTimeBase = 0;
    m_vStartPos.X = 0;
    m_vEndPos.X = 0;
    m_vDestination.X = 0;
    m_vStartPos.Y = 0;
    m_vEndPos.Y = 0;
    m_vDestination.Y = 0;
    m_vStartPos.Z = 0;
    m_vEndPos.Z = 0;
    m_vDestination.Z = 0;
    m_fActivationTime = 0.2f;
    m_fDeActivationTime = 0.2f;
    m_fLife = 0;
    m_tcpTarget = target;
    m_tcpHome = home;
    m_bTemporary = false;
    m_bMainBeam = false;
    m_bRightHand = true;
    m_nFrameCount = 0;
    m_fDecapitationTime = -1;
    m_fDecapitationClock = 0;
    m_fDelay = 0;
    PDef.Reset(0);
    m_OnhitEveryFrame = 0;
    SuccessfulHit = false;
}

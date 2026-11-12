// NCreatureMode::CModeDragonFireBeam::UpdateBeam() -- while the dragon's fire beam is active and more than the def's
//   FramesBetweenFireBeamUpdate frames have passed since the last explosion: take the mouth dummy of the owner's graphic appearance,
//   cast a ray 1000 units along the negated Up axis through the camera manager's landscape tools, and on a hit spawn a
//   "DRAGON_FIREBEAM_EXPLOSION" at the hit point (owner immune, LastExplodeFrame stamped) and an invisible
//   "DRAGON_FIREBEAM_EXPLOSION_INVISIBLE" half way between mouth and hit (owner immune). The CJackDragonDef sub-def is released at exit.
// Leaf: ?UpdateBeam@CModeDragonFireBeam@NCreatureMode@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DB810, 630 bytes, plain `ret` (private, void, no arguments). Name from Ego_r.pdb and
//   FableWin.pdb. Caller: CModeDragonFireBeam::FrameUpdate 0x008E07BB.
// Callees: CDefClassBase::GetSubDef<CJackDragonDef> 0x008E3300 (out of line, real body from engine/CDefClassBase_GetSubDef.h);
//   CWorld::GetFrame 0x0049D870 (twice); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
//   NGraphicAppearance::CTCGraphicAppearance::GetDummyObject 0x004C08C0; CTCCreatureModeManager::CModeBase::GetWorld 0x0073A680;
//   CAIGameCameraBase::GetCameraManager (const, vtable slot 10 +0x28); NCameraModes::CTools::GetLandscapePointIntersectionFromTo
//   0x00875550; CGameDefinitionManager::Get 0x0044C6B0; CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410;
//   CTCDExplosion::Create 0x0080C460; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inside the inline GetTC,
//   TCI_EXPLOSION 0x41); CTCDExplosion::SetThingAsImmune 0x0080CBF0; the def's slot-1 Delete on the last release.
// Shape: the beam end is built in place (`end = up * -1 * 1000 ; end += mouth`): retail spills end.X into end's own slot (+0x20)
//   before adding the mouth. Each def index is its own statement (its CCharString temp dies before Create), and the middle point is a
//   named local computed before the second name. No frame pointer, no EH state; the release drops the null test after the deref.
// Layout: RTTI (ghidra_out/rtti_bases.tsv .?AVCModeDragonFireBeam@NCreatureMode@@) gives CModeDragonFireBeam : CModeDragonHover :
//   CModeStandBase : CTCCreatureModeManager::CModeBase, as the landed caller 0x008E0780 models it. CModeStandBase (0x68) comes from
//   the shared engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE); CModeDragonHover has no shared header, so it is declared here
//   with its Ego_r members +0x68..+0x80 kept opaque (Filter +0x68, PTarget +0x74, UseHeadTurning +0x7C; this row reads none of them).
//   CModeDragonFireBeam's own members (Ego_r): Timer +0x80, PBeamWeaponEffect +0x84, LastExplodeFrame +0x8C, BeamActive +0x90.
//   Overlay opt-ins: FABLE_RETAIL_CJACKDRAGONDEF, FABLE_RETAIL_NCAMERAMODES_CTOOLS_LANDSCAPEFROMTO.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CJACKDRAGONDEF
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_RIGHT_COMPONENTS
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_CAMTOOLS_TYPED
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_LANDSCAPEFROMTO
#define FABLE_RETAIL_CMODESTANDBASE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CJackDragonDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/NCameraModes_CTools.h"
#include "engine/CTCDExplosion.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"

namespace NCreatureMode {
class CModeDragonHover : public CModeStandBase { public:   // retail sizeof 0x80 (no shared header)
    unsigned char _hover[0x80 - sizeof(CModeStandBase)];     // Filter +0x68, PTarget +0x74, UseHeadTurning +0x7c
};
class CModeDragonFireBeam : public CModeDragonHover { public:
    long          Timer;                   // +0x80
    CIntelligentPointer<CThing> PBeamWeaponEffect;    // +0x84
    long          LastExplodeFrame;        // +0x8c
    bool          BeamActive;              // +0x90
private:
    void UpdateBeam();
};
}

void NCreatureMode::CModeDragonFireBeam::UpdateBeam()
{
    if (!BeamActive)
        return;
    CDefPointer<const CJackDragonDef> def;
    ParentThing.PeekBaseDef().GetSubDef(&def);
    if (CWorld::GetFrame() - LastExplodeFrame > def->FramesBetweenFireBeamUpdate) {
        C3DVector mouth;
        CRightHandedSet orientation;
        ParentThing.GetTCGraphicAppearance()->GetDummyObject("DRAGON_MOUTH_DUMMY", 0, 0, mouth, orientation, 1.0f);
        C3DVector end = orientation.GetUp() * -1.0f * 1000.0f;
        end += mouth;
        const CGameCameraManager* manager;
        C3DVector hitPos;
        C3DVector hitNormal;
        if (GetWorld().PeekGameCamera()->GetCameraManager(&manager)
            && manager->GetCameraTools().GetLandscapePointIntersectionFromTo(mouth, end, hitPos, hitNormal, true)) {
            long index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("DRAGON_FIREBEAM_EXPLOSION");
            CThing* explosion = CTCDExplosion::Create(index, hitPos);
            if (explosion) {
                CTCDExplosion* tc;
                if (explosion->GetTC(TCI_EXPLOSION, &tc))
                    tc->SetThingAsImmune(ParentThing);
                LastExplodeFrame = CWorld::GetFrame();
            }
            C3DVector middle = (hitPos + mouth) * 0.5f;
            index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("DRAGON_FIREBEAM_EXPLOSION_INVISIBLE");
            CThing* invisible = CTCDExplosion::Create(index, middle);
            if (invisible) {
                CTCDExplosion* tc;
                if (invisible->GetTC(TCI_EXPLOSION, &tc))
                    tc->SetThingAsImmune(ParentThing);
            }
        }
    }
}
// CAIStateGroup_BirdGroundWander::IsRunnable() -- runnable when the creature's mode component is in the ground-wander mode and the bird never walked or finished walking more than two seconds ago
// Leaf: ?IsRunnable@CAIStateGroup_BirdGroundWander@@UAE_NXZ
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009143E0, length 126 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 7 =
// bitset word 0 bit 7 -> `mov cl,[eax+20h] ; test cl,cl ; jns`), CTCCreatureNavigation::GetNavigatorType() const @ 0x006CB6A0 (Ego_r.pdb / FableWin.pdb ?GetNavigatorType@CTCCreatureNavigation@@QBEKXZ)
// (`movzx` of [+0xc]->+0x10; the manifest's CIsHitEventBlocked::operator() label is propagated),
// CAIStateGroupBase::PeekWorld @ 0x008FCFA0 (result unused: object expression of a static call),
// CWorld::GetFrame @ 0x0049D870. Globals: the compared mode @ 0x0129CBA4, frames per second @ 0x01375550.
// Shape: the MazeTeleport (0x009543B0) form -- the lookup's bool is not tested, so the miss path reads the
// uninitialised out slot (`mov eax,[esp+8]`); `FrameLastFinishedWalking == 0 ||` short-circuits to the shared
// `mov al,1`; the frame rate is read into a local (esi) before the frame call and doubled with `lea`, the elapsed
// Identity: Ego_r.pdb / FableWin.pdb ?IsRunnable@CAIStateGroup_BirdGroundWander@@UAE_NXZ, vtable 0x01289670 slot 11 = 0x009143E0.
// Model: engine/CAIStateGroup_BirdGroundWander.h retail branch (new opt-in FABLE_RETAIL_CAISTATEGROUP_BIRDGROUNDWANDER).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGATORTYPE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUP_BIRDGROUNDWANDER
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorldMap.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CAIStateGroup_BirdGroundWander.h"
extern long g_groundWanderMode;                 // 0x0129CBA4

bool CAIStateGroup_BirdGroundWander::IsRunnable()
{
    CTCCreatureNavigation* navigation;
    GetCreature().GetTC(TCI_NAVIGATION, &navigation);
    if (navigation->GetNavigatorType() == g_groundWanderMode) {
        if (FrameLastFinishedWalking == 0)
            return true;
        if (PeekWorld().GetFrame() - FrameLastFinishedWalking > (unsigned long)(CMainGameComponent::GetConstantFPS() * 2))
            return true;
    }
    return false;
}

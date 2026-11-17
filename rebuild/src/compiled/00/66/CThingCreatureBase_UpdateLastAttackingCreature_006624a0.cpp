// CThingCreatureBase::UpdateLastAttackingCreature() -- forget the last attacker once 300 seconds' worth of frames have passed since it attacked
// Leaf: ?UpdateLastAttackingCreature@CThingCreatureBase@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006624A0, 66 bytes (+ int3 padding).
// Identity: Ego_r.pdb 0x0053D330 + FableWin.pdb 0x01D8C170 ?UpdateLastAttackingCreature@CThingCreatureBase@@IAEXXZ (protected).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (on PLastAttackedByCreature), CThing::PeekWorld @ 0x004C7AB0 (protected,
//          the landed callee's catalog name), static CWorld::GetFrame @ 0x0049D870, CThingCreatureBase::ClearAttackedByCreature
//          @ 0x006691C0 (tail jump; Ego_r.pdb / FableWin.pdb ?ClearAttackedByCreature@CThingCreatureBase@@QAEXXZ). Global: the
//          protected static CMainGameComponent::ConstantFPS (0x01375550) through the inline GetConstantFPS().
// Layout: engine/CThingCreatureBase.h (retail branch): PLastAttackedByCreature (CIntelligentPointer<CThingCreatureBase>) +0xf4,
//   WFLastAttackedByCreature (unsigned long) +0xfc.
// Shape: the stored frame is loaded AFTER the PeekWorld call but BEFORE GetFrame (a named local after the named world reference), and
//   the frame rate after GetFrame (the difference is a named local); the difference is unsigned
//   (`jbe`) because WFLastAttackedByCreature is unsigned long.
// Name fix (agentNF55, 2026-11-17): moved onto the shared headers; the local CThingCreatureBase / CWorld models (with the made-up
//   ClearLastAttackedByCreature, a public GetWorld at 0x004C7AB0 and the made-up g_GameFramesPerSecond) are gone; the leaf is the PDB's
//   protected IAE spelling (was QAE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ATTACKEDBY
#define FABLE_RETAIL_CTHINGCREATUREBASE_UPDATELASTATTACKINGCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"

void CThingCreatureBase::UpdateLastAttackingCreature()
{
    if (PLastAttackedByCreature.GetPItem()) {
        const CWorld& world = PeekWorld();
        unsigned long attacked_frame = WFLastAttackedByCreature;
        unsigned long frames_since = world.GetFrame() - attacked_frame;
        if (frames_since > (unsigned long)(CMainGameComponent::GetConstantFPS() * 300))
            ClearAttackedByCreature();
    }
}

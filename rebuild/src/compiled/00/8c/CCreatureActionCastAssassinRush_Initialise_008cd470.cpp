// CCreatureAction_CastAssassinRush::Initialise() -- install rush component, target and configure it, then pass optional final action.
// Leaf: ?Initialise@CCreatureAction_CastAssassinRush@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CD470, 298 bytes; vftable 0x01284254 slot 12 (Ego_r.pdb 0x009047C0 + FableWin.pdb 0x029F9410
//   ?Initialise@CCreatureAction_CastAssassinRush@@UAEXXZ).
// Callees: CCombatActionBase::Initialise 0x00857520, CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::RemoveTC 0x004C9840,
//   CThing::AddTC 0x004C9D60, CTCAssassinRush::SetPTarget 0x007A9AB0, CTCPhysicsBase slot 74 SetFacingAnglesFromRHSet (+0x128),
//   CTCAssassinRush::SetFinalAction 0x008CEAE0 (?SetFinalAction@CTCAssassinRush@@QAEXV?$CCountedPointer@VCCreatureActionBase@@@@@Z, both PDBs).
// NF266 port (q12743, 2026-11-22): the HEAD row modelled CThing, CCombatActionBase, CCreatureAction_CastAssassinRush, CRightHandedSet and a
//   CTCPhysicsBase slot wrapper locally behind renaming-macro includes; now on the shared headers (CThingCreatureBase.h chain,
//   CCreatureActionBase.h FULL_MEMBERS, CCombatActionBase.h MELEE + INITIALISE, CCreatureAction_CastAssassinRush.h retail branch with the
//   NF266 INITIALISE opt-in, CTCPhysicsBase.h, CRightHandedSet.h CTOR_INLINE). CTCAssassinRush has no shared header (none under
//   rebuild/include): its HEAD local model stays in place, with FinalAction / SetFinalAction respelled from the local CCountedAction
//   stand-in onto the shared CCountedPointer<CCreatureActionBase> (Ego_r.pdb FinalAction +0x90; 0x008CEAE0's landed leaf).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_REGIONLOAD_TCS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH_INITIALISE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCreatureAction_CastAssassinRush.h"
// CTCAssassinRush generated header absent. PDB native member offsets are donor minus4 (TC base).
class CTCAssassinRush { public:
 unsigned char _beforeDashDistance[0x70];float DashDistance;
 unsigned char _beforeSpellLevel[0x18];long SpellLevel;CCountedPointer<CCreatureActionBase> FinalAction;void* RadialBlurFadeKey;bool UseCollision;bool IsZTargetting;
 void SetPTarget(CThing*,bool); //007A9AB0 ?SetPTarget@CTCAssassinRush@@QAEXPAVCThing@@_N@Z (Ego_r.pdb)
 void SetFinalAction(CCountedPointer<CCreatureActionBase>); //008CEAE0; PDB906010; propagated CMeshDataBank label rejected
};
FABLE_STATIC_ASSERT(offsetof(CTCAssassinRush,FinalAction)==0x90);
FABLE_STATIC_ASSERT(offsetof(CTCAssassinRush,UseCollision)==0x9c);
void CCreatureAction_CastAssassinRush::Initialise(){
 CCombatActionBase::Initialise();PCreature->RemoveTC((ETCInterfaceType)(0xa9));
 CTCAssassinRush* rush;
 {CCharString name("CTCAssassinRush",-1);rush= (CTCAssassinRush*)PCreature->AddTC(name,true,0);}
 CThing* target=PTarget.Get();rush->SetPTarget(target,IsZTargetting);
 if(Override){
  rush->DashDistance=Distance;rush->UseCollision=UseCollision;
  CTCPhysicsBase& physics=*PCreature->GetPhysicsTC();
  CRightHandedSet orientation(C3DVector(0.0f,0.0f,1.0f),Dir);physics.SetFacingAnglesFromRHSet(orientation);
 }
 if(PCreature->GetType()==1)rush->SpellLevel=Level;
 if(FinalAction.Data)rush->SetFinalAction(FinalAction);
}

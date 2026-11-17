// CTCTargetingBase::~CTCTargetingBase() -- the implicit destructor: release the targeting def (PTargetingDef +0x30), destroy the four thing pointers (+0x25, +0x1D, +0x15, +0x0D) last first, then ~CTCBase (tail jump)
// Leaf: ??1CTCTargetingBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA8B7, length 64 bytes; thiscall, size-optimised TU.
// Identity (agentNF69, q10855 rejected): the class is CTCTargetingBase, not CTCTargeted. (1) The body destroys exactly Ego_r.pdb's
//   CTCTargetingBase members (struct_layouts_egor.tsv: CIntelligentPointer<CThing> PNearestInterestingThing 13, PScriptOverrideInterestingThing
//   21, PScriptOverrideRangedTarget 29, PTargetedThing 37, CDefPointer<const CTargetingDef> PTargetingDef 48); Ego_r's CTCTargeted is 24
//   bytes (TargetingFlags 12, flags 16..18, PTargetingDef 20) with no intelligent pointer. (2) Its callers are the destructors of
//   CTCTargetingBase's RTTI children (rtti_bases.tsv: CTCTargetingPlayer / CTCTargetingSpirit / CTCTargetingAI : CTCTargetingBase : CTCBase;
//   CTCTargeted : CTCBase is outside that chain): ~CTCTargetingPlayer 0x004E37BF, the CTCTargetingSpirit ??_G 0x004E1023, 0x004E0080 and the
//   CTCTargetingBase ??_G 0x007F5D00. (3) ??1CTCTargeted@@UAE@XZ already belongs to the landed row 0x004DBF56 (its vtable's ??_G 0x004DBF3A).
//   Ego_r.pdb / FableWin.pdb both carry ??1CTCTargetingBase@@UAE@XZ. The catalog's CTCTargeted::~CTCTargeted label is wrong.
// Layout: engine/CTCTargetingBase.h (the typed FABLE_RETAIL_UPDATE_NEAREST_INTERESTING branch, packed CIntelligentPointer<CThing> members)
//   over engine/CTCBase.h, engine/CDefPointer.h (forced-inline release: `and dword ptr [..],0` in the non-null arm) and
//   engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER: the members have no destructor of their own, so
//   ~CBaseIntelligentPointer 0x00A01C10 is called at each member address). Was a local CTCBase / CDefPointer / __declspec(novtable)
//   CTCTargetingBase model.
// Shape: no vtable store, VC7.1's implicit destructor (hdr/ overlay opt-in FABLE_RETAIL_CTCTARGETINGBASE_IMPLICIT_DTOR leaves out the
//   PDB-generated user declaration); DestroyOne (a destruction by name, ungraded) makes VC7.1 emit the out-of-line implicit ??1.
#pragma optimize("s", on)
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCTARGETINGBASE_IMPLICIT_DTOR
class C3DVector;
#include "engine/ETCInterfaceType.h"
#include "engine/CTCTargetingBase.h"

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1 (as the landed ~CTCTargeted 0x004DBF56).
void DestroyOne(CTCTargetingBase* object)
{
    object->CTCTargetingBase::~CTCTargetingBase();
}

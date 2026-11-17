// NThingShot::CTCShotDefinitionBase::IsAbleToBeDeflected() const -- the shot's CanBeDeflected flag (+0x68)
// Leaf: ?IsAbleToBeDeflected@CTCShotDefinitionBase@NThingShot@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BED60: `mov al,[ecx+68h] ; ret`.
// NF72 (round 7, q10867): was the generic stand-in C_007bed60::M (catalog _global::GetMember_byte0x68_007bed60). Retail 0x007BED30..0x007BED83
//   is the six-function accessor run SetKnockDown / SetDecapitate / SetCanBeDeflected (+0x66 / +0x67 / +0x68) then the getters 0x007BED60
//   (+0x68), 0x007BED70 (+0x66, IsKnockDown), 0x007BED80 (+0x67, IsDecapitate); Ego_r.pdb has the same run in the same order at
//   0x007BDC00..0x007BDC50 (IsAbleToBeDeflected at 0x007BDC30, a constant +0x1130 shift), FableWin.pdb carries the same spelling
//   (0x026FBA20), and engine/NThingShot_CTCShotDefinitionBase.h puts CanBeDeflected at +0x68. Its only caller, the ported
//   CHitParameters::SetAsModifiedByArmour 0x00869020, binds this symbol through the header opt-in ISABLETOBEDEFLECTED (NF67).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ISABLETOBEDEFLECTED
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"

bool NThingShot::CTCShotDefinitionBase::IsAbleToBeDeflected() const
{
    return CanBeDeflected;
}

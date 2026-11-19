// CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* pdef) -- loads the card-game sub-def of the parent thing's def into a local def pointer and assigns it to *pdef
// Leaf: ?GetCardDef@CTCTavernGameCardBase@@QAEXPAV?$CDefPointer@$$CBVCTavernGameCardBaseDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F0B20, length 92 bytes (ret 4). Name and signature: Ego_r.pdb + FableWin.pdb
// `?GetCardDef@CTCTavernGameCardBase@@QAEXPAV?$CDefPointer@$$CBVCTavernGameCardBaseDef@@@@@Z`.
// Callee: CDefClassBase::GetSubDef<CTavernGameCardBaseDef>(CDefPointer<const CTavernGameCardBaseDef>*) @ 0x008F27A0 (bool,
// result unused), called on the def object the parent thing holds at +0x70; the def's virtual slot 1 destroys it.
// Shape: the local def pointer is null-constructed in the `push ecx` frame slot after its address is pushed; then the
// inline CDefPointer::operator= and the local's inline destructor (null test, `dec [esi+4] ; jne`, slot-1 destroy).
// Name fix (NF96 part 2, q11339 self row): the old row defined ?GetCardDef@CTCTavernGameCardBase@@QAEXPAV?$CDefPointer@VCTavernGameCardBaseDef@@@@@Z
//   (non-const CDefPointer) as a stand-in body over local CDefPointee / CDefPointer / CDefClassBase / CThing / CTCTavernGameCardBase
//   models and a made-up callee FindDef_CDefClassBase_GetSubDef_0. It now sits on the shared headers with the define / include set of
//   its landed TU-mate GetPackPosition 0x008F0B80 (engine/card_sorting_types.h, the Ego_r.pdb CTCTavernGameCardBase;
//   engine/CDefClassBase_GetSubDef.h with GETSUBDEF_NOINLINE, so GetSubDef stays the out-of-line call to 0x008F27A0). This body is
//   the one the TU-mates 0x008F0B80 / 0x008F0BD0 / 0x008F0EB0 / 0x008F10B0 carry as same-TU strong copies (local new_pdef and the
//   GetParentThing() accessor as the landed 0x008F0BD0 copy, whose Ego_d.pdb scopes name pdef / new_pdef: egod_e10 NOTES).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCTAVERNGAME_HIDEMESH
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDPACK
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_TYPENAME
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCARD_POSITIONS
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET
#include "stlport/fable_containers.h"
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCardDef.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
#include "engine/CDefClassBase_GetSubDef.h"

void CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* pdef)
{
    CDefPointer<const CTavernGameCardBaseDef> new_pdef;
    GetParentThing().PeekBaseDef().GetSubDef(&new_pdef);
    *pdef = new_pdef;
}

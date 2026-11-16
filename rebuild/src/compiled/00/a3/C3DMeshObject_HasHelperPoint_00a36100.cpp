// C3DMeshObject::HasHelperPoint(const CCharString& name) const -- whether the mesh statistics hold a helper point under the name's CRC
// Leaf: ?HasHelperPoint@C3DMeshObject@@QBE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A36100, 64 bytes, `ret 4`.
// Identity: Ego_r.pdb ?HasHelperPoint@C3DMeshObject@@QBE_NABVCCharString@@@Z at 0x00B8CEA0, directly before
//   ?GetNoHelperPoints@C3DMeshObject at 0x00B8CEC0 (retail 0x00A36100 then the landed GetNoHelperPoints 0x00A36140); called by the landed
//   CTCGraphicAppearance::GetHelperPoint 0x004C0560 as `object.HasHelperPoint(name)`.
// Callees: CCRC::Calc 0x004014A0 (the inline CCharString::GetCRC), C3DMeshStats::HasHelperPoint(unsigned long) const 0x00A2EA20.
// Shape: the string accessors (GetCRC inline: length test, then the empty-literal select) before `push esi`, the statistics object
//   (the counted pointer's Data, +8) in esi.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_C3DMESHSTATS_HASHELPERPOINT
#include "fable_string.h"
#include "engine/C3DMeshStats.h"
#include "engine/C3DMeshObject.h"

bool C3DMeshObject::HasHelperPoint(const CCharString& name) const
{
    return Mesh->HasHelperPoint(name.GetCRC());
}

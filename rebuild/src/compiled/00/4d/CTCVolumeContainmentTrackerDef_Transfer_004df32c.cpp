// CTCVolumeContainmentTrackerDef::Transfer(CPersistContext& context) -- persist the volume-containment-tracker def's four members in declaration order
// Leaf: ?Transfer@CTCVolumeContainmentTrackerDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DF32C, 57 bytes, thiscall `ret 4`, in the padding-free size-optimised def TU (the
//   previous function CCameraCollisionDef::Transfer 0x004DF31C ends with `ret 4` at 0x004DF329, the next function 0x004DF365
//   starts right after this row's `ret 4`): the even-but-unaligned start is a real function start.
// Identity (proven, two sources): (1) the only reference is slot 18 (the def Transfer slot) of vtable 0x0123E0C4, whose RTTI type
//   descriptor is .?AVCTCVolumeContainmentTrackerDef@@ (: CSubDefClassBase : CDefClassBase : CDefPointeeBase : CResource :
//   CIVCountedPointeeBase); (2) Ego_r.pdb and FableWin.pdb both list ?Transfer@CTCVolumeContainmentTrackerDef@@UAEXAAVCPersistContext@@@Z,
//   and the four transferred addresses hit exactly the Ego_r members of the matching types: UpdateEveryFrame bool +0x25 and
//   CalculateBlendedLightingChannel bool +0x26 (bool transfer 0x0043314A), LightingChannelSphereModelRadius float +0x28 and
//   LightingChannelSphereModelZOffset float +0x2C (float 0x00431061); sizeof 0x30. The byte twin 0x004DE253 is
//   CExplodingObjectDef::Transfer (bool/float/long/long members).
// Callees: CPersistContext::Transfer<T>(T&) instantiations (the naming wave 19 spelling): ??$Transfer@_N@CPersistContext@@QAEXAA_N@Z
//   0x0043314A, ??$Transfer@M@CPersistContext@@QAEXAAM@Z 0x00431061.
// Shape: `context.Transfer(Member);` per member (the landed CChestDef / CLookDef def-TU form): context in edi, this in esi, the
//   last address formed in place (`add esi,2Ch`).
// Layout: engine/CTCVolumeContainmentTrackerDef.h under FABLE_RETAIL_TC_VOLUME_CONTAINMENT_TRACKER_DEF (chunk 706 retail opt-in, hdr/ overlay): the shared def chain
//   engine/CSubDefClassBase.h (FABLE_RETAIL_DEF_CLASS_BASE, 0x25 packed bytes), then the Ego_r members at their PDB offsets.
#pragma optimize("s", on)
#define FABLE_RETAIL_TC_VOLUME_CONTAINMENT_TRACKER_DEF
#include "engine/CTCVolumeContainmentTrackerDef.h"

class CPersistContext {
public:
    template <class T> void Transfer(T& value);
};

void CTCVolumeContainmentTrackerDef::Transfer(CPersistContext& context)
{
    context.Transfer(UpdateEveryFrame);
    context.Transfer(CalculateBlendedLightingChannel);
    context.Transfer(LightingChannelSphereModelRadius);
    context.Transfer(LightingChannelSphereModelZOffset);
}
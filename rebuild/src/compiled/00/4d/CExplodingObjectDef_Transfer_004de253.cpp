// CExplodingObjectDef::Transfer(CPersistContext& context) -- persist the exploding-object def's four members in declaration order
// Leaf: ?Transfer@CExplodingObjectDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE253, 57 bytes, thiscall `ret 4`, in the padding-free size-optimised def TU (the
//   previous function CChestDef::Transfer 0x004DE204 ends with `ret 4` at 0x004DE250, the next CLookDef::Transfer starts at
//   0x004DE28C right after this row's `ret 4`): the odd start is a real function start.
// Identity (proven, two sources): (1) the only reference is slot 18 (the def Transfer slot) of vtable 0x0123AD7C, whose RTTI type
//   descriptor is .?AVCExplodingObjectDef@@ (: CSubDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase);
//   (2) Ego_r.pdb and FableWin.pdb both list ?Transfer@CExplodingObjectDef@@UAEXAAVCPersistContext@@@Z, and the four transferred
//   addresses hit exactly the Ego_r members of the matching types: TriggeredOnCreatureProximity bool +0x25 (bool transfer
//   0x0043314A), TriggerRadius float +0x28 (float 0x00431061), ExplosionDef long +0x2C, ProximityParticleEffect long +0x30 (long
//   0x00431102); sizeof 0x34. The byte twin 0x004DF32C is CTCVolumeContainmentTrackerDef::Transfer (other member types).
// Callees: CPersistContext::Transfer<T>(T&) instantiations (the naming wave 19 spelling): ??$Transfer@_N@CPersistContext@@QAEXAA_N@Z
//   0x0043314A, ??$Transfer@M@CPersistContext@@QAEXAAM@Z 0x00431061, ??$Transfer@J@CPersistContext@@QAEXAAJ@Z 0x00431102.
// Shape: `context.Transfer(Member);` per member (the landed CChestDef / CLookDef def-TU form): context in edi, this in esi, the
//   last address formed in place (`add esi,30h`).
// Layout: engine/CExplodingObjectDef.h under FABLE_RETAIL_EXPLODING_OBJECT_DEF (chunk 706 retail opt-in, hdr/ overlay): the shared def chain
//   engine/CSubDefClassBase.h (FABLE_RETAIL_DEF_CLASS_BASE, 0x25 packed bytes), then the Ego_r members at their PDB offsets.
#pragma optimize("s", on)
#define FABLE_RETAIL_EXPLODING_OBJECT_DEF
#include "engine/CExplodingObjectDef.h"

class CPersistContext {
public:
    template <class T> void Transfer(T& value);
};

void CExplodingObjectDef::Transfer(CPersistContext& context)
{
    context.Transfer(TriggeredOnCreatureProximity);
    context.Transfer(TriggerRadius);
    context.Transfer(ExplosionDef);
    context.Transfer(ProximityParticleEffect);
}
# Chunk 706 (NA049) -- 10 of 10 byte-exact (RELOCATION_MATCH), five opt-in retail headers + two new headers (2026-11-02)

Rows (`verify.log`: WINS 10, RELOCATION_MATCH + PASS; no asm, volatile; 0x004DE253 under `#pragma optimize("s", on)`):
CVtable_012bf0c8::ReleaseBlock 0x00C77470 (descriptive: libpk, no PDB; the "CLandscapeBackgroundPatch::ClearTextures" label was wrong, the real
ClearTextures is 0x00BE5C30), CLandscapeBackgroundPatch::GetEdgeStrip 0x00BE5C60 (switch; table entries checked), CExplodingObjectDef::Transfer
0x004DE253 and CTCVolumeContainmentTrackerDef::Transfer 0x004DF32C (vtable slot 18 each), CAnimComponentCombatFlourishTargetOffset::Alloc
0x005DF790, CTCPhysicsRigidBody::UpdateInterpolation 0x00759970 (slot 94; "vfunc_94"), CTCHeroReceiveItems::OnKill 0x007CA630,
CTCTargetingPlayer::GetPTargetedThing 0x00802610, the private CCreatureAction_SpiritAttack::FinishAction 0x008547D0 and
CCreatureAction_GoUnderground::FinishAction 0x008B6A20 (byte twin of chunk 707's RiseUp 0x008B6A90; two symbols). Every name except
0x00C77470 in both PDBs.

Coordinator round: the three local models that dropped trailing members (CTCPhysicsRigidBody, CTCTargetingPlayer, CTCHeroReceiveItems) were
completed to the retail sizeof (real types, opaque storage only for CRigidBody / CEnginePrimitiveHandle) and regraded before landing.

Headers (hdr/README.md): opt-in branches in engine/CExplodingObjectDef.h, CTCVolumeContainmentTrackerDef.h, CLandscapeBackgroundSubNodeBase.h,
CLandscapeBackgroundPatch.h and a guarded CPlayer::IsFirstPersonTargeting declaration; new engine/CPatchTesselationEdgeStrip.h and
CEdgeStipVertexArray.h (Ego_r spellings). The agent's own CMovableResource branch (FABLE_RETAIL_MOVABLE_RESOURCE, dtor only) duplicated
chunk 708's fuller FABLE_RETAIL_CMOVABLERESOURCE: one class, one retail model, so CLandscapeBackgroundSubNodeBase.h now selects 708's branch and
706's CMovableResource.h was not copied; the ten rows regraded 10/10 against the merged headers (merged_header_regrade.log). Default text of
every changed header unchanged under `cpp` (static-assert line names aside); 144 landed includers rechecked: 15 MATCH + 129 RELOCATION_MATCH,
all PASS (includer_recheck_grades.tsv).

Second spellings at landing, queued PRIORITY (each binder needs a row rename or class-model change): 0x006AF780
`?UpdateInterpolation@CTCPhysicsBase@@MAEXXZ` (the landed 0x00723730, itself really CTCPhysicsStandard::UpdateInterpolation, binds a
TrollWhackGroundBase stand-in); 0x00BE5C60 GetEdgeStrip (the landed 0x00BF6930 is modelled in an invented `Native::` namespace); 0x00C77470
(the landed CReverbProcessor::Shutdown 0x00C6A3D0 uses a second descriptive class name, CCountedBlock_00c6a780, for the same libpk class).
The agent's other 19 NAME_FIXES rows queued as c706; labels in ghidra_out/labels_chunk706_2026-10-27.tsv.

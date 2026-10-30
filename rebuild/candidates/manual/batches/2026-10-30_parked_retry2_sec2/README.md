# Parked-row retry sweep, wave 2 section 2 (2026-10-30): 6 of 353 parked rows landed, 4 improved

Section 2 = retail 0x00647575-0x007805D0 (engine range), the untouched wave 1 remainder split into six address-contiguous
sections (`$S/retry/split.py`). All 353 rows have a STATUS.tsv line. Landed (RELOCATION_MATCH + PASS): `NPlayerGui::
CDrawStatUpdates::Update` 0x0064A060 and `CTCWeaponTrail::CalculateInterpolatedWeaponHelpers` 0x0076E2F0 (same-TU noinline
real callee body lets VC7.1 see the callee's reads/writes), `CTCLook::ChangeHeadAngleXY` 0x00752120 and
`GetHeadCreatureAnimFrame` 0x00752510 (PDB inline getter `GetHeadTurnRangeXY() const { return 0.5f * (range * 1/360); }` with
retail bracketing), `NTCScriptedControl::CActionTalkBase` protected ctor 0x00717950 (/Oa) and
`CTCPhysicsRigidBody::SetPhysicsMeshBankIndex` 0x0075A530 (/EHsc) from a 25-combination flag sweep.
Improved (../../pending/2026-10-30_parked_retry2_sec2/differ/): 0x00698A30 (matches once engine/CDefPointer.h gains a
`CDefPointer(ENullDef)` ctor: migration wave 10), 0x006BB390 259v265 (named fraction locals), 0x006CA380 115v115 register
choice only, 0x00752E00 286v296 (arithmetic exact; frame: retail homes the Sign temp in the dead turnAngle slot).
Held 337 with residue (the engine-range parks are real register-colouring / scheduling residues: `and byte ptr` bitfield
clears, `xor al,al` zero stores, x87 operand order, eax-then-`mov ecx,eax` loads, merged presence-bit tests, inline /
out-of-line splits); 6 rows have no oracle (0x006E77F0, 0x006BD900, 0x00702D10, 0x00725F10, 0x00739030, 0x00772490).
0x0077FDE0 is an over-long catalog row (BOUNDS_FIX: len 208; the tail is Transfer<EHeroCentreDoorType> at 0x0077FEB0).
15 parked rows have a `// Leaf:` matching no symbol, so their old grades came from the largest function (LESSONS.md item 4).
Coordinator note: this section ran in 56 minutes, mostly a mechanical flag sweep plus LowerBound injection; the 109
same-length rows did not each get a hand attempt with the lever that fits their residue. Sections 3-6 run with an amended
brief (hand attempt mandatory for every row within 4 bytes); section 2's same-length rows are queued for a second pass.
NAME_FIXES.tsv: 6 rows (CTCHeroExperience::GetStatExperience 0x006D6510, ChangeHeadAngleXY AAE private, GetHeadSettingsSlowInFastOut
ABE float&, CTCBase::PeekWorld 0x00686A80 (byte-identical twin of GetWorld 0x00686A70, bindings pair them), CMeshDataBank::
PeekPhysicsMeshInfo 0x00A24250, NGraphicAppearance::CTCGraphicAppearance::GetScale 0x004BBFF0) + 3 review rows (the 0.0 pool
constant 0x0122ED70 with three invented names; the +0x15c bitfield spellings; the CDefPointer ENullDef ctor). Descriptive:
GetMatrixFromRHSet 0x00A358F0, CActionTalkBase::DefaultCameraTagsNeedInitialising 0x01380FB9.
Header debt: local CTCBase, CPlayer, CPlayerGui, CTCHeroExperience, CWorld, CTCMapwho, CMeshDataBank models still in use;
engine/CTCBase.h models the vptr as data so it cannot be a polymorphic base; shared headers lack method declarations.
Coordinator: vland --land 6/6, multi-check 0.

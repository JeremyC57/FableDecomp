# 2026-11-11_acctB_r3001 -- acctB hand-back, retry chunk 3001 (4 rows parked by chunks 2001 / 2005: 1 matched, 3 still parked)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed. This folder supersedes the
parked sources of these four rows in `2026-11-11_acctB_c2001/rows/parked/` and `_c2005/rows/parked/`.

| address | result | notes |
|---|---|---|
| 0x00A4D920 C3DAnimation2::CalcLocalBoneTransformationRelative | RELOCATION_MATCH 1341v1341, test PASS (g++ and VC7.1) | real nesting: a local keyframe from CalcLocalBoneTransformation (0x00A4D330, same TU) expanded inline + `PostApplyRelativeAnimation`. Grade `retail_asm` through the shared GFFloatToLongNear helper (0x00A4DA78, 0x00A4DAE9, 0x00A4DB59); no asm in the row. CalcLocalBoneTransformation is `FABLE_FORCEINLINE` (ASM_RULES 3.3: VC7.1 never auto-inlines a function that reaches asm) -- review at landing |
| 0x00A9EB90 C3DAnimationManager::BlendAnimations | parked, DIFFER 1337v1337, one instruction | 0x00A9EC5A retail `fld dword ptr [eax+edx]`, ours `[edx+eax]`. Diagnostic only: unread locals + dead stores flip it to RELOCATION_MATCH, so the shape is right and the residue is a per-function c2 operand-order key |
| 0x00AB2520 C3DAnimationBuilder::BuildMeshAnimation | parked, DIFFER 1265v1265, 19 lines (was 66) | x87 only: SetInverse E22 0x00AB2698, E41 0x00AB2707, PostMultiply E22/E23 0x00AB27E0..0x00AB2815. Pair landing: needs the same-TU body of 0x00AB1CC0 BuildBoneInfoTree (not an acctB row) |
| 0x00A12EE0 CPhysicsMeshObject::GetNavigationLevelVolumes | parked, DIFFER 1118v1144 | retail spills a second induction pointer `it+8` at [esp+14h] (frame 0x64 vs 0x60) |

Coordinator re-verification of the matched row (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade,
both test runs, read-only vland: 1 win, no warnings). Purity and padded-struct greps are clean.

Review at landing (from the agent): the matched row's object also emits COMDATs for CalcLocalBoneTransformation (0x00A4D330,
not landed; ours 1084 bytes against a 1083-byte retail extent, unchecked) and ConvertTimeToFrames -- check the link.
`hdr/` is the union of the c2001 and c2005 overlays plus new opt-ins (`hdr/README.md`); its CMatrix3x4.h carries a verbatim
copy of c2006's SETMULTIPLY block (merge once). 0x013CB530 and 0x013CD550 each have two bound spellings in
reloc_bindings.tsv (NOTES.md). The agent used far fewer than 30 q.py grades per parked row and about 2,500 scored probe
compiles instead (STATUS.tsv). Rebuild the oracle from Fable.exe (no retail bytes are in this folder).

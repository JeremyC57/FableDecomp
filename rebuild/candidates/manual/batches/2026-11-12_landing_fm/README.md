# 2026-11-12 landing fm -- name-fix pass NF32: 27 queue entries, 34 landed rows changed, 24 renamed

Source: `naming/2026-11-12_agentNF32/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, QUEUE, hdr README). Run as the workflow
`lane_tools/workflows/name_fix_queue_pass.js`, all Opus: apply, adversarial verify, one repair, verify again.

**Queue (tags land1011fe .. fi):** 18 done, 9 partial, 0 rejected.
**Rows:** 34 landed rows changed: 24 renames of the row's own name (RENAMES_applied.tsv section 1) and 23 caller / member
respelling lines (section 2). Examples: 0x006590BF `TailJmp_...` -> CTCVillage::EndSecuritySweep; 0x00971B00 IsStillRunnable is
protected; 0x00A66730 CMemoryAllocatorBase::operator new; 0x0075BA30 the nested CRigidBody::CRigidBodyConfig::operator=;
0x0079CE80 / 0x0079DE00 the two byte-identical PlaySoundW of CTCBulletTime and CTCPhysicalShieldSpell, each with its own callers;
0x004DF885 map<long,long>::~map, named from what its 5-byte jmp targets and from its two callers. STLport instances
(0x00493911, 0x004428F0, 0x005B16FF) take their STLport spelling (descriptive-by-library; retail is STLport, the PDBs are not).
All 34 rechecked in this landing: MATCH or RELOCATION_MATCH, every test PASS (`logs/recheck.log`).
**Verifier findings repaired before landing:** making Simulate 0x0075AD50 private changed its mangled leaf, and its landed binder
0x0075AEC0 still bound the public spelling (blocker); the self rows 0x00759D30 ComputeForces and 0x00759EC0 CalculateVertices still
emitted public leaves while their callers bound the private PDB ones. All three respelled and regraded.
**Final verifier (no grade blockers):** two record findings fixed by the coordinator: 0x006C0330 is partial / PORT, not done (only
the row's local CThing model was respelled); NOTES counts brought up to the repair pass.
**Header:** CSurface.h, one declaration-only opt-in (GETBYTELENGTH). Default unchanged.
**Bindings:** rebuilt by rebind.py for the 34 rows (33 removed, 25 added); bindings_multi_check at baseline;
funclet_includes_check 1317 checked, 0 broken.
**Carried over (queue tag land1011fm):** eight partial entries marked AUTHORING / PORT / CARRY (stand-in-class binders that must
move onto shared headers first; 0x006924B0 has about 50 free stand-in callers) plus the ICF-folded 0x004DD2B1. Two entries that
were only skipped because their address was reserved (0x00875FB0, 0x00A358F0) stay ordinary entries.

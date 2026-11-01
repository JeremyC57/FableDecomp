# Chunk 689 (NA032, 621-937 band) -- 8 of 10 landed (2026-11-01)

Eight rows landed (`verify.log`, every row RELOCATION_MATCH + PASS; bindings multi-check 0): zlib 1.2.1 trees.c `send_all_trees` 0x00C04850 and
libvorbis 1.0.1 res0.c `local_book_besterror` 0x00CA02B0 (vendor text), DirectShow BaseClasses wxutil.cpp `llMulDiv` 0x00CB0370 (SDK text,
USE_LARGEINT undefined), CEaxSource::SetAllParameters 0x00C67C00 (EAX 4.0 source block; retail does not range-check flExclusionLFRatio, kept),
the implicit quest-script destructors ~CQ_AmbushTradersScript 0x00CFA340 and ~CQ_FireHeartScript 0x00D3B010 (slot 0 of the vtables the landed
ctors store), QuadricOptimiserInternals::COptimisedPrimitive::MeasureSegments 0x00AC9D50 and ~CTCInventoryMap 0x00606F80 (engine/CBaseClass.h,
engine/CRegion.h, engine/CCountedPointer.h). Nine of ten catalog labels were stand-ins; 20 labels applied
(`ghidra_out/labels_chunk689_2026-10-27.tsv`), incl. the two deleting destructors (0x00619F50, 0x00D3AFF0 -- the landed 0x00D3AFF0 file is
misattributed to CLandscapeBackgroundPatch), the manifest's CQ_HangingTreeGoodScript label on the CQ_FireHeartScript ctor 0x00D34470, and the
STLport map `_M_erase` / vector instantiations reached from the landed rows.

Coordinator corrections to the agent's NAME_FIXES.tsv (before queueing): the two map `_M_erase` rows 0x00613AE0 / 0x006196D0 now carry the
compiler-emitted symbols bound from 0x00606F80 (the agent hand-typed the template back-references wrong; 0x006196D0's did not demangle);
0x00440990's size_type is `I` (unsigned int: the landed source's typedef and two bound callers), not the agent's `K`.

Decision (coordinator, under the owner delegation): 0x00AB1990 stays parked. Its 812v812 residue is operand order only (`[edx+eax]` vs
`[eax+edx]` in three instructions of the inlined `__uninitialized_copy` loop), but the bytes differ and coverage is byte-exact or nothing.

Parked: C3DAnimationBuilder::BuildBoneInfoTree 0x00AB1990 (812v812, 20 probes), CPhysicsMesh::GenerateLineListFromBaseline 0x00A73E50
(782v811, 12 probes: loop-2 `i` homed in a new slot where retail reuses the dead `baseline` arg slot, CSE of re-read index loads, deferred
`side0 == -1` block). Best sources in `../../pending/2026-10-27_linux_hand689/differ/`. NAME_FIXES.tsv: 20 rows (queued c689). Header debt
(engine/COptimisedPrimitive.h flattens containers; no engine/CTCInventoryMap.h / CPlane / C2DLineF / CCharString; engine/CLevelScriptBase.h
declares an out-of-line dtor where retail's is implicit) -> migration wave. Lessons in LESSONS.md.

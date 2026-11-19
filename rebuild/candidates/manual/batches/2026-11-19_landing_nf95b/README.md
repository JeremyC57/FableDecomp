# 2026-11-19 landing nf95b: naming round 8, NF95 part 2 (59 of 91 rows; 32 held)

Round 8: see `batches/2026-11-19_landing_nf93a/README.md`. The second NF95 instance finished the slice (q11253 on, plus the follow-ups
q11444 / q11447 / q11448 / q11465). Records: `naming/2026-11-19_agentNF95_part2/` (APPLIED / STATUS hold parts 1 and 2; RENAMES_all.tsv is the
whole hand-back; `held/` keeps the sources that did not land).

## Held back (32 rows; HELD.txt)
- **29 rows define a new local model of a class that has a shared header** (land_naming_prep NEWLOCAL; the Round-1 rule: renaming a local
  stand-in to the real class name counts as a new local model). Mostly the CGameEventDataPackage family (22 rows: local `CMemoryBuffer`
  renamed to `CGameEventDataPackage`), plus CPlayerGui, C2DVector, CThing, CIsHornTaskWithThisHorn, CThingCreatureBase, CThingAICreature,
  CTCPerceiveThing, CEnginePrimitiveMorphedStaticMesh and CBankFile models (several in tests). They need a port onto the shared headers.
  No landed rename depends on them: RENAMES section 2 and `reloc_bindings.tsv` checked both ways (22 landed and 11 held self renames, 0 crossings;
  the binding table keeps one caller per symbol, so a held row that is a second caller of a landed rename would not show there). Their two header opt-ins (CGameEventDataPackage.h `_GETUHUGE`,
  CIsHornTaskWithThisHorn.h `_CTOR_OUTOFLINE`) have no landed user and stay with them.
- **3 rows, the CSalSwBuffer::Release set** (0x00C5CE40 and binders 0x00C5A1F0 / 0x00C54BD0): NF95 held them itself until the fourth binder
  0x00C5B300 is ported, so one address never carries two names.
- Their queue entries are annotated `blocked: rows ... not landed`.

## Entries (part 2, 66 incl. 4 follow-ups)
27 done and 22 done-already landed whole; 14 blocked (5 ports NF95 reported, 9 by the held rows); 2 deferred for context and left open
(q11405 0x00C5A500 re-author as CSalSwBaseVoice::SetPitch; q11436 0x0055FC90 port onto the STLport map); q11414 was Account B's (B has since
landed 0x00C77800).

## Rows (59): 22 self renames (RENAMES_applied.tsv, 28 file moves) + 37 respellings
Recheck from the repo after landing (`logs/recheck.log`): 59 MATCH / RELOCATION_MATCH, every test PASS.

## Headers
Only CCodaVector.h `FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT_OUTOFLINE` (one landed user; default path token-identical; 0 asserts moved).
Includer recheck over the 26 rows that include CCodaVector.h or the CODA headers: 14 good; the other 12 are EH funclets graded standalone
(DIFFER with the pre-nf95b headers too: a recheck artifact, not a regression). `logs/includer_recheck.txt`.

## Checks
Synced with FullDecomp-B first (B's hand4d_b10 landing of 0x00C77800 and its snapshot). `naming/rebind.py`: removed 226 binding lines, added
203; `bindings_multi_check.py` at baseline (`NullCoord`); funclets 1,355 checked, 0 broken.

## Queued: `land1119nf95` (8 new of NF95's 27 NAME_FIXES lines), `land1119Bhand4db10` (2, Account B).

## Rule slips (NF95 NOTES)
The 29 new local models above (the landing caught them); st.py overwrote three placeholder STATUS lines (rewritten); two greps waited on
stdin; ed.py misses graded unchanged files; RENAMES evidence corrected for 8 heap rows.

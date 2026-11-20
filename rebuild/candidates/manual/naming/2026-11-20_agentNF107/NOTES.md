# NF107 notes (round 10, 2026-11-19): resume of NF106's search-tools ports (q11280 / q11283; q11281)

## Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log)
- Entries 3: blocked: port 2 (q11280, q11283 = same claim; 20 binders ported this round, 20 rows left); open 1 (q11281, not started);
  done 0, done-already 0, rejected 0, deferred 0 (HELD.txt empty).
- Rows in STATUS.tsv 27 (rows.txt): ported 20 (rows/: 20 sources, no test changed); not-a-binder 2 (self rows 0x0049C6F0 / 0x00436500,
  already the PDB spelling, NF94); blocked 5 (0x008E60D0 after 3 failed forms; 0x0078954D / 0x0078971D / 0x007FDB10 / 0x006CE300 not
  attempted, 0 grades).
- recheck_final.log: one vland --recheck-landed run over the 20 changed rows (see the log; every line RELOCATION_MATCH + PASS).
- Header opt-ins 5 (hdr/README.md): CThingSearchTools.h BESTROUGH_THISEND (+ byte-offset cell branch), CThingMarker.h STATTYPEFLAGS,
  CTCDExperienceOrb.h STATGETDRIVERTYPE, CTCCarrying.h ISTHINGCARRYINGMELEEWEAPON, CThingDeadCreature.h STATTYPEFLAGS.

## What changed (explicit instantiations of the shared engine/CThingSearchTools.h templates)
1. K016 vector define set (0x0074D710: ORIGINAL_ROUGHAREA + THISEND + CELL_BYTEOFFSET + STLP_VECTOR_OVERFLOW_OUTOFLINE) PLUS
   ROUGHAREA_TYPEDLOCAL: every Calls vector row stores the typed pointer before the filter calls (`mov [esp+38h],edi`), which the plain
   K016 set misses (DIFFER same size). Spec t/specK.py + generator t/gen.py:
   0x007F9C00, 0x00969FB0, 0x0095DB90, 0x007A9300, 0x007A9580, 0x00905D10, 0x009793D0, 0x00851490 (CThingMarker opt-in),
   0x008786E0 (vector<CIntelligentPointer<const CThing> >: + CINTELLIGENTPOINTER_COPY for the inline copy-construct), 0x007F4500,
   0x008E4100 (CThingAICreature include set + CTCCarrying opt-in), 0x009595C0 (CThingDeadCreature opt-in).
2. std::list ROUGHAREA_EGOD set (0x0080E8E0 TU-mate): 0x0080E670, 0x00937080 (CThingDeadCreature opt-in).
3. GetAllTCDsInRoughArea (0x005185F0 TCDSINROUGHAREA_EGOD set): 0x0077ADF0 <CTCDExperienceOrb, CFilter_Bind2<IsNot, In2DRange>, vector<CThing*> >
   (its catalog label GetPBestThingInRoughArea<...> was propagated; driver type 26 from the body).
4. Caller-TU sets (CTCTargetingPlayer, 0x00804020 / 0x00803D60: TargetingPlayerSupport.h CFilter_Bind5 + anonymous-namespace filters)
   + OVERFLOW_OUTOFLINE / CLIPAREA / THISEND / TYPEDLOCAL (not CELL_BYTEOFFSET: MAP_VALIDATE makes the cells protected, C2248):
   0x00809A70, 0x0080A550 (third form).
5. GetPBestThingInRoughArea (BESTROUGH_BODY) on the caller sets 0x007F68B0 / 0x007F67C0 + the new BESTROUGH_THISEND opt-in with the
   byte-offset cell branch: 0x007F7340 (forms: 591v594, 593v594 with THISEND, 587v594 with the cell branch = the T type-flags define was
   missing, mask 0; + FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS -> RELOCATION_MATCH), then 0x007F70D0 and 0x00937420 first form.
- Self leafs: all 20 move to the Ego_r.pdb / FableWin.pdb instance spelling (RENAMES s1; 18 off CThingSearchTools_Calls stand-ins,
  0x009595C0 / 0x00937080 QAE -> QBE). Callee respellings (RENAMES s2, from work/b0_pairs.json -> work/fin_pairs.json): 0x0049C6F0 /
  0x0049C700 (20 each), the STLport vector _M_insert_overflow instances (0x00436AC0 x6, 0x0049C370 x2, 0x007A91A0 x2, 0x00788FFC,
  0x00739550, 0x0062E190, 0x00515CC0: off ThingVector stand-ins), and six anonymous-namespace filter callees.

## Split check (no new split; t/diff.py against reloc_bindings + callers.py exe scan)
- Every new self leaf: the landed callers that bind these instances already bind the PDB spelling (0x007F9960, 0x00969E70, 0x0095D4C0,
  0x007F2DC0, 0x00804020, 0x00803D60, 0x007F67C0, 0x007F68B0, 0x00959800) or bind a different stand-in that stays as it was (existing
  split, NAME_FIXES: 0x00877D20, 0x00978FD0, 0x00937B10); no landed row binds any old CThingSearchTools_Calls self leaf of these rows.
- Vector overflow callees: every new spelling but 0x007A91A0's is already bound by another landed row ("known"); 0x007A91A0 was already
  split (its own row spells a CUnprovenElement_007a91a0 placeholder; NAME_FIXES).
- Anonymous-namespace callees: the ?A0x hash differs per compile path (reloc_rename.py normalises it). 0x007F3D60 / 0x00808E40 / 0x0080DC90:
  the callee rows are anonymous already, so the binders now agree with them (old binders were global). 0x008090E0 / 0x008077B0 /
  0x00809210: the callee rows spell operator()(CThing*) at global scope, which the old binders (const CThing&) already did not bind
  (existing splits, unchanged in count); their right spellings are in NAME_FIXES (0x00809210 is ShotFiredByThing, not NoiseCameFromThing).

## Blocked / left
- 0x008E60D0 <CThing, vector<CThing*>, Bind3<CanCollideOrInteractWithThing, OverlappingSphere, HasTC> >: 3 failed forms (K016 + TYPEDLOCAL;
  without TYPEDLOCAL; PEEKCELL_INLINE): best DIFFER(704v704), one SIB operand order at 0x008E61E4 (retail `mov eax,[eax+ebx]`, the
  Cells[layer] row base; built `[ebx+eax]`). Draft work/try_008e60d0_f1.cpp. Its caller 0x008E57D0 binds a const-T spelling (NAME_FIXES).
- 0x0078954D / 0x0078971D (q11281) and 0x007FDB10: not attempted (context); prerequisites in STATUS.tsv. 0x006CE300: in rows.txt but not in
  this round's rowsA group, not attempted.

## Header ownership (coordinator)
- CThingMarker.h, CTCDExperienceOrb.h, CTCCarrying.h are outside the four headers the brief assigns; each got one additive opt-in
  (hdr/README.md). NF105 / NF108 overlays do not touch them.

## EH funclets
- None: no file in rebuild/src/compiled includes any of the 20 changed sources (grep of their file names).

## Locks
- All 27 rows were locked as NF107 (rows.txt). 0x008077B0 / 0x008090E0 were claimed while checking a split, found unnecessary, and the
  locks were removed again (rows untouched). No UNMATCHED_ROWS / Account B row touched; 0x0063F4B0 compiles RELOCATION_MATCH with the overlay.

## Descriptive names
- None added. All self leafs are PDB instance names; anonymous-namespace leafs are cut at ?A0x as the catalog convention.

## Rule slips
- Two header-edit scripts asserted (the BESTROUGH cell text occurs twice in CThingSearchTools.h) and the grade chained after them ran
  against the unchanged overlay: 2 wasted grades on 0x007F7340 (counted in grades_used, not counted as forms).
- 0x0077ADF0's first compile failed on my own copy slip (the `class CTCMapwho;` forward declaration dropped), 1 grade.
- 0x007F7340 took a 4th grade after 3 failed forms: the third form's residue was a missing prerequisite define (T's type flags), not a
  new body form; recorded here for the coordinator to judge.
- Several orientation calls ran without the env line (read-only).

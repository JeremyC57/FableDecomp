# NF333 hand-back (2026-11-24): T1 standin_alias / signature batch

Scope: the 173 entries of ENTRIES.tsv (89 standin_alias, 84 signature; no access entries were in the list). Every entry was triaged
(tools/tri332.py, tools/sacensus.py, census in work/census.tsv). 163 have a STATUS line; 10 signature entries were not reached (below).
Stopped at 31 changed rows because the context got heavy (the brief allows handing back sooner than 60).

## Counts (STATUS.tsv, 163 lines)
- done 35: 23-31 (CCountedPointer DeleteData), 19 34 36 41 45 (stale CClass_ lines, table-only), 24 25 (stale Destroy_ lines,
  table-only), 55 76 (CThing& ctor), 1 (GFCalcThingUID), 98 99 104 106 111 115 116 117 125 132 133 134 136 141 146 150 152 153.
- done-partial 1 (35), done-equivalent 1 (156), skipped-held 54 (145 included, below), skipped-port 72, rejected 0.
- 31 changed rows (rows.txt); one test changed (rows/00401f30_test.cpp, zero line delta). No header change (hdr/ empty).

## Proof (evidence/)
- Every changed row: HEAD and new RELOCATION_MATCH compiled at ONE path (nf.py grade), objsec ALL_EQ with `.drectve` exact and
  DEF / UND sets equal under the rename (proof_g1 / g2 / g3 / g5 / g6 / g7.txt), /FORCE link HEAD v new with the selves as controls
  (link_g1..g7.txt: HEAD leaves the old spelling unresolved, new resolves it, 0 duplicates).
- 0x0095C1F0: nf.py proof says NOT_EQ only because the jump table's compiler-local `$Lnnnn` labels renumber once the enum header is
  included; proof_g3_0095c1f0_labels.txt compares them by (section, value): ALL_EQ. Decision D1 below.
- vland --recheck-landed: 32 / 32 RELOCATION_MATCH PASS (31 handed back after 0x0089F4A0 was withdrawn) (recheck_final.log: rcA + rcB, then rcC for 0x00401F30 after its test fix).
- apply_table.py --dry: 35 replace + 1 delete, 0 misses, no new multi-target symbol (apply_table_dry.txt).
- xcheck_handback.txt: every old table line exists; 3 new symbols are in FableWin.pdb only (DeleteData<CExpressionSet>,
  DeleteData<CSpeechFrame>, GFCalcThingUID); their second source is the retail binding (the Reset body stores the deleter address as the
  count block's DeleteFunc; 0x0051E210 calls 0x0051D990, whose landed self already carries that Leaf), as in the tracker / NF282.
- Stale-line proofs: stale_g1.txt (24 / 25), stale_g4.txt (19 34 35 36 41 45, and the different wrong forms of 47-49, 55, 76, 11).

## Groups
- G1 (7 rows): free `Destroy_<addr>` deleter stand-ins -> the landed NF282 hybrid (FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY +
  engine/CCountedPointer.h, precedent 0x00AD65A0), full mangled Leaf and a Naming comment added (+6 lines; no row asserts).
- G2 (15 rows): pointer -> reference / const reference respells with their call-site `&` / `*` edits, one same-width return retype,
  one void -> bool return the caller discards (NF313 Q2, NF324 Q3).
- G3 (6 rows): access / const / return fixes written `private: ...; public:` on the one declaration line (the access in effect before
  is public in every case, checked), ReportCrime's enum through the row's existing EOpinionDeedType include and its existing cast
  form, the hornet-queen pair with engine/fable_ai_action_enums.h (+1 include line).
- G5 / G6 / G7: GFCalcThingUID (stdcall stand-in -> PDB fastcall, same call bytes for two __int64 stack arguments); EHeroAbility via
  engine/EHeroAbility.h with the member retyped (NF304 Q2); the two CThing& ctor factories (NF332 G5 shape).
- Withdrawn after a compile / precheck (STATUS skipped-port): 100 / 101 (0x00937D20: the shared CThingCreatureBase does not convert to
  the row's CThing*), 143 (0x00757F90: CWoundMorphs.h adds libcp to `.drectve`).

## Late checks (advisor review before hand-back)
- CScriptThing definer screen over all objects: 0x0089F4A0 (entry 145) DEFs ??1CScriptThing / ??0CScriptThing(const&) / ??_G, so it is a
  CScriptThing definer the brief keeps off. Withdrawn (rows/, orig/, RENAMES, TABLE_CHANGES); entry 145 is skipped-held. No other
  changed object defines a CScriptThing member.
- Name census: text grep of rebuild/src/compiled and rebuild/include for every old spelling (the Destroy_ / MakeUID stand-ins and each
  old pointer / return / access form): the only namers are the changed rows themselves (and comments / the selves, which already
  define the PDB names). ref_scan.py was NOT run; the coordinator should run it before applying the `replace` lines.
- Freshness: mkoff333.py rerun; no changed row is off-limits, an NF334 entry / witness / row, or locked by another owner; every orig/
  file equals the repo source (and test).
- Census sample (NF299 Q4): opened 2, 12, 21, 33, 38, 42, 44, 56, 64, 73, 78, 83 (reasons for 2 / 33 / 44 corrected after opening).
  portcensus.py (binders including a shared header that already declares the PDB member) was NOT run; for 56 engine/CTCChest.h does not
  declare GetNumberOfKeysToUnlock.
- Entry 132: the ReportCrime call casts the local-model `long` member deed.Type to EOpinionDeedType (the same row already casts it the
  same way for PeekPDeedEffectsDef); retyping COpinionDeed::Type to the enum instead (the entry-134 form) was not tried. Decision D5.

## Decisions needed
- D1: accept 0x0095C1F0 (entries 98 / 99) with objsec equality where the only name difference is the jump table's compiler-local
  `$L` labels (same section and value, same bytes and `.drectve`)? If not, drop that row and set 98 / 99 to skipped-held.
- D2: 0x00401F30's test placeholder global now reads `CCharString GVersionString(*static_cast<CAFile*>(0), 0);` so the stub keeps
  recording file address 0 (a null-referent reference, test only). Accept, or prefer a second test-only (CAFile*) ctor (+lines)?
- D3: entry 11's table line has the bare namespace `N_008fdb27` in its symbol column (malformed); 0x008FDB55's object binds
  `??0CInterestingToVillagersDef@N_008fdb27@@QAE@XZ` there. Respell the line to what the object binds, or leave it for the port?
- D4: entry 156 (toLowerCase): the binder binds the STLport V form; the self row's Leaf carries the PDB's Dinkumware U form
  (NAME_FIXES line). The split stays until the self is respelled; which side should move?
- D5: entry 132's cast form (above) v retyping the local member.
- D3 also covers 47-49: their table lines are stale CClass_ spellings, while the witnesses bind the ctor in the global scope (not
  NParticleEngine::); should the lines carry what the witness objects bind until the port?
- NAME_FIXES.tsv also lists the two CTCX_Alloc factory selves (NF332 D6 form, T3).

## Not reached (open for the next batch)
108 (0x00BD7A00: virtual + ERenderPassType on a local model), 118 (0x00AE2CB0 / 0x00AE2D20 already pass const CParticleContext&: the
table line looks stale, but PDB access is private, so a `friend class CParticleProgram` (NF305 Q5) is the likely fix), 121 / 122
(CPersistTraits<CIntelligentPointer<CThing>> pointer -> reference), 123 (0x00A25D20 CPhysicsMesh ctor PAV -> ABV; the same row's
entry 95 is a port), 142 (0x00799E30 CThing* -> CThingAICreature* parameter), 144 (~CTemporaryBuffer virtual), 148 (0x00733910;
0x00733950 is an NF334 witness), 149, 155.

## Rule slips (recorded)
- The clang precheck of rows/00401f30_test.cpp and the rcC recheck that compiles it were launched in the same call (the rule wants
  the precheck in an earlier call). The precheck passed before the recheck ran.
- One /FORCE link (link_g7, two objects plus two controls, a few seconds) ran in the foreground under `timeout 300` instead of the
  background. Everything else (grades, prechecks, /Zs, the other links, rechecks) ran in the background with `timeout 900`.
- No write into the repo, no git, no files outside my folder; scratch removed only with clean.py.

## Locks
Kept: the 31 changed rows (rows.txt), owner NF333, until the landing. Released: every control / precheck lock.

## Files
STATUS.tsv, APPLIED.tsv, REJECTED.tsv (header only), RENAMES.tsv (section 1 empty: no self renames; section 2: 32 caller lines),
TABLE_CHANGES.tsv (36 lines), NAME_FIXES.tsv (3), rows/ + orig/ (HEAD copies), evidence/, recheck_final.log, rows.txt, tools/.

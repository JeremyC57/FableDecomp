# NF186 notes (round 10, quest / script function names; 49 entries)

Counts (by script from APPLIED.tsv / STATUS.tsv / recheck_final.log): 49 entries: 14 done, 32 done-already, 2 blocked, 1 deferred,
0 rejected, 0 open. STATUS lists 63 rows: 14 changed (ported: 3 MATCH + 11 RELOCATION_MATCH, every test PASS in one final vland run,
recheck_final.log), 46 not-a-binder (self rows already right, binders already on the PDB spelling, the 7 q6495 vtable binders), 2 blocked
(0x00DEF730, 0x00EE1BE0), 1 deferred (0x00E2AB30, NF184). No made-up names: every new spelling is a PDB name.

## Done-already (32)
The Anniversary-wave entries q4852..q4882 were written before naming wave 35 re-modelled most of those rows: 21 self rows already define
the claimed nested leaf (Leaf line, build-catalog Module and catalog name all NScript-qualified) and REFS.tsv finds no landed referencer
(they are reached only through entity vtables). q6214 / q6217 / q6219 / q6220 / q6468 / q6469 / q6473 / q6508 / q6525 / q6527 are the
same (self Leaf already the PDB spelling; the 7 binders probed by HEAD compile bind it). q6495 (data 0x012EA430): all 7 exe-scan
referencers probed by HEAD compile already bind ??_7?$CPredicate_And@VCIsThingAlive@NScript@@VCIsVariableFalse@2@@NScript@@6B@; the
??_7CAndPredicate@@6B@ witness is stale. The locks claimed only for that probe were set back to `released`.

## Done (14 rows ported, all self renames unless noted; RENAMES s1 has 12 lines)
- Five ??_G stand-ins re-authored as the compiler-generated PDB ??_G on the shared header, with the PDB ctor declared by a new opt-in,
  defined `inline` in the row and referenced from an anonymous-namespace placement-new helper (`// Fidelity: scaffolding, not retail`,
  the landed 0x00E016F0 form): 0x00D0B3C0 CCROWDBANDITS, 0x00D40890 CSpeedFriend, 0x00E5B390 CBeggarBully, 0x00E70370
  CDF_SummonsCreatures, 0x00ED17B0 CManWithDoorName. None of the five classes has an out-of-line ctor in retail (each Alloc expands it),
  so the pick-any ??0 has no owner row.
- 0x00DEF700 GetParentScript / 0x00DEF720 OnPredicateFail of CSP_UndeadTrigger: gapscan `struct T` rows re-authored (MATCH).
- 0x00DED0A0 CQ_SecretPassageScript::InitialiseVariables (was the byte-twin label CMeleeApprentice::Init); test rewritten on the header
  (the HEAD test linked the old free function; PassPattern kept).
- 0x00DA8510 ProcessMission, 0x00DAEBF0 GetVillagerSpeechIndex, 0x00E15500 WatchForTermination, 0x00E4DF20 Should_I_Flee: global /
  Row-namespace local models ported onto their shared headers; each one's referencer already bound the PDB spelling (probed).
- Callee respells (RENAMES s2, 3 lines): 0x00DA8510 now binds GuardComment 0x00DA8970 as the self row's NScript leaf (exe scan,
  refs.py: 0x00DA8510 is the only landed referencer of 0x00DA8970, so it ends on one spelling); 0x00E15500 binds ??0CCharString@@QAE@PBDJ@Z (was the stand-in PBDH) and ?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ
  (was a Row00e15500 stand-in).
- In-place respells (no symbol change): 0x00E64DF0 local model member FinalSector -> PrizesWon (q6281; Ego_r +0x70 = retail +0x5c), and
  0x00E95F10 (q6497; claimed lock, was `released`) local CV_HauntedHouseScript declaration DoMission -> CheckRegionLeaving plus its comment.

## Pick-any / defined-symbol checks (objdump -t, HEAD vs new, tools/defs.py; tools cmpbody.py)
- The five ??_G rows newly define: own ??_G, ??_7 and pick-any ??0 of the class, the shared-header CEntityScriptBase inlines (??0,
  ??_7 at 0x012C3224, ??_GCEntityScriptBase), placement new ??2@YAPAXIPAX@Z, and the TU-local `?A0x...` helper. Compared with cmpbody:
  ??_GCEntityScriptBase against the owner row 0x00CDEBF0's object, ??_7 / ??0 CEntityScriptBase and ??2 against the landed 0x00E016F0
  object: bytes and relocations equal for all five. ??2 is bound in the table to 0x00405C10, whose row defines only
  ?CFourierAnalysis_00405c10@@YAPAXPAX0@Z (a folded body), so it has no owner copy (nf181 rule); its copy equals 0x00E016F0's.
- **Decision / debt for the coordinator (0x00E70370):** the landed Alloc 0x00E702C0 also emits ??_GCDF_SummonsCreatures, ??_7 and
  ??0 of the class as pick-any COMDATs. ??0 and ??_7 are byte- and relocation-equal to this row's copies; its ??_G copy has the same
  bytes but binds `??1CDF_SummonsCreatures@CV_DemonDoorsScript@NScript@@UAE@XZ` (the user-declared, never-defined dtor of its local
  model of a class that has engine/CV_DemonDoorsScript.h) where the owner row 0x00E70370 now binds ??1CEntityScriptBase@@UAE@XZ like
  retail. The new owner copy is the retail one; the Alloc's copy is pre-existing debt (if the linker picks it, ??1CDF_SummonsCreatures is
  unresolved). Fix: port 0x00E702C0 onto the header with the NF186 SUMMONSCREATURES_CTOR opt-in (not done: outside my locks).
  The other four classes have no landed Alloc (0x00D0B2D0 / 0x00D407C0 / 0x00E5B2D0 / 0x00ED16E0 are not landed).
- 0x00DA8510 newly defines pick-any ??4CTimer@@QAEAAV0@J@Z / ?GetTime@CTimer@@QBEJXZ (row-local plain `inline` definitions of the
  header-declared members, as 0x00D647F0 defines them; unbound, no owner row) in place of the local CTimer's ?Set / ?Get.
- All other changed rows: defined set = own leaf + the same literals / inlines as HEAD.

## Blocked (2): one definition of the entity ctor
- q4871 0x00DEF730 (CSP_UndeadTrigger ??_G) and q4879 0x00EE1BE0 (CTH_SnowTroll ??_G). The ??_G is emitted only with the vtable,
  i.e. where a ctor is compiled. HEAD compiles (tools/hc.sh + defs.py + cmpbody's relocation list): the ctor owners 0x00DEF6A0 and
  0x00EE02C0 each define the PDB ??0 strongly (selection 1) and emit ??_7 of the same name pick-any with ONE stand-in slot
  (?v00@CSP_UndeadTrigger@... / ?v00@CTH_SnowTroll@...); the SnowTroll Alloc 0x00EE0240 defines neither, so a ??0 clash would be new
  (the nf163 0x00DCF6E0 hold shape, not the 0x00E016F0 pre-existing-debt shape). The 0x00E016F0 scaffolding here would add a pick-any
  ??0 (LNK2005 against the strong copy) and a 7-slot ??_7 copy that differs from the owner's: held under the pick-any rule. No form
  graded. The proven ??_G leaves are in NAME_FIXES.tsv. Order: port 0x00DEF6A0 (NF186's UNDEADTRIGGER opt-in already declares the class
  and ctor; ~vector<CScriptThing> 0x008AC970 must stay out of line) and 0x00EE02C0 onto their headers with the ctor made pick-any
  (nf88), then re-author the two ??_G rows. Same family as NF178's 0x00E2AB30.

## Deferred (1)
- q4860 0x00E2AB30: held by NF184 (pending landing), per the coordinator. It is not in my rows.txt and nflocks/00e2ab30/owner reads
  `released`; not touched.

## Pre-existing, not changed
- 0x00E95F10 still models CSpawnedFuncBase / CSpawnedFunc / CV_HauntedHouseScript locally (vland SHARED_HEADER_AVAILABLE, same at HEAD).
- 0x00E64DF0 still models CV_ChickenKickingScript locally (in-place respell only).
- Tests of 0x00DAEBF0 / 0x00E4DF20 / 0x00E15500 / 0x00DA8510 and the five ??_G rows are unchanged standalone tests (they do not link the
  row); the 0x00DAEBF0 and 0x00E4DF20 tests model their classes locally (pre-existing).
- vland WARN STANDIN_LEAF on 0x00DED0A0 / 0x00E15500: the catalog names change with RENAMES s1.

## Funclets
None: no rebuild/src/compiled/01 file includes any changed row (grep of each source file name).

## Flags lines
- 0x00DA8510: new `// Flags: /d2inlS22` (its build entry has no CompilerFlags; the catalog entry needs it) with an `// Inline-budget:` line.
  Retail expands CTimer::operator=(long) / GetTime() at all three sites; plain inline definitions give DIFFER(1083v1110) at the default
  budget, /d2inlS20 / 21 DIFFER(1105v1110), /d2inlS22 / 25 / 30 / 40 / 60 / 80 RELOCATION_MATCH (22 is the smallest; /d2inlT500 differs).
  Preference 2 (per-row flag) replaces the FABLE_FORCEINLINE that the HEAD local model used; the final vland payload carried the flag.
- 0x00DED0A0 matches without flags (a /Gr line was tried and removed); 0x00E15500's old `// Flags: /O2 /Oy /W3` line (base flags only)
  was dropped.

## Rule slips
- 0x00DA8510 used 17 compiles (STATUS grades_used): 4 port grades, 3 HEAD compiles, an 8-grade /d2inlS sweep for the least invasive
  inline device (4 sweep values differ, the row had a matching FABLE_FORCEINLINE form throughout), and 2 final vland runs.
- The first final vland run was before the advisor review; the row 0x00DA8510 changed afterwards, so recheck_final.log is the second run.
- One grade (0x00E15500) was a SRC_FAIL: MasterScriptData is a pointer in the default CLevelScriptBase (reference only under
  ..._ORIGINAL_MEMBERS); fixed in the next grade.
- `tools/odis.py` was first named dis.py, which shadowed the stdlib module and failed one call (renamed).

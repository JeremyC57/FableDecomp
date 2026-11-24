# NF328 unit: ~CScriptThing (??1CScriptThing@@UAE@XZ, 0x004AA840) + copy constructor (??0CScriptThing@@QAE@ABV0@@Z, 0x004ABE90)
**Status: HELD (stop condition: copies that differ from retail; decisions D1-D6 below). Nothing lands partially.**
The nine rows that make every strong definer pick-any are drafted and fully proven (rows/, rows.txt); they are ready to land as one
unit if the coordinator rules the differing copies are debt (NF326 Q2) rather than holders (this brief).

## Members
| member | self | PDB spelling (Ego_r + FableWin) | form drafted |
|---|---|---|---|
| ~CScriptThing | 0x004AA840 | ??1CScriptThing@@UAE@XZ | `inline` dtor on engine/CScriptThing.h + ONE `?A0x` helper `ReferenceScriptThingDestructor` (qualified explicit dtor call, `#pragma inline_depth(0)`) |
| CScriptThing(const CScriptThing&) | 0x004ABE90 | ??0CScriptThing@@QAE@ABV0@@Z | `inline` copy ctor on the row's EXISTING local model + ONE helper `ReferenceScriptThingCopy` (returns a copy by value, inline_depth(0)); Leaf made the full symbol |
A constructor / destructor has no address, so NF326's `&M` helper becomes a call: the qualified explicit destructor call and a
by-value return. Neither adds any symbol beyond the helper (no placement new, no default ctor, no new vftable / ??_G).

## Census (by object; tools/srccensus2.py -> tools/hcensus.py -> tools/census2.py -> tools/census_report.py)
- Text gate: 1,023 landed rows whose comment-stripped text can make the object define either member (dtor body out of / in class,
  copy-ctor body, local `class|struct CScriptThing` model, FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY). Checks: no header in rebuild/include
  defines either body; no catalog `/D` opt-in; no folded `../src/compiled` include; an unanchored re-scan of every other landed source
  finds 0 more; every NF326 object that defines either symbol is in the set.
- Compiled at HEAD with VC7.1: 1,017 objects (all MATCH / RELOCATION_MATCH: 28 / 989). Holes: 6 Codex-reserved rows (pass 26 / 27):
  0x00CFCE90, 0x00D90290, 0x00DDB720, 0x00E4FA20, 0x00E96110, 0x00F00C90 (text: shared-header `inline` dtor / copy; 0x00DDB720's dtor
  copy is BYTES_EQUAL in NF326's own census). See D5.
- **~CScriptThing:** 529 objects define it. Strong (selection 1): 8 (0x004AA840 self, 0x004AB170, 0x00CE0920, 0x00CE0B00, 0x00CE0CE0,
  0x00D151D0, 0x00DDE230, 0x00E6B750). Pick-any equal to retail: 502. **Pick-any DIFFERENT from retail: 19** (D1). 299 more objects only
  reference it. One equal copy binds a stand-in base dtor (0x00904540, D6).
- **Copy constructor:** 296 objects define it. Strong: 1 (the self). Pick-any equal: 293. **Different: 2** (0x00CBD310, 0x00CF68C0; D1).
  138 more only reference it.
- Per-object table: evidence/objcensus_head.tsv; summary: CENSUS.tsv. A differing copy's relocation column lists symbols only.

## The drafted change (9 rows) and its proof
| row | change | pair grade HEAD / new (one work path) | objsec (tools/uproof.py) | vland --recheck-landed H / N |
|---|---|---|---|---|
| 0x004AA840 | inline + helper | RELOCATION_MATCH / RELOCATION_MATCH | OK: ??1 sel 1->2, bytes equal HEAD + retail; only new DEF = helper; vftable, ??_G, ??1CCountedPointer, UND, .drectve equal | RELOCATION_MATCH PASS both |
| 0x004ABE90 | inline + helper, full Leaf | RM / RM | OK: ??0 sel 1->2, equal retail; only new DEF = helper; its local-model COMDATs unchanged | RM PASS both |
| 0x004AB170 (??_G row) | strong dtor -> inline + helper | RM / RM | OK: ??1 sel 1->2 equal retail; ??_G unchanged (equal retail, 87) | RM PASS both |
| 0x00CE0920, 0x00CE0B00, 0x00CE0CE0, 0x00D151D0, 0x00DDE230, 0x00E6B750 | `CScriptThing::~CScriptThing() {}` -> `inline` (same line + comment) | RM / RM | OK: ??1 sel 1->2 equal retail; everything else equal; no helper | RM PASS both |
Evidence: evidence/uproof_p1.txt, uproof_p2.txt, uproof_p3.txt (run on the pair-grade objects, work/pobj, not on vland's), pairgrade.log,
recheck_final.log (9 RELOCATION_MATCH PASS per side; warnings identical: 8 SHARED_HEADER_AVAILABLE, 1 STANDIN_LEAF on 0x004AB170).
pickany_vs_retail.py (python3 -I, reloc_bindings.tsv) on the 9 new objects: every ??1CScriptThing copy EQ at 0x004AA840 (the tool
skips a row's own address; the selfs are covered by uproof + grade); its 4 NEQ lines (IsAlive 0x004AB130, CBasicString::operator==
0x004115A0 in 0x00D151D0 / 0x00E6B750) are identical on the HEAD objects (evidence/pickany_vs_retail.txt).
**COMDAT census after** (evidence/objcensus_after.tsv, CENSUS_after.tsv): 0 strong definers of either member; dtor 510 equal + 19
differing; copy constructor 294 equal + 2 differing (before: 8 + 1 strong; 502 / 293 equal).

**Links** (tools/link2.py, LINK= cleared, /FORCE, /OPT:NOREF, /MAP; all 1,017 census objects, selfs first; evidence/link2_all_*):
- HEAD 938 duplicate lines, new 115 (whole blocks compared as multisets by symbol). Removed: exactly 528 x ??1CScriptThing@@UAE@XZ +
  295 x ??0CScriptThing@@QAE@ABV0@@Z (= every other definer of each). Added: none; the other 115 lines are identical. So the new set
  has NO duplicate on either member. No strict (/FORCE:UNRESOLVED) link was run: the 115 pre-existing duplicates would stop it.
- Unresolved sets identical (1,960 symbols). The map keeps the self for both members in both sets.
- Remaining duplicates are pre-existing and outside the unit: ??4CScriptThing (operator= 0x008AB980) 75, CTimer ctor/dtor 12,
  Compare variants 7, GetDef / LowerBound / GetAllThings instances 6, single NScript ctors, CIsThingAlive / CIsThingConscious dtors,
  OnPredicateFail, CThingSwitch_CompositeScalarDeletingDestructor.

## Why it stops: the differing copies (D1)
With every definer pick-any, the linker keeps whichever copy comes first, so every emitted copy must have retail's bytes.
- **dtor, 14 "vptr-late" in-class bodies** (66 bytes; the release written in the dtor BODY of a local model, so VC7.1 schedules the
  `PImp.Info` load above the vftable store): 0x00D1F610, 0x00DA6740, 0x00DC8030, 0x00DCC140, 0x00DDE940, 0x00E0D6A0, 0x00E33B00,
  0x00E399D0, 0x00E6F040, 0x00E7F7F0, 0x00E82010, 0x00E9D800, 0x00ED41D0, 0x00ED45A0.
- **dtor, 4 implicit 5-byte bodies** (`jmp ~CBaseClass`: local models whose PImp is plain words, no release): 0x00704A80, 0x00896120,
  0x00DEF6A0, 0x00E9A5C0.
- **dtor, 0x00AF69D0** (60 bytes, no vftable store: the row models its record's member as an implicit-dtor CScriptThing on purpose,
  for its own ??_G record body).
- **copy, 0x00CBD310** (std::vector<CScriptThing>::push_back row: its local `CScriptThing` has a CCharString member, so the emitted
  ??0CScriptThing@@QAE@ABV0@@Z is a different function under the real name; identity / port).
- **copy, 0x00CF68C0** (declares an explicit specialisation of CCountedPointer<CScriptThing>'s copy ctor, so its emitted copy CALLS it).

Measured fixes for the vptr-late shape (0x00DC8030 probes s1-s3, 0x00D1F610 probe m1; evidence/probes.log). m1 is measured on 1 of the
14 rows only:
| spelling | own leaf | dtor v retail | DEF delta |
|---|---|---|---|
| s1: release moved to an inline member function called from the body | RM | DIFF | none |
| s2 / s3: release in a member struct's dtor (plain / FABLE_FORCEINLINE) | RM | EQUAL | + ??1CCountedPointerWords@CScriptThing@@QAE@XZ (made-up name; rejected) |
| m1: the real `CCountedPointer<CScriptThing> PImp` member (engine/CCountedPointer.h) + empty user dtor | RELOCATION_MATCH | EQUAL | + ??1?$CCountedPointer@VCScriptThing@@@@QAE@XZ only (pick-any, 53 bytes, identical to the self's own copy and to 0x008A1E50's) |
The vftable-store order needs a member destructor (a destructor-sequence boundary); every member type with a destructor is also
emitted pick-any (referenced from .debug$F). m1 is the retail-faithful form but adds a COMDAT (NF320 Q2), so it is a decision (D2).
Not attempted per row: the 4 implicit rows and 0x00AF69D0 need a user dtor (same D2 shape, and 0x00AF69D0's own ??_G body would gain
a vftable store); the two copy rows are identity / callee ports.

## Decisions needed
- **D1.** Do the 19 + 2 differing copies hold the unit (this brief: "any unit member you cannot fix holds the whole unit"), or are they
  queued debt as NF326 Q2 ruled for GetDataString (land the 9 rows now and queue the 21 rows; at HEAD these copies already sit beside a strong self, which is LNK2005)?
- **D2.** May the 14 vptr-late rows (and then the 4 implicit rows) take the m1 form: the local `PImp_Data` / `PImp_Info` words + body
  release replaced by `CCountedPointer<CScriptThing> PImp` from engine/CCountedPointer.h and an empty user dtor? Each object then gains
  exactly ??1?$CCountedPointer@VCScriptThing@@@@QAE@XZ (pick-any; FableWin.pdb spelling, emitted by the self and by the shared-header copies; bytes equal to the self's copy) — an NF320 Q2 exception.
  Rows with a local `CCPPointerInfo` (0x00DA6740, 0x00DC8030, 0x00DDE940, 0x00E33B00) must also drop it for the header's.
- **D3.** 0x004AB170 (the ??_G row) takes the same one-helper form as the self (measured: `inline` alone drops the vftable, ??_G and
  ??1 from its object; FABLE_FORCEINLINE alone the same; inline + helper keeps ??_G equal to retail). A second scaffolding helper
  outside the two selfs: accept?
- **D4.** The copy self keeps its local model. Its object already emits, pick-any under real names, a 1-slot ??_7CScriptThing@@6B@ and
  a ??_GCScriptThing@@UAEPAXI@Z with 2 relocations (the real row's has 4), plus ??0CBase / ??_7CBase / ??_GCBase /
  ??0CCountedPointer_CScriptThing stand-ins; all unchanged. Moving it onto engine/CScriptThing.h would replace that vftable (stop rule).
  Accept the helper-only change, or make the model port part of this unit?
- **D5.** The five Codex-reserved rows 0x00CFCE90, 0x00D90290, 0x00E4FA20, 0x00E96110, 0x00F00C90 were not compiled (0x00DDB720 is
  covered by NF326's compile). May they be compiled read-only for the census?
- **D6.** 0x00904540's dtor copy is byte-equal but binds the stand-in ??1CBase@@UAE@XZ at 0x0099A2E0 (14 census objects reference
  ??1CBase@@UAE@XZ; NAME_FIXES.tsv). Respell in this unit or queue?

## Report only (outside the unit)
- 108 census objects emit a pick-any ??_7CScriptThing@@6B@ with a slot count other than retail's 79 (1, 2, 4, 7, 8, 19, 21, 22, 27, 28,
  29, 31, 34, 43, 69, 76, 77, 78): truncated local-model vftables under the real name. Same hazard class as D1 for the vftable.
- operator= ??4CScriptThing@@QAEAAV0@ABV0@@Z: 75 duplicate lines in the same link (strong self 0x008AB980 v copies): a third unit of
  this shape.
- 0x004AB170 has no `// Leaf:` line (vland: STANDIN_LEAF / "graded the largest function"; the largest is ??_G on both sides).

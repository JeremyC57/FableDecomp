# NF331 unit: CScriptThing::operator= (??4CScriptThing@@QAEAAV0@ABV0@@Z, self 0x008AB980), T3 `nf328 0x008AB980 unit`
**Status: READY TO LAND (one row).** The census finds exactly one strong definer, the self. The self becomes `inline` plus one `?A0x`
helper in NF328's landed device; no other row changes. 20 pre-existing pick-any copies differ from retail: DEBT (NF326 Q2 / NF328 D1),
listed in DEBT.tsv. Nothing in this change adds or alters a copy, a vftable, a ctor or a `??_G` / `??_E`.

## Member
| member | self | PDB spelling (Ego_r + FableWin) | form |
|---|---|---|---|
| CScriptThing::operator=(const CScriptThing&) | 0x008AB980 | ??4CScriptThing@@QAEAAV0@ABV0@@Z | `inline` on the row's existing out-of-class definition (engine/CScriptThing.h, FABLE_RETAIL_NSCRIPT) + ONE `?A0x` helper `ReferenceScriptThingAssignment` (an assignment through the member, `#pragma inline_depth(0)` over that one function, restored right after, below the row's own code, `// Inline-budget: scaffolding, not retail` line); Leaf made the full symbol |

## Census (by object; tools/srccensus_op.py -> tools/hcensus.py -> tools/census_op.py -> tools/census_report_op.py)
- **Text gate** (work/srccensus_op.tsv): 458 landed rows whose comment-stripped text can make the object define the member: an out-of-class
  `CScriptThing::operator=` body (156 inline / 26 FABLE_FORCEINLINE / 1 strong = the self), an in-class `operator=(const CScriptThing&` body
  (15, all in local models), a local `class|struct CScriptThing` model (240), FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT (35), or a folded
  `../src/compiled` include of such a source. Checks (greps): no header under rebuild/include defines the member (engine/CScriptThing.h only
  declares it on its FABLE_RETAIL_NSCRIPT model) or defines FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT; no other header models CScriptThing;
  no catalog CompilerFlags names a CSCRIPTTHING macro; no source or test includes the self's file; every file under rebuild/src or
  rebuild/include with an out-of-class `CScriptThing::operator=(` text (183) is a gate row; **all 76 operator= definers of NF328's census link
  (its 75 duplicate lines + the self) are inside the gate**. Reasoning, not a compiled result: engine/CScriptThing.h's flattened default
  model is a packed POD of three words, whose assignment is trivial and never emitted, so rows on that default are outside the gate.
- **Re-scan control** (a 1-in-10 SAMPLE, not the full set): 415 other landed sources name `CScriptThing` and `operator=` but have no defining construct; 1 in 10 of
  them (42, minus 2 stay-off rows = 40) were compiled: 38 neither define nor reference the member, 2 only reference it (UND). No definer.
- **Compiled at HEAD with VC7.1:** 457 gate rows + 40 sample rows = 497 objects, all graded (15 MATCH / 482 RELOCATION_MATCH).
  **Hole: 0x00E97070** (B-claimed, COORDINATION_CLAIMS 2026-11-20; not compiled). Its text defines the member out of class as
  `FABLE_FORCEINLINE` after the shared header, so any copy it emits is pick-any (selection 2), never strong (decision D1).
- **Before** (CENSUS.tsv, evidence/objcensus_head.tsv): 91 objects define the member. Strong (selection 1): **1, the self 0x008AB980**.
  Pick-any equal to Fable.exe at 0x008AB980 (relocation fields zeroed, and every relocation binds the self's symbol `??3@YAXPAX@Z` at
  0x00BFE9BC): 70. **Pick-any differing: 20** (DEBT.tsv). 16 objects only reference it; 390 neither.
- **After** (CENSUS_after.tsv, evidence/objcensus_after.tsv): 0 strong definers; 71 equal (the self + 70); the same 20 differing.

## The change (1 row) and its proof
| row | change | pair grade HEAD / new (one work path) | objsec (tools/uproof.py) | vland --recheck-landed H / N |
|---|---|---|---|---|
| 0x008AB980 | `inline` + helper, full Leaf, comment lines | RELOCATION_MATCH / RELOCATION_MATCH | OK: ??4 selection 1 -> 2, bytes and relocations equal HEAD and Fable.exe (74 bytes, `??3@YAXPAX@Z` at 0x00BFE9BC); only new DEF = the helper; UND set, `.drectve` and every other non-debug section equal (the pre-existing pick-any CCountedPointer<CScriptThing> operator= / ShareData / ReleaseData COMDATs unchanged); no `??0` / `??1` / `??_7` / `??_G` in either object | RELOCATION_MATCH + test PASS both sides, no warnings |
Evidence: evidence/uproof_p2.txt (pair-grade objects), evidence/uproof_p2_vland.txt (the vland recheck objects: same verdict), recheck_final.log,
evidence/pickany_vs_retail.txt (HEAD and new self objects: ShareData 0x007044D0 and CCountedPointer<CScriptThing>::operator= 0x00704580 EQ on
both; the tool skips the row's own address).

**Link** (tools/link2.py, LINK= cleared, /FORCE, /OPT:NOREF, /MAP; all 497 census objects, the self first; evidence/link2_all_*_force.txt,
evidence/link_delta.txt): HEAD 108 duplicate lines, new 18. Removed: exactly 90 x `??4CScriptThing@@QAEAAV0@ABV0@@Z` (= every other
definer); added: none; the other 18 lines are identical (GetDef / LowerBound / GetAllThings instances, Compare_00411570, ...). The map keeps
the self's copy on both sides. Unresolved sets identical (1,761 symbols). No strict (/FORCE:UNRESOLVED) link was run: the 18 pre-existing
duplicate lines would stop it. With the self first the map keeps the retail-equal copy; in another link order a differing copy could win
(Debt).

## Debt (DEBT.tsv; pre-existing, unchanged; NF326 Q2 / NF328 D1 shape)
With every definer pick-any, the linker keeps whichever copy comes first, so **pick-any is not yet sound for this member** while these 20
differ. None is referenced in its own object (no call, vftable slot or leaf relocation to the symbol: DEBT.tsv column 5), none was changed
here, and at HEAD each already sat beside the strong self (LNK2005 / LNK4006): debt, not a hold (EXTRA_RULES 2026-11-21, NF326 Q2, NF328 D1).
- **17 copies, 25 bytes:** the PImp assignment is left as a call to CCountedPointer<CScriptThing>::operator= (retail 0x00704580) where
  retail's 0x008AB980 expands it: out-of-class `inline` / FABLE_FORCEINLINE definitions after the shared header (0x00CE0600, 0x00CF8970,
  0x00CF8A70, 0x00D17470, 0x00D23DC0, 0x00D2D930, 0x00D35090, 0x00D36620, 0x00D374A0, 0x00D52E90, 0x00E2FF00), the implicit-assignment opt-in
  (0x00CFE030, 0x00D1FD30, 0x00D30030, 0x00D41D00), and two local models (0x00D643A0, 0x00D67270).
- **1 copy, 25 bytes, stand-in callee:** 0x00CFD6C0 (local model) calls `??4CCountedPointer@@QAEAAU0@ABU0@@Z`, a stand-in for
  0x00704580 already queued (NAME_FIXES_QUEUE land0611f; tracker split_fixable).
- **2 copies, 29 bytes:** 0x00D6D670, 0x00E12580 (implicit-assignment opt-in) expand to a ShareData call (retail 0x007044D0).
- 0x00CF8970 and 0x00D2D930 are on other T3 lines (edit stay-off). The likely fix shape (not attempted: these rows are not in this unit's
  change and their own leaves are unaffected) is the visible / force-inline CCountedPointer<CScriptThing>::operator= in those TUs.

## Decisions needed
- **D1.** 0x00E97070 (B-claimed) could not be compiled. Its text gives only a FABLE_FORCEINLINE (pick-any) definition, so it cannot be a strong
  definer. Accept the census with this text-only hole, or compile it read-only when B releases it?
- **D2.** Queue the 20 differing copies as T3 `nf331` debt (one line each, or one line for the 17 call-shape copies)?

## Report only
- Pre-existing truncated `??_7CScriptThing@@6B@` emissions (slot counts other than 79) in the census objects: 1 slot x34, 76 x27, 7 x9,
  2 x5, 77 x4, 43 / 28 / 21 x3, 19 / 22 / 4 / 78 x2, 27 / 29 / 31 / 34 / 69 / 8 x1 (NF328 reported the same family).

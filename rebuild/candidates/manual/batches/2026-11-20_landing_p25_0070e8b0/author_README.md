# 0x0070E8B0 CAnimateThing::Update (p25A)

**Grade:** RELOCATION_MATCH (881/881). The strict vland run passes (LANDVERIFY_STRICT_HEADERS=1, overlay `hdr/`; `vland.log`), and so does the real-object behaviour test.
- **Mutants:** 30 of 31 killed. The survivor is equivalent: SetAlpha's upper clamp cannot fire in this row (see Test).
- **Oracle:** 881 bytes, the same as hand2c.
- **Flags:** none.

## Source

`src/0070e8b0.cpp` replaces the hand280 draft `UpdateFrameLogic`. The draft used local models of every class and now regrades DIFFER(893v881); the catalog had 882v881. The new source uses shared headers only: engine/CAnimateThing.h, CAnimSet.h, CAnimateThingUpdate.h, CThing.h, NGraphicAppearance_CTCGraphicAppearance.h, GFLimit.h and GFIsNonZero.h, plus the STLport containers.

**Identity:** `?Update@CAnimateThing@@QAEXABVCAnimateThingUpdate@@@Z` (Ego_r.pdb / FableWin.pdb). The shared header already names 0x0070E8B0.

**Structure:** follows Ego_d.pdb: the locals with their CodeView types and frame offsets (`egod_locals_all.py`), and the C13 line table `egod_lines/0070e8b0.tsv`. The full line-by-line reading is in the source header. Points that decide bytes:

| Ego_d evidence | Source | Without it |
|---|---|---|
| `anim` is type 0x212B8 = `CAnimateThing::CCompleteAnimation&`, not `CAnimSet&`. Line 221 is 9 B (`anim = CurrentAnim`, this at disp32). Line 223 is 22 B (the first list read through `anim`), line 224 is 28 B (`DelayedAnim` through `this`). | `CCompleteAnimation& anim = CurrentAnim;` | `this` and update / trans_anim swap esi and edi: 881v881 with 34 hunks. The same swap appears as a CAnimSet& to the main set, through GetMainAnim(), or with no local. Swap-normalised, the stream is identical. |
| `c1` is a `long` in its own block, and the function-level `trans_it` is a list iterator. | Loop 2 is `for (long c1 = 0; trans_it != end(); ++c1, ++trans_it)` with `new_trans_alphas[c1]`. | A walking `vector<float>::iterator` swaps esi and edi again (34 hunks). |
| Line 229 is 25 B; retail clamps in memory (0x0070E8FC..0x0070E924). | `GFLimit(main_alpha, 0.0f, 1.0f)`, the shared template. An if/else gives the same bytes. | n/a |
| Line 250 is 31 B, then 251 GFLimit, 253 push_back. | new_alpha | n/a |

**Accessor exceptions.** Two reads are spelled as member reads although Ego_d's line sizes fit accessors (CAnimateThing is CAnimSet's friend):
- `main_alpha = anim.MainAnimSet.CurrentAlpha + ChangeAlphaBy`. With `GetAlpha()`, VC7.1 loads +2Ch first; retail loads +8Ch first (2 instructions; GOTCHAS wf4 0x00ADDBE0).
- `trans_anim.ChangeAlphaBy` in new_alpha. `GetChangeAlphaBy()` gives DIFFER(884v881): an `fld / fsub st(1)` spill.

`GetAlpha()` stays in the erase test and in new_alpha, where it is byte-neutral.

## Inline budget (line 2) and ablation

There is no flag and no pragma, and no forceinline or noinline device. One header opt-in makes a body visible: it gives `CAnimSet::SetAlpha` its plain `inline` definition. The class already declares SetAlpha under FABLE_RETAIL_CANIMSET_STRAFE_ACCESSORS_PASS24. Retail expands it twice (0x0070E92C, 0x0070EB10) and keeps the out-of-line copy at 0x0070BFC0.

| change from the matching source | grade |
|---|---|
| SetAlpha declaration only (inline definition removed from the overlay) | DIFFER(793v881) |
| `/d2inlT50` / `/d2inlT100` / `/d2inlT300` | DIFFER(833 / 996 / 1112 v881). The default budget matches. |
| anim as `CAnimSet&` to the main set (a3) | DIFFER(881v881), esi/edi swap |
| loop 2 with a walking float iterator (k1) | DIFFER(881v881), esi/edi swap |
| main alpha via `GetAlpha()` (either operand order) | DIFFER(881v881), `fld`/`fadd` operands swapped |
| new_alpha via `GetChangeAlphaBy()` | DIFFER(884v881) |
| `new_trans_alphas[c1] * multiply` (k2); if/else instead of GFLimit (k3) | RELOCATION_MATCH (no change) |

## Header overlay (`hdr/`, `hdr.diff`)

The overlay is add-only: 38 `+` lines, 0 `-` lines; pin_asserts pinned 0.

**engine/CAnimSet.h**, opt-in `FABLE_RETAIL_CANIMSET_ANIMATETHING_UPDATE`. Needs TYPE + STRAFE_ACCESSORS_PASS24. It adds:
- a forward declaration of CAnimateThingUpdate;
- FableWin `HasHadUpdate() const` (inline; `test byte [set+51h],8` at 0x0070E9F6);
- the private declarations `Update(const CAnimateThingUpdate&)` (Ego_r/FableWin AAE, 0x0070D030) and `UpdateInternalAlphas()` (0x0070C2C0);
- SetAlpha's inline definition after the class. The body is the one the landed 0x0070BFC0 row writes.

**engine/CAnimateThing.h**, opt-in `FABLE_RETAIL_CANIMATETHING_UPDATE_CALLEES`. It adds the private declarations, with their Ego_r/FableWin spellings:
- `UpdateCombinationAnimList(std::list<CAnimSet>&, const CAnimateThingUpdate&)` (0x0070D390);
- `PreloadAnims(const CCompleteAnimation&)` (0x0070C590).

**Existing opt-ins the row also uses:**
- FABLE_RETAIL_STLP_LIST (erase / pop_front);
- FABLE_RETAIL_CTHING (GetTCGraphicAppearance);
- FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL + FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_UPDATE (the guards around the shared `SetAsChanged(unsigned long)` declaration);
- FABLE_RETAIL_CANIMATETHING_MAINANIM;
- FABLE_RETAIL_CANIMSET_TYPE;
- FABLE_RETAIL_CANIMSET_STRAFE_ACCESSORS_PASS24.

`newly_flagged_rows.tsv` lists five landed rows of the TU that model CAnimSet / CAnimateThing locally: 0x0070BFC0, 0x0070D030, 0x0070C2C0, 0x0070D390, 0x0070C590. They are re-port candidates; their bytes don't change.

## Test (`test/0070e8b0.cpp`, links the real row object)

The test links the row together with the TU's own STLport COMDATs:
- list<CAnimSet> clear / insert / _M_insert_dispatch;
- list<CDelayedAnimation>::insert;
- vector<float>::_M_insert_overflow / _M_clear.

**Fixtures** use the native layouts: CAnimSet 0x54, CAnimateThing 0xBC, STLport list nodes.

**Stubs** record every out-of-line call and its argument:
- UpdateCombinationAnimList and CAnimSet::Update. On the main set, Update advances the percentage to 0.75, so the test sees that the transition update is built after it.
- UpdateInternalAlphas, PreloadAnims, SetAsChanged, and the CAnimSet copy constructor.
- DELTA_NOTIONAL_ZERO = 1e-4.

**Runs** (6):
1. Seven transition sets. Four are dropped: alpha ≤ ε, a bad second index, a bad first index, and an index/alpha count mismatch. The three survivors have weights 0.5 / clamped-to-0 / clamped-to-1, so the multiplier is 0.2.
2. Main clamps to 1, and the total is ≤ ε, so the multiplier is 1.
3. Main clamps to 0 with no transitions.
4. Main clamps to 0 with two weight-0.5 sets. The remaining alpha is 1, not 1.4.
5. A 0.6 / 0.6 pair with main 0.4.
6. Main 0 with one weight-0.5 set: the product is 1.

**Checks per run:**
- main CurrentAlpha / LastAlpha;
- the surviving list order;
- each survivor's CurrentAlpha / LastAlpha / HadUpdate;
- the exact call order, with each transition update's {time, distance, the main set's updated percentage};
- the queue roll: old second at the front, and a copy of DelayedAnim's two sets, in order, at the back;
- stamp + 1;
- PreloadAnims(this), then SetAsChanged(2) on the thing's graphic appearance.

**Mutants:** `mutants/0070e8b0.tsv` holds 28 row-source and 3 overlay one-decision mutations; 30 are killed.
- The survivor removes SetAlpha's `> 1` clamp, which is unreachable here. The main alpha is GFLimit-ed first. Each transition's product `w * (1 - main) / total` is at most 1, and when total ≤ ε it equals w ≤ ε.

## vland warning (labels DB)

`RELOC_NAME`: DELTA_NOTIONAL_ZERO 0x0129BA3C is labelled `?g_NormaliseEpsilon@@3MB` (see NAME_FIXES).

## NAME_FIXES

- **0x0070E8B0:** catalog UpdateFrameLogic becomes `?Update@CAnimateThing@@QAEXABVCAnimateThingUpdate@@@Z`.
- **0x0129BA3C:** labels DB `g_NormaliseEpsilon` becomes `?DELTA_NOTIONAL_ZERO@@3MB`. Evidence: Ego_d.pdb plus the shared headers.

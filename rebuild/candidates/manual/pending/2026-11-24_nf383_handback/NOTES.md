# NF383 hand-back: identity research on two-addresses-one-name rows (no-ICF ruling)

## Scope
- NF378's 18 skipped-conflict rows (`agentNF378/STATUS.tsv`).
- NF380 q5: 26 addresses, "measure, then rule".
- Left alone:
  - NF380 `unknown(T3)` rows 00448380 00513700 00675230 006755a0 008899c0 00889f00. They are T3_PORT_QUEUE rows.
  - 0044c57d. It sits in NF381's q1 family, so it is recorded only.

## Files
- `RULINGS.tsv` (44 rows): the verdict for every target, the right name per address, the action, and the sources.
- `STATUS.tsv`: one line per address.
- Run dirs:
  - `batches/b1`: 6 ACCEPT.
  - `batches/b3`: 3 ACCEPT.
  - `batches/b2`: 5 HOLD, needs R1.
  - Each holds rows/, orig/, head/payload.json, RESULTS.tsv, UNITS.tsv, TABLE_CHANGES.tsv and evidence/.
  - b1 and b3 also hold SELF_SYMBOLS.tsv, plus tests/ with TESTS.tsv (respelt tests; the HEAD tests are in orig/tests/).
  - b1 also holds hdr/engine/CJoystick.h, orig/hdr_CJoystick.h and HEADERS.tsv.
- `RENAMES.tsv`: every old->new symbol, including the COMDAT co-renames.
- `SELF_SYMBOLS.tsv`: proposals for the 8 respelt selves.
- `NAME_FIXES.tsv` (29 rows), `QUEUE_PROPOSALS.tsv` (n1-n9), `evidence/`, `tools/`.
  - `tools/write_handback.py` regenerates RULINGS, STATUS, RENAMES, NAME_FIXES and QUEUE.

## Method and proof directions
- **Caller test.** `tools/fwc.py` and `tools/cand.py` take each retail call site and look at the FableWin caller body (debug exe, ILT resolved): which candidate spelling does it name?
  - FableWin is built /Od with no inlining.
  - So a caller that names exactly one candidate proves that call site.
- **CArray<T> : vector<T> pairs.** Only a vector-context caller proves vector. A CArray-context caller proves nothing, because the wrapper inlines into the vector call.
- **~CCountedPointer = { ReleaseData(); }.** Proven by a ReleaseData-context caller, or by the shape of the pair: a 44-byte release with a 5-byte `jmp` thunk beside it.
- **Supporting sources:**
  - RTTI COL and vftable slots;
  - object membership, order and size in FableWin and Ego_r (`tools/sheet.py`, `evidence/q5_pdb_presence.tsv`);
  - table binders;
  - object placement in retail, anchored on a non-template, single-object function.
- **Ego_r is secondary only.**
  - It is a later build with ICF and Dinkumware STL (`_Uninit_move` etc.), so its mangled names can be the folded pick.
  - Example: lib_3d_mesh_stats.obj lists `??1?$pair@J...` under the demangled `pair<unsigned long,...>`.
  - So Ego_r never breaks a tie here. Its "size N = retail N" lines are corroboration only.

## Landed work

### b1: ACCEPT (6)
- **00429d8e** = `~CCountedPointer<NUISystem::CList>`.
  - It is the 5-byte `jmp ReleaseData` at 00429288. This is the documented CComponent/CAction pattern.
  - Built with `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY` plus an explicit dtor instantiation.
- **009e42c0 / 00a66c00 / 00a66860** = `ClearAllMaintainedEvents` of CJoystick / CMouse / CKeyboard.
  - Binder 009f5540 (`CInputManager::ClearEvents`) is respelt to match.
  - Mouse and keyboard use their existing `FABLE_RETAIL_CMOUSE` / `FABLE_RETAIL_CKEYBOARD` branches.
  - CJoystick gets a new additive opt-in, `FABLE_RETAIL_CJOYSTICK_MAINTAINEDEVENTS_LIST`.
    - pin_asserts: pinned 0.
    - equiv_check: 0 differing over 41 closure rows; the control flagged 28.
- **006f0810** = `vector<C3DVector>` copy ctor. The local STLport model's class keys were wrong; with them fixed, the allocator now mangles in.
- **Table changes:**
  - delete `??1<CList> 00429288`, add `??1<CList> 00429d8e`;
  - three replaces `Clear -> ClearAllMaintainedEvents`.
- **Gates:**
  - apply_table dry: 0 misses; multi-target 1 -> 1.
  - Link: duplicates 3 -> 0, unresolved 7 -> 3; nothing HEAD resolved is left unresolved.
  - The pvr NEWLY_NEQ on 00429d8e comes from the old table line and clears with TABLE_CHANGES (evidence/pvr_table_after.txt).
- **Moved out of b1:** the stale-line delete `??0C3DAnimationBlendState 00c59ba0`. 0x00C59BA0 is the T3 nf348x row, which is research-only, so it is queued as n8 instead.

### b3: ACCEPT (3)
- **The rows:**
  - 0050e500: `fill<unsigned short*>` (MATCH).
  - 00a326d0: `~pair<unsigned long,C3DMeshVolume>`, together with the helper DestroyIndexedMeshVolume.
  - 00a335e0: `fill<pair<unsigned long,...>*>`.
- **Proof, caller chains:**
  - 005149f0 is `vector<unsigned short>::_M_fill_insert`. Its FableWin callers (Sprintf, `NSpeechGainManager::Initialise`) call `vector<unsigned short>::resize`.
  - `C3DMeshStats::LoadBinary` -> `resize<pair K>` -> `_M_fill_insert` 00a35010.
- **Proof, object anchor** (evidence/proof_b3_object_anchor.txt):
  - Both pair addresses sit between `C3DMeshStats::LoadBinary` 0x00A2F2D0 and the `C3DMeshStats` scalar deleting dtor 0x00A353F0.
  - FableWin lib_3d_mesh_stats.obj defines 144 K and 0 J pair functions.
  - The J family is 0x00A6FE00-0x00A73C40, ending at `CVectorMap<long,C3DMeshVolume>` Sort/Update beside the CPhysicsMesh members.
- **Gates:** objsec ALL_EQ, pk GATE PASS, reach flips 0, link 0 duplicates, no table change, apply_table dry 0 misses.

### Tests (b1 and b3)
- The landed tests modelled the old identities: `CMemoryDataOutputStream::Clear`, `Device::Clear`, `pair<long>`, `fill<short>`.
- They are respelt to the new identities. PassPatterns are unchanged.
- New rows plus new tests: all RELOCATION_MATCH/MATCH PASS (evidence/vland_tests_new.log).

### b2: HOLD (5 binders: 0064f93f 0064fa0f 0064fa77 0064fadf 00458833)
- **Change:** each binder opts into `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA`. Its call then binds ReleaseData at the 44-byte addresses 00654b4f/94/d9, 00654c1e and 0044f3b2 (EQ).
- **Thunk check:** the dtor COMDAT the binder now emits is a 5-byte `jmp`. It is EQ to the retail thunks 0065536c/71/76/7b and 00450195 (pk --addrs, evidence/b2_measure_thunks.txt).
- **Why HOLD:** objsec is NOT_EQ by design, because that pick-any COMDAT is re-bodied (evidence/b2_cmpfn.txt).
- **Other gates:** with its 5 table replaces, pk GATE PASS, 0 flips, link 0 duplicates, apply_table dry 0 misses.

## Rulings needed
- **R1 (b2).** Should an objsec NOT_EQ that only re-bodies a pick-any COMDAT toward retail (measured EQ to the retail thunk) block ACCEPT? If it should not, b2 lands as is.
- **R2 (q5 vector-vs-wrapper unknowns).**
  - CArray pairs: 00515ad0 005295b0 00a3cdd0 00a46790 00a5e0a0 00a5e2e0 00a87090 00ae5d80 00f3b640.
  - Wrapper classes: 006b6840 00a49c60 009f6a70.
  - In every case the retail body is the full vector body, and the only caller context is CArray or the wrapper.
  - Question: does "full vector body plus a 28-byte forwarding wrapper" prove vector? I did not apply it.
- **R3 (T3 overlap check).**
  - b1's Clear trio resolves T3 nf292 (row 0x00996B60; the trio appears only in its text column) without touching 00996b60.
  - b3's 0050e500 is mentioned only in T3 nf340's text (row 0x005149F0).
  - Please confirm that is acceptable at landing.
- **Unresolved:**
  - 0043e1c0: unknown, leaning dtor (53-byte /O2 body, no thunk beside it, dtor-context callers only).
  - 006f15e0: swap vs iter_swap; there is no caller context.

## NF378 conflicts in brief
- **Owners:** proven for all 17 non-stay-off rows, each with at least 2 sources (see RULINGS).
- **BSim donors:** rejected at the 0x00Cxxxxx vendor addresses. Those keep their stand-ins.
- **NAME_FIXES:** owners whose selves still carry free-function or `_Methods` stand-ins: 006afcb0, 005eedcf, 00bddd50, 00419270, 00a026f0, 00a02710, 00a01f80, 00a75430.
- **n3 (binding conflicts to fix now):** the vendor selves 00c22040, 00c222b0 and 00c35170 carry engine PDB names.
  - 00c22040 and 00c222b0 carry AddLearnedExpression, which belongs to 005eedcf.
  - 00c35170 carries `??0CBaseIntelligentPointer`, which belongs to 00a01b10.
  - n3 overlaps T3 nf348 and nf372, so that unit is the T3 owner's call. My evidence agrees with nf372.
- **004cb6e0 and 00640460:** their owners elsewhere are proven, but their own identity is not.

## Landing notes
- Stale landed file names: the 00429d8e row is in `Destval...COptimisedPrimitive_00429d8e.cpp`. Also stale: `CMemoryDataOutputStream_Clear_*`, `stdpairlong*` and `std_fillshortshort_*`.
- Stale catalog labels: the vland STANDIN_LEAF warning on the trio is the catalog label (`CMemoryDataOutputStream::Clear`). Rename the files and labels at landing if wanted (n9).
- 0056ad40 still has a local 53-byte `~CCountedPointer<CList>` body (n6). Its relocation already maps to 00429d8e.
- About 93 of the 120 compiles were used. Nothing was written into the repo and no git was run.

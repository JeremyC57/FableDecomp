# acctC re-cut rows — note for the account that lands this lane (2026-11-14)

Two reserved rows were not function starts. Both are fixed on this branch the way the repo already does it
(`rebuild/candidates/manual/BOUNDS_FIX.tsv` for false starts, `rebuild/manifest/boundary-starts.tsv` +
`tools/decomp_pipeline/crawl/manifest_add_boundaries.py --write` for real starts the manifest lacks).

## 1. 0x00564D5D -> 0x00564D50 `std::char_traits<char>::copy` (42 bytes) — MATCHED, staged in `2026-11-13_acctC_c40b/`

Evidence (retail bytes): int3 pad up to 0x00564D4F; 0x00564D50 `mov ecx,[esp+0Ch]; test ecx,ecx; jne 0x00564D5D;
mov eax,[esp+4]; ret` (0x00564D5C); 0x00564D5D `push esi ...` rep movsd / rep movsb `... ret` (0x00564D79); int3 pad.
The old row 0x00564D5D (catalogued `_Copy_impl<CAppearanceModifierGraphics::CEntry*,...>`, 29 bytes of code, 723 in the
catalog) is the `n != 0` arm of that function: it has no prologue of its own and is reached only by the `jne`.

What this branch changed (three one-line additions, nothing else in those files moved):
- `rebuild/manifest/boundary-starts.tsv`: + row `00564d50 std::char_traits<char> copy 42` with the evidence above.
- `rebuild/manifest/functions.tsv`: + row `00564d50` (`agent_verdict=discovered`, `agent_source=boundary`), written by
  `manifest_add_boundaries.py --write` (not by hand).
- `rebuild/candidates/manual/BOUNDS_FIX.tsv`: + row `00564d5d ... MIDSTART skip` so chunk cutters stop offering the fragment.

For the lander:
- If `rebuild/manifest/functions.tsv` conflicts when this branch is merged, take the target branch's file and rerun
  `python3 tools/decomp_pipeline/crawl/manifest_add_boundaries.py --write` (it re-adds 00564d50 from boundary-starts.tsv;
  addresses already present are skipped). Do not hand-merge the row.
- The stale manifest/catalog row 0x00564D5D itself is NOT removed by this branch (removing rows is the lander's call; the
  existing tool is `verify_and_land.py`'s fragment prune, which takes a report of fragment addresses). It is unlanded, so
  nothing compiled depends on it.
- Land `2026-11-13_acctC_c40b/` like any other row; before the manifest row existed vland said OUTSIDE_MANIFEST, with it the
  row grades MATCH / PASS (`c40b/coordverify.log`).

## 2. 0x00537E70 -> 0x005379F5 `NUISystem::CList::ComputeChildrenVisibility` (2,102 bytes) — NOT matched, back in the open pool

Evidence: 0x00537E70 is `lea edx,[ebp-20h]` in the middle of an instruction sequence (the preceding instructions at
0x00537E65..6E load the vtable it then calls through); the enclosing function starts at 0x005379F5 (`push ebp;
lea ebp,[esp-78h]; sub esp,0ECh`, right after the previous function's `ret 0Ch` at 0x005379F2) and runs 2,102 bytes to
0x0053822B. Name: CList vtable 0x012462E4 slot 185, Ego_r + FableWin `?ComputeChildrenVisibility@CList@NUISystem@@MAEXXZ`,
landed ScrollToFirstChild row.

Nothing needed changing in the shared files: `BOUNDS_FIX.tsv` already lists 00537e70 as `MIDSTART skip` (inside 005379f5)
and the manifest already has 005379f5 (gapscan, name ComputeChildrenVisibility). The mistake was in this lane's own
reservation list, which had taken the fragment. Fixed there only:
- `2026-11-13_acctC/RESERVED.tsv` and `STATUS.tsv`: row 00537e70 replaced by 005379f5 (2,102 bytes), state OPEN, closest
  earlier source `pending/2026-11-03_linux_hand726/differ/005379f5.cpp` (DIFFER 2126v2102).
For the lander: nothing to land for this one yet. If another lane also holds 005379f5, this lane's claim dates from
2026-11-14; the stale catalog row 0x00537E70 is, as above, left for the lander to prune.

## Also seen (catalog lengths only; no re-cut needed)
0x00A3E830 and 0x00A3E520 (CSoundManagerLH) are catalogued at 752 / 736 bytes but are 24-byte functions; both are matched at
their true length in `2026-11-13_acctC_c40a/`.

# Vendor naming wave V1 (2026-11-04): libpk, GameCODA, DirectShow strmbase

Owner request 2026-11-04: "start a naming pass for vendor / other code like libpk".
- **In scope:**
  - libpk (Lionhead audio, `d:\dev\audio\ver_fable_1.5_b\libpk\`), 0xC08AE0..0xC57C90 and 0xC5E180..0xC89000;
  - GameCODA (Sensaura), 0xC57C90..0xC5E180;
  - strmbase (DirectShow BaseClasses), 0xCA4000..0xCB2000.
- **Out of scope** (true third-party): Vorbis/Ogg, Miles, Bink and zlib.
- **Process:** one agent built the decisions and an overlay in scratch (`scripts/`, brief `scripts/BRIEF_VENDOR.md`). The coordinator
  applied it with `../2026-11-04_wave35c/scripts/apply_w35c.py` and re-graded from the working tree.

## Result
- **Decisions** (`DECISIONS_V1.tsv`, 1,376 multi-spelled or placeholder targets): 48 proven (all strmbase), 1,063 placeholder
  (one address = one spelling), 265 deferred, 0 ICF.
  - The spellings were counted by compiling all 5,286 landed definers and callers. `reloc_bindings.tsv` keeps only the first binder
    of each symbol, so it understates the conflicts.
- **198 rows respelled:**
  - **22 strmbase definers.** They were rewritten on `engine/CBaseVideoRenderer.h` from the DirectShow BaseClasses source (the DX9-era
    SDK, UNICODE, cross-checked with the Win7 sample text), and renamed to `<Class>_<Method>_<addr>.cpp` (`RENAMES.tsv`).
  - **176 automatic respells.** One was accepted only if its graded bytes are identical and every changed relocation names the
    target's decided spelling. 152 of them are callers (`CALLERS.tsv`).
  - **Re-grade from the working tree:** 199 / 199, counting the header's other includer 0x00CA5850. 24 MATCH, 175 RELOCATION_MATCH,
    all PASS, no RELOC_CONFLICT (`logs/regrade_worktree_a.log`, `_b.log`). The RELOC_NAME warnings left are the ones the landed
    objects already had.
- **Header:**
  - The agent's `CBaseVideoRenderer.h` added the strmbase override declarations on the default path (hdrcheck2 `default=DIFF`).
  - The coordinator moved them behind the new sub-macro `FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES`, which the 22 rows define.
  - Now `default=same` and `FABLE_RETAIL_CBASEVIDEORENDERER=same`.
- **Labels:** `ghidra_out/labels_vendor_v1_2026-11-04.tsv`, 75 strmbase names. Each has two sources: the BaseClasses declaration
  order and the retail vtable slot (38 catalog / 39 manifest renames).
  - They replace misattributed propagated labels. For example, CVideoPlayer_00ca9a90 is
    CDynamicOutputPin::Active/Inactive/DeliverBeginFlush/DeliverEndFlush, and CAppearance_00caddc0::Rebuild is
    CRendererPosPassThru::ResetMediaTime.
- **Bindings:**
  - 79 ADD applied.
  - The 98 RETIRE candidates are not applied (wave 35 precedent).
  - `bindings_multi_check.py` prints 0, and `sync_manifest_from_catalog.py --dry-run` prints 0 / 0 / 0.

## Not applied (`DEFERRED.tsv`, `QUEUE_V2.tsv`): owner decisions are listed in the coordinator's report
- **47 rows that pass the grade but add a RELOC_NAME warning:** the decided placeholder replaces a propagated (wrong) catalog label.
- **0x00CACAF0 should be `??1CMyBaseObject@@QAE@XZ`** (RTTI: Fable's strmbase renamed CBaseObject). 0x00CAA790 and 0x00CAAAC0 bind
  the engine's `??1CBaseObject@@QAE@XZ` 0x0099A430. FILTER_STATE should carry strmif.h's `_FilterState` tag (0x00CA6670).
- **The 6 libpk rows held from chunks 769/770** (`HELD.tsv`). Their structural class names stand, but the method names have one
  source each (PROPOSAL rows in `NAME_FIXES.tsv`).
- **QUEUE_V2.tsv:**
  - 1,397 rows the automatic respell could not do (mostly a parameter type or class key differs);
  - 265 deferred decisions;
  - 42 strmbase label respells;
  - 18 definer moves.

# Chunk 716 header hand-back

## fable_ime.h (non-engine header; overlay path hdr/fable_ime.h)
- **Change:** new opt-in branch `#ifdef FABLE_RETAIL_CIME` ... `#else` (the existing text, unchanged) `#endif`, placed around the
  `namespace NUISystem { class CIME ... }` block; the trailing `FABLE_STATIC_ASSERT(sizeof(NUISystem::CIME) == 0x54)` stays
  outside and holds for both branches. The default path is byte-for-byte the repo text (diffed against the live repo copy at
  hand-back; the repo file did not change during the chunk).
- **Retail branch:** `IMEUI_APPEARANCE` (ImeUi.h, 0x40, Ego_r.pdb member names/offsets, class key U) and `NUISystem::CIME`
  with the Ego_r.pdb members (EnableIME +0, WindowHandle +4, IsInitialised +8, Appearance +0xC, PEditBox +0x4C,
  CancelOnOKClick +0x50; 0x54), the inline ctor `CIME() { ResetToDefaultSettings(); }` and dtor `~CIME() { Shutdown(); }`,
  and the PDB members Get (SI, 0x00404A80), ResetToDefaultSettings (SI, 0x00404A90), LoadIMESettings (SI, 0x00405350),
  Initialise (0x00405650), Shutdown (0x004051C0), private Reset.
- **Evidence (two sources each):** members/offsets: Ego_r.pdb CIME layout + retail Shutdown 0x004051C0 (`[esi+8]` test,
  `[esi+4]` window, `[esi+1Ch]` symbolFont) and ResetToDefaultSettings 0x00404A90 (stores from gIME+0xC). Ctor/dtor:
  FableWin.pdb ??0CIME@NUISystem@@QAE@XZ / ??1CIME@NUISystem@@QAE@XZ (debug, out of line), retail gIME initializer 0x01219E90
  (`call ResetToDefaultSettings`, no member store) and its atexit thunk 0x012288B0 (`mov ecx,offset gIME ; jmp Shutdown`); both
  graded RELOCATION_MATCH from this branch. Member manglings: Ego_r.pdb + FableWin.pdb name sets (SI = static __fastcall).
- **Used by:** src/01219e90.cpp (`#define FABLE_RETAIL_CIME`). Overlay use-check: without FABLE_HDR_OVERLAY the row emits no
  `_$E1` (OBJDUMP_ERR), with it RELOCATION_MATCH.
- **Includers rechecked (default path, overlay active):** INCLUDERS_fable_ime.txt; `vland.py --recheck-landed` 0x00405350
  LoadIMESettings and 0x00405650 Initialise RELOCATION_MATCH + PASS (tmp/recheck_fable_ime.log). recheck.py skipped
  rebuild/src/compiled/00/40/CIME_LoadIMESettings_00405350.cpp: a second landed source for the same address 0x00405350
  (review at landing: duplicate file).
- **Follow-up (header debt, not done here):** the landed CIME rows (Get 0x00404A80, Shutdown 0x004051C0, ResetToDefaultSettings
  0x00404A90, Initialise, LoadIMESettings) keep local models / stand-in globals (`g_primitiveVTableOrData`,
  `_g_FableImeEnabled_013B7CD8`); they could move onto this branch and `gIME`.

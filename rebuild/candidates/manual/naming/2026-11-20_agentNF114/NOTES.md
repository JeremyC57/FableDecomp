# NF114 notes (2026-11-20, round 10: PRIORITY slice 2 remainder, round 2: NF112's 13 entries + the CUIScreenUnitConverter statics)

Handed back early (context), as the brief asks. Counts come from APPLIED.tsv / STATUS.tsv.

**Entries (18): 3 done (q11634 q11637 q11638), 0 done-already, 0 rejected, 4 blocked (q4447 port; q6212 / q11635 / q11636 authoring:
one Leaf-less row each), 0 deferred, 11 open** (q4449 q4452 q4454 q4524 q4525 q4645 q4744 q4975 q5002 q5080 q5205; APPLIED notes
carry the probe findings).

**Rows: 8 ported / respelled (rows/: 7 respells + 1 port, all RELOCATION_MATCH + PASS), 2 not-a-binder (0041cc14 0041cc2b, regraded at HEAD), 3 blocked** (00516200 port
after 3 forms, DIFFER(423v423); 00644627 and 0040f0e0 Leaf-less, left alone). recheck_final.log: one vland --recheck-landed run over the
8 rows in rows/.

## Probe
work/probe3.tsv: every landed REFS row of q4449 q4524 q4452 compiled once at HEAD (248 lines) with the symbol it binds at the entry
address (NF112's probe.py). The other entries use NF112's work/probe.tsv / probe2.tsv (taken before the nf112 landing; nf112 changed
none of their rows except the statics family, re-read from the landed sources).

## Statics (q11634..q11638)
- Spelling (two sources, settled at the nf112 landing): Ego_d.pdb PUB32 `?IsUsingRelativeCoords@CUIScreenUnitConverter@NUISystem@@0_NA`,
  `?BackBufferWidth@...@@0MA`, `?BackBufferHeight@...@@0MA`, `?DefaultBackBufferWidth@...@@0MA`, `?DefaultBackBufferHeight@...@@0MA` +
  FableWin.pdb LF_STMEMBER attrs = private. engine/NUISystem_CUIScreenUnitConverter.h FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS
  declares exactly these.
- Respelled without declaring the statics public:
  - 00531ec0 0060eeb0 0061e6d0 (on the shared header): `#define FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE` -> `_ORIGINAL_DIMENSIONS`. Their
    data reads are inside the class's own inline ConvertVector / ConvertX / ConvertY (member context). The overlay adds the
    ORIGINAL_DIMENSIONS arms those two inline per-axis bodies lacked (hdr/README.md).
  - 004299a8 0052e530 0052e580 0052f5c0 (pre-existing local CUIScreenUnitConverter models; the code is a member of that class or calls its
    inline members): in-place respelling, `private:` around the static data and ScreenWidth / ScreenHeight -> DefaultBackBufferWidth /
    Height (NF44 round-1 "access-only fixes: wrap the local declaration in private:"; round-9 rule: in-place respelling inside a local model
    the row already had). 0052f5c0's two local inline helpers ConvertWidth / ConvertHeight now carry the PDB names ConvertX / ConvertY
    (Ego_r.pdb / FableWin.pdb ?ConvertX@CUIScreenUnitConverter@NUISystem@@SIMM@Z; inline, no symbol; bodies unchanged).
  - 00543ee0 (CParticleEffect::Draw, not a member): the invented NUISystem externs g_bScreenSizeOverride / g_OverrideScreenWidth / Height /
    g_DisplayWidth / Height and the local ScaleScreenX / ScaleScreenY helpers replaced by the header's public inline ConvertX / ConvertY
    (ORIGINAL_DIMENSIONS + CONVERTXY_INLINE); RELOCATION_MATCH in one form.
- Every REFS row of 0x013B8768 / 0x01375CD4 / 0x01375CD8 now binds the @@0 spelling. 0x013B876C / 0x013B8770 keep one old binder: 0040f0e0
  (ResetDimensions), a Leaf-less July row (invented free NUISystem_CUIScreenUnitConverter_ResetDimensions writing ?g_dimW@@3MA /
  ?g_dimH@@3MA through a local Device / Dims model). Left alone (EXTRA_RULES); its re-author is the header's RESETDIMENSIONS member
  ?ResetDimensions@CUIScreenUnitConverter@NUISystem@@SIXXZ (Ego_r.pdb), a self rename with callers: authoring.
- No split created: every respelled row moves onto the spelling the landed 0041cc14 / 0041cc2b / 0052e470 / 0052e4d0 / 0054ef00 already
  bind; among the exe-scan REFS rows the @@2 / ScreenWidth / ScreenHeight / g_Override* / g_Display* / g_bScreenSizeOverride spellings are
  gone (0040f0e0's g_dimW / g_dimH stay). The shared header's FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE block still declares the statics
  public (@@2, ScreenWidth / ScreenHeight) and seven landed rows still define INLINE (00622a00 00603000 0060c900 005379f5 00537776 005ff190
  0061b610); none of them references the data in retail, so they bind nothing there today. Making that block private / renaming it is a
  rename of shared-header members: **needs header claim** (it unblocks nothing in the binder sense; it removes the last public @@2 text).
- Seen in passing (not in my slice, not changed): 0041e3f6 (CManager::CManager) declares 0x004299A8 as a FREE function
  `void __fastcall SetIsUsingRelativeCoords(bool)` while the self row 004299a8 defines ?SetIsUsingRelativeCoords@CUIScreenUnitConverter@NUISystem@@SIX_N@Z:
  a pre-existing split at 0x004299A8 (NAME_FIXES.tsv).
- Other rows define FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE (0054ef00's twins 00622a00 00603000 0060c900 005379f5 00537776 005ff190
  0061b610) but reference none of the five data words in retail (not in the exe-scan REFS); they call the out-of-line getters /
  ConvertVector. Left alone. Settling the INLINE block's public @@2 names (a rename of shared-header members) stays a header item.

## q4447 (0x00516200 self row, no binders)
Blocked after 3 forms at DIFFER(423v423) (STATUS.tsv has each form). The two new devices that took it from 130 to 423 bytes:
a CFilter_Bind3.h opt-in FABLE_RETAIL_CFILTER_BIND3_CALL_FORCEINLINE (operator() FABLE_FORCEINLINE; precedent fable_string.h
FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ; draft in work/hdr_drafts/, not handed back) and the existing
FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE. Last residue: a register swap in the expanded C2DBoxI area copy. Inline-control note: the
forced Bind3 operator() is an EXTRA_RULES preference-3 device and would need an `// Inline-budget:` line if it lands.

## Flags lines
None added (no row needed flags its build entry lacks).

## Descriptive names
None new. 0052f5c0's helpers took PDB names (ConvertX / ConvertY). The test files are unchanged (self-contained copies; 00543ee0_test
still models the old globals locally, test-only).

## Pre-existing local models in handed-back rows (verifier WARN SHARED_HEADER_AVAILABLE, not added by me)
004299a8 0052e530 0052e580 0052f5c0 keep their local CUIScreenUnitConverter model (respelled in place; no base list); 0052e530 0052e580
0052f5c0 00543ee0 local C2DVector; 0052f5c0 / 00543ee0 local CManager; 00543ee0 local CRightHandedSet / CParticleEmitter /
CParticleEmitterManager. No base-list model was added; 00543ee0's `class CParticleEffect : public CChangingStateComponent` is pre-existing.

## Locks
Taken outside rows.txt (claim.sh, all free, none handed off / reserved for acctB in UNMATCHED_ROWS.tsv): 0052e530 0052e580 00531ec0 0060eeb0 0061e6d0.
Self rows of rows.txt touched: 004299a8 0052f5c0 00543ee0 (respelled), 0040f0e0 00516200 (blocked, unchanged).

## EH funclets
No landed funclet in rebuild/src/compiled/01/21 includes any changed row file (grep by file name).

## Header overlay
hdr/engine/NUISystem_CUIScreenUnitConverter.h (hdr/README.md).

## Rule slips
- Two Bash calls ended non-zero: a `wc` over a mistyped path list (orientation) and a mkpay + JSON one-liner that raised AttributeError on the payload's shape; no compile was involved in either.
- One `sleep 60` call was refused by the harness (no effect).

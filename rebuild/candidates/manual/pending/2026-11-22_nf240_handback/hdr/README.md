# NF240 header overlay (2026-11-22)

## engine/CEngineInternalPrimitiveBase.h
- Base: the repo working tree (`orig/engine/CEngineInternalPrimitiveBase.h`, identical to the tree when I started).
- New macro `FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTRENDERFRAME`, at the top of the generated (default, `#else`) class branch:
  `void SetAsRendered()` (FableWin.pdb / Ego_d.pdb ?SetAsRendered@CEngineInternalPrimitiveBase@@QAEXXZ, the same text as the PASS27
  block), `static void FABLE_FASTCALL IncrementFrameCounter()` (FableWin.pdb / Ego_d.pdb ?IncrementFrameCounter@...@@SIXXZ; retail
  FrameStart 0x00B3271B `inc word ptr [0x0143B8D8]`), and `private: static unsigned short CurrentRenderFrame;` (all three PDBs; the inline copies the rows emit are EQ to retail's out-of-line
  0x00B31D30 / 0x00B91620,
  ?CurrentRenderFrame@CEngineInternalPrimitiveBase@@0GA). Static members and inline functions only: no layout change.
  The ATTACHED and CLASS branches are not touched (the ATTACHED branch has the same members under PASS27).
- Used by rows 0x00B32660 and 0x00B92C20.
- The default path is token-identical (the block is inside a new `#ifdef`). pin_asserts pinned 0: every assert of the generated branch
  below the insertion is already `FABLE_STATIC_ASSERT_AT`.
- Includers: 130 landed sources / tests include the header directly (116 sources); 14 headers include it directly, 38 directly or transitively; 220 rows
  have a source or test in that closure. None defines the new macro.
- Equivalence: `equiv_check.py` over two include-tree copies (repo tree v repo tree + this header), on those 220 rows' sources and
  tests: 0 differing (work/equiv.tsv empty, work/equiv.log). assert_names_check (old tree v new tree): 0 mismatches.
- No other naming agent overlays this header (NF241 has no hdr/).

## engine/NCreatureMode_CModeSitFloor.h
- Base: the repo working tree (`orig/engine/NCreatureMode_CModeSitFloor.h`, nf238's ALLOC opt-in included).
- The existing line `virtual ~CModeSitBench();` (inside the PASS27 class) now sits under `#ifndef FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_IMPLICIT_DTOR`;
  with the new macro defined the class has no user-declared destructor (the implicit one is still the virtual
  ??1CModeSitBench@NCreatureMode@@UAE@XZ, FableWin.pdb 0x024B9E50), so the vtable emitter's ??_G expands ~Filter + ~CModeSitBase in place
  as retail 0x007400E0 does (`call 0x00739DB0 ; call 0x008D71D0`). The CModeGuildmasterPowerUpShield PASS22_IMPLICIT_DTOR pattern.
  Used by row 0x00740040 only. No layout change.
- Default path token-identical. pin_asserts pinned 0 (the asserts below are all _AT already).
- Includers: 22 landed rows (sources / tests); no header includes it. None defines the new macro.
- Equivalence: equiv_check (tree with both NF240 headers v repo tree) over the 22 rows: 0 differing (work/equiv2.tsv empty);
  assert_names_check 0 mismatches.

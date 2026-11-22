# NF233 header overlay (2026-11-22): four add-only, member-specific opt-ins (entry 1, the CNewFrontendGameComponent vtable / ctor / dtor family)

Base copies: `orig/engine/{CNewFrontendGameComponent,CGameComponent,CStopWatch,CEngineInterpolationInfo}.h` (the repo tree with nf232 landed).
Other overlays: `ls $S/w22/*/hdr/engine/<h>`: CNewFrontendGameComponent.h only in older NF47 / NF76 / NF229 / NF230 copies (landed since; NF231's
equals the repo), CGameComponent.h in NF228 (= repo) and the old retry11a; neither of the other two anywhere. `grep -rlwE` of the four new
macros over rebuild/ and every `$S/w22/*/hdr`: hits only in this hand-back. Default paths are token-identical (every change is inside a new
`#ifdef`, with the old text as its `#else`). No layout change (the pinned size asserts hold in every mode built).

- `FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_CTOR_DTOR` (engine/CNewFrontendGameComponent.h; needs _RUN and _FILENAMES_FILETIMES): types the
  members the ctor builds / the dtor destroys as Ego_r.pdb (struct_layouts_egor.tsv) at the retail offsets: CurrentMovement
  `NGameComponentHelpers::ECursorMovement` +0x18 (the enum's namespace from the Ego_r / FableWin mangled ?ChangeSelection@CFrontendGameComponent@@AAE_NW4ECursorMovement@NGameComponentHelpers@@@Z;
  declared empty, as landed 0x004963A0, no enumerator name in the extracts), m_bInitMainGame `CMainGameComponentInit` +0x2C, PAVITextureShow /
  PAVITextureDecode `CTexture*` +0x88 / +0x8C, AVISrcBox / AVIDstBox `C2DBoxI` +0x90 / +0xA0, AVIPlaying / XMVCodeLoaded +0xB0 / +0xB1,
  m_LastInterp `CEngineInterpolationInfoSet` +0xCC; declares the ctor `(CGame&, const CNewFrontendGameComponentInit&)`, the virtual dtor,
  `virtual void Init()` (slot 1 on the RUN path), and the CDeviceResetCallback overrides OnPreDeviceReset / OnPostDeviceReset. Spellings:
  Ego_r.pdb, FableWin.pdb and Ego_d.pdb all carry ??0CNewFrontendGameComponent@@QAE@AAVCGame@@ABVCNewFrontendGameComponentInit@@@Z,
  ??1CNewFrontendGameComponent@@UAE@XZ, ?Init@..@@UAEXXZ, ?OnPreDeviceReset@..@@UAEXXZ, ?OnPostDeviceReset@..@@UAE_NXZ; retail RTTI vtables
  0x01230CA0 (5 slots) / 0x01230C94 (2 slots) hold 0x0042F75E / 0x0042DC5F / 0x0042DC60. Includes engine/CMainGameComponentInit.h,
  engine/C2DBoxI.h, engine/CEngineInterpolationInfo.h under the macro.
- `FABLE_RETAIL_CGAMECOMPONENT_DTOR_INLINE` (engine/CGameComponent.h): `virtual ~CGameComponent() {}` in place of the out-of-line declaration.
  ??1CGameComponent@@UAE@XZ is in FableWin.pdb and Ego_d.pdb but not Ego_r.pdb (inline in release); retail expands it in 0x0042F50B
  (0x0042F594..: the two vtable stores, tail jump to ~CBaseClassNonCopyable 0x0099A300) and 0x00415940 (??_GCGameComponent). Plain inline
  (no __forceinline): measured at stock flags, it expands in the size TU 0x0042F50B.
- `FABLE_RETAIL_CSTOPWATCH_CTOR` (engine/CStopWatch.h): `CStopWatch();` declaration (??0CStopWatch@@QAE@XZ in all three PDBs, public;
  out of line at 0x0062F740, called three times by 0x0042EA8F).
- `FABLE_RETAIL_CENGINEINTERPOLATIONINFOSET_CTOR` (engine/CEngineInterpolationInfo.h): the inline default ctor zeroing the three floats
  (Ego_d.pdb + FableWin.pdb ??0CEngineInterpolationInfoSet@CEngineInterpolationInfo@@QAE@XZ, none in Ego_r: inline; retail 0x0042EB41..0x0042EB58
  three `fldz ; fstp`). The PDBs NEST the class in CEngineInterpolationInfo; the header's top-level spelling is kept (header item, NOTES).

Users: rows/0042f50b.cpp (CTOR_DTOR, DTOR_INLINE, STOPWATCH_CTOR + RUN / FILENAMES_FILETIMES / SETQUIT), rows/0042ea8f.cpp (CTOR_DTOR,
STOPWATCH_CTOR, CENGINEINTERPOLATIONINFOSET_CTOR + RUN / FILENAMES_FILETIMES / SETQUIT), rows/0042dc5c / 0042dc5f / 0042dc60.cpp (CTOR_DTOR + RUN /
FILENAMES_FILETIMES), rows/0051d8e0.cpp (DTOR_INLINE). Each includes the overlaid headers itself, first (NF213 lesson).

Proof (work/):
- pin_asserts.py rebuild/include hdr: CGameComponent.h 0, CNewFrontendGameComponent.h 0 (both already pinned: AT(22), AT(89)),
  CStopWatch.h 1 (its plain FABLE_STATIC_ASSERT became FABLE_STATIC_ASSERT_AT(22, ...)), CEngineInterpolationInfo.h 1 (AT(24)).
- assert_names_check.py rebuild/include hdr: mismatches 0.
- equiv_check.py rebuild/include v work/t/include (= rebuild/include + this overlay, after pinning; `work/t/src` links rebuild/src so the funclet
  rows' `../src/...` includes resolve) over ALL 57,022 landed rows (sources + tests): 0 differing (work/equiv2.tsv empty; the first pass's
  1,355 lines were all cpp-error-new on that relative include, rerun with the link). So no landed includer (44 / 8 / 0 / 5 direct includers of
  the four headers) changes.

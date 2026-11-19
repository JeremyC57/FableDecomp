# Audit h3c b24 / b28 / b29 / b30 / b31 (2026-11-19)
All grades strict (LANDVERIFY_STRICT_HEADERS=1) unless noted; oracle bytes re-read from Fable.exe (all 5 equal). Logs: logs/, fixed_*/vland_strict.log.

## Rows
- b24 0x00C41FB0 CCoda3DSource::Commit: RM PASS (orig hdr and rebased hdr). Identity: CODAApi.lib Release Coda3DSource.obj (1622 B, 1271 non-reloc diffs -> no library grade) + CODAApiDbg CodeView proc. Locals = CodeView exactly (pObject, pListener, f, v; {vDirection, vRight, pNearestListener, fDistance}; {pEnvironment, fGain, fLevel, env}). Calc* + sal_gainToLevel RM at own addresses in this TU. SDK release COMDATs byte-identical (reloc-masked) to retail: CalcDirectionAndDistance, CalcDistanceGain, CalcDoppler, CalcEnvFxGain, sal_gainToLevel, sal_ftoi (0x00C3FF70); CalcConeGain differs (125).
  FAIL hdr/engine/CCodaVector.h stale (drops NF95 ORTHOGONALRIGHT_OUTOFLINE; landed 0x00C40370 SRC_FAIL C2084). Fixed: fixed_b24/hdr/engine/CCodaVector.h.
  Ablation: only CalcDoppler + CalcConeGain bodies byte-required.
  REVIEW: purity.py grades b24 `genuine` even with overlay (sal_ftoi reaches libpk/FtoL.h only through SalMath.h inline: documented purity limitation) while retail runs 4 asm fistp sites.
- b28 0x0063F4B0: RM PASS (WARN STANDIN_LEAF = catalog label truncated; NAME_FIXES). Text == landed 0x0063F7C0 except Bind2 type/includes. Ego_d locals of the instance == shared EGOD body. Mutants 5: 4 killed, 1 equivalent survivor (new_layer > -> >=).
- b29 0x008A8260: RM PASS, no warnings. Locals = Ego_d (names, types, blocks). Mutants 5/5 killed.
- b30 0x00B51AD0: RM PASS (RELOC_NAME GPCamera, NAME_FIXES). Locals = Ego_d. Symmetric loop DIFFER 965v969 (shape byte-required); GetBoxCoord after BuildBox (Ego_d order) RM. Mutants 6/6 killed.
- b31 0x00B53F60: RM PASS, retail_asm. Window confirmed T93 DIFFER 358 (GetVolumeTexture out of line), T94 RM, T422 RM, T423 DIFFER 798 (UpdateBoxPositions expanded); noflag 358. Inline-budget residue: yes.
  Locals FAIL (minor): old_mipmap_delay / old_reuse_busy / index declared const; Ego_d types 0x22 / 0x30 / 0x22 (not const). noconst RM.
  Alternatives: Ego_d source order (FrameStart first) RM; bank-first DIFFER 411v411 (x2); range test inside GetVolumeTexture DIFFER 358 without flag; dropping either callee body DIFFER 416.
  Mutants 7/7 killed.

## One text (b31 / b30 / 0x00B52990)
- Literal landed 0x00B52990 text in b31: SRC_FAIL C2011 (own class CEngineWeatherRenderer) + C2065 FreezeBoxes/BoxSize.
- Landed statements, members mapped onto Settings.*: FrameStart RM, 0x00B52990 RM. Plus the landed #pragma optimize("y",off): FrameStart DIFFER 403v411 (pragma leaks), UBP still RM -> pragma redundant.
- Author respelling at 0x00B52990: RM (no flags = landed catalog flags) and RM at T94; BuildBox at 0x00B51AD0 RM both.
- b31 BuildBoxPositionMatrix + GetBoxCoord text == b30 lines 33-64 byte for byte.
- Fixed: fixed_b31/src (const dropped, Fidelity lines, Ego_d order, b30 block verbatim), owner re-port fixed_b31/owner_00b52990 (RM PASS strict, 6/6 mutants killed), alt fixed_b31/alt_landed_statements_00b53f60.cpp.

## Headers
hdrcheck2 default=same for all; all additive vs today except b24 CCodaVector.h (fixed). Includer regrades: see NOTES.md.

## Second pass (after advisor review)
- CTextureManager.h: all 19 landed includers regraded (00b2c460, 00b3cb30 strict HEADER_POLICY, identical with overlay off; non-strict RM PASS on and off).
- CCodaObject.h: the 7,663 count is textual (C3DVector.h -> CodaEnvironment.h under opt-ins). The opt-in-aware CODA set is 20 rows; all 20 were regraded with fixed_b24/hdr: 19 MATCH/RM PASS, 01214fe0 DIFFER(282v21) pre-existing.
- fixed_b31 rebuilt on the AUTHOR's order (const dropped, exact Fidelity lines). Window T93 DIFFER 358 / T94 RM / T422 RM / T423 DIFFER 798. UBP RM at 0x00B52990, BBM RM at 0x00B51AD0 (no flags and T94). Strict vland RM PASS. Owner UBP text and b30 BuildBox/GetBoxCoord text are verbatim in it.
  The Ego_d-order candidate and the mapped-landed-statements variant are moved to probes/ (graded by q.py only).
- fixed_b24: Fidelity lines exact (extra note on separate line); SalMath.h sal_gainToLevel(const float fGain) (CodeView 0x1259 const float); strict RM PASS; 0x00C40F50 RM.
- b24 CodeView types resolved: Listeners/Environments const CCodaObjectList&, pObject CCodaObject*, pListener/pNearestListener CCodaListener*, v/vDirection/vRight CCodaVector, pEnvironment CCodaEnvironment*, env const _CENVIRONMENT&: match.
- b28 functor: Ego_d + FableWin ??RCThingFilter_OverlappingBoundingBox@@QBE_NABVCThing@@@Z (const CThing&); b28 (and landed 0x0063F7C0) use const CThingPhysical&. fixed_b28 (src + test) strict RM PASS.
- Landed 0x00B52990 object defines ?UpdateBoxPositions@CEngineWeatherRenderer@@QAEXXZ (public), PDB AAE (objdump -t).

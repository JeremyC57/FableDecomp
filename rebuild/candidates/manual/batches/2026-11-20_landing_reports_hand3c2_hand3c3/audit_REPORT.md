# Audit hand3c2 b8..b12, hand3c3 b1..b2, and the re-port sets e17, navedge, setvs, stockdisplay, hermite, leaftype -- 2026-11-19/20, Account B
Repo = today's working tree, read-only (no git, no repo writes). Strict = LANDVERIFY_STRICT_HEADERS=1. Tools: $AW/tools (prior audits' set +
xrefs.py: E8/E9 and absolute references in Fable.exe mapped to catalog rows; objrelocs.py: relocations of the leaf section; odlines2.py: VC7.1
/Od per-line sizes; egod_loopsurvey.py). Each fixed_<addr>/ holds src, test, payload.json, oracle.tsv, hdr + hdr_*.diff, NAME_FIXES.tsv,
mutants.tsv/.log, vland_strict.log.

## b8 0x00B99300 CEnginePrimitiveManagerRepeatedStaticMeshes::ProcessLightingSW -- FAIL as delivered (names; invented friend); fixed_00b99300 PASS
- Oracle 871 B == Fable.exe (`ret 1Ch`, int3 to 0x00B99670). Author text + author overlay: strict RELOCATION_MATCH PASS (re-run by me).
- FAIL 1 (made-up names where the original exists). The two debug switches are NOT nameless: Ego_d.pdb holds two S_LDATA32 records `aa` and `bb`
  (type 0x74 = int) for Engine_Primitive_Manager_Repeated_Meshes.obj, section 4 offsets 582556 / 582560 (procmod.tsv file offsets 111591996 /
  111592016, repeated in the linker module; the only `aa` / `bb` in the PDB and this module's only data statics). The author's search used the
  decorated-name tables, which cannot hold an undecorated static. Retail: 0x0143B954 / 0x0143B958 are adjacent, read only by this function
  (0x00B9944C / 0x00B99438) and never written; the neighbours 0x0143B950 / 0x0143B95C belong to other TUs. Which is which: VC7.1 lays file statics
  out in declaration order whatever the use order or initialiser (probes s1..s4: aa at .bss+0, bb at .bss+4 in all four) and Ego_d has aa below
  bb, so aa = 0x0143B954 (the red main light, tested second) and bb = 0x0143B958 (count = 16, tested first). Cross-check: the row with real
  `static int aa; static int bb;` is RELOCATION_MATCH and its relocations are +0x138 -> _bb (.bss+4), +0x14C -> _aa (.bss+0) = retail's layout.
  Residual assumption (stated): VC10 also allocates the two in declaration order (its record order and address order agree).
  Fix: `extern int aa; extern int bb;` (the TU-local static modelled extern under the PDB name, as landed 0x013D27B0
  NumberOfPeopleCalculatingRoute), `if (bb) count = 16; if (aa) {...}`. The object binds ?bb@@3HA at +0x138 and ?aa@@3HA at +0x14C.
- FAIL 2 (unsourced construct): FABLE_RETAIL_CSHADERRENDERMANAGER_FRIEND_REPEATEDSTATICMESHES invents a friendship. The class has the PUBLIC
  inline `void SetVertexShaderConstantsDirect(unsigned long index, const void* data, unsigned long count)` (already declared in the shared header;
  FableWin.pdb ?SetVertexShaderConstantsDirect@CShaderRenderManager@@QAEXKPBXK@Z 0x02E8EB50; Ego_d GPROC32, 33 debug bytes on one line =
  three arguments forwarded to one call). Retail keeps its unreferenced COMDAT copy at 0x00B9C060 = `jmp 0x0098B930`, the only E9 to
  SetVSConstants in the image, inside this TU's COMDAT run. Fix: the friend opt-in is dropped; new opt-in
  FABLE_RETAIL_CSHADERRENDERMANAGER_SETVERTEXSHADERCONSTANTSDIRECT_INLINE gives the wrapper its body (appended at the end of the header: no
  assert moves) and the row calls it. The object still binds ?SetVSConstants@CShaderRenderManager@@AAEXJPBXK@Z (the PDB's private spelling) and
  emits the wrapper's 5-byte COMDAT = retail 0x00B9C060 (0 non-relocation differences). Without the opt-in the row is still RELOCATION_MATCH but
  binds the wrapper out of line (wrong symbol for 0x0098B930), so the opt-in is required.
- Fidelity fix 3 (byte-neutral, measured): the light array is `primitive.PeekLights()`. Ego_d emits PeekLights (21 debug bytes, header line 103,
  `const CEngineLocalLightArray& () const`) and PeekRenderInfo (19, line 101, `const CEngineRenderInfoRepeatedStaticMesh* () const`) right after
  ProcessLightingSW (its first user in the module); FableWin has both mangled names exactly; retail keeps both unreferenced COMDAT copies after
  GetRenderInfo 0x00B98B90 (Ego_d line 100) in class order: 0x00B98BA0 `lea eax,[ecx+0A4h] ; ret`, 0x00B98BB0 `mov eax,[ecx+0A4h] ; ret`.
  New nested opt-in FABLE_RETAIL_CENGINEINTERNALPRIMITIVEREPEATEDSTATICMESH_PEEKLIGHTS (17 asserts pinned); the row object's two COMDATs are
  byte-identical to retail's.
- Fidelity fix 4: the `n` loop is braced (Ego_d line 289, 2 B = its back jump).
- Statement structure (GOTCHAS last line): the FIXED text compiled /Od with VC7.1 gives the same 30 line records as Ego_d
  (fixed_00b99300/odlines_fixed_vs_egod.txt). 21 lines are byte-for-byte the Ego_d sizes, including all four lighting lines (71 / 74 / 56 / 51)
  and both upload lines (49); the author's text had 29 lines with those six off by one. The nine others differ only by VC10 code generation:
  the C3DVector lines +9 (= +3 per float argument), Initialise +13 (three zero constants +4, 1.0f +1: the author's "four float pushes + one
  call" is right), the `for` headers +9 (far exit) / +6 (near exit; the same pair of sizes is common across Ego_d: 39 / 35 / 32 / 29 / 26
  families in a scan of 102,130 line blocks -- I can not reproduce VC10 here, so the 3-byte far/near difference stays unexplained).
- Locals / scopes == Ego_d (types: aligned_count long, use_simd const bool 0x3D55, obj / n long, pos / normal C3DVector). The Ego_d RECORD order
  (use_simd before aligned_count) is not declaration order: Ego_d's local records are in hash order (calibrated on ConvertSamplingRate), so the
  line table decides (247 = 12 B aligned_count, 252 = 9 B use_simd): the author's order stands.
- Names: every object symbol is in Ego_r / FableWin / Ego_d (pdbsyms) except ?aa@@3HA / ?bb@@3HA (Ego_d undecorated statics). VECTOR opt-in
  parameter names == the Ego_d records of both batch lighters. The landed RENDERSUB declaration of ProcessLightingSW spells the parameters
  normal_x / normal_y / normal_z / offsets (Ego_d: normals_x / normals_y / normals_z / object_offsets): review item, not changed (not additive).
- Purity genuine; no `// Retail-asm:` needed; no Inferred / Bracket construct. Object: leaf COMDAT-NODUP + 7 COMDAT-ANY, no strong copies.
- Overlay (3 headers, all additive vs today's tree; only pinned asserts change): CEngineLightingManager.h (author's VECTOR opt-in, unchanged),
  CShaderRenderManager.h (inline wrapper opt-in; NO friend), CEngineInternalPrimitiveRepeatedStaticMesh.h (PEEKLIGHTS). Ablation: without VECTOR
  or PEEKLIGHTS SRC_FAIL; without the wrapper opt-in wrong binding (above). hdrcheck2: default same for all three (the nested opt-ins are
  invisible to it), so ppcmp over all 142 landed includers: 142/142 raw-identical. Strict regrade of all 142 with the overlay: 103
  RELOCATION_MATCH + 11 MATCH PASS; 28 HEADER_POLICY (004022b0 009a25d0 009c01c0 009c0e50 009d9a30 00b25c00 00b32230 00b3cb30 00b45fb0 00b46280
  00b46660 00b46a30 00b46b90 00b54310 00b67510 00b76bc0 00b991d0 00b99670 00b99bb0 00ba1b00 00ba6fa0 00ba77b0 00bac0f0 00bad2e0 00bb8090
  00bb8690 00bcb800 00bcfb00) with identical policy lines with the overlay OFF and RELOCATION_MATCH PASS non-strict with it ON: pre-existing.
  No new header, so no new SHARED_HEADER_AVAILABLE trip; the row defines no local class (also no `class X : public B {` form).
- Account A headers: none touched. SetVSConstants re-port (other Account B author): I found no overlay of it on disk to test jointly. With the
  fix there is no friend line to collide with; my block sits at the very end of CShaderRenderManager.h. For that author: Ego_d's SetVSConstants
  parameters are index, data, count (the header says reg) and it has one local, hr.
- Test: links the real row object; I added 64 cases with a negative second switch. Mutants on the ROW SOURCE / overlay (mut.py): author 33
  (respelled) + audit 28 (switch swaps, `> 0` / `== 5` switch tests, 17, override moved before the lighting, Initialise orders, n start /
  bound / index / array, aligned scalar bound, .W, colour arrays, upload data, use_simd = true, PeekLights / PeekRenderInfo / wrapper bodies)
  = 61/61 KILLED.
- Strict vland of the fixed set on today's tree: RELOCATION_MATCH PASS (fixed_00b99300/vland_strict.log).

## e17 ($S/w26/reports/e17, current files incl. the Uninitialise respell) -- 9 re-ports PASS on bytes / behaviour; 1 name FAIL (0x00A67A20 is CVolume::DrawBox) -> fixed_e17 (see also the e17 ADDENDUM below: the widened-policy fix of three rows)
Audited copy: $AW/snap/e17 (== the author's directory when I finished). Landable fixed set: $AW/fixed_e17 (all ten rows; only 0x00A67A20 differs).
- Strict vland on today's tree, overlay hdr/, --recheck-landed: 10/10 (8 RELOCATION_MATCH + 2 MATCH) PASS; 0x009FDE80 alone without
  --recheck-landed: RELOCATION_MATCH PASS (e17/logs/). RELOC_NAME warnings are the NAME_FIXES rows (Swizzle, Unswizzle, Uninitialise,
  WaitWhileResourceIsBusy; 0x009FC440 RemoveFromResourceList is the earlier audit's row).
- 0x009FDE80 recheck: src / test identical to the audited fixed_9fde80; C3DExtentsI.h differs only by the added rebuild_abi.h include;
  CBankFileAsyncData.h / CTextureManager.h blocks unchanged; 17/17 audit mutants KILLED again.
- Same bytes AND same bindings: I compiled the landed text and the re-ported text of each of the nine rows and compared the leaf's ordered
  relocation symbols (e17/obj/*.rel). Identical for eight rows; 0x00A68910 differs only by ?ReleaseVolume@CVolume@@QAEX_N@Z ->
  ?Uninitialise@CVolume@@QAEX_N@Z (twice, target 0x00A67460: the announced respell). No leaf symbol changed.
- Text changes beyond the model move (complete list of leaf-body differences, each tied to a shared declaration):
  0x00A67860 / 0x00A67950 `CPixelFormat format; format.Initialise(f);` -> `CPixelFormat format(f);` (forced: the shared default
  constructor stores FormatIndex(-1), which retail does not); 0x00A67A20 / 0x00A68910 the scoped GetDesc + Initialise block -> the PDB inline
  GetPixelFormat(); 0x00A68910 also GetDimensions() -> GetDimensions(0) (PDB ..@@K@Z) and ESurfacePool_1 -> SURFACE_POOL_MANAGED;
  D3DVOLUME_DESC -> _D3DVOLUME_DESC (the owner's declarations). The five small rows change only .X / .Y for the prefix casts. The README
  table does not list the constructor form for 0x00A67860 / 0x00A67950 (the source headers do).
  Retail confirms the new inline bodies: the rows' COMDAT ?GetPixelFormat@CVolume@@QBE?AVCPixelFormat@@XZ (43 B) and
  ?GetDimensions@CVolume@@QBE?AVC3DExtentsI@@XZ (46 B) are byte-identical to retail 0x00A67480 / 0x00A674B0.
- Copies (GOTCHAS 455): the Lock body in 0x00A67950 / 0x00A67860 / 0x00A67A20 is the re-ported owner 0x00A677D0's body verbatim (483
  characters, programmatic), under the exact Fidelity line, no pragma / noinline; each copy is COMDAT-NODUP 138 B with 0 differences at
  0x00A677D0. The copies are verbatim against the RE-PORTED owner, so the owner must land in the same set (it does).
- NAME FAIL (re-ported row, new `// Leaf:` line): 0x00A67A20 is ?DrawBox@CVolume@@QAEXABVC3DBoxI@@ABVCRGBColour@@@Z, not CVolume::ClearArea.
  Ego_r.pdb (0x00BE8FB0) and FableWin.pdb (0x033302A0) both list DrawBox directly after Clear with exactly this parameter list; Ego_r's run
  Lock, CopyFromMemoryAutoSwizzle, Clear, DrawBox is retail 0x00A677D0, 0x00A67860, 0x00A67950, 0x00A67A20; no PDB has a CVolume::ClearArea
  (the row says its name came from the catalog's "CSurface ClearArea" label). Nothing landed binds 0x00A67A20. fixed_e17: source, test,
  payload name / leaf / target file names and the oracle name respelled; strict RELOCATION_MATCH PASS.
- Other names: all 69 external symbols of the ten objects are in Ego_r / FableWin / Ego_d except that one and a pre-existing inline helper
  ?Volume@CManagedTextureData@@.. in 0x00A699D0 (FableWin has GetVolumeTexture by value; not touched by e17). NAME_FIXES rows agreed
  (0x00A67410 / 20 / 40 / 50 / 60 / 80 follow Ego_r's order Unlock, ReleaseVolume, Swizzle, Unswizzle, [static], Uninitialise, GetPixelFormat
  with equal gaps); + my 0x00A67A20 row.
- Headers: all five changed headers additive (0 non-assert lines removed; CVolumeTexture.h's assert pinned _AT(15)); pin_asserts.py on a copy
  pins 0 more; hdrcheck2 default same (the three nested opt-ins are invisible to it). ppcmp over all 776 landed includers (my includer union ==
  the author's list): 776/776 raw-identical. Strict regrade with the overlay: the 5 opt-in users + the first 168 includers (I stopped my own
  full run there; raw-identical preprocessed text is the proof): 136 RELOCATION_MATCH + 16 MATCH PASS; 21 HEADER_POLICY (00b54310 + 004022b0 004167da 00417418 0041e6d3 0042df9e 0043b570 0047d478 0047eee4 0048f40a 00494900 004a5a40 004b0b90 0055d180 00584a73 005e41e0 005ee288 00634560 00636e90 00644424 006448a4 )
  identical with the overlay OFF and RELOCATION_MATCH PASS non-strict with it ON (00b54310 from the author's control): pre-existing.
- NEW header engine/C3DExtentsI.h: the landed rows that define a local C3DExtentsI are exactly the nine + 0x00A68370
  (`class C3DExtentsI : public C2DExtentsI {`, invisible to the gate's regex), so no SHARED_HEADER_AVAILABLE trip at landing.
  Review: the README's "no local model of a class with a shared header" holds for the regex only. Regex-blind local models that stay
  (all pre-existing): 0x009FD9B0 CBankFileAsync, CBankFileAsyncData, CBaseClassNonCopyable, CManagedTexture, CResourceBank,
  CThreadedFileCallback, CVolumeTexture; 0x00A685E0 CBaseClassNonCopyable, CDisplayManager, CInitBaseClass, CRenderManager,
  CRenderManager2D, CRenderManagerCore, CSystemManager, CVolumeTexture; 0x00A699D0 CVolumeTexture (the README names only CVolumeTexture).
- Tests / mutants (fixed_e17/mutant_logs): row-object tests 0x00A67950 16/16, 0x00A67860 15/15, 0x00A67A20 24/24 (author 10 / 9 / 14 + audit
  6 / 6 / 10). One audit mutant first SURVIVED in 0x00A67A20 (top clamped against the width: the test only had height < width); two cases
  with the extents the other way round added. Model tests (the pasted row body is the row's, checked): 0x00A677D0, 0x00A67530,
  0x00A685E0 10/10 each, 0x009FD9B0 12/12, 0x00A699D0 9/9 (author + audit). 0x00A68910: 6/6 on the model, but that test is an abstract
  model that neither contains nor links the row text (pre-existing), so it cannot see this re-port's body changes: only the bytes vouch.
- Purity genuine x10 (author's pur.py; no asm in any text). Account A headers: none touched.

## b9 0x00646762 NPlayerGui::CStatBar::DrawStatBarOnLeft -- FAIL as delivered (GOTCHAS 455: the same-TU copy is not the landed owner's text); fixed_00646762 PASS (2 rows)
- Oracle 740 B == Fable.exe (`leave ; ret 1Ch`; CStatBar code follows). Author text + author overlay: strict RELOCATION_MATCH PASS (re-run).
- FAIL (the coordinator's check 2): the strong same-TU definition of GFAnchorToBottomLeft carries the exact Fidelity line, but its text is not
  the landed owner 0x00643CDA's: the owner is a local-model row (`C2DVector __fastcall GFAnchorToBottomLeft(...)` inside `namespace NPlayerGui
  { }`, `GetRenderTargetDimensions().Height` on a local C2DExtentsI {Width, Height}), the copy says `FABLE_FASTCALL NPlayerGui::..` and `.Y`
  (the shared C2DExtentsI: X / Y in every PDB). The copy cannot be made verbatim on the shared headers, so the owner moves: fixed_00646762
  holds a re-port of 0x00643CDA onto engine/C2DVector.h / CDisplayManager.h / NPlayerGui_Graphics.h (FABLE_RETAIL_NPLAYERGUI_ANCHORS) whose
  definition IS the copy's text (checked programmatically), RELOCATION_MATCH, strict PASS with --recheck-landed, with a new test that links
  the row object (the landed one was a model) and 7/7 row-source mutants. Land both rows together (repair_remove 00643cda).
  Measured: the copy is byte-required (declaration only: DIFFER(740v740)) and grades 0 non-relocation differences at 0x00643CDA (50 B);
  `#pragma optimize("y", off)` correctly sits after it (the owner has no frame); the TU's `#pragma optimize("s", on)` is line 37.
- Header fix: the author's CFontBank.h edit rewrote `#ifdef FABLE_RETAIL_CFONTBANK_VSTRINGWIDTH` into `#elif defined(..)` (1 line changed).
  fixed overlay is purely additive (the INLINE block before the untouched line; defining both is a redefinition error). Both overlay headers:
  0 lines removed, asserts already pinned (pin_asserts: 0). Opt-in ablation: without VSTRINGWIDTH_INLINE DIFFER(734v740), without
  CSTATBAR_DRAWCHANGETEXT SRC_FAIL.
- Also fixed (byte-neutral, RM): the row re-declared `extern const float DELTA_NOTIONAL_ZERO;`, which engine/C2DVector.h (included) declares.
- Includer regrade (the coordinator's check 2): hdrcheck2 default same; ppcmp 52/52 landed includers of CFontBank.h (47) / NPlayerGui_CStatBar.h
  (6) raw-identical; strict regrade of all 52 with the overlay: 39 RELOCATION_MATCH + 6 MATCH PASS; 7 HEADER_POLICY (00418b7a 00494900 00550010
  00550020 00644424 009a25d0 009a4160) identical with the overlay OFF and RELOCATION_MATCH PASS non-strict with it ON: pre-existing. No new header.
- `// Bracket:` (one line, no Inferred note: the 2026-11-19 rule): confirmed byte-required (without it DIFFER(738v740)).
- `// Retail-asm:` line correct: 0x00646798 `fsub [ebp-0Ch]`, 0x0064679B `fistp [ebp-4]`, constants at 0x0064677D / 0x00646792. Purity retail_asm
  through the shared GFFloatToLongFloor only.
- Locals / scopes == Ego_d (types: tl_pos / text_pos / vtext_pos / change_text_offset / v_text_pos C2DVector, value_text CWideString,
  effect_index long, interpolate float, it the const list iterator, change_text a const reference in both scopes). Line table: the row's 47
  /Od statements map 1:1 onto Ego_d's 48 line records (the 48th is line 7426, a 2-byte jump at the closing brace of the fully-faded-in block).
  Sizes are not comparable in this TU: the source's `#pragma optimize("s", on)` makes even the debug build optimise it (Ego_d stores zero with
  4-byte `and [m],0`), which also explains the author's two "unexplained" lines 7378 / 7381.
- Names: 32 object symbols in the PDB sets; the rest are STLport list internals and CWideString helpers from shared headers (pre-existing
  spellings: ?GetAsCharArray@CWideString@@QBEPBGXZ, ?Sprintf@CWideString@@QAAXPBGZZ and ??H@YI?AVCWideString@@PBGABV0@@Z are in no name
  table -- shared fable_string / GFIntToWideString.h, review item). NAME_FIXES (2 rows) agreed. The row's COMDAT GetVStringWidth differs from
  retail 0x0043E4D0 (19 bytes; a size-optimised pick-any copy, as in b2): no grade impact.
- Mutants (row source / overlay; the test links the row object): author 68 + audit 12 = 80/80 KILLED (1 by timeout: the double iterator
  step). One audit mutant (`Value < -1`) first SURVIVED: the first change text only took 3 / -2 / 0; the test now uses 1 / -1 / 0. One further
  audit mutant was LINK_FAIL (not counted).
- Account A headers: none touched.

## b10 0x00ADF960 CParticleSimulation::SatisfyDistanceConstraints -- PASS on bytes; two fixes built (the max() macro per the coordinator's check; Ego_d type of `limit`) -> fixed_00adf960
- Oracle 766 B == Fable.exe (`pop ebx ; ret 8`). Author text: strict RELOCATION_MATCH PASS, flags /Zi (vland's usual RELOC_UNCHECKED multi-block note).
- The coordinator's check 1 (max macro): the delivered row defines NO macro and includes none: the maximum is the expansion written out by
  hand (`MaximumAdjustment > (dist - constraint.Distance) ? MaximumAdjustment : (dist - constraint.Distance)`), like the shared
  CParticleSystem.h does. Nothing home-made in the row or the test. Where retail's would come from: the Platform SDK's windef.h (through
  windows.h); the lane has no Platform SDK (engine/Win32Types.h says so and is its shim: "Spellings and layouts are the SDK's own"), and
  no shared header defines `max`. Fix built: additive opt-in FABLE_RETAIL_WIN32_MINMAX in engine/Win32Types.h with windef.h's two macros
  verbatim (`#ifndef NOMINMAX` / `#ifndef max` guards as the SDK), the row includes it last and writes
  `MaximumAdjustment = max(MaximumAdjustment, dist - constraint.Distance);`: RELOCATION_MATCH, strict PASS.
  Evidence for the macro text: with the SDK expansion `(((a) > (b)) ? (a) : (b))` the line is 71 bytes under VC7.1 /Od = Ego_d line 461 (71)
  exactly; the hand expansion is 69 (the author explained the 2 by "VC10 adds 2 to float compares"). Alternatives measured: the CRT's
  `__max` (<stdlib.h>, the toolchain's real header, same expansion, no overlay) RELOCATION_MATCH; the clamp through the macro
  (`divisor = max(divisor, limit)`) DIFFER(766v766), so the clamp stays the one-line `if`. Which macro NAME the source used is not
  recorded: labelled `// Inferred:` in the fixed source. DECISION for the coordinator: shim opt-in (built), `__max` (no header change), or
  the author's hand expansion with an Inferred line.
- Fix 2 (Ego_d type, byte-neutral, RM): `limit` is a plain float in Ego_d (type 0x40; const locals carry an LF_MODIFIER, as list_stride
  0x1809 does); the row declared `const float limit`. All other locals / scopes == Ego_d (pos C4DVector*, max_adjust / temp / divisor_limit
  C4DVector, alphas const float*, list_size unsigned long, list_stride const unsigned long; c1 unsigned long; vect / pos1 / pos2 C3DVector,
  constraint const CParticleDistanceConstraint&, five floats; no `k`).
- Statement structure: the fixed text's /Od lines (x87 loop) against Ego_d 447..466: constraint 12, pos1 23, pos2 24, inv_mass1 17, inv_mass2
  18, vect 19, dist 17, divisor 18, error 30, max 71 all EXACT; clamp 25 v 27, pos1 / pos2 updates 40 v 52 (disclosed by the author, still
  unexplained), Initialise 25 / 26 v 29 / 30 (+4 = VC10's zero float argument, as measured in b8), `for` 30 v 39 (VC10 loop header).
- `// Retail-asm:` line: the SSE kernel is the shared GF_SSE_SATISFY_DISTANCE_CONSTRAINTS (purity retail_asm through the header only; I did
  not re-derive the kernel's addresses beyond the oracle compare). `// Inferred:` (operand order in `error`, both orders RM) is labelled honestly.
- Header: Win32Types.h additive (2 asserts pinned). 383 landed includers: ppcmp 383/383 raw-identical; strict regrade of a seeded sample
  of 40 with the overlay: 31 RELOCATION_MATCH + 6 MATCH PASS, 3 HEADER_POLICY (00418b7a 00581fd1 00a5b370) identical with the overlay OFF
  and RELOCATION_MATCH PASS non-strict with it ON. No new header; Account A headers untouched.
- Names: GEnableSIMD (b8's row). Mutants (row source + overlay; the test links the row object): author 34 kept (the 4 on the hand
  expansion replaced) + audit 13 = 47/47 KILLED. One audit mutant (`if (divisor <= 0.0f)`) first SURVIVED: no case had a small positive
  divisor under the limit; one case added (alphas 2^-16, dist 1).

## b11 0x00591B7F CTCInventory::UpdateContextSensitiveItems -- PASS (no source change; fixed_00591b7f = the author's set + audit mutants, a corrected mutants.tsv and a confirmed NAME_FIXES row)
- Oracle 735 B == Fable.exe. Strict RELOCATION_MATCH PASS on today's tree (flags /Gr), 4 RELOC_NAME warnings = the NAME_FIXES rows.
  Purity genuine. No `// Inferred:` / `// Bracket:`; none needed. New widened policy (shared_header_redefinitions with the overlay): [].
- Headers (overlay hdr/engine, diffs hdr_*.diff): CContextSensitiveItemDef.h purely additive (+31 -0; new opt-in
  FABLE_RETAIL_CCONTEXTSENSITIVEITEMDEF_CLASS, three _AT asserts 1032 / 1034 / 1036 inside it). CTCInventory.h +20 -1: the one changed
  line is `#ifdef _MSC_VER` -> `#elif defined(_MSC_VER)` under the new `#ifdef FABLE_RETAIL_CTCINVENTORY_CONTEXTSENSITIVEITEM` (the enum's
  forward declaration cannot be opted into any other way); default path unchanged. pin_asserts: nothing to pin. hdrcheck2 default = same
  for both. 56 landed includers (54 + 2): ppcmp 56/56 raw-identical. Strict regrade with the overlay ON: 43 RELOCATION_MATCH + 6 MATCH
  PASS; 6 HEADER_POLICY (0044c72b 0058301a 00583e26 0058400a 005d9b90 00898940) identical with the overlay OFF and RELOCATION_MATCH PASS
  non-strict with it ON; 1 DIFFER 0x00589A30 CTCInventory::OpenPCMapMenu (549v549), which DIFFERs the same way with the overlay OFF
  = a pre-existing regression of the landed row on today's tree (review item). No new header. Account A headers untouched.
- Locals / scopes / types == Ego_d (things vector<pair<long,long>>, context_things vector<pair<long,CDefPointer<const
  CContextSensitiveItemDef>>>, override_found bool[4], it the non-const vector iterator, pthing_base_def / pcontext_def CDefPointers,
  things_size / i / thing_def_index / slot_index / button_index long). CContextSensitivePriorityCompare : std::binary_function per
  Ego_d class record 0x46FD8 (one LF_BCLASS).
- Statement structure: all 54 Ego_d line records (929..1056) map onto the row's 48 VC7.1 /Od statements in the same order
  (odlines_vc71.txt). Exact sizes: things_size 11 (Ego_d 958, after the zero loop), else 2, button_index 11, `!= -1` 6, `slot_index >= 0`
  10, the Type / ExtraData1 condition 42, override_found init 16, `override_found = true` 8, `if (!override_found[..])` 12. The six
  extra Ego_d records: 944 (12 B, the debug-STL iterator `it` leaving scope: Ego_d's iterators are objects, 933 `for` 110 B and 938
  erase 88 B say the same), 956 / 979 / 1034 (2-byte back jumps on closing-brace lines), 1010 / 1013 (the three-part condition is on
  three source lines; measured as nested ifs under VC7.1 /Od: 25 / 24 / 12 against Ego_d 34 / 29 / 12) and 1056 (epilogue; 1037..1054
  carry no code). One size I cannot explain without Ego_d's bytes: line 962 (the GetDef statement) is 497 B in Ego_d against 34
  here; the row uses the existing GETDEF_GLOBAL_BODY opt-in (146 landed users), so this is recorded, not blocking.
- Names: every callee in the PDB spelling. NAME_FIXES (7 rows incl. header) agreed; the author's tentative row is now CONFIRMED:
  landed 0x00581BDE is `?GetContextSensitivePreferredItemPriority@CTCInventory@@SIJJ@Z` (retail body returns `[eax+8]` = Priority;
  0x00581BFE returns `[eax+4]` = ButtonIndex; Ego_r 0x005EF73E / 0x005EF75E and FableWin 0x0211EC6B / 0x0211ED68 in that order). Today
  two landed files carry the ButtonIndex name (global_GetContextSensitivePreferredItemButtonIndex_00581bde.cpp and _00581bfe.cpp), and
  landed 0x005928A8 binds 0x00581BDE as `?GetContextSensitivePriority_00581bde@@YIJJ@Z` (respell). No other landed row binds
  0x00594ED4, 0x00581BFE, 0x005819F2, 0x00594BE8, 0x00594E15 or 0x00592962, so the set adds no binding split.
- Mutants (row source + overlay; the test links the row object): author 47 + audit 12 = 59/59 KILLED. One author mutant
  (`long things_size = things.size();`, occurrence 1) is stale against the delivered source: its first occurrence is now in the
  header comment (added after the author's mutant run, whose log says KILLED), so it mutated nothing and SURVIVED here; with
  occurrence index 2 it is KILLED (mutants.tsv corrected, mutants_rerun.log).
- Review item: landed 0x00594ED4 (std::sort row) declares `class CContextSensitivePriorityCompare {` without the std::binary_function
  base that Ego_d and this row give it (byte-neutral there: the mangled name is the same).

## navedge ($S/w26/reports/navedge, the final files incl. the `CCountedPointer<CNavRegionConnection>& pconnection` refinement) -- 7 rows PASS on bytes / behaviour; name fixes + the widened-policy fix built -> fixed_navedge (8 rows, 8/8 strict PASS)
Audited copy: $AW/snap/navedge (== the author's directory when I finished). Landable set: $AW/fixed_navedge.
- Author set, strict vland --recheck-landed with its overlay: 7/7 PASS. New rows 0x00A793D0 / 0x00A79680: src / test / CNavConnectorNode.h /
  CNavQuadTree.h identical to the audited $S/stage/audit_h3c2/fixed_00a793d0 (nothing to add to that audit).
- FAIL 1 (names, three places; fixed): (a) engine/CNavRegion.h opt-in: Ego_d.pdb has GPROC32 records for
  CNavRegion::AddConnectionToConnectionList (28 B, parameter `pconnection`) and RemoveConnectionFromConnectionList (parameter
  `pconnection_to_remove`); the overlay spelled other parameter names and its comment called AddConnectionToConnectionList inferred.
  Fixed: `AddConnectionToConnectionList(CCountedPointer<CNavRegionConnection>& pconnection)`,
  `RemoveConnectionFromConnectionList(const CNavRegionConnection* pconnection_to_remove)`; the `friend` stays labelled Inferred.
  (b) rows 0x00A786C0 / 0x00A78060: the row-local CNavRegionConnection constructor parameters are Ego_d's `first_region, second_region`
  (GPROC32 43 B); the comment said "no PDB record". Byte-neutral, both rows RELOCATION_MATCH PASS.
- FAIL 2 (widened shared-header policy, coordinator 2026-11-20): 0x00A78800 defined CNavSwitchableLeafNode locally (plus the CSwitchMap /
  CRegionVector stand-ins with same-TU strong bodies). Fixed: re-ported onto engine/CNavSwitchableLeafNode.h
  (FABLE_RETAIL_CNAVSWITCHABLELEAFNODE; one NEW opt-in FABLE_RETAIL_CNAVSWITCHABLELEAFNODE_SETASBLOCKED declaring the two private
  callees IsEveryUniqueIDOpen / Unlock), the shared CVectorMap<unsigned __int64, bool> (header's noinline LowerBound) and
  std::vector<CNavRegion*> (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE): no stand-in class is left, and the object now binds the real
  instantiations at 0x00A7DF60 / 0x00A814B0. Text rewritten in Ego_d's spelling and scopes (parameters unique_id / blocked; it,
  node_should_be_blocked, regions_connecting_to, neighbours, neighbour_it const / non-const, pneighbour, pmy_old_region, c1 long,
  pmy_region, pswitchable_neighbour, pnon_const_switchable_neighbour; nested `if (node_should_be_blocked != Blocked)`). Strict
  RELOCATION_MATCH PASS. Line table (VC7.1 /Od listing of the fixed text against Ego_d 605..680): the row's 40 code lines map in order
  onto 40 of Ego_d's 41 records; the 41st is line 607 (407 B, a debug-only check between the Find and the store, no retail bytes).
  Exact sizes on 14 lines (pneighbour 8, the three `}` back jumps 2, both `!= region` tests 19, both connect / disconnect calls 23,
  else jump 5, `!empty()` 19, the IsEveryUniqueIDOpen test 15, pnon_const 6, ...). Systematic +3: Ego_d's frame is over 127 bytes, so
  each `this` load is 3 bytes longer (611, 613, 625, 630, 676). Not explained: 645 / 668 (77 / 74 v 18 / 21: Inferred note 2) and 662
  (13 v 6, the pswitchable_neighbour cast).
  - Same-TU copy: retail expands IsEveryUniqueIDOpen at the first site and calls 0x00A773E0 at the second. The copy must be the owner's
    text (GOTCHAS 455), and the landed owner 0x00A773E0 was a view-struct model, so the OWNER IS RE-PORTED TOO (row added: MATCH PASS,
    Ego_d spelling `for (long c1 ..)`; /Od sizes against Ego_d 592..601: 9, 23, 4, 2, 2, 4 exact, the `for` 34 v 40 (VC10's loop
    header); new row-linked test; 7/7 mutants). The copy grades 53 B, 0 differences at 0x00A773E0.
  - FITTED, labelled `// Inferred:` in the source: VC7.1 expands the copy at BOTH sites in this text (both sites flip together between
    /d2inlT71 and /d2inlT72, so there is no threshold that separates them), where retail expands the first and calls the second (and
    Unlock's). Seven natural spellings did not split them (merged / split conditions, early return, non-const receiver, the body after
    the caller, three larger callee bodies). The split is forced with a scoped `#pragma inline_depth(0)` round the second call. The
    earlier text had the same residue in another form (a made-up inline member AllSwitchesOpen() plus the extern declaration); a free
    inline helper in its place is DIFFER(488v494). REVIEW: the mechanism retail's build had is not known. (Ego_d's lines 610 / 663
    are 24 / 15 B, a call's size: I cannot see Ego_d's code, only sizes.) The pragma is not a grouping bracket, so the 2026-11-19
    `// Bracket:` rule does not apply; it is labelled `// Inferred:`.
  - Reverse copy check: the landed Unlock 0x00A77420 (NOT in the set) carries its own copy of IsEveryUniqueIDOpen under
    `#pragma auto_inline(off)`, with no Fidelity line and in a third spelling (`unsigned int i`). It was not the owner's text before
    this set either; it must be re-synced to the re-ported owner when that row is re-ported (it also fails the widened policy).
  - Two more Inferred notes: the C-style casts, and `regions_connecting_to[0]` (Ego_d lines 645 / 668 are 77 / 74 B, more than its
    operator[] lines, so the source may be `*regions_connecting_to.begin()`: same retail bytes).
  - Test rewritten: links the row object, fixtures are a layout twin (the real class has no default constructor), 5 scenarios; the
    row's own IsEveryUniqueIDOpen copy runs for the neighbours.
- Other rows against Ego_d: 0x00A78A90 ctor locals pregion1 / pregion2 and 0x00A780B0 dtor locals neighbour1s_pointer_to_2 /
  neighbour2s_pointer_to_1 / pregion1 / pregion2 as recorded; 0x00A786C0 / 0x00A78060 take Ego_d's `CCountedPointer<CNavRegionConnection>&
  pconnection` bound at the top of the loop body (type 0x354C6; lines 71 / 111). Inferred notes (casts; the block-scoped `connection`)
  are labelled; the two FITTED notes in the new rows are the audited ones.
- Headers: CNavRegion.h +18 -0, CNavNavigableLeafNode.h +13 -0, CNavQuadTree.h +7 -0, CNavSwitchableLeafNode.h +9 -0 (mine), NEW
  CNavConnectorNode.h: all additive, asserts already pinned, hdrcheck2 default = same. 31 landed includers (CNavRegion.h 16,
  CNavNavigableLeafNode.h 16, CNavQuadTree.h 14, CNavSwitchableLeafNode.h 12, CNavConnectorNode.h 0): ppcmp 31/31 raw-identical; strict
  regrade ON 13 RELOCATION_MATCH + 8 MATCH PASS, 10 HEADER_POLICY (00a14e50 00a1c010 00a76c30 00a76c70 00a77420 00a78dd0 00a79240
  00a85560 00a85b90 00a85cc0) identical with the overlay OFF and RELOCATION_MATCH PASS non-strict with it ON = pre-existing.
  NEW header CNavConnectorNode.h: no landed row outside the set is newly tripped (newtrips.py over all landed sources with the
  widened function). shared_header_redefinitions(src, [overlay, rebuild/include]) = [] for all 8 rows. Account A headers: none touched.
- The coordinator's binding check. bindings_multi_check on a scratch merge (landed bindings minus the set's rows, plus the 33 bindings
  of the 8 row objects, fixed_navedge/set_bindings.tsv): 1 (NullCoord) = the baseline. Per address:
  0x00A77710 RemoveConnectionFromConnectionList: bound by landed 0x00A778B0 as `?Release@XResource@@QAEXPAUCFrameData@@@Z` (WRONG:
  0x00A778B0 is really ?RemoveConnection@CNavRegionConnection@@QAEXXZ, Ego_r 0x00BBE510; catalogued as CGraphicInfoFrame::CFrameData::
  ReleaseTexture) and by 0x00A78060 (in the set, now the PDB spelling; its old `?RemoveConnection@CNavRegion@@QAEXPAUSNavConnection_
  00a78060@@@Z` row goes away). Caller to respell: 0x00A778B0 (and rename that row).
  0x00A7C0A0 DeleteData: bound only by 0x00A786C0 (in the set; old `?DeleteNavRegionConnection@@YIXPAX@Z` goes away); the exe's other
  reference is 0x00A7DE26 (uncatalogued). No split.
  0x00A77420 Unlock: bound by the owner itself (AAE, PBV) and 0x00A78800 (in the set; old QAEXPAV spelling goes away). No split.
  Splits the set does NOT cause but touches (other landed callers with a different spelling of a target the set binds correctly):
  0x00A7DB10 resize: landed 0x00A772D0 binds `..QAEXKABQB..` (K; PDB is J); 0x00A770B0 GetNodeAt: landed 0x00A77480 binds
  `..QBEPAVCANavQuadTreeNode@@PBUC2DVector@@J@Z` (PDB: PBV.. / ABVC2DVector); 0x00A7D050: landed 0x00A76A80 binds a global-namespace
  `?find@@YI..` (the set binds std::__find). All in fixed_navedge/NAME_FIXES.tsv (16 rows).
- Mutants (row source; tests link the row objects; fixed_navedge/mutant_logs): 0x00A773E0 7/7, 0x00A78060 7/7, 0x00A786C0 8/8,
  0x00A78A90 8/8, 0x00A780B0 9/10 (survivor `< Size - 1` -> `< Size`: equivalent, a self-swap of the last entry), 0x00A78800 22/23
  (mine; survivor: the connect loop from index 1 -- equivalent, entry 0 is the adopted PRegion, which GetRegion() returns and the loop
  skips), 0x00A793D0 / 0x00A79680 36/37 + 4/5 each (survivors are the `+= 0.0f` control mutants).
- Landing notes: repair_remove needs 00a773e0 added to the author's list; copy five headers (four changed + the new one); drop the
  reloc_bindings rows of the old stand-in spellings (NAME_FIXES).

## e17 ADDENDUM (widened shared-header policy, coordinator 2026-11-20) -- all three flagged rows moved onto the shared headers; fixed_e17 is 10/10 strict PASS and shared_header_redefinitions = [] for all ten rows
Nothing was too large: 0x009FD9B0 moved too. fixed_e17/vland_strict_recheck.log (overlay fixed_e17/hdr, --recheck-landed): 8 RELOCATION_MATCH
+ 2 MATCH, all PASS. The earlier e17 section stands for the other seven rows (and for the DrawBox name fix).
- 0x009FD9B0 CGraphicDataBank::LoadVolumeGraphic (14 local models -> 0): RELOCATION_MATCH PASS. The classes are the shared retail
  headers with the define set of the sibling 0x009FDE80 where it applies (CGraphicDataBank.h FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME declares
  the private member; CBankFileAsyncData.h GetData; CPixelFormat.h GetByteColourDepth out of line; CVolumeTexture.h GetVolumeLevel).
  ONE new opt-in pair: FABLE_RETAIL_CMANAGEDTEXTURE_GETVOLUMETEXTURE in engine/CManagedTexture.h and engine/CManagedTextureData.h
  (FableWin.pdb 0x03124C00 / 0x03124C50 ?GetVolumeTexture@..@@QAE?AV?$CCountedPointer@VCVolumeTexture@@@@XZ, inline in retail; the
  twins of the landed GetTexture opt-in). The function text is unchanged, so the model test is unchanged: author 8/8 + audit 4/4 = the
  earlier 12/12.
  Still local (no shared header exists): CVolume, CVolumeLockedBox.
- 0x00A685E0 CVolumeTexture::Lock (9 -> 0): RELOCATION_MATCH PASS. The manager chain is engine/CSystemManager.h / CDisplayManager.h /
  CRenderManager.h with the define set of the landed 0x009FB750, read through the headers' inline accessors
  `system->GetDisplayManager()->GetRenderManager().WaitWhileResourceIsBusy(..)` (the members are private in the shared classes; same
  bytes). CVolumeTexture is engine/CVolumeTexture.h (the set's own GETASTEXTURELIST opt-in; the inline GetDimensions body is defined in
  the row as 0x00A68910 does). No new opt-in. The model test got the two accessors and the new call line: author 6/6 + audit 4/4 re-run
  = the earlier 10/10.
  Still local: CVolumeLockedBox, IDirect3DVolumeTexture9 and three D3D volume structs.
- 0x00A699D0 CManagedTextureData::CreateVolumeTexture (3 -> 0): RELOCATION_MATCH PASS, and rewritten in Ego_d.pdb's spelling while
  there: parameters width / height / depth / mipmap_levels / format, ONE local rounded_dimensions (the row's x / y / size / volume locals
  are not in Ego_d), the rounded extents assigned back to the width / height parameters (lines 96 / 97), `Allocated = VolumeTexture->
  Initialise(..)` then `return Allocated;` (lines 125 / 127). TWO new opt-ins: FABLE_RETAIL_CMANAGEDTEXTUREDATA_CREATEVOLUMETEXTURE (the
  member's declaration) and FABLE_RETAIL_CVOLUMETEXTURE_INITIALISE in engine/CVolumeTexture.h: the RTTI base CBaseClass (the row's inline
  constructor calls 0x0099A2D0), FableWin.pdb's inline default constructor, Initialise 0x00A68370 and Ego_d's enum EVolumePool
  { VOLUME_POOL_MANAGED = 1, VOLUME_POOL_SYSTEM_MEM = 2, VOLUME_POOL_DEFAULT_LOCATION = 0 } (field list 0x22D8) -- the row had a made-up
  `VolumePoolManaged`. The model test carries the new text (through its Volume().Data accessor); mutants 10/10 (the author's 6
  respelled + 4 new; the earlier audit list named the old text and is replaced). /Od sizes against Ego_d 93..128: rounded_dimensions
  30, both parameter assignments 6, Width / Height 11, the Initialise statement 57 and `return Allocated;` 6 are all EXACT; Reset(new ..)
  54 v 74 (VC10 EH states); Ego_d's extra record is line 99 (276 B, a debug-only check).
- Same bindings: for all three rows the leaf's ordered relocation symbol list is identical to the author's re-port (21 / 2 / 6
  symbols; compared from the objects), so the policy fix changed no binding. The 25 "split" lines my binding scan prints for the set
  are all old alias spellings of shared callees in OTHER landed rows (CBaseClass ctor / dtor, GFGetSystemManager, ...), none new.
- engine/CTexture.h in fixed_e17 is the author's CURRENT file (two retail addresses in a comment corrected at 00:21, after my first
  snapshot; verified: `call 9FA180h` at 0x00A68A04, `call 9FA140h` at 0x00A68A69). Comment-only; not regraded.
- Headers (diffs in fixed_e17/hdr_diffs): CManagedTexture.h +7 -0, CManagedTextureData.h +11 -0, CVolumeTexture.h +31 -1 (the -1 is the
  author's pinned assert; my opt-in wraps the unchanged `class CVolumeTexture {` / `public:` lines in an #else), no unpinned assert,
  hdrcheck2 default = same for all eight overlay headers. 112 landed includers of the three headers (CManagedTexture.h 107,
  CManagedTextureData.h 111, CVolumeTexture.h 2): ppcmp 112/112 raw-identical; strict regrade ON 88 RELOCATION_MATCH + 5 MATCH PASS; 19
  HEADER_POLICY = the two landed texts this set replaces (009fd9b0, 00a699d0) + 17 pre-existing (00a69c20 00a69ed0 00b087c0 00b239c0
  00b2aee0 00b2af50 00b2b790 00b2b820 00b2c460 00b3cb30 00b54310 00b81f30 00ba6fa0 00ba77b0 00bad670 00bad8a0 00bc7f70), all identical
  with the overlay OFF and RELOCATION_MATCH / MATCH PASS non-strict with it ON (fixed_e17/includer_regrade_policyfix_strict_on.txt).
- NEW header engine/C3DExtentsI.h under the widened policy: one landed row outside the set gains a trip, 0x00A68370
  CVolumeTexture::Initialise (`class C3DExtentsI : public C2DExtentsI {`). It is ALREADY HEADER_POLICY today (six local models: CDisplayManager,
  CSystemManager, CPixelFormat, C2DExtentsI, CMemoryAllocator, CVolumeTexture), so nothing changes at landing, but it is the natural next re-port: the EVOLUMEPOOL / INITIALISE declarations it needs
  are now in the overlay's CVolumeTexture.h. Not built here.
- bindings_multi_check with the set merged: 1 (baseline). Purity genuine x10. Account A headers: none.

## setvs ($S/w26/reports/setvs) -- PASS as delivered (3 rows; no header change; no fix needed)
- Strict vland --recheck-landed on today's tree: 0x0098B930 SetVSConstants MATCH PASS, 0x00989CB0 UpdateObjectSpaceLighting
  RELOCATION_MATCH PASS, 0x00989230 UpdateObjectSpaceTextureTransform RELOCATION_MATCH PASS ($AW/setvs/logs). Widened policy: [] x3.
  Purity: genuine, genuine, retail_asm (0x00989230, through the shared SSE header, as landed).
- Copies (GOTCHAS 455): in both callers the function text is the owner's text verbatim (programmatic), directly under the exact
  `// Fidelity: same-TU strong definition of 0x0098B930 (retail TU-mate)` line; `typedef long HRESULT;` sits above that line in all three
  files. Each copy is now a strong definition (COMDAT-NODUP; the landed texts had a COMDAT-ANY inline) and grades 37 B, 0 relocations,
  0 differences at 0x0098B930. Landed-vs-new leaf relocation symbols: identical for both callers (0x00989CB0 differs in local label
  numbers only).
- Ego_d: locals index (long), data (const void*), count (unsigned long), hr (HRESULT) == the text; line table 1166..1184: 1170 6 B
  and 1172 5 B = `if (count == 0)` / `return;`, 1180 43 B the device call assigned to hr; 1168 (274 B) and 1183 (155 B) are debug-only
  checks. Agreed with the README.
- 0x00989230's `// Inferred:` unit table, re-measured with the new copy text: WITHOUT the RHSREF opt-in the row is DIFFER(635v636), so
  the opt-in is still needed; with the copy's `HRESULT hr =` dropped it is still RELOCATION_MATCH (the table's "0, byte-neutral" entry
  for hr holds); the delivered strong if / return form is RELOCATION_MATCH. I did not re-measure the other units.
- Tests / mutants: 0x0098B930 6/6, 0x00989CB0 6/6, 0x00989230 6/7 (test-model mutants; the survivor drops the zero test in the copy,
  unreachable in that row: it only calls with rows >= 1; the owner's test kills it). The owner's test cannot link the row object
  (private member: the access is part of the symbol) and carries the row text under `#define private public` -- accepted, it is
  the same constraint I hit for 0x00A773E0 (there I used a friend declaration instead).
- Interaction with b8: none in files (setvs changes no header). My b8 fix replaces the author's friend opt-in by the public inline
  CShaderRenderManager::SetVertexShaderConstantsDirect wrapper, which calls this private SetVSConstants; the two land independently.
  Header debt noted by the author stands: the shared declaration names the first parameter `reg` (Ego_d: index).

## stockdisplay ($S/w26/reports/stockdisplay) -- PASS as delivered (1 row; no header change)
- Strict vland --recheck-landed: 0x00774C60 CTCShop::StockDisplay RELOCATION_MATCH PASS. Policy []. Purity genuine.
- Copies: GetFreeSaleItemPlaces is the LANDED owner 0x00774B40's body verbatim (733 characters, programmatic) and GetSaleItemsToPlace
  the landed owner 0x00773EE0's (888 characters), each under its exact Fidelity line; they grade 274 B / 540 B with 0 non-relocation
  differences at their owners' addresses. Leaf relocation symbols identical to the landed text for all three functions.
- Ego_d: locals goods_places, goods_to_place, block-scoped long n == the text; lines 536..543 (two declarations 21 / 16 B, the `for`
  with std::min 64 B, AddSaleItem 36 B) match the statement structure.
- Test: the landed ABSTRACT model test, unchanged (it neither contains nor links the row; pre-existing). Test-model mutants 6/6; the
  seventh (std::max for std::min) does not compile in the test, as the author says. REVIEW (pre-existing): only the bytes vouch for
  this row's body; a row-linked test would need the two producers mocked at link level, which the same-TU strong copies prevent.

## hermite ($S/w26/reports/hermite) -- PASS as delivered (2 rows; no header change)
- Strict vland --recheck-landed: 0x00A56670 GFGetHermiteSplinePosition RELOCATION_MATCH PASS, 0x00A567A0 CCardinalSpline::GetSplinePosition
  RELOCATION_MATCH PASS (RELOC_ALIAS note for the _CIfmod import thunk, as landed). Policy [] x2. Purity genuine / retail_asm (through
  GFFloatToLongFloor.h, as landed).
- Ego_d: parameters dest / from_pos / from_tan / to_pos / to_tan / s and exactly six float locals s2, s3, wpos_from, wpos_to, wtan_from,
  wtan_to == the text (the landed two_s3 / three_s2 temporaries are gone, correctly). VC7.1 /Od line sizes of the delivered text
  against Ego_d lines 37..46: `{` 12 = 12, s2 9 = 9, wpos_from 29 = 29, wpos_to 23 = 23, wtan_from 18 = 18, wtan_to 9 = 9, `}` 6 = 6
  all exact; s3 6 v 9 (VC7.1 keeps s2 on the x87 stack across the two statements), the dest sum 119 v 131 (+3 for each of the four
  float arguments under VC10, the delta measured in b8 / b10). The one-statement-per-weight structure is confirmed.
- Copy: the function in 0x00A567A0 is the re-ported owner's text verbatim under the Fidelity line and grades 295 B, 0 non-relocation
  differences at 0x00A56670; so the two rows must land together (they do).
- Mutants: owner 8/8 (test-model); caller (row source, the test links the row object) copy list 3/3, author list 43/45, earlier audit
  list 5/5 + 2 NOT_FOUND (they name the removed h4 / three_s2 lines; the 3 copy mutants replace them). The 2 survivors are the
  `EndPointsConnected && from_index > to_index` pair the h3c2 audit classed equivalent.

## leaftype ($S/w26/reports/leaftype) -- PASS; overlay REBASED AWAY (today's header already has the opt-in) -> fixed_leaftype (no hdr)
- The coordinator's note: FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_STATCLASSTYPENAME landed with hand3c2 b2. Compared: today's
  rebuild/include/engine/CTCActionUseScriptedHook.h and the set's overlay differ ONLY by where that block sits in the class and by its
  comment; the body is identical (`static CCharString FABLE_FASTCALL StatGetClassTypeName() { return CCharString("CTCActionUseScriptedHook"); }`)
  and the assert is pinned in both. So the set's own copy is dropped: fixed_leaftype has no hdr/ at all (landing the stale overlay would
  have reverted b2's comment and moved the block). Strict vland --recheck-landed with NO overlay: RELOCATION_MATCH PASS
  (fixed_leaftype/vland_strict_recheck.log). The author's includer regrade is moot (no header change).
- Row: no local class (policy []), purity genuine. Ego_d: parameter test_thing, local ptc_type; lines 486..493: 488 19 B the call and
  test, 490 4 B `return true;`, 492 2 B `return false;` == the text. Today's engine/CThingFilter_HasTCOfLeafType.h (b2's
  FABLE_RETAIL_CTHINGFILTER_HASTCOFLEAFTYPE_BODY) carries the same four statements as an in-class body; this row defines the operator out
  of class without that opt-in. Same text in two places, consistent today. REVIEW (optional): the row could define the BODY opt-in and
  keep only the explicit instantiation, so the text exists once; not tried.
- Landed-vs-new relocations: the object now binds the shared spellings (CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020,
  CBasicString<char>::Compare 0x00411570, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0); the stand-in strong
  LowerBound of the landed text is gone. bindings_multi_check with the set merged: 1 (baseline). NAME_FIXES (2 rows) agreed.
- Test: the landed self-contained model test with the operator in the row's form (it does not link the row object; the author's reason --
  a constructed CThing and a concrete CTCBase subclass -- is fair). Test-model mutants 5/5 KILLED + 1 NOT_FOUND (`always_true`: its
  search text spans the `template class` line that the test does not contain; not counted).

## SUMMARY (all paths under $AW = $S/stage/audit_h3c2b)
| item | verdict as delivered | landable set | grades (strict, today's tree) | mutants |
|---|---|---|---|---|
| b8 0x00B99300 ProcessLightingSW | FAIL (made-up names aa / bb; invented friend) | fixed_00b99300 | RELOCATION_MATCH PASS | 61/61 |
| b9 0x00646762 DrawStatBarOnLeft (+ owner 0x00643CDA) | FAIL (copy not the owner's text) | fixed_00646762 (2 rows) | RM PASS x2 | 80/80 + 7/7 |
| b10 0x00ADF960 SatisfyDistanceConstraints | PASS on bytes; 2 fixes (max macro, `limit` type) | fixed_00adf960 | RM PASS (/Zi) | 47/47 |
| b11 0x00591B7F UpdateContextSensitiveItems | PASS | fixed_00591b7f (= author + audit files) | RM PASS (/Gr) | 59/59 |
| e17 (10 rows) | 9 PASS, 1 name FAIL (DrawBox); 3 rows fail the widened policy | fixed_e17 | 8 RM + 2 MATCH PASS; policy [] x10 | see sections |
| navedge (7 rows) | PASS on bytes; names FAIL; 1 row fails the widened policy | fixed_navedge (8 rows) | 7 RM + 1 MATCH PASS; policy [] x8 | see section |
| setvs (3 rows) | PASS | the author's directory | 2 RM + 1 MATCH PASS | 18/19 (1 unreachable) |
| stockdisplay (1 row) | PASS | the author's directory | RM PASS | 6/6 (model test) |
| hermite (2 rows) | PASS | the author's directory | RM PASS x2 | 8/8, 51/53 (2 equivalent) |
| leaftype (1 row) | PASS; overlay obsolete | fixed_leaftype (no hdr) | RM PASS | 5/5 (model test) |
hand3c2 b8..b11 are in this table; b12, b13 and hand3c3 b1 / b2 arrived later: see the SUMMARY ADDENDUM at the end.

## NAMES FOR ACCOUNT A (landed spellings proved wrong, or new real names; full evidence in each set's NAME_FIXES.tsv)
- 0x0143B954 / 0x0143B958: Ego_d S_LDATA32 `int aa` / `int bb` (statics of the repeated-static-mesh manager TU), not the made-up
  debug-switch names (b8).
- 0x00B9C060 ?SetVertexShaderConstantsDirect@CShaderRenderManager@@QAEXKPBXK@Z (a `jmp 0x0098B930` COMDAT copy; public inline wrapper);
  0x00B98BA0 ?PeekRenderInfo@ / 0x00B98BB0 ?PeekLights@CEngineInternalPrimitiveRepeatedStaticMesh@@.. (b8). Landed 0x00B99670
  UploadShaderConstants binds SetVSConstants with a public (QAE) spelling and should call the wrapper.
- 0x00A67A20 ?DrawBox@CVolume@@QAEXABVC3DBoxI@@ABVCRGBColour@@@Z (was CVolume::ClearArea) (e17).
- 0x00581BDE ?GetContextSensitivePreferredItemPriority@CTCInventory@@SIJJ@Z (landed under the ButtonIndex name of 0x00581BFE; landed
  0x005928A8 binds `?GetContextSensitivePriority_00581bde@@YIJJ@Z`); 0x005819F2 ?GetQuickAccessIndexFromButtonIndex@CTCInventory@@QBEJJ@Z
  (landed as ValidateIndex); 0x00581BFE ?GetContextSensitivePreferredItemButtonIndex@CTCInventory@@SIJJ@Z (b11).
- 0x00A778B0 ?RemoveConnection@CNavRegionConnection@@QAEXXZ (landed as CGraphicInfoFrame::CFrameData::ReleaseTexture, binding
  0x00A77710 as ?Release@XResource@@..); 0x00A77710 ?RemoveConnectionFromConnectionList@CNavRegion@@AAEXPBVCNavRegionConnection@@@Z;
  0x00A7C0A0 ?DeleteData@?$CCountedPointer@VCNavRegionConnection@@@@SIXPAX@Z; 0x00A77420 ?Unlock@CNavSwitchableLeafNode@@AAEXPBVCNavRegion@@@Z;
  0x00A7DB10 resize `..QAEXJ..` (landed 0x00A772D0 binds K); 0x00A770B0 GetNodeAt `QBEPBVCANavQuadTreeNode@@ABVC2DVector@@J@Z` (landed
  0x00A77480 binds PAV / PBU); 0x00A7D050 std::__find (landed 0x00A76A80 binds a global `?find@@YI..`) (navedge).
- EVolumePool enumerators VOLUME_POOL_MANAGED = 1, VOLUME_POOL_SYSTEM_MEM = 2, VOLUME_POOL_DEFAULT_LOCATION = 0 (Ego_d 0x22D8);
  FableWin ?GetVolumeTexture@CManagedTexture@@ / @CManagedTextureData@@ (e17 addendum).
- Account A headers (engine/NThingMap_CThingMap.h, CThingSearchTools.h, CTCWeapon.h, the CFloat* reverb headers): NOT touched by any
  audited set or any fixed set.

## REVIEW ITEMS FOR THE COORDINATOR
1. navedge 0x00A78800: the per-site inline split (first site expanded, second called) is not reproduced by any natural spelling; the
   fixed row forces it with a scoped `#pragma inline_depth(0)` and says so (`// Inferred:`). The row's earlier text carried the same
   residue as a made-up inline member. Decide whether that device is acceptable; the alternative is to leave the row out of the set.
2. navedge now has EIGHT rows: owner 0x00A773E0 IsEveryUniqueIDOpen is re-ported (needed for a verbatim same-TU copy).
3. Landed rows outside the audited sets that the widened policy flags and that sit next to these sets: 0x00A77420 Unlock (5 local
   models incl. CNavSwitchableLeafNode), 0x00A68370 CVolumeTexture::Initialise (6 today, 7 once C3DExtentsI.h lands).
4. Landed 0x00589A30 CTCInventory::OpenPCMapMenu is DIFFER(549v549) on today's tree with NO overlay (found in the b11 includer
   regrade): a pre-existing regression of a landed row.
5. b10 macro: decide between the Win32Types.h FABLE_RETAIL_WIN32_MINMAX shim (built), the CRT `__max` (no header change, same bytes)
   and the author's hand expansion with an Inferred line. The macro NAME is not recorded anywhere.
6. b8: 142 landed includers regraded; the GEnableSIMD binder list is in logs/simd_binders_b8.txt; the shared ProcessLightingSW
   declaration's parameter names differ from Ego_d (header debt).
7. Tests that do not exercise the row object (pre-existing abstract / model tests): 0x00A68910 (e17), 0x00774C60 (stockdisplay),
   0x00518DC0 (leaftype), 0x0098B930 (private member). Only the bytes vouch for those bodies.
8. Landed 0x00594ED4 (std::sort row) declares CContextSensitivePriorityCompare without the std::binary_function base Ego_d gives it.
9. Ego_d line sizes I could not explain without Ego_d's code bytes (recorded, not blocking): b11 line 962 (497 B for the GetDef
   statement), b10 the pos1 / pos2 updates (40 v 52). Note for all Ego_d size comparisons: Ego_d's containers are the VC10 STL with
   checked iterators (`std::_Vector_iterator<std::_Vector_val<..>>` in its type records), not STLport, so iterator statements are much
   larger there (b11 lines 933 / 938 / 944; navedge 645 / 668).
10. Shared-header debt (not changeable additively): CShaderRenderManager::SetVSConstants' first parameter is `reg` (Ego_d: index).
11. Process disclosures: I stopped only processes I started (the over-long e17 full regrade after 168 rows; one hung test compile of my
    own at 01:15). An `rm -rf` of my own scratch was blocked by the harness and I did not work around it: scratch under $AW/lv, $AW/q,
    $AW/tmp is left for cleanup. Outside $AW I wrote only two scratch diff files (/tmp/claude-0/r.txt, /tmp/claude-0/o.txt); no repo
    write, no git command.
12. I added no mutants of my own to setvs, stockdisplay and leaftype (model tests; the authors' lists were re-run); hermite's caller
    row was covered by the earlier audit's list.

## LANDING NOTES
- Each fixed_* directory is self-contained: payload.json + oracle.tsv (vland input), src/, test/, hdr/ (overlay = today's headers +
  the opt-ins), header diffs, NAME_FIXES.tsv, mutant lists and logs, the strict vland log.
- Header overlays are full files based on TODAY's tree (2026-11-20 00:40..01:20 UTC). If a header moved since, re-diff before copying:
  b8 (CEngineLightingManager.h, CShaderRenderManager.h, CEngineInternalPrimitiveRepeatedStaticMesh.h), b9 (CFontBank.h), b10
  (Win32Types.h), b11 (CContextSensitiveItemDef.h, CTCInventory.h), e17 (C3DExtentsI.h NEW, CBankFileAsyncData.h, CPixelFormat.h,
  CTexture.h, CTextureManager.h, CVolumeTexture.h, CManagedTexture.h, CManagedTextureData.h), navedge (CNavConnectorNode.h NEW,
  CNavNavigableLeafNode.h, CNavQuadTree.h, CNavRegion.h, CNavSwitchableLeafNode.h). leaftype needs no header.
- No two fixed sets share a header or a row; b8 and setvs are independent (see setvs).
- FINAL RECHECK, 01:23 UTC, after the coordinator's b2 landing changed the tree at 00:48: every fixed set re-run through strict vland on
  the then-current tree (fixed_*/vland_strict_final.log): b8 1/1, b9 2/2, b10 1/1, b11 1/1, e17 10/10, navedge 8/8, leaftype 1/1, all
  PASS; every overlay header re-diffed against the current tree: additive (0 non-assert lines removed; b11's CTCInventory.h has its one
  known `#ifdef` -> `#elif` line).

## ---- batches that arrived after the first summary (cap lifted by the coordinator) ----

## hand3c2 b12 0x00BADE40 CEnginePrimitiveManagerSprites::RenderSprite -- FAIL as delivered (unpinned overlay assert; a cast where the shared enumerator exists) -> fixed_00bade40 PASS
- Oracle 793 B == Fable.exe. Author text: strict RELOCATION_MATCH PASS; widened policy []; purity retail_asm through the shared
  GFFloatToLongNear.h only, with the `// Retail-asm:` line (the README's "genuine C++" is loose wording).
- FAIL 1 (header): engine/CEnginePrimitiveManagerSprites.h adds 6 lines above an UNPINNED `FABLE_STATIC_ASSERT(` (unpinned in today's
  tree too), so the assert's typedef name changes for the landed includers: ppcmp 4 raw-identical + 1 "differs (assert)".
  pin_asserts.py says why that is not neutral (stack-slot order can depend on those names). Fixed: pin_asserts.py on the overlay
  (1 pinned, _AT(18)); ppcmp 5/5 raw-identical.
- FAIL 2 (name, byte-neutral): `GetGraphicBank((EGraphicBankNo)0)`. The shared engine/EGraphicBankNo.h has Ego_d's enumerators
  (MAIN_GRAPHIC_BANK_NO = 0, MAX_GRAPHIC_BANKS = 1; Ego_d field list 0x66776). Fixed: include it, write MAIN_GRAPHIC_BANK_NO.
  (CEngineResourceManager.h's "no enumerator name recovered" comment on its forward declaration is stale: header debt.)
- Fixed set: strict RELOCATION_MATCH PASS (vland_strict_final.log). Also corrected in the row comment: Ego_d's debug length is 865, not 949.
- Ego_d: locals world_pos (const C3DVector&), size_x / size_y (const float), distance, alpha (unsigned long), capture_block, subprim ==
  the text. VC7.1 /Od listing of the fixed text against Ego_d 334..366: 19 lines onto 19 of 20 records in order; the 20th is line
  364 (280 B, a debug-only assert). Exact: size_x / size_y / world_pos 9, alpha 13, the distance call 11, IsParticle 20, the BeginLoading
  line 45. The radius line is 90 here v 117 in Ego_d (the C2DVector(..).GetMagnitude() alternative is 77): the author's reading (four
  float arguments + the float return spills under VC10) accounts for most of the 27; I accept the text, but it is the one statement
  chosen by scheduling + a size argument rather than read off a record: the row's "Radius:" paragraph says so, without an Inferred
  label. REVIEW. The queue call 29 v 33 fits the direct five-argument renderer call as the author says.
- Headers: CEnginePrimitiveManagerSprites.h +6 -0 (additive); NEW engine/CEngineSubPrimitiveSprite.h. 5 landed includers: strict regrade
  3 MATCH PASS + 2 HEADER_POLICY (00b33b50 00baf9e0) identical with the overlay OFF, RELOCATION_MATCH PASS non-strict ON.
  NEW HEADER TRIPS (widened policy): two landed rows define CEngineSubPrimitiveSprite locally and will be flagged once the header
  lands: 0x00BB1740 and 0x00BACE10 (the class's own out-of-line constructor copy). Not re-ported here.
- Names: GPCamera / GPSubPrimitiveRenderer / GPResourceManager in NEngineInternal (Ego_d data names; NAME_FIXES 2 rows agreed).
  bindings_multi_check with the row merged: 1 (baseline).
- Mutants (row source + overlay; the test links the row object): author 69 (one respelled for the enumerator) + audit 5 = 74/74 KILLED.

## hand3c3 b1 0x009ACEE0 CDefinitionManager::ParseEnumForSymbols -- FAIL as delivered (unpinned overlay assert, 50 landed includers affected) -> fixed_009acee0 PASS; source unchanged
- Oracle 1017 B == Fable.exe. Strict RELOCATION_MATCH PASS; widened policy []; purity genuine; no Inferred / Bracket (none needed
  that I can see: the three "what closed it" facts are existing opt-ins and a `for` increment placed by Ego_d line 810).
- FAIL (header): engine/CParsedItem.h adds 5 lines above an unpinned `FABLE_STATIC_ASSERT(sizeof(CParsedItem)==12)`: ppcmp over the
  1236 landed includers of the two overlay headers = 1186 raw-identical + 50 "differs (assert)". Fixed with pin_asserts.py (_AT(33));
  ppcmp 1236/1236 raw-identical. CDefinitionManager.h +18 -0 (the typed ErrorCallback sits in an #ifdef / #else, sizes unchanged).
- Strict regrade, seeded sample of 40 of the 1236 includers (CParsedItem.h 506, CDefinitionManager.h 757): 29 RELOCATION_MATCH + 2 MATCH
  PASS; 9 HEADER_POLICY (004656a6 004665b6 0065b854 006648d0 00671d40 00671fe0 00739360 007ad7f0 00830150) identical with the overlay
  OFF and RELOCATION_MATCH PASS non-strict ON. The sample was taken before the pin; with every includer raw-identical after it, the
  remaining 1196 cannot change.
- Ego_d: locals and scopes == the text (parser; symbol_type_name, parsed_item, current_val; block { val, symbol_name, comma_or_equals;
  { persist_context, val_traits } }), types long / CCharString / CParsedItem. I did not re-derive the 43-record line table statement by
  statement (the author's egod_lines.tsv mapping is in the row header); spot checks agree (810 = the `for` test + increment).
- Names: 20 bindings, all PDB spellings; NAME_FIXES (4 rows: Error is protected, GetSymbol / GetString const, TransferIn's class) agreed.
- Mutants (row source; the test links the row object): author 38 + audit 5 = 43/43 KILLED.

## hand3c3 b2 0x00A4C1F0 / 0x00A52760 / 0x00A53860 (keyframe interpolation) -- bytes PASS x3; FAIL on labelling and an unpinned assert (fixed -> fixed_c3b2); the coordinator's helper check is NOT MET and NOT FIXED here
- Oracles 1006 / 1004 / 1005 B == Fable.exe. Strict RELOCATION_MATCH PASS x3; widened policy [] x3; purity retail_asm through
  GFFloatToLongNear.h only, `// Retail-asm:` lines present.
- FAIL 1 (labelling): the README declares one fitted choice (`goto rotation_done;`), but none of the three sources had a `// Inferred:`
  line. Added to all three (comment only).
- FAIL 2 (header): engine/C3DAnimationSequence.h (+7 -0, the nested _DECL opt-in) sits above an unpinned assert: ppcmp 2 raw-identical +
  13 "differs (assert)" of 15 includers. Pinned; 15/15 raw-identical. Strict regrade of the 15: 11 RELOCATION_MATCH + 2 MATCH PASS, 2
  HEADER_POLICY (00a4df10 00a4e250) pre-existing.
- THE COORDINATOR'S CHECKS ON THE HELPER CHAIN:
  (1) "every changed helper text under NEW nested opt-ins, default path unchanged": NOT as described. The rewritten helpers
  (GFFastArcCos, GFFastSin, CQuaternion::Dot / SetIdentity / SetSlerp, C3DAnimationSequenceData::InlineGetKeyframeInterpolate) are NOT in
  any shared header: each row defines them itself (the shared headers only declare them; that is the existing convention of these
  rows). The helper block is byte-identical text in 0x00A4C1F0 and 0x00A53860 and differs in 0x00A52760 only where that row IS the
  function. So no shared text changed and no landed row can be affected by this batch -- but the result is one more private copy of each
  helper: landed rows that define their own (older) texts: GFFastArcCos 13 rows, GFFastSin 19, SetSlerp 6
  ($AW/logs/c3b2_helper_definers.txt), and engine/GFFastSin.h holds yet another body.
  (2) "regrade every landed user of those helpers, strict": only meaningful once the texts are shared. I re-measured the author's one
  data point: 0x00A4D920 with the new texts is RELOCATION_MATCH (his regrade file, compiled here). The other definers were NOT
  re-measured with the new texts (it needs their private helper bodies replaced one by one): NOT DONE.
  Proposal for the follow-up: NEW engine/GFFastArcCos.h + a nested opt-in in engine/GFFastSin.h and engine/CQuaternion.h (Dot /
  SetIdentity / SetSlerp bodies) and engine/C3DAnimationSequenceData.h (the forced-inline body), the three rows reduced to opt-ins, then
  the 19 definers moved one by one with a strict regrade each. I did not build it (budget), so fixed_c3b2 keeps the row-local helper
  texts. DECISION for the coordinator: land the three rows as they are (they follow the existing convention and add no risk to
  landed rows) or hold them for the shared-header move.
- Names: Ego_r / FableWin ?InlineGetKeyframeInterpolate@C3DAnimationSequenceData@@QBEXAAVC3DKeyframe@@KM@Z (FableWin 0x032274E0) and
  @C3DAnimationSequence@@ (0x032274B0); Ego_d GDATA32 GFastArcCosTable / GFastCosTable: the two row renames and the table name are
  right (NAME_FIXES agreed). Ego_d locals: GFFastArcCos (value; pos, index, interp, result), GFFastSin (angle; pos, index, interp,
  result), SetSlerp (q1, q2, weight2; sign, scale1, scale2, cosa; block { angle, sina }), InlineGetKeyframeInterpolate (dest, frame,
  fraction; next_frame, w1, w2; blocks { index, next_index } and { index, next_index, position, next_position }) == the texts.
- Binding note: my scan shows GFastCosTable / GFastArcCosTable at two targets each (+0 and +4). That is my tool not subtracting the
  object's addend for `table[index + 1]`; it is one symbol at one address. Baseline unchanged.
- Mutants (row source; the tests link the row objects; the author's lists re-run here): 57/57, 56/56, 57/57 KILLED. No audit additions.

## hand3c2 b13 0x00A48F10 CLipSyncSampleData::GetFrameInterpolate -- FAIL as delivered (unpinned overlay assert) -> fixed_00a48f10 PASS; source unchanged
- Oracle 811 B == Fable.exe. Strict RELOCATION_MATCH PASS; widened policy []; purity retail_asm through the shared GFFloatToLongFloor.h
  only, `// Retail-asm:` line present. No Inferred / Bracket declared.
- FAIL (header): engine/CLipSyncFrameData.h (+23 -0: FABLE_RETAIL_CLIPSYNCFRAMEDATA_INLINE_BODIES, the two inline bodies in an
  #ifdef / #else) sits above an unpinned assert: ppcmp 1 raw-identical + 4 "differs (assert)" of the 5 landed includers (006e2ed0
  00a495f0 00a4aac0 00a4ad80 00a4adb0). Pinned; 5/5 raw-identical. Strict regrade of the 5: 5/5 RELOCATION_MATCH PASS.
- Ego_d: locals and scopes == the text (dest, t; pos, index1, index2, weight2, frame1, frame2; { c1 { phoneme1 { phoneme2 } } } and
  { c2 { phoneme2 } }). I did not resolve each local's type index or re-derive the line table for this row (light audit: it arrived
  last); the author's egod_lines.tsv is in the set.
- Names: NAME_FIXES (vector<CLipSyncPhoneme>::erase 0x00A4A320, _M_insert_overflow 0x00A4A710) read, not re-derived.
  bindings_multi_check with the row merged: 1 (baseline). No new header.
- Mutants (row source + overlay; the test links the row object; the author's list re-run here): 59/59 KILLED. No audit additions.

## SUMMARY ADDENDUM (rows audited after the first summary)
| item | verdict as delivered | landable set | grades (strict, today's tree) | mutants |
|---|---|---|---|---|
| hand3c2 b12 0x00BADE40 RenderSprite | FAIL (unpinned assert; cast for a shared enumerator) | fixed_00bade40 | RELOCATION_MATCH PASS | 74/74 |
| hand3c2 b13 0x00A48F10 GetFrameInterpolate | FAIL (unpinned assert) | fixed_00a48f10 | RELOCATION_MATCH PASS | 59/59 |
| hand3c3 b1 0x009ACEE0 ParseEnumForSymbols | FAIL (unpinned assert, 50 includers) | fixed_009acee0 | RELOCATION_MATCH PASS | 43/43 |
| hand3c3 b2 0x00A4C1F0 / 0x00A52760 / 0x00A53860 | bytes PASS; label + unpinned assert fixed; helper check NOT MET | fixed_c3b2 | RELOCATION_MATCH PASS x3 | 57/57, 56/56, 57/57 |
Totals: hand3c2 b8..b13 (6 rows), hand3c3 b1..b2 (4 rows), six re-port sets (25 rows incl. the added owner 0x00A773E0). Queue empty at
01:50 UTC (newest: hand3c2 b13, hand3c3 b2).
Additional review items:
13. RECURRING: four of the six newest overlays (b12, b13, hand3c3 b1, b2) add lines above an UNPINNED FABLE_STATIC_ASSERT, which renames
    the assert typedef in every landed includer (1 + 4 + 50 + 13 includers). The authors' hdrcheck2 "default = same" does not see it;
    ppcmp does. Ask the authors to run lane_merge/pin_asserts.py on every overlay before hand-off.
14. hand3c3 b2: decide whether the three rows land with row-local helper texts or wait for the shared-header move (section above);
    the "regrade every landed user with the new helper texts" was NOT done beyond 0x00A4D920.
15. New-header trips to schedule: 0x00BB1740 and 0x00BACE10 (CEngineSubPrimitiveSprite, b12), next to 0x00A68370 (e17) and 0x00A77420.
16. b12's radius statement is chosen by scheduling plus an Ego_d size argument (90 v 117 B), explained in the row but not labelled
    Inferred.
17. The later batches (b12, b13, hand3c3 b1 / b2) got the full mechanical audit (oracle, strict vland, policy, purity, header
    additivity + pin, ppcmp over every includer, includer regrade, bindings, the authors' mutants re-run, Ego_d locals) but a lighter
    evidence pass than b8..b11: line tables re-measured for b12 only, audit mutants added for b12 and hand3c3 b1 only.
Additional names for Account A: 0x01436EA0 ?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A, 0x01436E74 ?GPSubPrimitiveRenderer@NEngineInternal@@..,
0x013CB530 ?GFastArcCosTable@@3PAMA, 0x013CD550 ?GFastCosTable@@3PAMA, 0x00A52760 / 0x00A53860 the two InlineGetKeyframeInterpolate
copies, 0x009AC9F0 Error is protected (IAE), 0x009B9870 / 0x009B9880 GetSymbol / GetString const, 0x009E2590
?TransferIn@CPersistTraitsSLONG@@SIX..AAJ@Z, 0x00A4A320 vector<CLipSyncPhoneme>::erase; enumerator MAIN_GRAPHIC_BANK_NO.
Additional overlays (full files on today's tree, asserts pinned): b12 CEnginePrimitiveManagerSprites.h + NEW CEngineSubPrimitiveSprite.h;
b13 CLipSyncFrameData.h; hand3c3 b1 CDefinitionManager.h, CParsedItem.h; hand3c3 b2 C3DAnimationSequence.h.

# Set e17: 0x009FDE80 CGraphicInfoFrame::CFrameData::BeginLoading + nine C3DExtentsI re-ports (Account B, w26, 2026-11-19)
One landable set: the audited new row (fixed_9fde80, text unchanged) and the nine landed rows that define a local `C3DExtentsI`, re-ported
onto the new shared engine/C3DExtentsI.h. Strict vland (`LANDVERIFY_STRICT_HEADERS=1`, overlay `hdr/`, `--recheck-landed`): 10/10
RELOCATION_MATCH / MATCH + PASS (`logs/vland_strict_recheck.log`); the new row alone without `--recheck-landed`: RM PASS (`logs/vland_strict_new.log`).

| address | row | grade | grades used | what changed |
|---|---|---|---|---|
| 0x009FDE80 | CGraphicInfoFrame::CFrameData::BeginLoading | RELOCATION_MATCH | 0 (audited text) | new row, as fixed_9fde80 |
| 0x00A677D0 | CVolume::Lock | MATCH | 1 | local C3DExtentsI (8 opaque bytes + Z) -> engine/C3DExtentsI.h; `.X` / `.Y` instead of the prefix casts |
| 0x00A67530 | CVolume::ReadOnlyLock | MATCH | 1 | the same |
| 0x00A685E0 | CVolumeTexture::Lock | RELOCATION_MATCH | 1 | the same + local CCountedPointer -> engine/CCountedPointer.h |
| 0x009FD9B0 | CGraphicDataBank::LoadVolumeGraphic | RELOCATION_MATCH | 1 | the same + local CIVCountedPointer -> engine/CIVCountedPointer.h |
| 0x00A699D0 | CManagedTextureData::CreateVolumeTexture | RELOCATION_MATCH | 1 | local C2DExtentsI / C3DExtentsI -> the two shared headers; `// Leaf:` line added (full PDB symbol) |
| 0x00A67950 | CVolume::Clear | RELOCATION_MATCH | 2 (+2 trial) | local CRGBColour / CPixelFormat / C3DExtentsI -> shared; same-TU Lock stand-in -> owner 0x00A677D0 verbatim + Fidelity line |
| 0x00A67860 | CVolume::CopyFromMemoryAutoSwizzle | RELOCATION_MATCH | 2 | the same |
| 0x00A67A20 | CVolume::ClearArea | RELOCATION_MATCH | 2 | the same; the format through the PDB's inline CVolume::GetPixelFormat |
| 0x00A68910 | CVolumeTexture::GetAsTextureList | RELOCATION_MATCH | 3 (1 SRC_FAIL) | local C2DExtentsI / C3DExtentsI / CPixelFormat / CSurfaceLockedRect / CTexture / CVolumeTexture -> shared, two new opt-ins; the inline ~CVolume's callee 0x00A67460 respelled Uninitialise(bool) (PDB) |

## Scope correction (found while orienting)
Only 0x00A677D0 and 0x00A67530 were strict-clean before this set. The other seven already carried local models of classes that have shared
headers (CRGBColour, CPixelFormat, CCountedPointer, CIVCountedPointer, C2DExtentsI, CSurfaceLockedRect, CTexture, CVolumeTexture), so a
C3DExtentsI-only swap would have left them strict HEADER_POLICY. Every flagged class is moved; `tools/hp.py -o hdr src/*.cpp` (a static replay of
the verifier's policy that also sees overlay-new headers -- verify_and_land tests the REPO include dir, not the overlay) prints `[]` for all ten.

## Same-TU strong copies (GOTCHAS line 455)
The three CVolume rows carried a noinline stand-in body of CVolume::Lock (not the owner's text). It is replaced by the re-ported owner
0x00A677D0 body verbatim over the owner's own D3D / CVolume declarations, under the exact
`// Fidelity: same-TU strong definition of 0x00A677D0 (retail TU-mate)` line; no `#pragma auto_inline`, no `__declspec(noinline)` (measured: RM
with and without the pragma). The copy grades MATCH at 0x00A677D0 in each of the three TUs, and each row stays RELOCATION_MATCH.

## Headers (hdr/, diffs in hdr_diffs/; additive, opt-in only; hdrcheck2 default=same)
- NEW engine/C3DExtentsI.h (e17's): now also includes rebuild_abi.h -- e17's copy used FABLE_STATIC_ASSERT with only fable_compiler.h included
  (C2501 in a TU that had not included rebuild_abi.h first: 0x00A68910's first grade).
- engine/CBankFileAsyncData.h, engine/CTextureManager.h: e17's blocks, unchanged from fixed_9fde80.
- engine/CPixelFormat.h: FABLE_RETAIL_CPIXELFORMAT_CONVERTRGBTOPFDDATA_OUTOFLINE (the ConvertRGBToPFDData declaration for TUs that call both it
  and GetByteColourDepth out of line; the existing _CONVERT branch brings the inline accessor bodies).
- engine/CTexture.h: FABLE_RETAIL_CTEXTURE_SWIZZLE_RECT (Swizzle / Unswizzle(long, CSurfaceLockedRect&) 0x009FA140 / 0x009FA180, declarations).
- engine/CVolumeTexture.h: FABLE_RETAIL_CVOLUMETEXTURE_GETASTEXTURELIST (declarations of Lock, Unlock, GetVolumeLevel, Swizzle, Unswizzle,
  GetDimensions, GetPixelFormat, GetAsTextureList; PDB names); its assert pinned by pin_asserts.py (_AT(15)).
- Includer regrade: tools/ppcmp.py over all 776 landed transitive includers of CPixelFormat.h / CTexture.h / CVolumeTexture.h /
  CBankFileAsyncData.h / CTextureManager.h: 776/776 preprocess raw-identical with the overlay (logs/ppcmp_includers.log). Strict vland with the
  overlay over the 5 opt-in users (009fb750 00b3c5d0 00b54310 00b54890 00b67270) + a seeded sample of 10: 11 RM PASS, 4 HEADER_POLICY
  (00b2c4e0 00b54310 00b68610 00f37390) that are identical with the overlay off and RM PASS non-strict with it on (logs/includer_regrade_*.log).

## Tests and mutants
- 00a67950 / 00a67860 / 00a67a20: new tests that LINK THE ROW OBJECT (the row's own declaration prologue + a TestVolume device + stubbed
  CPixelFormat callees); mutants on the ROW SOURCE (tools/mut.py): 10/10, 9/9, 14/14 killed (logs/mutants_rowsrc.log). The first ClearArea test
  let 4 mutants live (width == height, no front > 0 case); the extents are now 4 / 3 / 5 and a front > 0 case was added.
- 00a677d0 / 00a67530 / 00a685e0 / 009fd9b0 / 00a699d0: the landed tests with the pasted row text synced (shared C3DExtentsI, `.X` / `.Y`);
  009fd9b0's test keeps a local CIVCountedPointer (it destroys a CManagedTexture, and the shared destructor needs the full pointee model).
  00a68910: the landed abstract model test, unchanged. Test-model mutants (tools/mutt.py): 6/6, 6/6, 6/6, 8/8, 6/6, 6/6; 0x009FDE80: the
  audit's 17/17 re-run (logs/mutants_testmodel.log).
- Purity (tools/pur.py): all ten genuine.

## For the coordinator
- 0x00A68370 CVolumeTexture::Initialise is a tenth landed row with a local C3DExtentsI (`class C3DExtentsI : public C2DExtentsI {`, which
  escapes the verifier's regex). Not in the nine; it stays RM and keeps its (pre-existing) HEADER_POLICY for six other classes.
- Local models that the regex does not see and that stay in these rows: `class CVolumeTexture:public CBaseClass{` (0x00A685E0, 0x009FD9B0,
  0x00A699D0) although engine/CVolumeTexture.h exists; CVolume and CVolumeLockedBox have no shared header yet (five different local spellings).
- Not respelled (item scope = header moves): locals of these rows. Ego_d has them (e.g. GetAsTextureList: texture_list, dimensions2D,
  texture_format, src, slice_row_size, format_size, dimensions, vlock, {ct {old_src, tlock, dst {cy}}}).
- Landing: `repair_remove.py 00a677d0 00a67530 00a685e0 009fd9b0 00a699d0 00a67950 00a67860 00a67a20 00a68910`, copy hdr/engine/*.h, then
  fixmove / keypayload / vland --land. Three rows changed test (PassPattern unchanged: `<addr>_TEST PASS`); 0x00A699D0 gained a `// Leaf:` line.

## Names for Account A
NAME_FIXES.tsv: e17's three (GetAsyncEntry, TextureLoadingPriority, DrawSquare) + 0x00A67460 Uninitialise (already queued), 0x00A67410
CVolume::Unlock, 0x00A67440 / 0x00A67450 CVolume::Swizzle / Unswizzle, 0x00A0AA70 CRenderManagerCore::WaitWhileResourceIsBusy.

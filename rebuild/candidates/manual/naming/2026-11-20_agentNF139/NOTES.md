# NF139 notes (round 10, PRIORITY slice 11 third pass: 9 queue lines)

Counts from APPLIED.tsv / STATUS.tsv / recheck_final.log:
- Entries (9): 1 done (q6308), 8 deferred: held by NF136 (q4291, q4292, q6168, q4295, q4296, q4299, q4659, q6305). 0 done-already,
  0 rejected, 0 blocked, 0 open. Every deferred entry except q6305 has its self row ported and finished; q6305's self row is already the claim.
- Rows (STATUS.tsv, 36 rows): 17 ported (4 MATCH + 13 RELOCATION_MATCH, every test PASS, one recheck_final.log run, no HARD /
  RELOC_CONFLICT / HEADER_POLICY / RELOC_CHECK_ERROR), 2 not-a-binder (0x009F2E20, 0x009CC270), 17 blocked: 13 held by NF136 (not
  touched) + 4 of mine (q4296: 0x009E15E0, 0x0041BB00, 0x0122A600; q4299: 0x009F61C0).
- RENAMES: 7 section-1 self renames (0x009BEDC0, 0x009BEF20, 0x009E3790, 0x009F9F70, 0x00A03B30, 0x009FA1C0, 0x009CC2A0), 11 section-2
  caller respellings. NAME_FIXES: 11 lines. selfcheck.log: 0 problems.
- Header opt-in (hdr/README.md): engine/CBankFile.h FABLE_RETAIL_CBANKFILE_ENTRYDATA_INLINE (add-only; token-identical without it).

## Coordinator decisions needed
1. **Locks.** The brief says rows.txt holds my WORKLIST binders, but on disk every one of them is still locked NF136 (not in NF136's
   nf136 hand-back; NF136 never reached these entries). I treated them as held (the owner file is not `released`) and did not touch them:
   0x00405650, 0x0040F0E0 (q4291), 0x00499D90 (q4292 / q6168), 0x0066CA80 (q4295), 0x009C1AC0, 0x009C2520 (q4296), 0x009E45D0 (q4299),
   0x0082CD00 (q4659), 0x00642A60, 0x0066C7A0, 0x00A10E10, 0x00B4B830, 0x009C1050 (q6305). Re-own them to finish the 8 deferred entries:
   each APPLIED line and NAME_FIXES has the target spelling and a port sketch.
2. **Separable self ports.** The deferred entries' finished rows are in rows/ in NF133's q4727 form (finished, separable). Landing them
   without the held binders only shrinks each address's spelling set (APPLIED gives before / after counts: 0x009BEDC0 4->3,
   0x009BEF20 4->2, 0x009E3790 3->2, 0x009F9F70 8->6, 0x00A03B30 4->3, 0x009FA1C0 4->2) and creates no split (each address already has
   more than one spelling, so the gate's CREATED test cannot fire). The brief's literal rule says such a rename waits; your call.
   0x00A0B480 (callee of 0x009BEF20): 0x009BEF20 leaves ?BeginRender@CRenderManagerCore@@QAEXXZ for the PDB QAE_NXZ that 0x009BF350
   already binds (its own row is still the CRenderManagerCore_Methods stand-in).

## Rows and evidence
- 0x009BEDC0 / 0x009BEF20: View stand-ins -> engine/CDisplayManager.h (+ CRenderManager.h / IDirect3DDevice9.h). Objects define the leaf
  plus shared-header inline pick-any COMDATs (??0C2DExtentsI, boost::scoped_ptr<CRenderManager>::operator->).
- 0x00499880 (lock was `released`; claimed): in-place respell inside its HEAD-local CDisplayManager model, ToggleRendering (a label
  invention, in no PDB) -> bool BeginRender(); defined-symbol set identical to HEAD; its standalone test respelled the same way.
- 0x009E3790: CPixelFormat_Methods -> engine/CPixelFormat.h retail branch. 0x009F9F70 / 0x009FA1C0: CTextureUninitialiseView /
  CTextureAssignmentView (fable_texture_lifecycle.h) -> engine/CTexture.h + engine/Direct3DTexture9.h
  (FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES). 0x009FA1C0 keeps Uninitialise's body written out (retail expands it; a same-TU strong
  copy would clash with 0x009F9F70).
- 0x00A03B30: placeholder C00a03b30::M -> engine/CInputEvent.h ~CInputEvent (claim checked against the body: a lone `ret`).
- 0x009CC2A0 / 0x009CC240: ReadEntryData moved onto engine/CBankFile.h with 0x009CC240's define set. Retail expands GetEntryDataSize /
  GetEntryDataOffset there, so the bodies moved into the header (NF139 opt-in) and 0x009CC240 became the nf86 / nf88 pick-any scaffolding
  (`// Fidelity: scaffolding, not retail`). Link proof (LINK= cleared, /DLL /NOENTRY /NODEFAULTLIB /FORCE:UNRESOLVED; work/q6308/link/):
  ctl.log = HEAD 0x009CC240 + new 0x009CC2A0 -> LNK2005 ?GetEntryDataSize@CBankFile@@QBEKK@Z + LNK1169 (exit 145); new.log = both new ->
  exit 0 (LNK4088 only). 0x009CC270 (the GetEntryDataOffset owner) is a CBankFile_Retail stand-in, so no clash today; when it is ported it
  needs the same scaffolding (NAME_FIXES).
- 0x0049AE30 / 0x009DE6F0: in-place respell of the pre-existing local CTexture model's operator= to the PDB const& return (0x0049B3B0 gets it from engine/CTexture.h, below).
  Declarations only; defined-symbol sets unchanged.
- q4296 family (ported): 0x0049B3B0 / 0x0049B480 / 0x0049B5A0 / 0x0049B7F0 / 0x00495620 / 0x009E1650 modelled CTexture locally with an
  out-of-line ~CTexture (bound ??1CTexture at 0x009F9F70). Replacing only that local class with FABLE_RETAIL_CTEXTURE + engine/CTexture.h
  (work/q4296/tx.py; each row's own leaf and its local vector / owner models unchanged) binds Uninitialise through the header's inline
  destructor; each object gains only ??1CTexture@@QAE@XZ (shared-header inline pick-any COMDAT; ??_GCTexture was already emitted at HEAD).
  A first in-place try on 0x0049B3B0 (inline dtor inside the local model) was reverted for the hold rule before the header route.
- Re-probed at HEAD (modified today): 0x004299A8, 0x00643CDA, 0x00646762 (bind the q4291 claim), 0x00499880, 0x00A62120, 0x00F37170
  (work/reprobe.tsv). Everything else uses NF133's work/NF133_round1/probe.tsv.

## Tests
Rewritten on the shared headers (each links the row; PassPattern unchanged): 0x009BEDC0, 0x009BEF20, 0x009E3790, 0x009F9F70, 0x009FA1C0,
0x00A03B30 (was a print-only stub), 0x009CC2A0. Respelled in place: 0x00499880's standalone test. Unchanged (standalone): 0x009CC240,
0x0049AE30, 0x0049B3B0, 0x0049B480, 0x0049B5A0, 0x0049B7F0, 0x00495620, 0x009E1650, 0x009DE6F0.

## Locks claimed beyond rows.txt
- Changed (rows/): 0x00499880 (was `released`), 0x009CC240, 0x0049AE30, 0x0049B3B0, 0x009DE6F0, 0x0049B480, 0x0049B5A0, 0x0049B7F0,
  0x00495620, 0x009E1650.
- Claimed but left untouched (please release them for the next agent): 0x009CC270, 0x009F61C0, 0x0041BB00, 0x009E15E0, 0x0122A600.
- None is handed off / reserved for Account B in UNMATCHED_ROWS.tsv.

## Files the coordinator may rename
E2_E2_00a03b30.cpp (now CInputEvent::~CInputEvent). No EH funclet or other source includes any changed row (grep of rebuild/src, rebuild/tests).

## Descriptive names
No new made-up names. `ReferenceGetEntryDataSize` is the TU-local scaffolding helper (anonymous namespace, never called), as nf86 / nf88.

## Flags lines
None added: every row grades with its build entry's flags (none of the changed rows has CompilerFlags).

## Rule slips
- Entries were not taken one at a time to a terminal state: I ported every self row first (q4291 .. q4659), then q6308, then returned
  to the free binders of q4659 / q4296 (advisor ordering, so the one completable entry was not starved).
- One Bash call ended non-zero (a `wc` over a missing `..._agentNF9{0}` brace path while reading the required files); it wrote nothing.
- The q4659 three-row vland payload printed one WARN RELOC_CHECK_ERROR for 0x0049AE30 that a solo rerun (work/b7.log), the HEAD run
  (work/h6.log) and the final recheck do not show.

# NF38 notes (round 3, slice land1115ga..land1116p23, 299 entries)

Scratch, tools and decision records: `$S/w22/agentNF38/work/` (dec.tsv = per-entry decisions, later lines win; status.tsv; renames1/2.tsv).
Final recheck: `recheck_final.log`, one run over all 379 changed rows: 2 MATCH + 377 RELOCATION_MATCH, 379 PASS.

## Counts (APPLIED.tsv)
done 30, done-already 7, rejected 32, partial 17, blocked 13, carry 200, deferred 0 (whole entries).
Of the 200 carries, 23 were examined with a specific finding (header lacks the member or layout, body and claim disagree,
port needs header additions) and 177 were only probed (fresh ok/bad/unverified binder counts and the spelling each wrong binder
uses are in the note; the fix is a port of each binder onto its shared header, not attempted). About two thirds of the slice
(200 carry + 17 partial) is therefore still open work.

## Separately droppable set: entry 204 (`_afxPchNil`, 0x0139C900)
126 changed rows bind `?_afxPchNil@@3PADA` (from the recheck objects' symbols; list with each row's old spelling in
`D4_AFXPCHNIL_SET.tsv`): 113 by data respell, the rest by hand or by moving onto engine/CString.h, which spells it itself. Naming decision D4 records 0x0139C900 as HOLD in engine/CString.h (DECISIONS_W25_RESOLVED). If the coordinator
keeps D4 on hold, drop these respells as one set; rows 0x00C4BC80 / 0x00C19FA0 / 0x00C19340 / 0x01224B60 / 0x0122AE00 bind it only
because they now use engine/CString.h (their own renames stand either way).

## Conflicts and holds
- Entries 50-54 (ScreenWidth/... public statics): rejected. The Codex two-PDB evidence names the statics private
  DefaultBackBufferWidth/Height; 0x0052E470 / 0x0052E4D0 moved onto `FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS`.
- Header-wide stand-ins (blocked: header, coordinator decision): 157-159 engine/CThingSearchTools.h ROUGHAREABODY inline body
  spells SearchCount_013b89a8 / SearchWork_013b89ac / SearchID_013b89b0; 163 engine/NVectorMath.h / CMatrix4x4AlignedSSE.h
  declare `g_SSEAvailable`; 70 engine/CEngineMap.h declares `g_pEngineWorldMap` (partial: 21 rows respelled).
- 0x00B242A0 (entries 73 / 168, GPCamera) is held by NF37: deferred, not touched.
- 0x00C27540 (entry 43): a 5-byte jump thunk landed as extern "C" `_Forward`; naming it needs authoring (blocked).
- 0x00A763F0 (entry 294): one real port attempt on engine/CANavQuadTreeNode.h, DIFFER(113v116) (`mov al,1` vs retail
  `mov eax,1`); reverted.
- 0x00C497E0 (entry 261, partial): binder models CCodaFrame / CLockedRingBuffer / the mailbox locally; a port needs header
  additions (CLockedRingBuffer::Pop, a static OnFrameDone) and moves the landed Pop 0x00C4A0E0 family from `struct CCodaFrame` (U) to
  the header class (V). Not attempted.

## Descriptive (no PDB) names used, kept one address one name
g_CString_0143bb3c (0x0143BB3C static CString), g_ForceTestTone (0x00C73A00, as 0x00C73AF0 spells it), CODA_BufferCreate
(0x00C36080), CODA_SourceQueueBuffer (0x00C35A10), CODA_SourcePlay binders, CCodaFrame::GetData (0x00C491D0),
CMailboxQueue::ProcessMessage (0x00C49B90), PKCMailbox::GetResult (0x00C4BC80), PKCDefaultFileManager::ReleaseFileObject
(0x00C19FA0). All are the names libpk/CCodaFrameStreamer.h, libpk/PKCMailbox.h, libpk/PKCDefaultFileManager.h or a landed caller
already use; descriptive-by-library, no SDK-only evidence. GameCODA SDK-only claims with no landed caller binding them were rejected.
The Sal group keeps its existing placeholder-style names where no header names the class (noted in each carry/rejected line).

## PDB self renames this round (RENAMES.tsv section 1, 19 rows)
0x00B6D510 CEngineWaterRenderer::InitialiseDynamicTextures (needs FABLE_RETAIL_CENGINEWATERRENDERER + _CTOR: the flattened default
branch lacks the declaration), 0x0079F730 CTCWillLightning::CLightningBeam::AllChildrenInactive, 0x00556570
NUISystem::CRedefinerList::SetAcceptButton, 0x0077C360 CTCGraphicAppearanceSprite::GetInterpolatedAnimStage, 0x00E4E0E0
CV_BanditTollScript::CTollBandit::SetUpBanditToll, 0x0075C9B0 CKeyCamera::GetEvent, 0x0052E470 / 0x0052E4D0 ConvertX / ConvertY,
0x00E1FEB0 ResetTraderRandomSpeechTime, plus the libpk/CODA descriptive set above.

## Funclet rows that include a changed parent by path
228 landed funclet rows (122 changed parents, listed in work/funclets.tsv) `#include` their parent's landed source by repo path,
so they follow the parent when it lands. They were regraded against the changed parents in one separate run,
`recheck_funclets.log` (parents staged at fov/src/compiled/<landed path>, reached through FABLE_HDR_OVERLAY=hdr:fov/inc,
funclet_offset from each funclet's own comment): all 228 RELOCATION_MATCH, PASS. Their tests are standalone.

## Header opt-ins used (existing; no overlays)
FABLE_RETAIL_CENGINEWATERRENDERER(_CTOR), CTCWILLLIGHTNING_CLASS / _BEAM_FRAMEUPDATE, CREDEFINERLIST_SETACCEPTBUTTON,
CTCGRAPHICAPPEARANCESPRITE_PASS22, CV_BANDITTOLLSCRIPT_TOLLBANDIT, SCRIPT_GETDISTANCEBETWEENTHINGS, CCODAFRAMESTREAMER_PASS23,
PKCMAILBOX_BUFFEREDREADER_PASS23, PKCBUFFEREDREADER_RETURN_CONTEXT_PASS23, CSTRING_BUILDERS / _ALLOCCOPY / _DEFAULT_CTOR,
CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS / _CONVERTXY, CGENERICVERTEX_ORIGINAL_RHW_PASS23, CAIBRAIN_STATEGROUPINFO.

## Section 2 completeness
Section 2 comes from the probe of queue targets plus a full old-vs-new relocation diff of every changed row (work/sec2_extra.tsv):
the header ports also changed 0x0143BB2C (g_pfnLogSink_0143bb2c -> g_pfnAssertHook, rows 0x00C4BC80 / 0x00C19FA0), 0x0099EC30
(CRetailCharString copy ctor -> CCharString copy ctor, 0x0075C9B0) and 0x00C0DEC0 (CDebugStream::operator<< -> CString, 0x00C19340).
Four script Main rows (enumerator respell only) had no old object; their symbols do not change.

## Left as landed in changed rows (disclosed)
0x00B6D510 keeps the landed `#define CPixelFormat CPixelFormatLayout` renaming wrapper around engine/CPixelFormat.h and its
local CEngineResourceManagerNative view (binder 0x01436E98 = ?gResourceManager@@3PAUCEngineResourceManagerNative@@A, wrong,
entry 71's family); only the CWaterDynamicTextures stand-in was replaced.

## Recheck warnings (not failures)
STANDIN_LEAF on the self renames (the catalog still carries the old names; RENAMES section 1 updates them). SHARED_HEADER_AVAILABLE
warnings (986 row/class pairs over 313 rows) are the landed rows' own local models: checked, every pair's class definition is
already in the landed source (CRGBColour, CPersistTraits, ...); none was added here.

## Rule slips
- Wrote dec.tsv and nobad into the repo root through relative paths (moved by the coordinator to root_strays/).
- /home/user/FableDecomp/vc70.pdb (17:46) is probably a by-product of one of my compiles; not removed (no writes under the repo).
- Two helper calls ran with S unset (a lock release and changed.sh); both were no-ops (nothing removed or written).
- Early status.tsv lines carried PASS from single-row grades before the full recheck; recheck_final.log now backs every line.

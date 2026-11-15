# 0x00C5ACB0 paused for Sol 6.1 ultra transfer

Parent asked to finish current compiler only, save checkpoint, then pause. No probes after sol02 have run.

## Selected current candidate
- Source: `rows/00c5acb0/00c5acb0.cpp`, SHA256 `fde5e5cfafea60425104bd8ecb6be54d4b50b2217074745d88577187fa66c872`.
- Selected/best immutable tag: `history/00c5acb0/sol02_original_looping_typed_dispatch`.
- MASKED_MATCH_PENDING_AUDIT: 662/662 bytes, zero strict hunks, 240/240 instructions, no first difference. No claim of audited relocation match or completed row yet.
- Flags `/Oi`, ordinary grade defaults `/O2 /Oy /W3`, RTM VC71.
- Row-local header overlay: `rows/00c5acb0/hdr`; based on **main merged_headers_03** (already contains main patches), plus this row new opt-in/header additions. Do not treat every overlay file as this row's changes.
- Physical include tree for selected grade: `runtime/include_cache/b7be63927af32f521f3e0902e10420875c53853b43f18f73cf5844d02078f38f`.

## Counts/history
2 genuine source families, 3 compiler invocations:
1. sol00_original_sdk_model: true class, original helper/accessor declarations and full reconstructed logic. COMPILE_FAIL due duplicate CCodaObject definitions from independent existing headers.
2. sol01_header_include_repair: same source family, repaired include guards under NEW row macro only. DIFFER661/662, six strict hunks (one semantic field, two constant widths, three push placement changes).
3. sol02_original_looping_typed_dispatch: second source family, fix original IsLooping (status+10, not GetCaps+0c), use -1.0f constant, name original Get3D/Get2D returned pointers before downstream calls. MASKED_MATCH662/662.
Header-only compile repair excluded from genuine source strategies. ROW.json updated accordingly.

## Identity/naming evidence
Input C3DMeshFileXBoundingBoxChunk::LoadData is plainly propagated wrong name. Supplied neighbouring audio source bodies and landed caller CSoftwareVoice::StartSound00C5B250 call it as descriptive ScheduleSound, but that old name is superseded by original SDK evidence:
- True original class/method: **private void CSalSwBaseVoice::CalcParams(const CCodaSource*, CREFTIME)**.
- Exact original leaf `?CalcParams@CSalSwBaseVoice@@AAEXPBVCCodaSource@@_J@Z` in original CODAApi.lib Release/SalSwBaseVoice.obj.
- Original debug CodeView proves private target, real full class/base/member names and accesses, helper signatures, CSalSwVoice intermediate base, public derived 2D/3D CalcParams.
- SDK release body653 bytes vs retail662, same entire logic skeleton and helper/constant symbol list. Compiler/version differences; DO NOT claim SDK byte identity. Need structured SDK-vs-retail comparison/audit as next work.
- Original Anniversary Ego_r.pdb and FableWin.pdb queries for SAL types each return0; saved ORIGINAL_TYPES_Ego_r.json and ORIGINAL_TYPES_FableWin.json. No SAL original names there.
- Exact local official SDK archive path `/workspace/work/pass10_100_20261031/worker/research/gamecoda/CODA/lib/Win32/{CODAApi.lib,CODAApiDbg.lib}`. Only original binaries examined; no leaked source.
- Extraction script `extract_sdk.py` parses Microsoft archive names directly (ar p cannot match backslash member paths). `sdk/RECEIPT.json` records exact archive/member/object hashes. Extracts release+debug SalSwBaseVoice and SalBaseVoice objects.
- Release SalSwBaseVoice.obj SHA256 `5c553c6ae55367cfd3d6e2d8885c1221504c1e400212e00287c7b859da9f9b12`, debug `99e57aab62ad10ac31d449f7956c3784b6f087b44749ffad6f0cde3a8b07f975`.
- `sdk/ORIGINAL_TYPES.json` extracted from DEBUG object (release lacks .debug$T), using `/workspace/work/pass10_100_20261031/worker/research/read_coff_types.py` (pure Python extends query_types PDB decoder).
- `sdk/EXTRA_TYPES.json`: original CSalSwVoice has no fields (only dtor), derives CSalSwBaseVoice; CSalIIRFilter fields m_iA/m_iB/m_iZ, size12.
- CSalPlatform global original release binding `?g_pSal@@3PAVCSalPlatform@@A`; native013? see native selected relocation at offset562 for exact address. Original GetUpdate reads CSalContext protected m_ffUpdate+54; new public getter accesses existing field. Native target function uses global0143BC2C.
- Original SDK2.10 virtual interface broader than TLC: release table InitParams+78, TLC+58. New narrow branch preserves actual TLC order, validated by selected full byte match. CSalVoice retail slots: destructor0, Create4, SetBuffer8, Releasec, IsSoftware10, Is3D14, IsCreated18, PlayBuffer1c, PlayQueue20, Update24, Resume28, Stop2c, ReleaseEnvelopes30, GetPlayCursor34, SetPlayCursor38, EAXSet3c, EAXGet40, InitPlay44, SetPlayBuffer48, GetPlatformObjectSize4c, LoopStart50, BufferEnd54, InitParams58, etc. Existing CCodaObject default branch retains later-SDK query slots; new opt-in hides them. CCodaSource new opt-in hides IsSource, leaves Is3D native slot4. Native vtable012B5C60 confirmed.
- Type JSON raw records are all saved, but some broad output was truncated; only compact relevant class/member summaries personally consumed. Next worker should read precise evidence needed rather than claiming every raw JSON record already reviewed.

## Header changes and caveats
New macro **FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS**, defined in source after required comment header. Existing default and old opt-in branches must remain equivalent; preprocess proof not yet run.
Modified own overlays: CCodaObject.h, CSalVoice.h, CSalBaseVoice.h, CSalSwBaseVoice.h, CSalSwBuffer.h, CCodaSource.h, CSalPlatform.h, CodaEnvironment.h, CCodaContext.h. New headers CSalSwVoice.h, CSalSw2DVoice.h, CSalSw3DVoice.h.
`setup.py` reflects INITIAL header state only; later include/IsLooping fixes applied after it. **Do not rerun setup.py** (would regress selected headers). Current hdr and immutable sol02 hdr are authoritative.
- CSalSwVoice retains real original intermediate base, no storage.
- 2D/3D derived voice headers currently declare actual base and public method only; unused derived tails explicitly not projected. Need assess owner/header standards; no guessed members introduced. If complete projection required, full original DEBUG types available, but TLC/SDK layouts differ in unused tail; do not silently assert SDK donor size as retail.
- Existing CSalBaseVoice includes native-only EAX pointer+50 and correct retail size58; do not replace with SDK debug base size58 solely because same total (debug offsets differ).
- Existing CSalSwBaseVoice fields through m_PlayBuffer+0xb4 reproduce retail used offsets. Old neighbouring local facsimiles describe different unused tail; not copied.
- New helper declarations match original SDK access: CalcLevelEnvelope protected on SwBase (hides Base), CalcMultiFuncEnvelope and LFO helpers private, static CalcExpDelta/CalcPitchDelta protected cdecl on Base.
- Original Get3D/Get2D pointer locals restore native argument evaluation timing. -1.0f restores original 32-bit constant relocation.
- No new asm/intrinsics.

## Required next work before batch acceptance
1. Inventory EVERY emitted function from selected object. If helper bodies emitted, grade/audit each actual native helper or explicitly remove optional visibility preserving main grade. Full selected main relocation audit against exact targets remains mandatory (20 entries, including zero/-1 float contents and ftol2).
2. Full naming audit: original SDK symbols/types + native receiver/calls/layout; search all names/ labels/FSE/strings per README. Create NAMING_EVIDENCE and NAME_FIXES with original corrected target/helper symbols. Existing old landed descriptive SAL spellings likely differ; enumerate honestly.
3. Full verbatim production-body behavior fixture (VC71 /Od /W3 noEHsc and g++64 C++03). Cover no-updates/platform-updates, starting, looping, scheduled release time comparisons incl 64-bit halves, both envelope absence and level/multifunc branches, pitch-only/cutoff-only/both, already-released flags, LFO reinit conditions, 2D/3D/null-source-type pointer dispatch, final starting-bit clear. Dependency doubles explicit. Math log conversion behavior/domain constraints must be stated; no fabricated NaN parity.
4. Header preprocess proof default + each existing affected opt-in profile; preserve all original static assert names/line identities if any shifts (these target headers mostly no line-derived static asserts; inspect).
5. Rebase tiny own header changes on final current main union, regrade selected and all affected main-owned rows (coordinate parent). Do not modify include_cache.
6. Correct metadata/probe/naming documents and freeze batch with immutable attempts/header old-new pairs, exact hashes. 2 strategies already MATCH means no need arbitrary10; user rule says continue plausible distinct strategies if parking, not minimum10 after match.

## Runtime/process state at pause
Current grade sol02 invocation COMPLETE, process session53596 closed; prior sol01 session35819 and sol00 session65803 also complete. No row-owned diagnostic/Wine/compiler processes intentionally running. No tracer used on this row. Shared compiler reaper stays running; never kill shared wineserver/server/compiler. Main owns compiler reaper PID974; all calls source `env.sh` and network IPC additionalpermission. Shell ps snapshot below added separately if needed. No new probe will run by this worker.

## Prior frozen work
Do not edit frozen00750e30; parent reviews `review/sol_batch7_final` (selected sol22_final_frozen, manifest af275a6f74afb280ee9e9f3943e9290b01ee40a8c25a29dd187bc8387cadb892). Earlier Sol rows0040c790/00409fd0/006ec850 independently accepted. Boundary007c977c completed EXTENT_WRONG inside already-landed007c9770, `review/sol_batch8`, manifest c764eb49e04554b75500cbe066361def7643c6d421581579a65bba92b75023a2. Other worker00779ef0, main00e4aa00; avoid these ownerships.

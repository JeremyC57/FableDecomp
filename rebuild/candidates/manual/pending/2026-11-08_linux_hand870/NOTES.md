# Chunk 870 notes (agent870)

## Result

- **Matched:** 16 of 17. 0x00C28640 is MATCH; the other 15 are RELOCATION_MATCH.
- **Parked:** 1, 0x00C1D1C0.
- **Skipped:** 0x00C02470 (zlib, linked from vendor source).

Every matched row has a behaviour test that passes under g++ and under VC7.1 (`behaviour_of`). Every call and data
relocation of each graded leaf was paired with its retail target (calls.py against the `_src` objects); none is wrong.

## Device per row

| Row | Name | Device |
|---|---|---|
| 0x00C08D40 | CSystem::LoadBank | Same-TU `#pragma auto_inline(off)` bodies of PKCAutoDelete::GetChecked 0x00C09920 and Release 0x00C099F0 (the landed text). Their mod/ref lets the failure paths reuse esi for the bank instead of reloading the holder, and fixes the holder/name frame slots. |
| 0x00C0E1F0 | CString::Left | Shared engine/CString.h with the new `FABLE_RETAIL_CSTRING_LEFT` opt-in; `dest` block-scoped, as in the landed Mid. |
| 0x00C174A0 | LoadBankFile | A same-TU static with its same-TU caller 0x00C17640 makes VC7.1 pick the private convention (processor ecx, log edx, name esi, host ebx, settings on the stack). See the bullets after the table. |
| 0x00C18E20 | PKCTRRedblackTree::AddValue | Earlier run. |
| 0x00C19340 | PKCTRRedblackTree::RemoveValue | `y` declared at function scope next to `z`; frame-slot packing follows declaration scope. |
| 0x00C1A510 | PKCDefaultFileManager::OpenFile | Same-TU auto_inline(off) body of the landed map Insert 0x00C1B720; the record's own address `(void* const*)&file` passed as the value. |
| 0x00C1A870 | PKCDefaultFileManager::ReleaseFile | Same-TU auto_inline(off) body of PKCTRRedblackMap::Find (the landed GetFileSize / CloseFile form); shared libpk/PKCProtectionInstance.h. |
| 0x00C1D320 | CMatrix3x4::Invert | Flat E11..E43 model; first minor spelled `E22*E33 - E32*E23`. |
| 0x00C28400 | Defragment | The tail area named (`CHeader* tail`), so the temp becomes a c2 candidate (eax/ecx/edx rotation). |
| 0x00C28640, 0x00C28690, 0x00C28870, 0x00C32CD0 | (pool and array rows) | Earlier run; regraded unchanged. |
| 0x00C313C0 | the 0x012B4D34 record reader's OnRecord | The landed CWaveDataReader::OnRecord shape (inline find-or-create, key homed in the dead argument slot). Each "store the value, then OR the flag" pair is written in that order. |
| 0x00C3BF30 | CAsyncFileReader::ReadRemote | Straight transcription. The loop's `blocksLeft -= blocks` comes before `bytesLeft -= bytes - padding` (that order gives retail's frame slots). |
| 0x00C42850 | CPKRiffReader::Open | Two separate early RIFF returns; the "no loop" zero stores written in both else arms (keeps 0 in ebx); `found = true` inside `if (FindChunk(...))`. |

**0x00C174A0 in detail:**
- The callees ProcessEntry / RunProcessor (landed text) are same-TU statics, and so are the .lut loader 0x00C16F00 and DeleteTrackNodeCommand 0x00C17400.
- 0x00C16F00 must carry a body written from its retail bytes. A stand-in body gets another convention. The segment-reader model must have no char arrays: a char array draws a /GS cookie and changes the register choice.
- 0x00C16F00 and 0x00C17400 take the log first. VC evaluates arguments right to left, so GetChecked runs before the log is pushed or moved to edx, as retail does.

## Parked: 0x00C1D1C0 (InvertMatrix3x4)

- **Best:** md 8 (`differ/00c1d1c0.cpp`).
- **Residue:** two x87 operand orders, m.E41 * out->E11 and m.E41 * out->E12.
- **Probe:** c2 CSEs m.E41 across the three translation rows (temp 24); the out elements are store-forward temps. Over 599 grades of textual and model variants never moved it.
- **Over budget** (150 grades).

## Extents with extra functions (function starts to add to the manifest)

- **0x00C28400 (373 bytes):** the grader cuts it to 310. The extent also holds two unreferenced accessors at 0x00C28540 and 0x00C28560.
- **0x00C0E1F0 (368 bytes):** the grader cuts it to 208. The extent also holds MFC SpanIncluding 0x00C0E2C0 (strspn import 0x0143FFD8) and SpanExcluding 0x00C0E310 (strcspn 0x0143FFF4), both unreferenced and both calling Left.

## For the coordinator

### Names and types

- **One type, two names.** The bank object and the load descriptor carry two landed names each:
  - The bank: CUnprovenPKObject_00c099f0 (PKCAutoDelete instantiations, LoadBank) and CScriptHost (the ProcessEntry / RunProcessor / CBankProcessor TU).
  - The descriptor: CBankLoadDesc_00c08d40 and SProcessorSettings_00c16ce0.
  - Row 0x00C174A0's not-graded stand-in caller casts between them. Unify when one name is proven.
  - The bank loader object is CProcessor_00c09110 (the landed Process_00c172f0 spelling). Both my rows now use it for 0x00C17640 (`?Load@CProcessor_00c09110@@QAI_NPBGPAVCUnprovenPKObject_00c099f0@@PAUCBankLoadDesc_00c08d40@@@Z`).
- **Wrong catalog labels on the bank loaders.** 0x00C17400 (landed DeleteTrackNodeCommand) is the .met bank loader; 0x00C17940 (LookupSelectedWaypoint) is its file read. Both labels are propagated; descriptive renames are open (for example LoadMetBank). Row 0x00C174A0 also proves the parameter order of 0x00C17400 (see NAME_FIXES).
- **Inconsistent global spellings.** These globals have many spellings across landed rows (see reloc_bindings):
  - 0x0139C900: g_pchNil / g_pDebugBuffer / _afxPchNil / ...
  - 0x0143BB2C: g_pfnAssertHook / g_pfnErrorReport / g_pfnLogSink_0143bb2c / ...
  - 0x0143BB14: g_lock_0143bb14 / g_settingsLock / ...

  My rows follow each landed neighbour (g_pchNil + g_pfnAssertHook in most; 0x00C19340 / 0x00C1A510 / 0x00C1A870 use g_pDebugBuffer + g_pfnErrorReport). The coordinator should choose one spelling per address.
- **Unbound epsilon.** 0x0139C978 (the matrix epsilon) is unbound; I spell it g_fMatrixInverseEpsilon.

### The CRACBuffer and master-object family (0x00C4xxxx, chunk 871's range; I graded none of them)

- **CRACBuffer.** The PKRACBuffer.cpp accessors called on ReadRemote's CRACBuffer local carry propagated placeholder names (DrawGetEnvironment, PH::h, GetFramesRemaining, E2, ...). NAME_FIXES gives the CRACBuffer spellings, each with the field offset it reads.
- **Master object.**
  - 0x00C4CC90 builds it (under the landed name CreateConsoleVariable_00c4cc90).
  - 0x00C4C8B0 / 0x00C4CAB0 / 0x00C4CD80 are its LoadScriptInfo / LoadSlave / Finalize.
  - Its source file is PKRACMasterObject.cpp; the vendor wave already spells the slave PKRASlaveObject. A real class name (PKRACMasterObject?) needs a second source.
  - I declared LoadScriptInfo / LoadSlave with the argument types ReadRemote passes (block numbers and byte counts), so their mangled names differ from the landed (void* / char*) ones.

### Header debt

- **Local CString models.** The libpk rows still model the libpk trace string locally (`CString` with `operator<<` 0x00C0DEC0 / 0x00C0DF00 / 0x00C0DF10 / 0x00C0DF20), like every landed libpk row. A shared `libpk/` header for it would remove those copies.
- **Local CMatrix3x4.** 0x00C1D320 keeps a local flat CMatrix3x4 (documented in its Shape line): engine/CMatrix3x4.h models the game's inline-only class with C3DVector rows, which gives another CSE order.

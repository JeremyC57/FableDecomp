# Chunk Q1 notes (agentQ1)

**Grading:** every grade ran with `FABLE_HDR_OVERLAY=$A/hdr` exported (gall.sh / tools/gg.sh do it) and with the corrected
extents of `SPLITS.tsv` (tools/q.py `QLEN`: the oracle bytes are cut from, or extended in, the retail exe at the true length).
Rows that need the overlay: 004e0adc, 00f29f60, 00462fac (new headers), 00644263 (CPlayerGui friend opt-in), 00a446a0
(CExceptionExit opt-in). See hdr/README.md.

## Matched (19)
| address | identity | name source |
|---|---|---|
| 004e0adc | CTCGatherPointBase::OnInitialActivate (5 bytes) | Ego_r + FableWin PDB, vtable slot 6 |
| 004ddaa4 | CTCTargetingAI::BulletTimeFrameUpdate (5) | FableWin PDB, vtable slot 10 |
| 00c57490 | CVtable_012b8ed4::Close (5) | descriptive (libpk, no PDB/RTTI) |
| 00c51a00 | CEAXManager::GetEAXProperty (25) | descriptive |
| 00c088b0 | CVtable_012b1b40::Invoke (7) | descriptive |
| 00c5c090 | CVtable_012b5d40::MixAdd16Ramp (32) | descriptive; calls the landed MMX kernel 0x00C5BEF0 |
| 00c5d530 | CVtable_012b5de0::MixAdd16Ramp (32) | descriptive; calls the landed SSE2 kernel 0x00C5D1F0 |
| 00ca53c0 | CBasePin::QueryInterface (15) | strmbase DECLARE_IUNKNOWN, IPin table slot 0 |
| 00c3c4b0 | CVtable_012b4f74::SetDesc (18) | descriptive |
| 00a446a0 | CSuspendableProcessMicrothread::ThreadEntry (158, true extent) | Ego_r PDB |
| 00caa470 | CBaseAllocator::QueryInterface (15) | strmbase, table 0x012BF810 slot 0 |
| 00c35460 | CEnvelope_012b37dc::Release (5) | descriptive |
| 00462fac | NUISystem::CUILocaleGraphicsDef::Copy (5) | Ego_r + FableWin PDB |
| 00f29f60 | CRegionScriptDef::Copy (5) | FableWin PDB |
| 00644263 | NPlayerGui::CDrawMoney::ClearPrimitivesIfNecessary (25) | Ego_r + FableWin PDB |
| 00c51eb0 | CEAXManager::SetFXSlotProperty (74) | descriptive |
| 00caf430 | CDeferredCommand::QueryInterface (15) | strmbase, table 0x012BF8B8 slot 0 |
| 00ae5c60 | std::vector<CUnprovenElement_00ae5730>::vector(const vector&) (87) | STLport; element name = the landed 0x00AE5730 row's |
| 00c2e610 | CBankProcessor::AddSampleNameTriggers (395) | descriptive |

**Descriptive (made-up) names**, original found nowhere (libpk / CODA audio has no PDB or RTTI): Close, Invoke, SetDesc,
ChangeEventNotification (slot 3 of 0x012B4F74), Release / Stop (envelope), GetEAXProperty, GetListenerProperty (0x00C4E730),
SetFXSlotProperty, SetSlotProperty (0x00C68F90), CEAXFXSlots, MixAdd16Ramp, AddSampleNameTriggers, m_bSampleNameTriggers,
SDriverRecord::m_sample, SSampleHeader::m_fileName, CString::MakeUpper (0x00C0C910: proven to call MSVCR71 `_strupr`).
The vtable-named classes (CVtable_012b8ed4, CVtable_012b1b40, CVtable_012b4f74, CVtable_012b5de0, CEnvelope_012b37dc) and
`CUnprovenElement_00ae5730` / `CUnprovenMixKernel_00c5bef0` are the spellings the landed neighbours already use for the
same addresses (one address, one name); CVtable_012b5d40 is new in the same scheme (the MMX mix-buffer table).

**Things a reviewer should know**
- 00c5c090 / 00c5d530 carry a `// Retail-asm:` line and include `libpk/MixKernelsSIMD.h`, but the graded thunks hold no asm:
  the landed kernel is repeated in the TU as a `static` only because VC7.1 gives a same-TU static its private convention
  (`initial` in ecx, caller cleanup), which a declaration alone cannot produce. They may grade `retail_asm` at landing.
- 00c3c4b0, 00c51eb0, 00c2e610 and the libpk rows use local class models with `_pad` members, as every landed neighbour of
  those classes does (no shared header exists for libpk's CBankProcessor / CEAXManager / CVtable_012b4f74).
- 00ca53c0 / 00caa470 / 00caf430 model CUnknown locally exactly as the landed CBasePin::AddRef 0x00CA53F0 does (the shared
  engine/CBaseVideoRenderer.h CUnknown has no GetOwner()); no `// Vendor-proof:` line was added (lib_verify not run).
- 00f29f60 / 00462fac call a *declared* copy assignment: retail keeps the implicit operator= out of line right behind Copy and
  tail-jumps to it; the row's bytes are just the `jmp`.
- 00644263: the shared NPlayerGui_CDrawBase.h declares ClearPrimitives before ClearPrimitivesIfNecessary, but the retail
  CDraw* vtables hold ClearPrimitivesIfNecessary in slot 0 (0x0125A544 / 550 / 55C / 568) and 0x00442760 in slot 1: header
  debt (virtual order), not changed here because it does not affect this row's bytes.

## Catalog rows that are not functions (15)
Interior fragments of other functions; each has a two-line note in differ/ and a line in STATUS.tsv:
00bfef20, 00bfeea9 (funclets of the landed CRT `eh vector destructor iterator` / __ArrayUnwind), 00f3fbe0 (funclet of
`eh vector constructor iterator` 0x00F3FB96), 004a1c8e, 007fa26c, 007fae34, 0080bfec, 009277cb, 0080aad1, 00a46594 (function
tails), 008f92fd (inside LSocket::Send), 00a446ce / 00a446fa / 00a44719 (inside ThreadEntry, now matched whole), 00a43e30
(mid-instruction). The landed row 0x00A446F4 ("Catch@00a446f4", a constant-return shape) is ThreadEntry's catch funclet and
overlaps the matched 0x00A446A0 extent (NAME_FIXES.tsv).

## Not reached (16)
008f61a0 CHeightMap::Resize (read, not graded: see differ/008f61a0.cpp), 0052f1b0, 008e0a10, 006e3b90, 0080c6d0, 007e6750,
007348c0, 008f8f50 (true length 970, not 927), 004199c0, 008dfb00, 00cdd6b0, 008e20f0, 008e24f0, 00b4ef80, 006cddf0, 00a6a670.

## New function starts exposed (SPLITS.tsv)
Thirty-odd starts inside the over-long catalog rows (vtable slots behind 5-byte thunks, AddRef/Release thunks, the two
operator= bodies, CDrawRenown / CDrawMorality twins, small STLport constructors at 0x00AE5CC0..0x00AE5D70). None was graded
(not in ROWS.tsv).

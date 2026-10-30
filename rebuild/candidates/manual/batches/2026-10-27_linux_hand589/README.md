# Chunk 589 (2026-10-30): 8 of 10 engine-range released pass 9 rows landed (four C++ EH-frame rows among them)

All RELOCATION_MATCH + PASS (g++ and VC7.1). Proven: C3DBoundingBox::BuildFromPoints, std::vector<CVertexShader>::
_M_fill_insert, vector<pair<unsigned long,CCountedPointer<CVertexBufferWin32>>>::insert, strmbase CBaseFilter::Run (/EHsc).
Descriptive (libpk / GameCODA, no PDB): PKCDefaultFileManager::QueueRead (/EHsc), CTimerHost::CTimerHost (/EHsc),
CVtable_012b8ed4::QueueChunk, CCodaVoice::UpdateModulation.
Parked (../../pending/2026-10-27_linux_hand589/differ/, over the 4-grade budget, tests pass): 0x00C4AEB0
PKCBufferedReader::WaitForSlot (284v281: retail homes the CString, a CDebugBreak temporary and the `more` flag in the dead
`slot` argument slot; ours opens an 8-byte frame -- likely a lane gap, see GOTCHAS chunk 146) and 0x00C74BB0
CDspFilter::SetHighPass (293v293: retail loads the sample rate before `fmul [reference]`; six orders, QFE-4035, /G5, /G6 tried).
Coordinator convention (owner rule "names right the first time" + the descriptive-placeholder rule): when the class is proven
and only the method name is descriptive, no address suffix (chunks 586-589); a per-address suffix only when the class itself
is unproven (CVtable_<vt>::X, CUnproven*_<addr>). Labels mark every descriptive name.
NAME_FIXES.tsv: 10 rows queued, incl. wrong landed sources 0x00CAABE0 (CBaseFilter::Pause landed as
CAudioBank::ResumeAllSounds), 0x00C25C20 (a dtor landed as ::Close), 0x00C25A20 (PKCTimerManager ctor stand-in),
0x00B8EC90 (pair operator= placeholder); 0x00C355E0 carries two spellings. CTimerHost may really be PKCTimerManager (its own
trace names PKCTimerManager.cpp). Header gaps: CVertexShader.h is flattened and data-only; no headers for C3DVector /
C3DBoundingBox, CCountedPointer, libpk / GameCODA / strmbase.

# Hand chunk 589 (C3DBoundingBox .. CBaseFilter): 8 of 10 byte-exact

8 RELOCATION_MATCH (no plain MATCH, no retail_asm), 2 parked; every test PASS under g++ and VC7.1 (the two differ tests too).
All call/data relocation targets checked against the retail bytes (objdump -r of each object; every callee address correct).

## What matched (8)
- **C3DBoundingBox::BuildFromPoints 0x00ADED70** (proven: Ego_r + FableWin PDB name sets; bound by 0x00ADEEA0). Zero box
  for `num_points == 0.0f`, else min/max over `for (unsigned long i = 0; i < num_points; ++i)` with `a < b ? a : b` /
  `a > b ? a : b` per component (the float compare of an unsigned counter gives the 2^32 fixup constant 0x0122DCB4). 1 grade.
- **std::vector<CVertexShader>::_M_fill_insert 0x00B6C5D0** (proven: bound by the landed resize 0x00B6CBD0). STLport 4.6
  vector.c verbatim with inline `__copy_backward_ptrs` over out-of-line `__copy_backward` (0x00B6B790), same-TU noinline
  `__uninitialized_copy` / `uninitialized_fill_n` / `__copy_backward` / `fill` (live-slot tag rule). Shape: CVertexShader's
  operator= must be the inline FableWin.pdb Release/AddReference body, as in the landed fill 0x00B6B540; then x_copy's inline
  destructor loses its own-vtable store and Release() keeps Data in ecx for `dec [ecx+10h]`. 4 grades.
- **std::vector<pair<unsigned long, CCountedPointer<CVertexBufferWin32> > >::insert(iterator, const T&) 0x00B8F950**
  (proven: the landed _M_insert_overflow 0x00B8F840 and __copy_backward 0x00B8EF60 are this vector's and name this caller;
  only caller CEngineOcclusionManager::SelectFlareTestVertexBuffer 0x00B8DCD0). STLport 4.6 vector.h insert verbatim; pair's
  implicit operator= is called out of line (0x00B8EC90) on its own; _M_insert_overflow needs its real same-TU body (a stub
  body let VC7.1 delete the call). 2 grades.
- **PKCDefaultFileManager::QueueRead 0x00C1A110** (/EHsc; method descriptive). Lock, map Find (same-TU nothrow body under
  auto_inline(off) drops the EH state-0 store), take the handle and ++pending, unlock, build SFileRequest, PKCMailbox::Send;
  on failure relock and --pending. 1 grade.
- **CTimerHost::CTimerHost 0x00C25B10** (/EHsc; class descriptive). Init list `m_timer(rate, arg, target), m_pThread(0)`,
  `new CWorkerThread_00c355e0(this, params)`, libpk EMEM trace (PKCTimerManager.cpp line 10) on a null allocation. The +0
  PKCTimerManager is a member, not a base (no own vtable store). 1 grade.
- **CVtable_012b8ed4::QueueChunk 0x00C57B60** (descriptive). OpenAL stream queue step through the landed CAudioContext lazy
  resolvers (alGetError, alBufferData at 44100 Hz, alSourceQueueBuffers), write-index wrap, ++queued. 1 grade.
- **CCodaVoice::UpdateModulation 0x00C5AB50** (method descriptive; class from the bound SetModulation 0x00C5A9D0). Twin of
  the landed UpdateVibrato 0x00C59890: switch on flags & 0x18000 over the SModulationParams depths. 1 grade.
- **CBaseFilter::Run 0x00CAACA0** (/EHsc; proven: strmbase amfilter.cpp body, only caller CBaseRenderer::Run 0x00CA6987,
  between CBaseFilter::Pause 0x00CAABE0 and the landed FindPin 0x00CAADB0). The landed FindPin strmbase model as is. 1 grade.

## Parked for Fable 5.1 (2)
- 0x00C4AEB0 PKCBufferedReader::WaitForSlot DIFFER(284v281): retail keeps the trace CString, the CDebugBreak temporary and the `more` flag all in the dead `slot` argument slot ([esp+20h], no `sub esp,8`, prologue starts `mov eax,fs:[0]`); ours opens an 8-byte frame, so every [esp+N] is off by 8 | tried: forceinline loop helper, temporary CString through a forceinline Report, `more` at function scope, no inner braces, unsigned char flag, `ok &=`, QFE 4035 | next: lane gap: several short-lived EH locals sharing one dead argument slot (GOTCHAS chunk 146)
- 0x00C74BB0 CDspFilter::SetHighPass DIFFER(293v293): retail loads the sample rate (`mov eax,[esp+14h] ; test eax,eax`) before `fmul [reference]`, ours after it; every other byte matches | tried: six source orders of the pow/clamp/convert, QFE 4035, /G5, /G6 | next: convert the rate to float in a named local before the pow product, or lane gap: x87 scheduling

## Skipped
None.

## Label corrections
- 0x00ADED70 sub_00aded70 -> C3DBoundingBox::BuildFromPoints
- 0x00B6C5D0 Vector_Insert_Generic -> std::vector<CVertexShader>::_M_fill_insert
- 0x00B8F950 Vector_InsertWithMove_Element -> std::vector<pair<unsigned long, CCountedPointer<CVertexBufferWin32> > >::insert
- 0x00C1A110 FileManager_IncrementAndCheckReference -> PKCDefaultFileManager::QueueRead
- 0x00C25B10 CLandscapeBackgroundSubNodeSplitBranch_Ctor -> CTimerHost::CTimerHost
- 0x00C4AEB0 PKCBufferedReader_PrimeBuffer -> PKCBufferedReader::WaitForSlot
- 0x00C57B60 CAudioSource_BufferNextFrame -> CVtable_012b8ed4::QueueChunk
- 0x00C5AB50 CAudioSource_UpdateRotationDamping -> CCodaVoice::UpdateModulation
- 0x00C74BB0 CAudioChannel_ComputePeakFilterCoefficients -> CDspFilter::SetHighPass
- 0x00CAACA0 CAudioBank_SetAudioFormat -> CBaseFilter::Run
- Neighbours (NAME_FIXES.tsv): 0x00CAABE0 (landed CAudioBank_00caabe0::ResumeAllSounds) is CBaseFilter::Pause;
  0x00C25C20 (landed CTimerHost_00c25c20::Close) is ~CTimerHost; 0x00C25A20 (landed OnPostParentRelocation stand-in) is
  the PKCTimerManager ctor; 0x00B8EC90 (landed SCountedRecord_00b8ec90::operator=) is pair<...>::operator=;
  0x00C1A070 / 0x00C1AE50 / 0x00C1B130 respelled onto PKCDefaultFileManager / PKCTRRedblackMap / PKCMailbox<SFileRequest>.
- 0x00C74AB0 (catalog "Shelve"-style label) is SetHighPass's second-order sibling (fptan), not a shelf filter.

## Names and evidence
- Proven (two sources): C3DBoundingBox::BuildFromPoints (PDB name sets + binding 0x00ADEEA0); vector<CVertexShader>::
  _M_fill_insert (binding from resize 0x00B6CBD0 + STLport source + callee rows); vector<pair<...>>::insert (callee rows +
  sole caller's vector at CEngineOcclusionManager+0x24054 + STLport source); CBaseFilter::Run (strmbase source + caller
  CBaseRenderer::Run + neighbour rows).
- Class proven, method descriptive: PKCDefaultFileManager::QueueRead (trace file name + landed dtor/vtable),
  CCodaVoice::UpdateModulation (bound SetModulation spelling + UpdateVibrato twin), PKCBufferedReader::WaitForSlot (trace
  file name PKCBufferedReader.cpp line 84).
- Descriptive: CVtable_012b8ed4::QueueChunk (class as landed), CTimerHost (from the landed Close row's class),
  CDspFilter::SetHighPass. These carry no address suffix (chunk 588 precedent); see LESSONS "Review at landing".

## Header debt
- engine/CVertexShader.h is flattened and data-only (no CShaderBase base, no virtual dtor, no inline operator=), so 0x00B6C5D0
  models CShaderBase / CVertexShader / CVertexShaderData locally as the landed 0x00B6B540 / 0x00B6CBD0 rows do.
- No engine headers for C3DVector / C3DBoundingBox, the libpk classes (PKCDefaultFileManager, PKCMailbox, PKCTimerManager,
  PKCBufferedReader, CString/CDebugBreak), GameCODA (CCodaVoice, CVtable_012b8ed4, CAudioContext, CDspFilter), strmbase
  (CBaseFilter/CBasePin) or CCountedPointer / CCPPointerInfo; all modelled locally from landed rows.
- The landed 0x00C25A20 source is a non-genuine stand-in (stores the vptr through a global `g_vtbl_c25a20`); it should be
  re-authored as `PKCTimerManager::PKCTimerManager(float, long, PKCTimerTarget*)`.

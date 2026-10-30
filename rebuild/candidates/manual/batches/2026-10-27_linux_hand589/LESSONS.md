# Chunk 589 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- An STLport `_M_fill_insert` whose x_copy destructor keeps a stray own-vtable store and splits the refcount decrement
  (`mov esi,[eax+10h]; dec esi; mov [eax+10h],esi`) is missing the element's inline operator=: with operator= only declared,
  the same-TU helpers call unknown code and the store stays; give the class its real inline operator= (here FableWin.pdb
  Release/AddReference) and write the destructor as `if (Data) Data->Release();` -> `dec dword ptr [ecx+10h] ; jne`
  (0x00B6C5D0, 315v295 -> RELOCATION_MATCH).
- A same-TU noinline STLport helper given a stub body that has no effect is deleted with its call: the "same-TU body" for the
  live-slot tag rule must be the real body (0x00B8F950: an empty `_M_insert_overflow` dropped the call, 254v293; the real
  STLport body -> RELOCATION_MATCH).
- STLport `vector::insert(iterator, const T&)` on a pair with a counted pointer calls pair's IMPLICIT operator= out of line
  (0x00B8F950 -> 0x00B8EC90) while `__copy_backward` inlines it; no declared operator= needed.
- A constructor that stores no vtable of its own while the object's +0 part has one: the part is a member (`m_timer(...)`
  in the init list), not a base (0x00C25B10, first grade).
- The sweep1 libpk trace block spellings (CString::operator<< 0x00C0DEC0 / 0x00C0DF00, ~CString 0x00C0BA40, CDebugBreak
  0x00C16750, _afxPchNil 0x0139C900, g_pfnAssertHook 0x0143BB2C) reproduce the EH-framed EMEM / assert traces as is
  (0x00C25B10, 0x00C4AEB0).
- A same-TU nothrow callee under `#pragma auto_inline(off)` removes the EH state store around its call (PKCTRRedblackMap::Find
  in 0x00C1A110).
- The landed FindPin strmbase CBaseFilter model (IBaseFilter subobject +0x0C, m_State +0x14, m_tStart +0x20, m_pLock +0x38)
  matched CBaseFilter::Run on the first grade (0x00CAACA0).
- 64-bit g++ tests: `long` is 64-bit, so `(HRESULT)0x80004005L` is positive; use `int` for HRESULT/long in test models. VC7.1
  test builds reject `std::pow(double, float)` as ambiguous (C2666): cast the arguments to double.

Review at landing:
- 0x00C4AEB0 (8+ probe grades) and 0x00C74BB0 (6+ grades) went past the 4-grade budget before parking; 0x00B6C5D0 matched on
  grade 4.
- Descriptive names without an address suffix (QueueChunk, QueueRead, WaitForSlot, UpdateModulation, CTimerHost,
  CDspFilter::SetHighPass), following chunk 588; the brief's older rule asks for an address suffix. Decide one convention.
- CTimerHost naming: the ctor's own trace names `.\PKCTimerManager.cpp` line 10, so the 0x18-byte outer class may itself be
  libpk's PKCTimerManager and the vtabled +0 part (ThreadMain 0x00C259E0, InitiateShutdown 0x00C25A50, ctor 0x00C25A20)
  another class; kept the landed PKCTimerManager naming of those rows. 0x00C25C20 is ~CTimerHost (NAME_FIXES), not Close.
- 0x00C355E0: the CWorkerThread ctor has two spellings: the landed row's `CWorkerThread_00c355e0(void*, const
  SThreadParams_00c355e0*)` (used here, and the SThreadParams_00c355e0 type is in 0x00C25B10's own leaf) vs the caller
  0x00C48640's binding `??0CWorkerThread_00c48640@@QAE@PAUCAsyncDecoder_00c48640@@PBUSThreadParams_00c48640@@@Z`; pick one.
- Already-landed sources found wrong: 0x00CAABE0 (is CBaseFilter::Pause), 0x00C25C20 (is a destructor), 0x00C25A20
  (stand-in storing the vptr through a global), 0x00B8EC90 (SCountedRecord placeholder for pair::operator=).
- The 0x00B6C5D0 and 0x00B8F950 TUs carry same-TU bodies for their helpers (0x00B6BAB0, 0x00B6BB10, 0x00B6B790, 0x00B6B540;
  0x00B8EF60, 0x00B8F840) that are already landed separately; land only the graded leaf.
- vland (read-only) result: 8 RELOCATION_MATCH + PASS, no RELOC_IDENTITY. Its warnings are expected: RELOC_NAME on catalog
  labels (0x00B6BAB0, 0x00C25A20, 0x00C355E0, 0x00C0DEC0 / 0x00C0DF00), and SHARED_HEADER_AVAILABLE CBaseObject on 0x00CAACA0,
  a name clash: that row's CBaseObject is DirectShow strmbase's (combase.h, as in the landed FindPin 0x00CAADB0), not the
  engine's CBaseObject (engine/CBaseObject.h, vtable 0x0129A7D8).

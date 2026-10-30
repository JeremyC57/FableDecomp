# Chunk 592 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **A struct returned by value from a template queue** (0x00C48E00 PKCStaticQueue<SDecoderMessage>::Pop): when one member
  has a user copy ctor, VC7.1 emits the implicit copy ctor out of line. Retail inlines the memberwise copy, so give the
  struct a `__forceinline` memberwise user copy ctor (228v288 became a match).
- **STLport `_M_fill_insert`: which copy-backward helper is out of line varies per instantiation.**
  - `__copy_backward` is out of line in 0xBCDCA0 / 0xB6C700.
  - `__copy_backward_ptrs` is out of line in 0xB7CAB0.
  - Read the retail callee's argument count (5 arguments with the `int*` distance = `__copy_backward`; 4 = `_ptrs`) and
    give exactly that one a same-TU `__declspec(noinline)` body. `__uninitialized_copy` is always a same-TU noinline body
    (live-slot `__false_type` rule).
- **A bit tested through `shr reg,8 ; test dl,1`** (0x00C5B300): this needs an inline `bool` accessor
  (`return (m_flags >> 8) & 1;`). The raw `(m_flags >> 8) & 1` in a condition becomes `test dh,1`. As a byte argument the
  raw expression gives retail's `shr ; and 0FFFFFF01h`.
- **One accessor, inlined at one site and called at another** (0x00C5B300 / 0x00C35D10): the inlined site is a
  hand-written expression (or a different inline accessor). The call goes to the declared-only out-of-line method. This is
  not a lane gap.
- **CCharString arguments in a frame-pointer size NScript TU** (0x00CBF6DC): pass named locals in their own `{ }` block.
  A bare `CCharString("x")` temporary passes the ctor's returned `this` (eax) and adds bytes (301v290).
- **x87 CMatrix3x4 * C3DVector** (0x00AF2640): a `__forceinline` operator* removes the call but keeps a separate result
  temp. Retail writes each row back into the source vector's home slots. Not matched in 4 grades; treat it as a likely lane
  gap of x87 home-slot reuse.

Review at landing:
- PKCStaticQueue method names differ by instantiation: Pop/Push here (0x00C48E00, landed Push 0x00C49010, SFileRequest
  0x00C1B370 / 0x00C1AEC0) versus Take/Put on the CUnprovenStaticQueueWord rows (0x00C49F10 / 0x00C4BEB0). Pick one
  vocabulary.
- 0x00C0DEC0 is spelled two ways: `CDebugStream::operator<<` (Push 0x00C49010, and this row) and `CString::operator<<`
  (the Take rows). This is chunk 590's open item.
- The CEaxContext and CEAXContext spellings both exist across the EAX rows.
- The sound argument has two names. CCodaVoice::Play / CAudioSource::BeginSound / CSoftwareVoice::StartSound and this
  row's 0x00C5B300 call it CAudioSound (the struct key in their leaves). The object's own rows (0x00C43xxx: Update,
  ReportQueueInfo, CanUseBuffer, PlaySource) call it CAudioObject. It is one libpk object (+0x10 flags, +0x54 queue,
  +0x78 buffer), and 0x00C5B300 casts to keep ReportQueueInfo's bound CAudioObject spelling.
- The CSoftwareVoice slot-18 SetBuffer loop argument has two types: the landed SetBuffer leaf 0x00C5A2E0 says `bool` (`_N`),
  while CCodaVoice::Play models it as `unsigned char`. Both are virtual calls, so no relocation names them, but one type
  should win.
- The landed 0x00C54BD0 (CAssistanceOwner_00c54bd0::Reset) is a CSoftwareVoice-family `m_ring.Clear(); m_pBuffer = 0;`.
  It is the same tail as 0x00C5B300 and is wrongly named.
- The landed 0x00C48770 models 0x00C16900 as `CIntelligentPointer_00c48770::operator=`, but 0x00C16900 is CHeapBlock's
  copy ctor (operator= is 0x00C16950). Its class is mislabelled too.
- Landed CPSC* rows use v01.. filler vtables. This row (differ/00af2640.cpp) carries a named 32-slot CPSCBase vtable
  model built from the derived classes' slots; it can seed a shared header.
- The libpk catalog labels for every row in this chunk were wrong (see NOTES "Label corrections").

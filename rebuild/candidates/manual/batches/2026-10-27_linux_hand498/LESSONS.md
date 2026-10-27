# Chunk 498 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A `sub dword ptr [esp+4],N ; jmp X::f` row is the compiler's `[thunk]:X::f`adjustor{N}'` for a __stdcall COM method of a
  later interface base (0x00CAA770, N=0x94 = IPinFlowControl +0xA0 minus IPin +0x0C): define the class's destructor in the
  TU so the vftables and their thunks are emitted, and take the Leaf from objdump (`?Release@CDynamicOutputPin@@WJE@AGKXZ`,
  `WJE@` = 148). Name line 1 `Release`adjustor{148}'` so the thunk does not reuse the target's name.
- A DECLARE_IUNKNOWN QueryInterface/AddRef/Release row (`mov eax,[esp+4] ; mov eax,[eax-8] ; mov ecx,[eax] ; mov
  [esp+4],eax ; jmp [ecx+N]`) matches when the in-class inline is declared and defined out of the class (same symbol)
  (0x00CA9700).
- A base NonDelegatingQueryInterface reached by a qualified call inlines when its body is defined earlier in the same TU,
  giving retail's single compare chain with one tail jump to CUnknown (CPosPassThru 0x00CAD380 over CMediaPosition).
- strmbase SDK drop in Fable.exe: CMediaSample::SetMediaTime has no E_POINTER test for a null end; COARefTime is
  `(LONGLONG)(t * UNITS)` without +0.5; the CPosPassThru LONGLONG getters go through GetSeekingLongLong; IDeferredCommand
  slot 4 is Confidence.
- The landed strmbase models (0x00CA9C80, 0x00CA93F0, 0x00CAF090, 0x00CAE170) matched 39 rows at the first grade when
  pasted verbatim; reuse them before writing new ones.

Review at landing:
- 0x00CAA770's TU defines an empty stand-in `CDynamicOutputPin::~CDynamicOutputPin()` only to emit the vftables; the leaf
  is the thunk. Make sure the lander takes only the thunk symbol.
- 0x00CA9700 uses a model variant whose CDynamicOutputPin declares QueryInterface/AddRef/Release instead of the inline
  DECLARE_IUNKNOWN (same symbols and code).
- Already-landed misnames: 0x00CAEC80 `CAdviseHolder_00caec80::Unadvise` is CDeferredCommand::Cancel; 0x00CAECB0
  `gapscan_sub00caecb0` is CDeferredCommand::Confidence; 0x00CA96F0 / 0x00CA9710 are CDynamicOutputPin::QueryInterface /
  Release (catalogued as Thunk_VirtualMethod_0/2).
- reloc_bindings.tsv binds 0x00CAE170 as `NativeDeferredCommandQueue::CheckTime`; the landed symbol is `CCmdQueue::CheckTime`.
- Catalog row 0x00CAA470 holds three functions (0x00CAA470 / 0x00CAA480 / 0x00CAA490, CBaseAllocator DECLARE_IUNKNOWN).

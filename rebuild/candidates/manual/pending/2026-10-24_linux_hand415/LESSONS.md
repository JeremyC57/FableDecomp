# Chunk 415 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Named locals pick retail's vtable register.** `CSoundSystemLH* system = m_pSystem; system->m_pEngine->...` loads the
  engine vtable into eax and hoists the bank and sample into ecx/edx before `push 0 ; push 0`. The engine-local form
  (`CSoundEngineLH* engine = m_pSystem->m_pEngine`) gives edx and interleaved argument loads under both RTM and QFE.
  Seen at 0x00A0B790/850/900/A40.
- **Forwarders drop the tag argument.** In STLport 4.6, calling `__uninitialized_copy_fill` /
  `__uninitialized_fill_copy` through the inline `uninitialized_copy` / `uninitialized_fill` forwarders makes VC7.1 drop
  the tag argument's `lea edx` (187v191). Direct calls with `__false_type()` restore it (0x00A0EBC0/EC80).
- **A `__fastcall` hidden-pointer C2DVector result needs a named local and a user copy constructor.** Returning the call
  straight passes the hidden pointer through (20v35, 0x00A043A0). This is the 2D twin of the wave-25 C3DVector lesson.
- **The STLport 4.6 deque bodies match verbatim in `_STL`.** That covers 18 rows (0x00A0E2A0..0x00A0FC80).
  - `_M_advance` stays out of line (a declaration only).
  - In `resize`, `operator+(int)` must be declared without a body, because retail calls 0x00A0E980 there while
    `operator[]` inlines the same arithmetic.
  - `pop_back` inlines `_M_pop_back_aux`.
- **The parameterless `CArray<T>::push_back()` is `push_back(T())`.** The one-argument push_back is inline and the
  overflow insert takes an empty 1-byte tag temporary. This gives the exact Ego_r.pdb symbol (0x00A03AF0).
- **`new T*[n + 1]` with no `operator new[]` in the TU binds to the scalar import** `??2@YAPAXI@Z` (0x00BFEA1A), as the
  DirectX word-wrap sample does (0x00A109C0/9F0, 0x00A10720).
- **The DirectX-sample word-wrap helpers are an LTCG lane gap** (IsNonEndingChar 0x00A10170, IsEastAsianChar 0x00A10240,
  IsNonBeginningChar 0x00A0FE80).
  - Retail passes the char in ax, and callers keep ecx/edx live across the calls.
  - Our cdecl form is retail plus one `mov ax,[esp+4]`: every other instruction is identical.
  - Park these after one grade; only a /GL + /LTCG link-and-extract grader can match them.

Review at landing:
- 0x00A0B850 and 0x00A0B900 took 5 grades (one over budget, a confirmation run after the named-local fix).
- Local base models sit where no engine header exists or the header is incomplete; check them against
  `rebuild/include/engine/` before landing:
  - CSoundBankLH / CSoundSystemLH / CSoundEngineLH / CSampleLibraryLH / CSampleQueryLH / CSampleLH (sound slots
    numbered from the callers).
  - CInputEvent with a flattened `Data` union (0x00A043A0).
  - `CHandleBasedMemoryPool::CHandleInfo` (0x00A03AF0).
  - The STLport 4.6 deque model, which every deque-family source repeats.
- 0x00A0EBC0 and 0x00A0EC80 hold same-TU `#pragma auto_inline(off)` stand-in bodies for their callees (0x00A0E3D0 and
  0x00A0E740), which are graded on their own rows. Make sure the lander keeps only the target leaf.
- Errors in already-landed sources:
  - 0x00A0ED80 (`std_copyaux`, "SPair8") is `__copy_backward_aux`, and it calls 0x00A0E850, not 0x00A0E860.
  - 0x00A0EF90 (`std_copy`) is `copy_backward`.
  - The CDequeIterator8 0x00A0E980 comment calls 0x00A0E2A0 `operator+=`; it is `_M_advance`.
  - Landed callers name 0x00A043A0 GetAnalogueVector / GetAnaloguePosition; the PDB name is GetJoystickPosition.
  - The two landed callers disagree on CSoundBankLH vtable slot 1: 0x0042E17C passes a CWideString (matches 0x00A0B630),
    but 0x00A3E430 declares `Load(long bank)`. The 0x00A3E430 declaration should be retyped.
- Chunk rows holding several functions: none. The catalog lengths for 0x00A0E3D0, 0x00A0EA30 and 0x00A0FDB0 include only
  int3 padding.

# Chunk 598 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A libvorbis `static` helper whose retail convention is private (args in eax/edi, `ret 4`) is a same-TU `static __declspec(noinline)` with its real caller in the TU, compiled /Gr: VC7.1 then picks the custom convention itself (floor0_map_lazy_init 0x00CA36E0, todB 0x00C90D40 under 0x00C90D60). Land the leaf only.
- `mov ecx,esi` before a call whose callee never reads ecx means a thiscall member that ignores `this`; a `__stdcall` free function drops the ecx load (0x00C621B0 -> 0x00C62150).
- `mov edx,[eax+14h]`-style field loads in a 3-float copy are a memberwise copy; a struct assignment emits `add ecx,14h` plus pointer-based moves (0x00C621B0).
- DirectSound Lock out-params (ptr1/bytes1/ptr2/bytes2) that reuse retail's dead argument slots are declared in their own block scope; function-scope declarations take other slots, and reordering them does not help (0x00C57140).
- Reading a member into a local right after a memset (`unsigned long requested = m_caps;`) moves the load after the stores and swaps which register holds 1.0f (0x00C42DD0).
- An inline accessor (`g_pEAXManager->GetContext()`) instead of a direct member read changes when the global is loaded relative to argument pushes (0x00C42DD0 `new CEaxSource(...)`).
- libpk float-to-long is `fld ; fistp dword [slot]` on a dead frame/argument slot: retail inline asm (FloatToLong), grade retail_asm. C++ `(long)x` gives __ftol2 and /QIfist gives `fistp qword` (0x00C6A460, 0x00C6DEA0).
- A function that runs one copy of a helper inline and calls the same helper out of line elsewhere (RebuildGain's front pair inline, rear pair -> ComputeGainRamp 0x00C6DE60) is modelled as an inline helper plus explicit calls to the out-of-line row.
- A strmbase (DirectShow BaseClasses) row matches verbatim from amfilter.cpp / renbase.cpp: get the source before byte-RE (0x00CA9510, 0x00CA56E0).

Review at landing:
- 0x00C6E050 parked (frame-slot residue only); test passes, so it is a good candidate for the parked-row sweep.
- NAME_FIXES.tsv:
  - respell 0x00C62150's landed callers (`?GetHandleTable_00c62150@@YGPAUCHandleTable_00c62180@@J@Z`) to the thiscall
    member; the stdcall spelling cannot be right;
  - respell 0x00C6A8B0's `?Build@BufferState@@...` to `?Allocate@CReverbProcessor@@...`.
- Parked callee spellings this chunk binds under the class names proven here:
  - 0x00C75F80: hand444 CReverbTap::Allocate -> CCombFilter::Allocate;
  - 0x00C69A70: hand342 `CReverbProcessor_00c6a460::AllocateMemory` with reference parameters -> CReverbProcessor
    member with pointer parameters;
  - 0x00C755B0 / 0x00C76AB0 / 0x00C76DD0 / 0x00C75050: hand444 class spellings vs CReverbDiffuser / CReverbDelay /
    CReverbPreDelay / CStereoDelayLine.
- Multi-spelled addresses seen in reloc_bindings.tsv (one address, one name is violated already):
  - 0x00C62440 (4 spellings, incl. CAmbientTrigger::GetHandleTable);
  - 0x00C6EBE0 (RebuildGain / UpdateGainRamp);
  - 0x0143BC2C (~10 spellings; this chunk used `class CAudioMaster` g_pAudioMaster, as the landed mixer rows do);
  - 0x0143BB58 (many);
  - 0x00C3C360 CPKObject::operator new (~20).
- 0x00C42DD0's EH unwind funclet deletes through 0x00C33100, a `jmp` thunk to 0x00C3C370. The model binds
  CPKObject::operator delete to 0x00C3C370, so the reloc check passes because the unwind is not part of the graded
  function. Confirm the funclet binding when this lands.
- The twin SetParameters 0x00C6EC80 (landed) models its first argument as `long`; at 0x00C6E050 it is a float pan angle in
  degrees (fsin / fcos of pan * pi/180). Re-check the twin.
- The landed strmbase model (JoinFilterGraph_00ca6110 and kin) declares `IQualityControl::Notify(IBaseFilter*, __int64)`.
  The real signature is `Notify(IBaseFilter*, Quality)` by value (0x18 bytes pushed). This chunk's 0x00CA56E0 corrects it
  locally.
- Some TUs carry stand-ins that must not be landed:
  - 0x00CA36E0 carries a floor0_inverse2 caller;
  - 0x00C90D60 carries the static todB 0x00C90D40;
  - 0x00C6DEA0 carries an inline helper.
  Land only the `// Leaf:` symbol.
- 0x00CA9510: vland's SHARED_HEADER_AVAILABLE CBaseObject warning is a name collision (strmbase vs Fable CBaseObject). Ignore it.

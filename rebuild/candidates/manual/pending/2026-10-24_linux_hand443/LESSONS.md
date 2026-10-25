# Chunk 443 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- In a loop VC7.1 unrolls by 4, an indexed pointer (`acc[i]`) is biased by two elements (`[eax-8]..[eax+4]`), while a
  post-incremented one (`*samples++`) is not. Mixing the two forms in one loop reproduces retail's mixed bias
  (MixDelayTapRampUp 0x00C6CAF0, MixDelayTapRampDown 0x00C6CB80).
- Copying an argument into a named local before another pointer local is formed loads it first. This moves the
  register allocation: `long* out = accumulator;` ahead of `samples = source - delay` puts the gain word in si, as
  retail (MixDelayTap 0x00C6CC10).
- `0.0f == m_member` loads the member first for `fucompp`, and `m_member == 0.0f` loads the constant first
  (CStateVariableFilter::SetParameters 0x00C6BC80).
- A named `float x = p->Gain;` local keeps the value in st0 across a `fcom`, which gives retail's `fcom ; fstp st0` on the
  constant arm. Testing `p->Gain` directly reloads it (SetParametersWithGain 0x00C6BD00).
- A pointer local to an embedded sub-object, formed first, gives retail's early `lea ecx,[eax+94h]` and moves `this` to eax
  (`CReverbStage94* stage = &m_stage94;`, CReverbProcessor::ProcessStage94 0x00C6A180).
- Same-TU `__declspec(noinline)` callee bodies under `auto_inline(off)` let VC7.1 keep ecx alive across a chain of
  thiscalls (CEAXContext::SetReverbFlags 0x00C67A10). They also keep ecx/edx alive around an x87 call
  (SetParameters 0x00C6BC80).
- `long result = 0;` as the first statement gives the early `xor eax,eax` ahead of the argument test
  (CEAXContext::SetSlotFlags 0x00C67430).
- When both arms of an if push the same trailing constants, VC7.1 hoists those pushes above the branch
  (MixVoiceFadeIn 0x00C70240). A cdecl call that takes the caller's own argument count compiles to a tail call that
  rewrites the argument slots (MixVoice 0x00C70390).
- A pointer called through its vtable (`T* p = &m_member; p->Virtual()`) keeps the `mov eax,[esi+N] ; call [eax+4]`
  shape; calling the member by name would devirtualise it (CVoiceEffect::Configure 0x00C6AEF0, parked for asm only).
- Lane gap: in a 4x-unrolled index loop with two induction pointers, VC7.1 orders the two `add` steps differently from
  retail. No source form tried moved them: index, post-increment, `a = a + ...`, do/while, a `scaled` local, operand
  order, local copies (MixGainRampUp/Down 0x00C6DA40 / 0x00C6DAD0 / 0x00C6E790 / 0x00C6E820).
- libpk retail_asm signatures:
  - A dead-slot float store and reload before a bare `fistp dword` is the rounding helper. A `fsub [0.5f]` before it and
    `test ; jge ; inc` after it is the truncation helper.
  - A `__declspec(align(16)) short gains[8]` frame (`push ebx ; mov ebx,esp ; and esp,-16`) with a /GS cookie marks an
    SSE2/MMX inline-asm mixer.
  - An ebp frame around `cvtdq2ps` / `cvtpi2ps` is the same thing for the conversion routines.
  - A `frndint ; fwait ; fscale ; f2xm1` block over C locals is an inline-asm 10^x. VC7.1's own exp expansion uses
    fldl2e and no fwait.
  - Rows: 0x00C6AEF0, 0x00C6B3A0, 0x00C6B8C0, 0x00C6BAE0, 0x00C6BDC0, 0x00C6BE60, 0x00C6C000, 0x00C6CEF0, 0x00C6D370,
    0x00C6D410, 0x00C6DE60, 0x00C6EBA0, 0x00C6EBE0, 0x00C6F0F0.
- libpk LTCG register arguments: the SSE2 ramp mixers 0x00C6AAE0 and 0x00C6D410 take the start gain in ecx and the caller
  pops the stack arguments (callers 0x00C6AD10, 0x00C6D590, 0x00C6D5C0). 0x00C6BE60 also keeps edx live across a call to
  0x00C6BDC0 and uses that callee's full eax. Neither can be written in C++ for VC7.1.

Review at landing:
- 14 retail_asm candidates are in differ/ with C++ reference bodies and a `Retail-asm:` line naming the retail
  instructions. Most need only the landed ROUND helper of 0x00BE6790 (`fld value ; fistp result`). The truncation rows
  add `value - 0.5f` before it and `if (r < 0) ++r` after it.
  - 0x00C6CEF0, 0x00C6DE60 and 0x00C6EBE0: the C++ around the helper already lines up byte for byte, and 0x00C6AEF0
    almost does. Land these first.
  - 0x00C6D410 and 0x00C6BE60 still carry an LTCG gap after the asm is restored.
- Already-landed source CVtable_012beda4 (dtor 0x00C6E630) models +0x20 as `long m_count`. Rows 0x00C6CEF0, 0x00C6DE60,
  0x00C6EBA0 and 0x00C6EBE0 read it as a float volume (`fld dword [ecx+20h]`, times 32767.0f). The +0x24 byte is the
  flag that 0x00C6EBE0 stores.
- Already-landed sources whose labels look wrong:
  - 0x00C6A5D0 "CEditControlCentre::GetCurrentMask" is the reverb's per-block Process.
  - 0x00C647B0 / 0x00C64940 "CEntity::SetVectorProperty_XY/_Alt" are CEAXSource obstruction/exclusion property setters.
  - 0x00C630D0 "CLight::SetSlotEnabled" and 0x00C67470 "CSkillTable_00c67470" are CEAXContext members.
  - The catalog's CTCCarrying::SetAllCarriedThingsAsBypassFilter at 0x00C6AC90 is a mixer ramp. It calls 0x00C6AAE0
    with ecx = gain.
- 0x00C6DA40/0x00C6E790 and 0x00C6DAD0/0x00C6E820 are byte-identical bodies in two TUs. The original was probably a
  `static` (or anonymous-namespace) helper that each TU expands, so give the second copy internal linkage or a TU-local
  name, so that two external `MixGainRampUp` symbols cannot collide.
- No row needed more than 4 grades. There are no engine headers for libpk, so every model is local.

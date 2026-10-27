# Chunk 496 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A bool built from a call result: `if (!f()) return false; return true;`, a ternary, a named bool and an
  `unsigned char` return all give `test al,al ; setne al`. `return f() != 0;` in a bool function gives
  `neg al ; sbb eax,eax ; neg eax`. For a dword result (`waveOutGetNumDevs`, COM HRESULT) the if form gives
  `test eax,eax ; setne al` or `neg eax ; sbb al,al ; inc al` (0x00C57420, 0x00C5D5F0, 0x00C57380).
- `shr eax,N ; test al,1` on a flag dword comes from an inline `unsigned char` getter of a 1-bit bit field. A direct
  bit-field test or a mask on the dword gives `test byte ptr [ecx+28h],20h` (0x00C597B0).
- A bit set done through a register (`mov eax,[ecx+28h] ; or eax,1 ; mov [ecx+28h],eax`) points to a bit field: the
  libpk voice flag words +0x28 / +0x2C. A lone bit store can still be `or dword ptr [mem]` (0x00C59770, 0x00C5A210).
- A class holding `__int64` members pads its vfptr to 8, so a local model with __int64 members needs `_pad08[8]`, not
  `_pad04[0xc]` (0x00C59770).
- `lo == 0 && hi == 0` over two dword parameters gives `test eax ; jne ; test edx ; jne`; `__int64 == 0` gives
  `mov esi,eax ; or esi,edx` with an esi save (0x00C5A210, 0x00C727B0).
- Six-element short and float clears written as two counted loops are merged by VC7.1 into 0x7FFF7FFF dword stores
  plus one shared 1.0f register. Explicit assignments give word stores, and `optimize("s")` gives fld1 stores
  (0x00C5C1B0).
- A float compare that loads the argument before the member (`fld [arg] ; fld [m] ; fucompp`) is `m_x != arg`
  (0x00C6EC80).
- libpk SIMD clears (MMX `pxor/movq/emms`, SSE2 `xorps/movdqa`) through a frame local
  (`mov [esp+4],eax ; mov edi,[esp+4]`) are a C++ pointer local plus an `__asm { mov edi, local ... }` block. They are
  retail_asm and match on the first grade (0x00C6D560, 0x00C6F300, 0x00C701E0, 0x00C70BC0, 0x00C700D0).
- The libpk float mix kernels inline the `__forceinline` fld/fistp FloatToLong helper. They match on the first grade
  as retail_asm (0x00C57F20 / 0x00C57F60 / 0x00C57FC0).
- VC7.1 itself unrolls `for (i < 128) dst[i] += f(src[i])` by 4, with the biased pointers retail shows (0x00C58000,
  0x00C58040, 0x00C580C0).
- One compile can probe several variants: put V1..V4 bodies in one probe file, grade it once, then read every variant
  with `objdump -d` of `$S/wine/q/<addr>/<addr>.obj`. This saved grades on 0x00C597B0, 0x00C57420, 0x00C5A210 and
  0x00C5C1B0.
- xref.py's walk back over a vtable can stop at a code reference to a single slot word (`call [vtbl+14h]` names
  0x012B8EE8). Take the vtable start from the constructor's `mov [esi],offset` store.

Review at landing:
- 0x00C57490 and 0x00C5EE20 each pack a 5-byte `jmp [eax+14h]` slot thunk plus the next function. The catalog must
  re-split them: 0x00C574A0 `lea eax,[ecx+4] ; ret`, and 0x00C5EE30, slot 3 of 0x012B9174.
- The landed 0x00C5A1D0 `CTCStealth_OnCreate` (a `__fastcall` free function) is wrong. It is the CSoftwareVoice
  virtual slot 1 (thiscall `ret 4`, a tail jump to the base slot 1 0x00C596E0).
- The name CAudioSource is landed on two different vtables:
  - 0x012B8AE0, through Setup 0x00C556E0;
  - 0x012BEE40, through 0x00C71F10 / 0x00C72AF0 / 0x00C72C40 and this chunk's 0x00C727B0.
- The landed CDSoundStreamBuffer rows (0x00C57070 / 0x00C570D0) number their slots from 0x012B8D90, but the ctor
  0x00C56D30 stores 0x012B8D98, so their slots 8/9 are really 6/7.
- The landed PKRASlaveObject::PassToSlave 0x00C62E60 declares slot 1 as `OnComplete(void* data, long)`. Both slot-1
  bodies test the first argument as a byte, so it is declared `bool` here. The call is virtual either way.
- The landed EAX destructor 0x00C71CF0 spells 0x0143BEA0 / 0x00C4D740 as `g_pfnEAXSet_0143bea0` /
  `CProcResolver_00c71cf0::Resolve`. These are second names of `g_AudioContext.pfnEAXSet` /
  `CAudioContext::GetProcAddress`.
- g_pCodaSystem 0x0143BC2C has both `U` and `V` CCodaSystem spellings in reloc_bindings.
- The landed models' CMixBuffer +0x1C `m_active` is the output shift (CMixBuffer slot 10 0x00C590E0 shifts by it), and
  the CVtable_012b9174 ctor's +0x604 `m_pCurrent` is a block count.
- The retail_asm rows live in retail_asm/, with tests in `retail_asm/<addr>.test.cpp` and a separate
  payload_retail_asm.json. Land them with `vland.py --land`.

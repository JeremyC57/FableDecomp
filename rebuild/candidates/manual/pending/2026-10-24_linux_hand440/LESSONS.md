# Chunk 440 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Result accumulator.**
  - `long result = -4; if (p && p->q) result = p->q->F(...); return result;` puts `mov eax,-4` ahead of the first `je`,
    and both failure exits share the call's `ret` (0x00C4E380, 0x00C51F00).
  - The early-return form builds its own `mov eax,-4 ; ret` block and comes out 3-5 bytes longer.
  - In a switch, `result = ...; break;` gives the `xor eax,eax ; sub ecx,eax ; je` dispatch. A case that forwards its
    own stack arguments unchanged becomes `pop esi ; jmp` (0x00C52980).
  - Per-case `return` adds 5 bytes.
- **Dead argument slots.** An address-taken out local (`unsigned long dirty; F(..., &dirty)`) is homed in a dead
  argument slot by VC7.1 without help (`lea eax,[esp+10h]` = the slot of an argument read before the call or never
  read: 0x00C4E380, 0x00C51F00, 0x00C52980). No same-TU stand-in is needed.
- **Member addresses.** Retail `lea edi,[ecx+N]` before the first branch, used for every call on a member object while
  a virtual call in another arm still uses `ecx = this`, is a named local `CVoicePool* pool = &m_poolA;` (0x00C4D520,
  0x00C4D5F0). `m_poolA.F()` calls rematerialise the address per call (5 bytes longer).
- **Loads around a switch.** A field loaded once between `dec eax` and `je` of a small switch, after an intervening
  call, is read before the switch: an inline helper taking it by value or a local (0x00C4E1B0). Reading
  `wave->m_channels` inside each case reloads it per case. `cond ? 0x1101 : 0x1103` is
  `setne al ; lea eax,[eax+eax+1101h]`.
- **Bools.** `if (x < y) return false; return true;` gives the byte form `sbb al,al ; inc al`; `return x >= y;` gives
  `sbb eax,eax ; inc eax` (0x00C57070).
- **Stack and store order.**
  - A `__declspec(align(16))` local array gives the `push ebp ; mov ebp,esp ; and esp,-16` frame (0x00C546E0).
  - WAVEFORMATEXTENSIBLE ctor stores follow the source order tag, channels, rate, bits, cbSize, blockAlign, avgBytes,
    validBits, mask, SubFormat (0x00C56EC0). The first grade, in a different order, was DIFFER on store order alone.
  - Absolute value as `if (d < 0) d = -d;` is `sub ; jns ; neg` (0x00C52DA0).
- **Global records.** A 0xa0-byte global record restored from a const default is a struct assignment
  (`mov ecx,28h ; mov esi,src ; mov edi,dst ; rep movsd`, 0x00C4D1F0). Fields of the same record read elsewhere
  (0x0139C9BC = record+0x34) model as members of one extern struct; the relocation addend carries the offset.
- **Library and loader shapes.**
  - strsafe workers are `extern "C" long __stdcall StringCopyWorkerA(...)` (`_StringCopyWorkerA@12`, 0x00C55750). An
    `extern "C" __inline` StringLengthWorkerA in the same TU inlines into StringCatWorkerA (0x00C56090).
  - The OpenAL loader resolves lazily with ONE ternary over the two GetProcAddress forms (0x00C4D790 family of 17).
  - A non-dllimport `strncmp` declaration reaches the 0x00BFEAF8 thunk (0x00C562F0).
- **Re-reads after byte stores.** A global pointer chain re-read after a byte store to `this`
  (`m_ramping = false; g_pAudioMaster->m_pScaler->Ramp(...)`) is reproduced by writing the chain twice (0x00C54EA0).
- **Inline members still emit.** An inline member defined in the class body still emits its own COMDAT even when it is
  inlined (Reset in 0x00C551D0): the `// Leaf:` must name the caller exactly.
- **Retail inline asm.** pushfd/popfd (0x00C52AA0), rdtsc with an `and esp,-8` frame (0x00C52CE0) and frndint / fscale
  / f2xm1 (0x00C53B10) are retail inline-asm bodies: retail_asm candidates. Even the word `__asm` in a comment trips the
  purity grep, so write "inline asm".
- **Check stated lengths.** Several of my header comments said N-2 bytes, missing the `ret imm16` immediate. The
  trimmed oracle length is the authority: check headers against it before hand-over.

Review at landing:
- **Parked.** 4 rows are in differ/:
  - 00c52bb0 (register choice only, a lane gap).
  - 00c52aa0, 00c52ce0 and 00c53b10, all retail_asm candidates under the owner's 2026-10-24 exception. A landing session
    may author them with the minimal retail asm.
- **00c52ad0.** The row is 220 bytes (186 of code plus the jump table at 0x00C52B8C). It grades RELOCATION_MATCH; the
  case-to-string pairing was verified by hand.
- **Already-landed sources with wrong (propagated) labels:**
  - 0x00C3E2A0 "CTCFishingSpot_00c3e2a0" dtor: the audio platform layer's destructor (vtable 0x012B5B80). Its "Cleanup"
    is this chunk's CAudioPlatform::Shutdown 0x00C4D1F0, and its four CSlot members are the voice pools.
  - 0x00C597E0 / 0x00C59820 "CThing::AddToRenderList": a voice's EAX Set/Get property, forwarding its +0x50 handle to
    the EAX manager 0x0143BB70 (this chunk's 0x00C51F00 / 0x00C4E380).
  - 0x00C55030 "CBodyOwner": the mixer voice's Reset (CAudioSource::Reset, inlined into 0x00C551D0).
  - 0x00C56370 "CEngineWaterRenderer" dtor and 0x00C56290 "DestroyWaterGenerator": CAudioContext.
  - 0x00C3DDE0 "CTCHeroStats::CreateScreenEffectAction": the libpk pooled-state factory on 0x0143BB58 (EAX listener
    path).
  - 0x00C3EB60 "CScriptThing::func_00c3eb60": the hardware EAX forwarder (via the pointer 0x0143BB5C).
- **Inconsistent models across landed sources:**
  - Pool helpers. 0x00C3C9A0 is landed as the free `__fastcall DrawGetEnvironment` (file CCombatActionPool_Take); it is
    the pool's GetFreeHead (returns +8). 0x00C3CA30 "Touch" is PopFree. 0x00C3C9B0 is called PushAction, PushActive or
    Add in different files.
  - The 0x0139C988 record is modelled three ways: SBlock160 (0x00C33400), SSubSystemSlot globals (0x00C3FD20) and
    CDefaultSettings (0x00C1C1A0). This chunk models it as SAudioConfig, with the two voice slots at +0x30 / +0x48
    (spare counts +0x34 / +0x4c) and the preallocation counter at +0x90.
- **No engine headers.** None of the classes used here has a `rebuild/include/engine` header (libpk); all models are
  local.

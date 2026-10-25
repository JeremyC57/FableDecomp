# Chunk 441 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **_CIpow is never tail-called in retail.** All 30 references to the import thunk 0x00BFEB40 are `call`s. The lane's
  VC7.1 tail-jumps it (`add esp,4 ; jmp _CIpow`) when a function returns `(float)pow(...)` directly.
  - Fix: give each return path a NAMED float local in its own scope (`{ float step = (float)pow(2.0, x); return step; }`).
    That gives retail's `call ; pop ecx ; ret` (0x00C59DE0, 66v64 -> match).
- **Two divisions by one float local become one reciprocal multiply.** VC7.1 turns `a / f; b / f` into
  `fld1 ; fdiv f` plus two `fmul`s (220 bytes).
  - Fix: re-read the member at each division (`x / (float)m_vibratoPeriod`). Retail keeps two `fdiv [esp+10h]` of a
    homed int-to-float temp (0x00C59C60).
- **Reusing a dead float parameter.** `seconds = (float)SecondsToMixBlocks(seconds)` stores the int result into the dead
  parameter slot and `fild`s it (`mov [esp+10h],eax ; fild`). A named local takes a fresh `push ecx` slot
  (0x00C59D40, 0x00C59D80).
- **Statement order ahead of a `cdq` decides registers.** In a switch arm, `state = 0x80; m_counter = m_period / 2;`
  keeps the phase in esi across the `cdq`/`sar`. With the counter store first, the phase lands in edx (0x00C59890).
  - The same rule applies to ring cursors: `m_queued -= frames;` before `m_readPos += frames;` gives the queued-first
    load into edx and the zero stores through edx (0x00C5CF20).
- **Inline accessors on embedded members materialise `lea ecx` early.** An inline `ReadPointer() const` on the ring at
  +0xB4 gives retail's early `lea ecx,[esi+0B4h]` and moves the 64-bit pointer difference into ebx:edi. Writing the
  same expression on the members directly allocates differently (0x00C5A3C0).
- **Narrow tests need narrow members.** `test byte ptr [esi+12h],1` needs an `unsigned char` member. A dword member
  tested with `& 0x10000` stays `test dword ptr` and VC7.1 does not narrow it (0x00C5C2D0).
- **Byte flags must be separate globals.** Four `bool` globals tested in a row keep four `cmp byte ptr` tests. Put them
  in an array or a struct and VC7.1 merges the first two into one dword load (`test al,al ; test ah,ah`, 40 bytes)
  (0x00C5DA70).
- **VC7.1 lays out overloaded virtuals in reverse declaration order.** Kernels that share a name (`MixAdd(const
  float*)` / `MixAdd(const long*)`) swap vtable slots, so give them distinct names (0x00C58270).
- **Switch case bodies follow source order.** A sparse switch lowers to a byte index table plus a jump table, and VC7.1
  emits the case bodies in SOURCE order. Write the cases in retail's body order.
  - The grader masks the table relocations, so check the targets by hand (0x00C5C980).
- **A counted pointer-array init loop is unrolled and scheduled.** `for (i < 6) p[i] = base + 128 * i` stores +8/+0xC/
  +0x10 before +4. Six explicit assignments keep source order instead (0x00C591F0).
- **Set-up local order is load-bearing in copy loops.** Of the 120 orders of the five set-up locals (src pointer, dst
  pointer, src stride, dst stride, byte count), exactly one gives retail's loads and push order (0x00C5D010).
- **`&(cond ? 0 : derived)->member` is the branchless `neg al ; sbb eax,eax ; not eax ; and ; add <off>`**, homed in
  the dead argument slot (0x00C5C2D0).
- **The retail ROUND idiom is retail_asm.** `fstp [slot] ; fld [slot] ; fistp dword [n]` with a by-value copy homed in
  a dead slot is the engine's inline-asm rounding helper. VC7.1 `(long)x` calls `__ftol2`, and /QIfist emits
  `fistp qword` (0x00C59B00, 0x00C5A500).
- **Whole-body inline SSE2 is recognisable.** An ebp frame in a leaf, `mov edi,edi` loop alignment, and `movdqa`/`paddd`
  (or `stmxcsr`/`ldmxcsr` + `cvtps2dq`) mark an `__asm` function, so park it after one grade (0x00C5D2E0..0x00C5D4B0).
- **Tests:** VC7.1 <math.h> `pow(double, float)` is ambiguous (C2666), so cast both arguments to double. A test
  needing 64-bit integers uses a typedef (`__int64` under `_MSC_VER`, `long long` under g++) (0x00C59DE0,
  0x00C5A3C0).

Review at landing:
- **No row used more than 4 grades** (the maximum is 3). SetVibrato 0x00C59C60 is parked after 2 grades: RTM and QFE-4035
  give the same `fstp ; pop edi` order.
- **Local models:** the voice, ring, mixer, limiter, OpenAL-wrapper and thread rows use local GameCODA models (shared
  text in gen/voice_model.inc.txt). No engine header exists for any of these classes. Consider generating
  `CCodaVoice.h` / `CSoftwareVoice.h` / `CAudioRingBuffer.h` from that model when landing.
- **Twins:** 0x00C5D3F0 and 0x00C5D4B0 are byte-identical (the same reference body). Land them together.
- **Errors in already-landed sources** (the bytes match; the names and types are wrong):
  - Voice classes:
    - Landed 0x00C59FC0 `CMaterialState_00c59fc0::SetAnimatedStage` is `CCodaVoice::StartVibrato`. It calls SetVibrato
      0x00C59C60, then sets `m_vibratoCounter` (+0x48) = SecondsToMixBlocks(vib->delay). Its callee is modelled as
      `long ConvertStageFrames(long)`, but 0x00C59B00 takes a FLOAT (seconds).
    - Landed 0x00C59BA0 `CVtable_012b5c10` is the CCodaVoice constructor. Its base 0x00C4CF20 (vtable 0x012B8778,
      nulls +4/+8) is labelled `CIndexBuffer` by propagation and modelled on engine/CIndexBuffer.h. All 8 callers of
      0x00C4CF20 are GameCODA constructors (0x00C3C593 .. 0x00C59BA3), so it is a GameCODA root base, not the D3D
      index buffer.
    - Landed 0x00C5B1A0 `CLandscapeBackgroundTree_00c5b1a0` is the CSoftwareVoice constructor (vtable 0x012B5C60).
      - Its `CSoundInit` member at +0xB4 is the voice's CAudioRingBuffer (ctor 0x00C5D0A0).
      - Its two `CElement` at +0xDC are the CVoiceFilters (ctor 0x00C6B9A0).
      - `g_matcherA`/`g_matcherB` (0x0143BF88 / 0x0143BF68) are the global mono scratch ring and the 6-channel mix
        ring, and `Register` 0x00C5CE00 is CAudioRingBuffer::Init.
  - Ring buffer and thread:
    - Landed 0x00C5D0A0 `CVtable_012b8fc4` is the CAudioRingBuffer constructor.
    - Landed 0x00C5E1C0 `CHandleOwner_00c5e1c0::CloseHandle` is `CThread::Close` (the same +4 handle as Create
      0x00C5E180).
  - Mixer and limiter:
    - Landed 0x00C581D0 `CalculateTextureChangeFlags` is `CMixBuffer::MixMono16`, the un-ramped twin of 0x00C58210.
      Its "7 inline texture ids" are the six channel pointers plus the channel count +0x20, and "ProcessTexture" slot 3
      is a mix kernel.
    - Landed 0x00C3EFA0 `CParticleSet_00c3efa0` is the GameCODA output device (CMixBuffer base 0x00C591F0, vtable
      0x012B5BE0), with six CLimiters at +0xC34.
    - Landed 0x00C592D0 `CChannelState_00c592d0::Clear` is `CLimiter::Clear`.
    - Landed 0x00C592C0 `CtorSetVtable012b8f64` is the CLimiter destructor.
- **Catalog rows holding several functions:** none in this chunk.

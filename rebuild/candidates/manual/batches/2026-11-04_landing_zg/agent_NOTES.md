# Chunk 769 -- notes

## Results

| address | function | grade | attempts | notes |
|---|---|---|---|---|
| 0x00C54190 | CDecodedStream::SetProperties | RELOCATION_MATCH | 4 | landed setter spellings bound exactly (conflict below) |
| 0x00862AA0 | CModeStandBase::UpdateWalkingAnim | RELOCATION_MATCH | 14 | one-arg inline CMovementSetup ctor for the stand setup (inferred from store order) |
| 0x00C89350 | CReverbDiffuserStage::ProcessRamped | RELOCATION_MATCH | 9 | caller binding respelled (NAME_FIXES) |
| 0x00C12940 | CSoundEventManager::StartEvent | RELOCATION_MATCH | 6 | descriptive; AcquireEvent 0x00C11F00 returns the event |
| 0x00C38D60 | CInstance::Update | RELOCATION_MATCH | 6 | counts unsigned (caller binding respelled) |
| 0x00C47F10 | CAsyncDecoder_00c48640::Run | RELOCATION_MATCH | 6 | CPKDecompressedStream ctor respelled |
| 0x00C12E50 | CSoundEventManager::PlayStreamed | DIFFER(741v741) | 10 | parked, dl=2 |
| 0x00C4DE00 | CAudioSoundBuffer::KeepShadowCopy | DIFFER(934v929) | 11 | parked |
| 0x00C3F9D0 | CUnknownSystem1D8::InitSubSystemB | DIFFER(836v848) | 10 | parked |
| 0x00C6F3E0 | CAudioMixer ProcessChannels | LANE_GAP | 0 | parked: LTCG register-arg callee + MMX copy block |
| 0x00C271F0 | CODA source setup | NOT_GRADED | 0 | out of time |
| 0x00C3BF30 | PKCFile / CRACBuffer read | NOT_GRADED | 0 | out of time |

Read-only vland: 6/6 RELOCATION_MATCH + PASS (`vland_result.txt`, WINS: 6). The first run failed the 0x00C54190 and
0x00862AA0 tests under VC7.1 (`decltype`, `T{...}` brace temporaries); both tests were rewritten in C++03 and pass.

No isolated asm helper, `__asm`, `naked`, `_emit` or `volatile` anywhere (no rounding/asm site in these rows).

## Header overlays / header debt
- No `hdr/engine` overlay change. 0x00862AA0 uses the shared `engine/ETCInterfaceType.h` / `engine/CVectorMap.h` and
  restates CThing / CModeStandBase locally (verifier WARN SHARED_HEADER_AVAILABLE for C3DVector, CTCPhysicsBase, CThing,
  CCountedPointer: debt -- the shared CThing header has no `UsingComponents` bitset / PeekTC out-param form matching retail).
- 0x00C54190 restates C3DVector (WARN SHARED_HEADER_AVAILABLE). 0x00C38D60 restates CString (shared header exists).

## Callee names (descriptive libpk unless noted)
- 0x00C12940: CSoundPlayParameters getters 0x00C1B9C0..0x00C1BB00 with hand595's spellings (+ GetStreamed 0x00C1BAE0,
  Initialise 0x00C1BC90), ApplyTo 0x00C20580, CheckExisting 0x00C11230, AcquireEvent 0x00C11F00, SPKPlaybackRequest()
  0x00C108A0, CPKVoiceParams track bits 0x00C27B70/80/90, CSoundEvent::Initialise 0x00C21C20, IsActive 0x00C20840,
  rand via the msvcrt import.
- 0x00C38D60: five mailbox ports' HasMessage (0x00C393F0, 0x00C395B0, 0x00C39770, 0x00C39930, 0x00C39AF0), CInstance::
  Query/CheckFrameStreamer/Current/Stop/Final, CRegistry_00c38b20::Attach 0x00C483A0 / Detach 0x00C48560,
  CRampedParam_00c3a8f0::IsSettledAtZero (bool) / Advance 0x00C3A7D0, ??0CCodaFrameStreamer_00c49650@@QAE@KJKKM@Z (landed).
- 0x00C47F10: CMailbox_00c48860::Take, the five request views 0x00C47EB0..0x00C47EF0 as one SDecoderRequest, the landed
  reply-queue posts, CPKDecompressedStream (ctor 0x00C62920, ~ 0x00C62750, Seek, Read, GetPosition, GetUserData,
  GetUnitSize, GetRate, GetChannels), C16BitSamplePtr, CCodaFrame::GetData, CIntelligentPointer_00c16900 (copy ctor, Get, dtor).
- 0x00C89350: the landed CReverbDiffuserStage::Process model (CDspOps virtuals, ReadRingScaledRamp, MixRampFromRing,
  CDspGain::Crossfade definer spelling).
- 0x00C54190 conflict (as briefed): the 16 setters it calls are bound under per-setter class spellings
  (CAttribute_00c53xxx, CUnprovenReverbParameters, CUnprovenPendingParams_00c53740) although they act on one pending block;
  this row binds them exactly through reinterpret_cast and proposes one class in NAME_FIXES.

## Parked residues and next ideas
- 0x00C12E50 (dl=2): retail stores the request's +0x14 offset (reloaded from its arg-slot spill) after the +0x20 bank
  store; ours before the +4 handle. Tried every order of the eight field stores (compile-only exhaustive) and 1532 random
  orders including the id; inline Initialise was worse. Next: an inline CPooledResource setter taking (offset, tail) or a
  different field type for +0x14 / +0x20 (pointer vs long) to change the scheduler's dependence order.
- 0x00C4DE00: edi/ebp swap at the upload error check (result vs the alGetError import) splits the `and [eax+8]` flag
  clear. Next: the GetError inline written as a member of the context with the cached pointer in a local of another type,
  or the result converted through a separate inline.
- 0x00C3F9D0: (1) VC7.1 folds the four flag clears into one mask (retail: separate `and` per bit on both words) -- tried
  masks, bitfields, an array of 0x18-byte records, an inline Clear; (2) device pointer copied to ecx before the null test;
  (3) setcc register choice. Next: the flags as members of two different struct types, or cleared through two calls of a
  non-inlined-per-TU helper that VC7.1 still inlines late.
- 0x00C6F3E0: lane gap (callee 0x00C6F1A0 is an LTCG static with ecx as a register argument and cdecl cleanup; MMX
  movq/emms tail) -- needs an LTCG-capable lane.
- 0x00C271F0 / 0x00C3BF30: not graded (time). Triage: 0x00C271F0 sets up a CODA source under lock 0x0143BAFC (~20 stdcall
  CODA callees 0x00C33DC0..0x00C36080, __ftol 0x00BFEA70, floor import 0x014400F4, ret 18h); 0x00C3BF30 is the PKCFile
  read path through CRACBuffer helpers 0x00C4C000..0x00C4C2D0 with the ".\\..." assert line 0x1C.

## C++-attempts table for helper use
None: no row uses an isolated helper.

## GOTCHAS one-liners
- VC7.1 `for(;;) switch` rotates the dispatch to the loop bottom; a `done`-flag `while (!done)` loop jumps back to the state load instead.
- VC7.1 drops empty `case`s that share the default's block (table shrinks to `dec eax ; cmp`); give them their own `done = true; break;` to keep the 0..N table.
- Jump-table case blocks are laid out in source order: order the cases as retail's block addresses.
- `x * 2.0f * k` folds into one constant; `float r = (float)rand() * 2.0f;` then `r * k - 1.0f` in a second local keeps `fadd st,st ; fmul k` (and the next call after it).
- `if (!(x <= 0.0f))` gives `test ah,41h ; jnp`, `if (x > 0.0f)` gives `jne`; `!(x < y)` gives `test ah,5 ; jnp`.
- A small reply posted from two branches: one post statement reached by `goto` keeps one register choice in both duplicated copies.
- Stack slots: a 1-byte empty ack struct packs into a byte hole; retail's 4-byte slot needs a 4-byte record, and a block-scoped one shares the `new` temporary's slot.
- `((n >> 7) + 1) << 7` is simplified to `and ; add 80h`; `(n / 128 + 1) * 128` keeps `shr ; inc ; shl`.
- Retail `rand` is the msvcrt import: declare `extern "C" __declspec(dllimport) int __cdecl rand(void);`.
- Behaviour tests are also compiled with VC7.1 by vland: no `decltype`, no `T{...}` temporaries.

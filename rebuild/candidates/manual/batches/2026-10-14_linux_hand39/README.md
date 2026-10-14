# 2026-10-14_linux_hand39 — hand-authored chunk 39 of the class-labelled 17–96-byte pool

Coordinator-authored (no sub-agent) on the Linux VC7.1 lane. Chunk 39 (`mkchunk.py 39`, 40 rows,
CEnvironment .. CFileInstaller): **36 of 40** landed as `RELOCATION_MATCH`/`MATCH` with behaviour tests.

## Label corrections (manifest labels are propagated across identical bodies)
- 0x00811C30 "CEventHitBy::`scalar deleting destructor'" is `NEntityEvents::CEventHitBase::??_G` (vtable 0x01272344 stored).
- 0x00ADD5F0 (same label) is `CLHFile::??_G` (vtables 0x0129F094 / 0x0129F06C).
- 0x00C43810 "CExclusiveAssert::~CExclusiveAssert" is a marked-buffer owner shutdown (cleanup, clear marks, mark callback, release).
- 0x00CDF430 / 0x00EF2150 / 0x00D9F660 are three `CFadeinSummoner::Init` variants (filter registration + virtual slot 447 / 517 / 510).
- 0x009943D0 / 0x00994360 are `CFileDataInputStream::InternalReadUnbuffered` / `InternalReadNextSourceChunk` (leaf given as the mangled name).

## Lessons (also in GOTCHAS, wave 24i)
- A constructor whose scalar stores all sit AFTER the vtable store initialises member objects with inline constructors
  (`CIndex() : m_value(-1)`, `CLink() : m_p(0)`); plain initialisers or body assignments are scheduled before the vtable store.
- A counter re-read from memory after every access is `volatile`; a flag byte read after the counter store must be `volatile` too.
- `??_G` with an inlined counted-pointer release: both the release and the virtual destructor need `__forceinline`
  (otherwise the release stays out of line, or `??_G` collapses to `call ??1 ; delete`).
- Four bytes loaded before any is stored (one spilled into the dead argument slot `[ebp+0Bh]`) = an inline helper taking the
  four values as arguments; the argument order fixes the load/spill order (here +0x1a, +0x19, +0x18, +0x1b).
- A 20-byte out record read by `0x00C442F0` (five words), not a C3DVector: the frame size tells the local's size.
- `fcomp ; fnstsw ; test ah,5 ; jnp` = "below": the 1.0 return is the below branch.
- A callee with no `add esp` after it inside a loop is `__stdcall` (0x00CA02B0).

## Parked (4; sources in the lane's `differ/`)
- 0x00663340 `GetSwitchDef` returning a CIVCountedPointer: retail copies the temp into the return slot (inc) then destroys the temp
  (dec) through one register with two `[eax+4]` stores; ours NRVO-constructs in place or re-reads the temp (template conversion
  and raw-pointer ctor tried).
- 0x00811190 `CEventKilledCreature` ctor: retail releases the def temp with `dec dword ptr [ecx+4] ; jne`; ours load/dec/store
  (three dtor forms + an inline accessor tried).
- 0x00CA0620 `LookupLocalThemeIndex`: register choice (retail keeps the context in a stack local and the block counter in ebp
  pushed inside the branch; ours the reverse) after the `__stdcall` callee fix (83 v 81).
- 0x00887380 `RestoreDefaultTheme`: the 12-byte fill's third word stored after the call-prep pushes and an immediate byte clear
  while eax is zero (five source shapes tried) — the parked "call-prep scheduling inside inlined store groups" gap.

# Chunk 435 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A scoped lock whose only calls inside are `extern "C"` functions gets NO EH frame even under /EHsc, because VC7.1
  treats extern "C" as nothrow. Declare the CODA API `extern "C" __stdcall`; you do not need "the module builds
  without /EHsc" (0x00C27620).
- An early `return` inside a scoped-lock block, with only extern "C" calls, keeps the EH frame without the state-0
  store: the prologue starts with `mov eax,fs:[0]` and there is one destructor exit. A C++ member callee that may
  throw adds `push -1` and the state-0 store (0x00C276B0 vs 0x00C27550).
- A same-TU body (under auto_inline(off)) of a callee that preserves ecx/edx lets VC7.1 skip reloading `this` or a
  zero register after the call. The stand-in must not be empty, or VC drops the call (0x00C20F60, 0x00C26010,
  0x00C25F40).
- Dinkumware `vector<T*>` pop-front: read through ONE named `iterator first = begin(); request = *first;
  erase(first);`. `front()` plus a second `begin()` reloads `_Myfirst` (0x00C26B20).
- Dinkumware VC7.1 `vector::erase(iterator)`: write the tail copy over raw pointers,
  `copy(_Where.base() + 1, _Mylast, _Where.base())` (scalar memmove). This gives retail's `_Where + 1` in eax and
  `_Mylast` in ecx. Copying through `_Ptrit`, with or without tag dispatch, swaps the pair (0x00C27020).
- Dinkumware `operator[]` = `*(begin() + _Pos)`, with `_Ptrit::operator+` written as
  `{ _Ptrit _Tmp = *this; return (_Tmp += _Off); }`, keeps the slot address in its own register
  (`lea eax,[edx+ecx*4] ; mov [eax],ebx`). A raw `_Myfirst[_Pos]` folds the address into the store (0x00C28580).
- For a returned class with a user ctor, `T v; v = f();` copies the return temporary into the local (a bigger
  frame). `T v = f();` builds in place (0x00C27550).
- 16-bit mask merges: compound assignments on the unsigned short argument (`base &= ~m; base |= g() | m;`,
  `mask |= m_mask;`) and a named `unsigned short own` with `own |= mask & value` give retail's split around the
  call and the 16-bit memory operands. One-expression forms load the mask once or zero-extend into another
  register (0x00C27BC0, 0x00C27D40, 0x00C27D80).
- Assignment order picks argument-load scheduling: assigning the first argument's member first makes VC7.1 load
  both arguments up front (0x00C28240).
- In a three-way count test (`-1` forever, `0` stop, `>0` count down), testing `!= 0` first puts the stop arm off the
  fall-through and tail-duplicates the shared store into the count-down arm (0x00C276B0).
- An unsigned compare against `0xFFFFFFFF` written as `key >= 0xFFFFFFFF` gives retail's `jae`; `==` gives `je`
  (0x00C21EB0).
- Jump-table switches with every case returning lay their case bodies out in source order. Order the `case` labels
  by retail's table targets (0x00C20F60, 0x00C21560).
- The libpk CHandleBasedMemoryPool variant differs from engine/CHandleBasedMemoryPool.h above +0x34: an embedded
  default handler (vptr) at +0x34, the handler pointer at +0x38, FreeLists[256] at +0x3C, and a Dinkumware
  `std::vector<CHeader*> Handles` at +0x43C (`_Myfirst` +0x440). Fields below +0x34 match the engine header.
- The two `vector::erase` rows (0x00C26BC0 / 0x00C27020) are byte-identical in retail, import slot included. Once
  one twin matches, apply its body to the other before spending grades.

Review at landing:
- 0x00C26BC0 is parked with the twin's matched body, not graded against this row (budget spent). Regrade first;
  it should be RELOCATION_MATCH.
- Local base models next to an existing engine header:
  - 0x00C22100, 0x00C28200 and 0x00C28580 model the libpk pool locally, because its layout differs above +0x34.
  - The CStreamBuffer rows 0x00C262E0, 0x00C26390, 0x00C26410 and 0x00C26480 declare a methods-only local
    `CHandleBasedMemoryPool` (Lock 0x00C283C0 / Unlock 0x00C283E0 / Free 0x00C28690) for the panel's pool at
    +0x360.
  - 0x00C27EA0, 0x00C27F80, 0x00C28240 and 0x00C28290 use `engine/CHandleBasedMemoryPool.h` through
    `struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool`. Their leaves therefore mangle as
    `CHandleBasedMemoryPool_Methods`, while line 1 says `CHandleBasedMemoryPool::`.
  - Decide whether the engine header should carry a libpk variant.
- Already-landed rows with fake shapes:
  - 0x00C283C0 `CHandleBasedMemoryPool::Lock` is a `__fastcall` free function with a dummy edx over a local pool
    struct.
  - 0x00C25FF0 `ConstructString` is an extern "C" `__fastcall` with a dummy edx; it is CFader::SetValue.
  - 0x00C27BA0 `vector_EWeaponClass_Umove` is a fake `__fastcall`; it is CMaskedChickenSign::SetWordC.
  - 0x00C25E80 `AddExclusiveObserver` is a `C_00c25e80` store; it is CFader::SetMaxRate.
  - 0x00C35AF0 `operatornew` (`New_c35af0`) is a CODA volume setter.
- Landed CCodaPlayback slots 0x00C275E0 / 0x00C27670:
  - They justify "no EH frame" with "the module builds without /EHsc". The module IS /EHsc (0x00C27550, 0x00C27620
    and 0x00C276B0 all need /EHsc); the missing frame comes from extern "C" CODA callees.
  - They also name the class `CCodaPlayback_<addr>`; unify on CCodaPlayback (vtable 0x012B4034).
- Already-landed names for the same panel item class:
  - 0x00C21620 `CSteeringAgent_c21620` (calls 0x00C21560 `Attach`)
  - 0x00C208E0 "EnterStateAndAttach"
  - The CSoundEvent_* files at 0x00C207A0..0x00C20BE0
  - Unify on chunk 431's CPanelItem.
- The landed CChickenSign accessors carry propagated game names: 0x00C1BAA0 CManager::GetKeyPressed, 0x00C1BBE0
  CThing...SetCurrentMapNumberc and 0x00C1BBF0 EnableNavigator.

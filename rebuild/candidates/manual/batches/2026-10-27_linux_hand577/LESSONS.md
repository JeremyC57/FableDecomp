# Chunk 577 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **One STLport template, per-instance inlining (0x00D34D40 v 0x00E54AA0).** vector<T>::operator= over def elements: retail
  keeps different helpers out of line per element type. vector<CFireHeartPatternDef>: `_M_allocate_and_copy`, `_M_clear`,
  `__copy_ptrs`, `_Destroy`, `_Construct` declared only, `__uninitialized_copy` `__forceinline` (plain inline: an out-of-line
  call, 290v315). vector<CConversation>: `__copy_ptrs` / `__copy` / `__uninitialized_copy` `__forceinline` (plain inline: __copy
  out of line, 341v354), inline placement `_Construct`. Read which helpers retail calls and declare exactly those.
- **A zero-stored `__false_type` tag = no user constructor (0x00D34D40).** `mov byte [esp+1Ch],0` before the out-of-line
  `__copy_ptrs` calls (one shared push for both arms) comes from `struct __false_type { };` + `__false_type()`; the
  LipSync model's `__false_type() { }` drops the store.
- **Counted-pointer assignment read order picks registers (0x00DCEDF0).** Retail `lea eax,[edx+ebp+4] ; mov eax,[eax]` after
  the Info load = `CCPPointerInfo* info = other.Info; T* data = other.Data;` (Data first loads `[edx+ebp+4]` into ecx,
  351v350); and STLport's `operator[] = *(begin() + n)` gives `mov ecx,[ecx] ; lea edx,[ecx+ebp]` (`_M_start + n` gives
  `mov edx,[ecx] ; add edx,ebp`).
- **A CScriptThing return copy with only the CScriptThing vtable store = compiler-generated copy ctor (0x00DCEDF0)**, while a
  local built with the out-of-line `CBaseClass()` 0x0099A2D0 needs `CScriptThing() { }` and an out-of-line base ctor; a by-value
  CScriptThing argument with both vtable stores (0x00E5DE80) needs the user copy ctor `: CBaseClass(), PImp(o.PImp)` with an
  inline `CBaseClass() { }`.
- **Four `==` tests on one `const CCharString&` (0x00E5E240) need Compare's body in the TU** (auto_inline(off), the chunk 576
  0x00F088E0 trick) for retail to keep PStringData in ebx; declared only, each test reloads `[ebx]` (267v261).
- **An unrotated value wait at the end of a thread (0x00F033F0)** is the chunk 576 do-while form with the success arm inside
  (`do { if (Count == 0) { term; Flag = true; term; return; } NewScriptFrame(); } while (!term);`).
- **Spawned-thread rows name themselves:** the `mov [edi+34h], <row>` after a `push <name literal>` + CSpawnedFuncBase ctor
  0x00CDD450 gives the method name directly (0x00E7A410 "DoMission", 0x00F033F0 "WatchForAllGuardsDead").

Review at landing:
- 0x00DCEDF0 took 4 grades (two register-choice details); no other row needed more than 2.
- 0x00E5E240 emits the extra `?Compare@?$CBasicString@D@@KIJPBD0@Z` body in its TU (as the landed 0x00F088E0 does); the
  `// Leaf:` line pins the graded symbol.
- Callee catalog labels to respell (NAME_FIXES.tsv): 0x00D34E80 / 0x00E54C10 `_M_clear` (vland RELOC_NAME warnings),
  0x00CDB7C0 / 0x00CDB790 `_Destroy<T*>`, 0x00E54E40 landed public (QAE) where STLport and its twin 0x00D35020 are IAE.
- Landed CCrateTeamMember rows 0x00DCEC70 / 0x00DCED10 are global structs (`CCrateTeamMember::GoOnPatrol/IsThingCarryingCrate`);
  Ego_r spells them private members of NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember (NAME_FIXES.tsv). The landed
  0x00DCED10 also models the crate vector as an invented `CRetailVector`.
- The landed 0x00D34FD0 / 0x00D34EC0 model CFireHeartPatternDef's PadActive as a 3-pointer `vector<bool>`/byte vector (12
  bytes; Ego_r has Dinkumware vector<bool>, owner table says STLport vector<bool> is 0x14) -- the 0x34 element size fixes it
  at 12 bytes in retail; worth one look when vector<bool> is settled.
- Local CDefClassBase model next to engine/CDefClassBase.h (flattened): `// Layout:` lines + NOTES Header debt.

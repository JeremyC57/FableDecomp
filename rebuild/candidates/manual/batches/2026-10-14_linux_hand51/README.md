# 2026-10-14_linux_hand51 — hand-authored chunk 51 of the class-labelled 17–96-byte pool

Chunk 51 (`mkchunk.py 51`, 40 rows, CInputProcessManager .. CItem) was authored by the session coordinator on the Linux VC7.1 lane;
the lander re-graded every row: **33 of 40** landed as MATCH/RELOCATION_MATCH with behaviour tests (compiled with the lane's cl.exe).
One row is skipped (an LTCG register assumption), six are parked in the lane's `differ/`. The lane notes follow.


Linux VC7.1 lane (Wine + DDK cl.exe), authored by the coordinator. Chunk 51 (`chunk51.tsv`, 40 rows): **33 of 40** graded
`RELOCATION_MATCH`/`MATCH` with behaviour tests (g++ and the lander's cl.exe); one row skipped (LTCG register assumption), six parked.
Sources from `gen.py`, tests from `gen_tests.py`, `payload.json` / `oracle.tsv` from `finish.py`.

## Label corrections
- 0x00E376D0 / 0x00ED0E60 "CInputProcessManager::CInputProcessManager" are two no-RTTI subclasses of the chunk-48 CVtable_012c1648 (vtables
  0x012E35B0 / 0x012F2B0C) with two key words at +0x40/+0x44 and CCharString arrays (five 16-byte entries / four strings).
- 0x00C10C90 "CInputProcessRebootGame::ProcessInput" is an unrelated subsystem update (0x00C21E50 = a slot-1 query on the +0x1f8 object,
  0x00C21EB0 = a lookup, 0x00C216D0 = "ApplyScriptedMapBrush" propagated).
- 0x006925E0 "CInputTypeMouseMovementEvent::operator==" returns a float ratio of two integer offsets (`fild ; fidiv`).
- 0x008FDB27 / 0x008FE95B "CInterestingToVillagersDef" are CAIStateGroup_Clean / CAIStateGroup_ScriptingBase constructors (vtables).
- 0x00810F90 / 0x008D43C0 "CIsHitEventHasHitByThing::operator()" are two different predicates (live hit-by thing; live hit thing with the
  functor's 16-bit id); 0x00810FD0 is `record->m_pointer58.GetPItem()` (labelled "_Cons_val<...>").
- 0x00C21EE0 "CIsThingAlive::operator()" is a registry lookup through the +0x108 object's slot 17; 0x00E241B0 / 0x00EF7AB0
  "~CIsThingAlive" are two CScriptThing-holding functors (vtables 0x012C32F0 / 0x012C2FE8 over the base 0x012C2FF4).
- 0x00C3EAA0 / 0x00C3EB00 "CIsThingConscious::operator()" are the increment / decrement of three state counters at +0x1c8..+0x1d0.
- 0x00A16960 "CItem::CItem" is the copy constructor of a 0x20-byte named counted record; 0x00A7FAF0 / 0x00B8EC90 "CItem::operator=" are a
  12-byte counted-record assignment; 0x00C61FB0 / 0x00C62180 / 0x00C62460 "CItem::GetItemThings" are handle release / place bodies
  (the getters 0x00C61CF0 / 0x00C62150 are `__stdcall` free functions, 0x00C62440 is a member; 0x00C257E0 is C3DVector operator+,
  `__fastcall` with the hidden result in ecx).
- 0x0070C050 "CInitRumble::CInitRumble" (chunk 49) and 0x00447A90 `vfunc_7` = `IsButton(long)`.
- 0x005C150B `CIsSameItemType::operator()` calls CItem::IsSlotFull with ecx left over from the previous call (LTCG knew the callee
  preserves ecx): not authorable on the lane, skipped.
- 0x00A04410 (labelled "CDefaultEngineMemoryInit", landed) is CInputEvent's constructor (0x34 bytes) and 0x00A03B30 its empty destructor.

## Lessons
- A non-virtual destructor in a polymorphic class (`virtual void v00(); ~T();`) keeps the `??_G` deleting destructor out of the TU so the
  extractor's substring match (`~T` in both `??1T` and `??_GT`, longest wins) picks the real body (0x00A0D810, the two functor destructors).
- A by-value counted-pointer temporary read through `.Object` into a raw pointer (`CWeapon* w = GetWeapon().Object;`) releases before the
  member read (0x00982890); its copy constructor/destructor share one null test.
- The CItem def release (`mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0`): the object copied to a second local for the virtual call,
  the count by address (`unsigned long* count = &old->IVRefCount; --*count;`) and the compare through a volatile cast
  (`*(volatile unsigned long*)count == 0`), `__forceinline` under `/Os` (0x005BD931 / 0x005BDE50).
- `movzx` of a 16-bit id compared with a long member = `thing->m_id == m_id` with `unsigned short m_id` (0x008D43C0).
- `shr eax,0Fh ; test al,1` = `(unsigned char)(flags >> 15) & 1` (a plain `(flags >> 15) & 1` folds to `test ah,ah ; jns`); a bit-field
  return type makes the call return through a hidden pointer instead (0x00941BE0).
- `lea ecx,[eax+50h]` for an embedded intelligent pointer = an inline accessor on the record (`GetHitBy()`), `add ecx,50h` otherwise.
- `cmp eax,esi` after two GetPItem calls = the second call's result on the LEFT of `==` with the first taken into a local (0x0065F116).
- The two words of a counted record assignment are read into locals BEFORE the compare (`object`, `count`), else the lane re-reads
  `other` after the release (0x00A7FAF0).
- `C3DVector operator+` (0x00C257E0) returns a class WITH a constructor through `__fastcall`: hidden pointer in ecx, `a` in edx, `b` on the
  stack; a POD result makes the lane pass the hidden pointer on the stack. A `sum` temporary then `position(sum)` reproduces the two
  frame copies (0x00C62460).
- A loop-invariant end pointer held in a register = a local before the loop; a call result consumed by a member chain = the call first.
- `dx*dx + dy*dy` with the left square first on the FPU = `d = dx*dx; d += dy*dy;` (the `a + b` form multiplies the top of stack, `dy`, first).

## Parked (6; sources in differ/)
- 0x005BF00B `CItem::RemoveItem`: retail `xor ebx,ebx ; cmp [eax],ebx ; jge ; mov [eax],ebx ; cmp ebx,ebx ; jne` (one compare serves both
  the clamp and the zero test); the lane fuses the `sub` flags (`push 0 ; pop ebx ; jns`) and re-compares, 48 v 49 (with /Os) or 61 v 49.
- 0x005BF556 `CItem::AddItem`: retail stores the price sum, divides the register value, stores the count, stores the quotient; the lane
  either reloads the sum after the count store (through a reference) or dead-store-eliminates it (71 v 78 / 78 v 78).
- 0x008E2E40 `CIsUsableIciclePoint::operator()`: everything matches but the squared radius lands in a new frame slot (`push ecx`) with the
  position kept in ecx, where retail homes it in the dead argument slot and holds the position in edi (89 v 89).
- 0x00982890 `IsSatisfied`: the count-up register (ecx retail, edx lane), 46 v 46.
- 0x00C10C90 subsystem update: retail keeps two separate exits (the `entry == 0` return before the body, the body after it); the lane
  tail-merges them and inverts the branch (55 v 60) for the nested, early-return and else forms.
- 0x00E376D0 `CVtable_012e35b0(a, b)`: the +0x44-then-+0x40 argument-order stores before the derived vtable (the chunk-49 gap; `volatile`
  on the first key changes nothing), 89 v 89.

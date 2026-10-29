# Chunk 572 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- VC7.1 shares the dtor tail of a scoped STLport list across its exits (each exit does its own `call clear ; push hdr`, then jumps to one `call free ; add esp,4 ; outer dtors ; epilogue`) only under three conditions:
  - every early `return` BEFORE the list scope is its own `if (...) { Terminate(); return; }`;
  - inside the list scope, a single `if (a || b)` is fine;
  - both `_List_base::clear` and `list::push_back` are same-TU noinline bodies, so the sentinel stays in esi / a spill slot.
  - A single `a || b` early return ahead of the list makes every list-scope exit carry its own free and epilogue (+93 bytes at 0x00919AE0).
- When `Mgr().Field->Call(arg)` evaluates GetWorld before the argument push, it comes from an inline accessor (`CWorld::GetMessageEventManager()`, FableWin.pdb). The flattened header field (`GetWorld().PMessageEventManager_Data->`) pushes the argument first. `CCountedPointer::operator->` on the member gives `add eax,60h ; mov eax,[eax]` instead (0x00919AE0).
- `list::size()` written as STLport 4.6's `distance(begin(), end())` (by-reference `__distance` with an iterator copy) gives retail's `mov eax,[esi] ; xor edx,edx ; cmp eax,esi ; mov ecx,eax`. A plain for-loop count schedules `mov ecx,eax` first (0x00919AE0).
- A HasTC test whose byte is stored to the GetTC `has` slot (`shr ; and al,1 ; mov [esp+13h],al ; je`) is a named bool (`bool isStockItem = thing->HasTC(...); if (isStockItem) ...`). An inline `HasTC()` in the `&&` chain gives `test dword [eax+28h],imm` (0x00919AE0).
- Two move loops whose `arrived` / `has` byte slots swap ([esp+12h]/[esp+13h] in one loop, [esp+13h]/[esp+12h] in the other) have one `bool arrived;` declared inside each loop body (0x00919AE0).
- FableWin.pdb (`ghidra_out/fablewin_pdb_names.tsv`) lists inline accessors that Ego_r.pdb inlines away, so look there for getter names before writing a field access:
  - CWorld::GetMessageEventManager
  - CThingCreatureBase::GetPCreatureInteraction
  - CCreatureInteraction::GetParticipants
  - CTCThingOwner::GetObjectList
  - CTCAIScratchpad::GetThankingPhrase
  - CMainGameComponent::GetConstantFPS

- Block layout (0x00919AE0, round 3): VC7.1 places a deferred cold block (an in-loop `if (...) { ...; return; }` whose
  fall-through is the join) at the first chain break after the loop, ONE block per break, earliest in source order. To put a
  post-loop `else` block right after the then-block's `jmp` and a later cold block after the next `jmp`, the earlier cold block
  must be an in-loop `break` block: write the timeout as `{ RemoveMessage(request); break; }` inside the wait loop and only
  `else accepted = false;` after it (retail's `GetWorld ; RemoveMessage ; xor bl,bl` falls into the GetTC that follows).
- `for (;;)` / `while (true)` / `do {} while (true)` loops (rotated by the optimizer) sink every in-loop `return` block to the
  end of the function and move the shared epilogue tail to the first exit; a `while ((x = f()) == 0)` loop keeps retail's
  tail-at-the-end shape. Goto label position, `if (0) { label: }`, inline helpers and scopes never move a cold block.
- The register allocator's phi-copy block (`mov eax,[esp+1Ch]` on a GetTC no-has edge) is inserted after layout: the `jmp`
  before it is not a chain break, so a cold block flushed "at the has-path's jmp" sits between the has-path and that copy.
- A reduced model (one Test() with the loop skeleton, ~2 s per cl.exe run, skeleton printed by tools/skel.py) is the
  cheapest way to map a layout rule; the full 2,220-byte row then confirms the form in one grade.

Review at landing:
- 0x00919AE0 is parked DIFFER(2220v2220) (round 3 closed the block-layout residue; two register-colouring ties remain, see NOTES.md). The row took about 110 probe grades in rounds 1-2 and about 45 full grades plus 60 reduced-model compiles in round 3; the brief lifted the 4-grade cap.
- Local models sit next to existing engine headers: CWorld (engine/CWorld.h flattens the counted pointer and lacks GetMessageEventManager) and CAIStateGroup_BuyFromSeller (flat header). See "Header debt" in NOTES.md.
- Errors in already-landed sources, all listed in NAME_FIXES.tsv with their correct spellings:
  - 0x0091A6F0 IsInWorkHours calls 0x00658B20 as `IsCookTime_00658b20`; it is IsWorkTime.
  - 0x00658A80 is bound as IsCookTime; it is IsStoryTime.
  - 0x00919A70, 0x00919A30, 0x00919A10 and 0x0091A6F0 are landed with non-PDB spellings (free functions or the wrong class key).
  - 0x0091A390 IsRunnable spells 0x01375550 `g_framesPerSecond`, while 568 binds it `?ConstantFPS@CMainGameComponent@@1JA`.
- The manifest labels all of 0x00658A35/A80/B20/B6B as CTCVillage::IsCookTime (propagated).

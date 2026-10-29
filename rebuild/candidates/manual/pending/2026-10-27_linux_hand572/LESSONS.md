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

Review at landing:
- 0x00919AE0 is parked DIFFER(2217v2220). The residue is in NOTES.md. The row took about 70 probe grades; the brief lifted the 4-grade cap.
- Local models sit next to existing engine headers: CWorld (engine/CWorld.h flattens the counted pointer and lacks GetMessageEventManager) and CAIStateGroup_BuyFromSeller (flat header). See "Header debt" in NOTES.md.
- Errors in already-landed sources, all listed in NAME_FIXES.tsv with their correct spellings:
  - 0x0091A6F0 IsInWorkHours calls 0x00658B20 as `IsCookTime_00658b20`; it is IsWorkTime.
  - 0x00658A80 is bound as IsCookTime; it is IsStoryTime.
  - 0x00919A70, 0x00919A30, 0x00919A10 and 0x0091A6F0 are landed with non-PDB spellings (free functions or the wrong class key).
  - 0x0091A390 IsRunnable spells 0x01375550 `g_framesPerSecond`, while 568 binds it `?ConstantFPS@CMainGameComponent@@1JA`.
- The manifest labels all of 0x00658A35/A80/B20/B6B as CTCVillage::IsCookTime (propagated).

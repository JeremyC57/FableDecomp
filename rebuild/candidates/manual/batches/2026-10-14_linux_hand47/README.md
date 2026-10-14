# 2026-10-14_linux_hand47 — agent-authored chunk 47 of the class-labelled 17–96-byte pool

Chunk 47 (`mkchunk.py 47`, 40 rows, CGroundNavigatorExternalInterface .. CHandleBasedMemoryPool) was authored by the session's
single sub-agent from `lane_tools/chunk_pool/BRIEF.md`; the lander re-graded every row: **37 of 40** landed as MATCH/RELOCATION_MATCH
with behaviour tests (compiled with the lane's cl.exe). Parked sources stay in the lane's `differ/`. The agent's notes follow.


Linux VC7.1 lane (Wine + DDK cl.exe). Chunk 47 (`chunk47.tsv`, 40 rows): **37 of 40** graded `MATCH`/`RELOCATION_MATCH`
(12 MATCH, 25 RELOCATION_MATCH) with behaviour tests passing under both g++ (-std=c++98) and the lane's cl.exe (`tall.py`).
Sources from `gen.py` (`src/`), tests from `gen_tests.py` (`test/`), `payload.json` / `oracle.tsv` from `finish.py`.
3 parked in `differ/` (evidence below). No extra flags were needed; three sources carry `#pragma optimize("s", on)`
(0x0048DAA2, 0x0065BE32) or `("y",off)+("s",on)` (0x0065FFCA, parked), two carry a `// Leaf:` (`??G` for the operator, `??0...` for the ctor).

## Label corrections (chunk labels are propagated; the body decides)
- 0x009C7E00 "CGroupEntry::UnloadResource" is **CTextEntry::UnloadResource** (PFileData at retail +0x38 = PDB +0x3c minus one 12-byte CArray, Owner +0x40);
  0x009C7EB0 is the real CGroupEntry one (+0x28 / +0x30). Both ALWAYS return true: the pending case jumps to the shared `mov al,1`
  (`if (!PFileData || PFileData->IsFinished()) release;`), unlike the landed CEmitterData twin which returns false.
- 0x006E8810 "CGuiControlComboBox::ToggleEntryWithName" is **CDefendingCombatantInfo::CCombatRing::GetAttackerProngPosition(CThing*)**: GetAttackerProng
  @ 0x006E87D0 (landed) then the prong's rotated position @ 0x006E7FD0 by hidden pointer with RotationAngle (+0) by value; unit X when no prong.
- 0x0048DAA2 "CGuiControlListBox::SelectEntryFromName" is a CCharString -> `const char*` forwarder to virtual slot 1 returning a class through the hidden pointer (kept the name).
- 0x00405F00 "CGuiControlNumber::SetSLongValue" is a free `__fastcall` **IntToWideString(long)**: CWideString temporary, the wide formatter @ 0x0099BA70 with L"%d" (0x0122DE44),
  copied into the hidden result (ecx), the value in edx. 0x0099AED0 is the folded CWideString/CCharString default constructor.
- 0x0065FFCA "CGuiControlTree::SelectNextEntryByName" is SGI **find_if(_List_iterator, _List_iterator, CIntelligentPointer<const CThing> pred)** under /Gr dispatching to
  `__find_if` @ 0x0065F96D with the input-iterator tag (parked, see below).
- 0x004B8C00 / 0x004B8C50 / 0x0070BA00 / 0x00A84110 "CGuiControlTreePane::SortTreeRecursively" are four **std::list::clear** bodies: a 24-byte record with three strings
  (CCharString +0xc, two CWideStrings), a CWideString, a nested list (clear @ 0x0070B8A0 + free of its head) and a {key, list<CCountedPointer>} pair (clear @ 0x00A80A20).
- 0x00BEFBC0 "CGuiControlTreePane::EnsureExpanded" builds a 28-byte {8-byte key, {2 words, 12-byte vector}} record (vector copy ctor @ 0x005BB346), pushes it
  (@ 0x00BEF9C0, a 28-byte push_back), frees the copy's storage and sets the byte at +0xd (named CEntryTable_00befbc0::AddEntry).
- 0x00A42830 "CGuiInputProcess::AddToInterface" is **CLipSyncDataBank::GetPEntry(unsigned long)** (CBankFile::EntryExists @ 0x009CC190, 8-byte entries at +0x164).
- 0x00C257B0 "CGuiWindow::OnBeginDrag" is `C3DVector __fastcall operator-(const C3DVector&, const C3DVector&)` (hidden result in ecx, `a` in edx).
- 0x00C432F0 "CGuiWindow::OnDelete" is the callback block of the landed CMarkedBufferOwner_00c43810::Shutdown on its own (GetMark @ 0x00C442F0 + the `__stdcall` callback at +0xd4).
- 0x00CA4A70 "CGuiWindow::OnResize" cancels a request through a COM-style `__stdcall` slot 6 of the interface at +0x18, ResetEvent (IAT 0x0143FED4) on +0x54, returns `request == 0`.
- 0x00A65EC0 / 0x00A65F00 "CGuiWindowManager::AddWindow": tail and head inserts of a doubly linked window list (manager +0xc1c / +0xc20, window +8 / +0xc).
- 0x00C3AAC0 "CGuiWindowManager::AddRootWindow" is a constructor over the EH-framed base @ 0x00C4B300 (vtable 0x012B8458, no RTTI) with a {size, 0, 2} parameter record, then @ 0x00C3A9C0.
- 0x00C25C20 "CGuiControlListBoxPane::Close": close @ 0x00C25A50, destroy (@ 0x00C354A0, the landed CScriptModule_00c354a0) + operator delete of +0x14 through a local, null after the join.
- 0x00C14B20 "CHTG_Executioner::OnPredicateFail" is a scoped-lock (0x00C0FFC0/0x00C0FFE0 over 0x0143BB14) flag setter in a non-EH TU.
- CHHUndead::Init (twins 0x00D292E0 / 0x00E95D80) and CHH_CellarDoor::Main use the ALIGNED script slots (`ghidra_out/cgsi_slots_aligned.tsv`): 70 GetHero (CScriptThing&),
  451 GiveThingBestEnemyTarget, 264 GetHealth (float), 447 SetThingAsUsable; `cgsi_vtable_slots.tsv` is off for these.
- CHandleBasedMemoryPool: the engine header's offsets after +0x438 are 4 too high (retail CArray<CHandleInfo> is 12 bytes: Handles {first,last,end} +0x438, the critical
  section at +0x44c, not +0x450); CHeader is 32 bytes {Handle, ?, Size, short Padding, char Type 'N'/'T', char, CHeader** PPrevFreeLink, PNextFree, PPrevPhysical, PNextPhysical}.
  0x00A02410 ResizeArea's parameters are (CHeader* area, long newSize).

## Lessons
- A member forwarder returning a FLOAT still tail-jumps (`jmp [eax+13Ch]`); `float speed = call(); return speed;` restores `push ; call ; ret 4` (0x006CDAE0).
- `mov al,1` / `xor al,al` arms negated by `sete cl ; mov al,cl` = `return !IsAlive();` with the inline helper returning `unsigned char` (a `bool` helper gives `mov eax,1` / `xor eax,eax`) (0x006CDB20).
- The out-parameter `GetInterface(&nav, 7)` template reproduces the single shared frame word (key temporary / never-written component local) and the `add eax,40h`
  on BOTH paths: the absent path reads the uninitialised local, so do not null-initialise it (0x006CDBD0 / 0x006CDC20 / 0x006CDD00; const map -> LowerBound 0x004365B0).
- A C2DVector built from a C3DVector temporary (`C2DVector(const C3DVector&)`, int moves), normalised out of line and returned by value copies X through the FPU and Y as a word
  with a plain `x(o.x), y(o.y)` copy constructor -- the DDK reproduces the asymmetry (0x006CDA50).
- `push dword ptr [esp+14h]` pass-through of a stack argument into a virtual call = `#pragma optimize("s", on)` (0x0048DAA2, 0x0048xxxx TU).
- A delete chain `test edi,edi ; mov ecx,edi ; call ~T ; push edi ; call operator delete` needs the pointer in a LOCAL; the null store after `pop edi` is a bare `m_p = 0;` after the if (0x00C25C20).
- A linked-list insert that loads `*list` ONCE before the store through the new node (`mov esi,[edx] ; test ; ... ; mov [edi],esi ; je`) is a local `head = *list` taken AFTER the back-link store (0x00A01DA0);
  without the local the lane re-reads `[edx]` after the aliasing store.
- A loop that re-reads a CArray's `first` at every iteration head and computes the count as `mov eax,[last] ; sub eax,[first]` is written through the CArray's inline
  `GetCount()` / `operator[]` accessors; plain member expressions let the lane CSE `first` into eax, `volatile` forces register loads (0x00A02AD0).
- In one full expression `PQuest->A(Thing, PQuest->B())` the interface pointer is re-read for the outer call but its VTABLE stays in edi; a following statement reloads both (0x00D292E0).
- Two `if (!IsAborted()) PQuest->Set(Thing, 0/1)` arms duplicated around a flag byte read first = an if/else on the byte with the abort test inside each arm (0x00E768B0).
- The unit fallback `C3DVector(1.0f, 0.0f, 0.0f)` built straight into the hidden result stores 1.0 as an immediate and the zeros from the register that compared the pointer with 0 (0x006E8810).
- A `__fastcall` free function returning a class by value in a /Gr module: hidden result in ecx, the first reference argument in edx, the second on the stack, `ret 4` (0x00C257B0);
  with a class temporary + out-of-line formatter the lane copies the named local out through the copy constructor exactly like retail (0x00405F00).
- A constructor over an EH-framed base that stores no vtable of its own is `__declspec(novtable)` on the derived class; the `{b, 0, 2}` record temporary is filled after its address is pushed (0x00C3AAC0).
- A COM-style call `push edi ; push eax ; call [ecx+18h]` with no ecx set up is a `virtual void __stdcall` slot (`this` pushed as the first argument); a Win32 import used once needs `__declspec(dllimport)` for `call dword ptr [__imp]` (0x00CA4A70).
- A 28-byte stack record copied as two 8-byte halves through ecx/edx then a vector copy constructor = `pair<POD key, value>` with a memberwise value copy constructor; the trailing
  flag store after the record's destructor needs the record in an inner scope (0x00BEFBC0).

## Parked (3; sources in differ/)
- 0x0065FFCA `find_if` (SGI, /Gr, frame pointer): retail homes the empty iterator-category tag in the high byte of the dead `last` argument slot (`lea esi,[ebp+0Fh]`), the lane
  gives it a frame slot (`push ecx ; lea esi,[ebp-1] ; leave`, 65 v 64) for a const-reference tag from a temporary or an inline helper; a by-value tag lands in the dead
  slot but is then pushed (60 v 64); `optimize("gsy",off)` spills everything (78 v 64). Everything else (iterator copies, predicate copy/destroy, hidden pointer) is byte-exact.
- 0x006CDB50 `GetMinPathCost` / 0x006CDB90 `GetMaxPathCost`: retail keeps ONE frame word (the def pointer, overwritten by the float before the release) and releases with
  `mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax ; jne`; the lane allocates two words (`sub esp,8`) and forms `&m_references` (`add eax,4 ; ... ; test edx,edx`) for the destructor
  form, the raw-release form and the direct-decrement form (62 v 52) -- the documented QFE/RTM frame-slot sharing gap. Semantics are fixed (GetSubDef @ 0x006CD2F0 on
  Creature+0x70 into a CDefPointer<CCreatureNavigationDef>, MinPathCost +0x28 / MaxPathCost +0x2c, release through slot 1 at zero).

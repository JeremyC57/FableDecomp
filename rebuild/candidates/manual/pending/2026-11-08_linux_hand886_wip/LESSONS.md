# Chunk 886 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- Wine lane: a FATAL cl.exe error (C1189 from a header `#error "... define FABLE_RETAIL_X"`, or C1075/C1004 from an unclosed
  brace/namespace) makes cl.exe spin with no output until q.py's 600 s timeout; plain C2xxx errors return at once. Run
  `g++ -E -P -x c++ -I<overlay> -Irebuild/include src.cpp` first: it reports the #error in 0.02 s.
- Overlaying stlport/fable_containers.h needs copies of rebuild/include/fable_string.h AND stlport/fable_string.h in the overlay
  too: stlport/fable_string.h includes "fable_containers.h" by its own directory first, so the repo copy otherwise wins.
- `mov [ebp-6xx],hdr` (a dead home store of a list header) then `[hdr+4]`, reload `[hdr+4]` after a call, `[x+4]` again =
  `std::list<T>::reverse_iterator it = l.rbegin(); *it ... *++it` (0x0041F9AA, 0x00420C03, 0x00422023); `l.end()` with `*--it`
  leaves no home store (0x0041EFAB, 0x004226D8).
- VC7.1 compiles a by-reference `CCountedPointer<CComponent>` argument from a named local exactly like the temporary
  (ctor 0x429C15 / call / dtor 0x4291DE): write the named local (portable; no non-const ref to a temporary).
- In a 35 KB size-optimised function, same-TU callees defined before it (setters 0x41CA31.., list begin 0x429A85) are inlined in
  early cases and called in late ones (0x0042247E inline vs 0x00422547 call of the same setter): the caller inline budget, not
  the callee; out-of-TU header-inline accessors (CTCQuestCompletionUI::IsActive) stay inline throughout.
Review at landing:
- Row needed > 4 grades (8 sessions; matched in session 8, RELOCATION_MATCH -- WORKLOG.md Session 8).
- Local models next to existing engine headers: CTCInventory, CTCInventoryStats, CTCInventoryMap, CTCInventoryTrade,
  CTCInventoryClothing, CTCInventoryExperience, CTCInventoryAbilities, CTCQuickAccessMenu, CTCInGameMenu, CPlayerGui, CWorld,
  CQuestManager, CTCBoastUI (see WORKLOG.md open problem 4).
- CUserProfileManager overlay accessors SetNewProfileToUseWASD / WereVideoChangesAccepted read opaque bytes (+0x78 / +0x13C);
  type the members when the layout opt-ins are merged.
Session 2 (2026-11-08):
- A big size-optimised function's biased frame (`push ebp; lea ebp,[esp-74h]; sub esp,N`) comes from `#pragma optimize("s", on)`
  alone; adding `optimize("y", off)` (as the landed neighbours do) gives a plain `mov ebp,esp` frame here.
- VC7.1 c2 packs disjoint-scope object locals (e.g. every block-scoped `CDeletion deletion`) into ONE shared frame slot -- but only
  while the function's count of frame-resident symbols stays under a limit: a probe switch packs at 360 cases and packs nothing
  at 380 (same per-case body); spilled scalars count, enregistered ones barely. Past the limit every object local gets its own
  slot and the frame balloons (0x0041E6D3: 0x8DC vs retail 0x800). `/d2inlS` does not move it. A frame that is uniformly too
  deep with retail sharing [ebp-xxx] across cases = too many symbols in our source, not a missing local. Diagnose with `/FAs`
  (the `_name$N = -off ; size` table before `PROC`): packed locals share one offset.
- A class with a user-declared ctor (CEngineGraphic: PDB ??0CEngineGraphic@@QAE@XZ) returned by value from a __fastcall function
  takes its hidden pointer in ecx (args shift to edx) and the caller reads the temporary in place; as a POD the hidden pointer is
  pushed and the caller copies it out (movsd x3 + movsw for 14 bytes).
- An inline bool getter (`CAction::IsValid()`) compiles to `mov al,[eax+0Ch]; test al,al`; the same member tested directly is
  `cmp byte ptr [eax+0Ch],0`.
Session 3 (2026-11-08) -- corrects the session-2 packing lesson above:
- VC7.1 c2 (13.10.3077) turns its global optimizer OFF for a function whose IL exceeds 10000 tuples (count at 0x1077bdd8,
  `cmp [esp+0Ch],2710h` at 0x1077be49; sets 0x10882b98). That is the "360 vs 380 cases" threshold. Above it ("regime 1") c2
  still packs frame slots, but only for TRACKED symbols: scalars, and objects whose address never reaches an external call.
  An object passed to an out-of-line, user-declared (external) ctor / copy ctor / by-ref param is address-exposed and gets its
  own slot. Retail's 35 KB Action is regime 1 (cutting it under 10000 tuples changes the codegen: CSE, merged tails, 70% vs 94%).
- The fix was the CLASS, not the source size: CDeletion's default and copy ctors are COMPILER-GENERATED in retail (0x42BED4 /
  0x42CD84 are the implicit members, emitted as same-TU COMDATs and called out of line). Declaring them as user-declared
  externals (the FABLE_RETAIL_CDELETION opt-in) gives the same call bytes but makes every `CDeletion deletion` address-exposed
  -> 27 slots and frame 0x8DC. Leaving them implicit -> c2 tracks the objects, packs them all at retail's [ebp-248h] and
  cross-jumps the shared `SetDeletion(copy)/~list` tail like retail (frame 0x804 v 0x800). Probe: probe/s4/gv.py HDR=impl
  (450-case regime-1 switch: implicit = 1 slot; user-declared, same-TU-defined, force-inlined helper, by-ref param = 450 slots).
  RULE: when retail calls a ctor/copy-ctor/dtor that the PDB lists but that is the compiler's implicit member, keep it implicit;
  never model it with a user declaration "because it is out of line".
- Regime-1 frame order: unpacked locals are laid out by reference density (refs / size) ascending from the bottom of the frame
  (rarely used big temporaries deepest, the dense shared scalar slots and the dead-parameter home [ebp+7Ch] nearest ebp);
  equal-density symbols come out in a scrambled (sort-unstable) order, so the exact slot order needs the exact symbol set.
- Measure c2 internals with probe/c2/nl.sh (tuple count, packing, frame) before spending a grade; probe/c2/work/regime.py
  scores any obj against retail (loose/strict) to tell which optimizer regime retail was built in.
- Exposure is about the callee's KIND, not out-of-line-ness: retail keeps ADD_CHILD's list iterator in the dead parameter
  home [ebp+7Ch] while passing `&it` to the out-of-line (not expanded) postfix `--` 0x42899B -- an inline (header-defined)
  callee does not expose; a user-declared external or a non-inline same-TU function does.
- /d2inlS is not monotonic here: S10/S13/S15 inline slightly more than the default S14 and S16 much more; none reproduces
  retail's late out-of-line begin() calls in a 35 KB caller.

## Session 4 (2026-11-08)
- Count the cases against the jump tables before polishing: a whole case (RESET_ASSIGNED_SPELLS, 211) was missing for three
  sessions; 206 distinct jump-table targets / 207 labels is the check (t4/src_cases.txt v t2/order.txt).
- Local class models drift: CTCBase is 0xC bytes, so a derived TC's first member is at +0x0C (CTCHero::WeaponsUsable,
  CTCQuestCompletionUI::Active), not +0x10. Use the shared header (engine/CTCHero.h opt-in) instead of a local model.
- VC7.1 c2 inline decision above the 10000-tuple limit (regime 1, path 0x107537A4; probe/inl/inl.py hooks it under gdb):
  cost = sz - sav0 - sav1 + pen - 10 (leaf callee) - 7 (inline-declared) + 4 (site flag 2), inline iff cost < S (9);
  pen = (sz/2) * N / 9000 once sz > S and N > 9000, N = the caller's IL tuples so far + inline growth (11998 at Action's entry,
  ~28500 at its end). A large callee (sz 18) crosses the threshold at a LOWER N than a small one (sz 13), so retail's split
  (begin rejected late, C2DVector ctor expanded late) has no uniform-N explanation: padding the entry by +6300..7000 tuples
  reproduces the setters / vector[] / list != but over-calls begin and makes the expanded-in-retail sites worse.
- `#pragma inline_depth(0)` is captured per FULL-EXPRESSION (at the statement's end): a region ending in the middle of
  `it != l.end();` still outlines both calls, one starting there outlines neither (probe/mini/m2.cpp). So a loop condition
  `it != c.end()` cannot be split; use a per-TU `__declspec(noinline)` (VC7.1 honours it; the body stays a COMDAT) when every
  site of that callee in the function is called.
- A `__forceinline` callee inside an inline_depth(0) region is still called (probe/mini/m1.cpp): forceinline twins cannot be
  combined with a region in the same statement.
- Once the inline split matched, the frame matched too (0x800): the "one extra unshared 4-byte slot" was an inlined callee's
  temporary, not a declaration-order problem.

## Session 5 (2026-11-08)
- In a regime-1 function (>10000 IL tuples, global optimizer off) a named `bool` local lives in memory: `bool b = x > 0; f(b);`
  gives `seta al; mov byte [ebp+7Ch],al; push dword [ebp+7Ch]`, while `f(x > 0)` gives `seta al; push eax` (0x0041F245, 13 sites).
- A `std::list<T*>::reverse_iterator it = l.rbegin(); ++it;` stores the list header to a home slot and reads [hdr+4]; it is a
  store through a pointer, so it kills the `action.Ptr` CSE temp and the next use reloads [ebp+7Ch] (0x00422101, CHANGE_CHILD_STATE).
- `iterator it; it = m.find(k); if (it != m.end())` with the later loop reusing `it` = a find temp slot copied into the iterator
  slot; `iterator it = m.find(k)` constructs in place and drops a slot (POP_MAP_STATE, frame 0x7FC v 0x800).
- c2 colours by priority, not by first use: the CAction* register is the CSE temp of `action.Ptr`, and it is coloured before
  `game` because its priority is higher. Reordering declarations or first uses of `game` does nothing (v18). The chooser's cost
  array (0x10880680, filled at 0x107833F2) adds weight for uncoloured neighbours' copy preferences and spill*100 for
  neighbours with one register left; the cheapest allowed register in eax,ecx,edx,esi,edi,ebx,ebp order wins.
- Byte totals mislead: adding retail's own memory homes raised ours from 35632 to 35697 (retail 35651) while the instruction
  match rose 92.5% -> 92.7%. Grade by difflib share and per-case counts, not by the size gap.

## Session 7 (2026-11-08)
- A same-TU COMDAT ctor that retail calls at every site: spell it as the real in-class body + `__declspec(noinline)` under
  `#ifdef _MSC_VER` (inline-budget device), not as a declaration only. Above c2's 10000-tuple limit the declaration-only ctor
  exposes the object's address, so it never shares a frame slot (0x0041E6D3 LOAD_PHOTO texture: -3C8h alone v retail -268h;
  frame 0x808 v 0x800); the visible, non-expanded body keeps the object tracked and the frame drops to retail's 0x800.
- In a regime-1 function every added/removed frame symbol reshuffles the density-sorted slot layout: raw instruction share can
  FALL while the code improves (0x0041E6D3: frame fixed, exact 87.0 -> 72.7, regs+frame-masked unchanged 98.4). Grade shape
  fixes with a frame-masked share and track the slot pairing separately (t7/m3.py, t7/fmap.py).
- `count->Value` read at the call (`CActionParamSLONG* count`) gives `push dword [eax+8]`; a `long count = ...->Value` local
  gives `mov eax,[eax+8]; push eax` (0x0041F5D8). An unsigned member tested `<= 0` gives `ja` where `== 0` gives `jne`.

## Session 8 (GOTCHAS-ready, one line each)
- VC7.1 c2 frame packer (regime 1, /GS): nodes listed by size asc / IL weight desc (0x10788642, +1 per IL reference); slots made
  in list order by 0x10791426 (join the latest slot with node.size <= 2*slot.size and no conflict, else new); qsort 0x107ae0c4
  by /GS-buffer flag then density w*1000/size desc (ties unstable); allocated top-down from [ebp+74h-4]. Address-exposed
  nodes conflict with everything. t8/fr/pack.py replays it exactly from a phook.py trace (0x0041E6D3: 284/284 slots).
- The packer's weights are IL references counted BEFORE cross-jumping: code duplicated in the source and merged by c2 still
  weighs once per copy. 0x0041E6D3 ADD_CHILD_AUGMENTATION: GetType/SetGraphicIndex per switch arm (retail's default edge
  0x0041ECB5 skips it) = `_augmentation` w 12 -> 18, slot -656 -> retail -636. Look for a default/fall edge that skips a "shared" tail.
- A declaration-only `const_iterator find(const K&) const` exposes the result object (global packer node); the landed
  FABLE_RETAIL_STLP_TREE_CONSTFIND body is still called out of line in regime 1 (0x0042BEF2) and unexposes it.
- `iterator it = c.begin();` under `#pragma inline_depth(0)` returns into `it` (RVO); `iterator it; it = c.begin();` returns
  into a weight-1 temporary as retail 0x00426CCA; `const iterator& it = c.begin();` gives the bound temporary weight 2.
- Named `CDefPointer<..> x = GetAbilityPDef(..)` per arm adds frame nodes retail lacks; `GetAbilityPDef(..)->Field` read in
  the call matches retail ASSIGN_SPELL_TO_ITEM_SLOT_PC (case 0x00422101).
- /GS keeps pointer-typed nodes (node+7 & 0x20: pointers AND references) out of the dead-parameter front slot; a CSE'd address
  is not flagged. 0x004266B2 LOAD_PHOTO homes `&photo->PTexture` in [ebp+7Ch]: write `photo->PTexture` twice, no
  `CCountedPointer<T>& photoTexture` reference local.
- Cross-jumping (regime 1, #pragma optimize("s", on)): the exit block's predecessors are break-edges in source order, then
  return-edges in source order; of the blocks sharing a tail the LAST in that list survives and the others jump into it.
  A retail survivor that sits before a jumper is a case ending in `return;` (0x0041E6D3: CLOSE_TELEPORT_MENU 0x0041F792,
  LOAD, PLAY_MOVIE, SCROLL_ACTIVE_LIST_DOWN, CLOSE_FRAME, LOAD_FILE_DESC). Mini repro: t8/mini/gen.py (2000 filler cases).
Session 8 (2026-11-09) -- the row MATCHES (RELOCATION_MATCH 35651v35651):
- Regime 1 (> 10000 IL tuples): a global pointer is itself a register candidate. `CMainGameComponent::Get()->` re-read at every
  site (the global ?PMainGameComponent@CMainGameComponent@@1PAV1@A) gets esi as retail (0x0041E712 re-reads it into esi; priority
  1141 v the action-pointer CSE temp 958); a named `CMainGameComponent* game` local had priority 574 and lost the order. A call
  kills the register copy of the global, so reads right after a call (the 12 `event.Player = ...GetMainPlayerNumber()` lines
  after a `CGameEvent event(..)` constructor) must use the named local, which shares esi through its copy.
- A named call-result intermediate (`CPlayer& p = ...GetMainPlayer();`) adds +1 K per referencing block (+3 priority) at identical
  code; a trivial nested inline (scoped_ptr operator*) adds no candidate.
- Call arguments: an inline accessor argument (`inventory->GetThingSlotToSwap()`, FableWin name) is evaluated before another
  argument's call (GetAbilityPDef, 0x00422251); a direct member read is pushed after it. Parameter POINTERS stepped first with
  `->Value` read in the call give retail's `push [reg+8]` at the call (0x00421D94 style cards, 0x004227FA MOVE_COMPONENT);
  `C2DVector delta(0.0f, -25.0f);` named before the call builds the delta before the receiver is read (0x00422EFE); a named
  `CActionParamString* movie` loads the parameter before the default-argument pushes (0x0041F90C).
- `if (moved) f(state); else f(state - 1);` (two whole calls) lets c2 hoist the identical argument code above the test and
  cross-jump the calls (0x00420EE4); `f(moved ? state : state - 1)` does not.
- CHANGE_ALPHA: `(*it)->ActionParamType` in each test and `static_cast<CComponent*>(*it)`, no `CActionParam* param` local --
  retail reloads the element and keeps the null check of the cast (0x00420C5D).
- CWideString::operator==(const wchar_t*) on a null block: the nested constant-return shape
  `if(!P) { if(GFStringsAreEqual(text, L"")) return true; return false; } return *P==text;` places the non-null arm after the
  next jump with a jump back to the caller's test (0x00424F4A -> 0x00424EC7, 0x004262A0 -> 0x00426260); `return GFStringsAreEqual(..)`
  keeps it in line (hdr/fable_string.h FABLE_RETAIL_CWIDESTRING_TEXT_EQ_HELPER).
- Cross-jump survivors (VC7.1 c2, optimize("s"); a model that fits the row and every t8/mini3 probe): the exit block's edges are
  ordered breaks (source order), then returns (source order), then edges into the `default:` label. The LAST edge whose block
  shares a tail is processed first and absorbs every other block with a matching tail (each gets a jump into it; the jumps are
  made walking the list backwards). The blocks that now jump to the same place are then merged the same way: the jump made last
  (= the EARLIER case) survives -- ITEM_ASSIGN_LEFT keeps the tail of ITEM_ASSIGN_RIGHT/DOWN/UP (0x0041F059), TELEPORT that of
  GUILD_SEAL_RECALL (0x0041EE8F). To make a LATER case keep a second-level tail, move the earlier case's edge after it:
  CHEAT_MORALITY ends `return;` so CHEAT_RENOWN keeps the GetTC/AddCloseInventoryEvent tail (0x0042650C jmp 0x00426A11), and
  CLOSE_TELEPORT_MENU stays the first-level survivor (0x0041F7DA) by falling into a `default:` written right after it (the
  default-label edges come after every return). Mini: t8/mini3/gen8.py (the row's real case values, distinct head calls).
- Do not use a user label as a lever in a big switch: any label in 0x0041E6D3 (even `default: goto done; } done:;`) reorders
  cross-jumps across the whole function (175 changed, 35662v35651); the minis did not show it (the row has object locals with
  destructors and early returns). `goto` to a label on `default:` puts that edge after the returns.
- Mini probes of cross-jumping need DISTINCT non-virtual head calls per case: a shared `g->a()` vtable head merges too and hides
  the tail order (t8/mini gen5 v t8/mini3 gen7).
Review at landing (session 8):
- The source places `default:` in the middle of the switch (after CLOSE_TELEPORT_MENU, which falls into it) and ends six cases
  with `return;` -- both are the retail edge order (comment at the switch head); keep them when retyping.
- The hdr/ overlay opt-ins must be merged into rebuild/include before landing (hdr/README.md lists each; default paths unchanged).

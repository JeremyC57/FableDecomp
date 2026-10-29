# Hand chunk 572 (CAIStateGroup_BuyFromSeller .. CAIStateGroup_BuyFromSeller): 0 of 1 byte-exact

## What matched (0)
None. The one row is parked at the retail length with two register-colouring ties left (below); round 3 closed the block-layout residue. Its best source is `differ/00919ae0.cpp` and its behaviour test is
`differ/00919ae0_test.cpp`. The test prints `00919ae0_TEST PASS` under g++ and under VC7.1 (`V.behaviour_of`).

## Parked for Fable 5.1 (1)
- 00919ae0 CAIStateGroup_BuyFromSeller::Update DIFFER(2220v2220): 38 diff lines, all register renames, same length, same 95 calls, same frame, same block layout and shared free tail. (1) the participant loop loads `*it` straight into ebx where retail does `mov ecx,[esi+8] ; cmp ecx,eax ; je ; mov ebx,ecx` (3 bytes; the two loop-align pads after it shift by 3 and one nop moves). (2) the stock-filter loop has the list node in ebx and `&node->_M_data` in ebp; retail has ebp and ebx. | tried: rounds 1-2 (about 110 grades, listed in the round-3 section below) plus round 3 (about 45 full grades and 60 reduced-model compiles): for the layout residue (closed): timeout block inside the loop as `RemoveMessage(request); break;` with the post-loop `else accepted = false;` (THE fix), plus `accepted = false` initialiser + no else (2224), for(;;)/while(true)/do-while(true) forms with the reply/timeout handled as `break` blocks (A/B/J in retail order but the DoNothing exit sinks to the function end and the shared tail moves mid-function, 2237), goto labels anywhere (no effect), gaveUp flags, inlined helpers around the DoNothing exit, `?:`/`&&` forms of `accepted`. For (1): two named locals (participant + cast copy), const& element local, static_cast, explicit IP temporary, template converting IP ctor, IP ctor with explicit base-ctor initialiser, `operator=(T*)` through a temp, `IsThing()` inline member compare, explicit cast of the right operand, iterator declared before the loop, `continue` / empty-then-else forms, post-increment, seller declared before participants (moves the IP ctor instead). For (2): reference / pointer element local inside or before the loop, null-initialised pointer local, `it->GetPItem()` form, `it.operator->()`, `while` form (2188), const list + const_iterator, iterator declared before the loop, `owner` declared before `stock`, IS_THING_ALIVE written inline / by pointer / plain inline, HasTC through an inline helper, unnamed bool (2204), isStockItem declared at the body top, declaration-order perturbations at function top (accepted/timeout/reply/result/shopKeeper), QFE-4035 compiler (identical). None moved either register. | next: lane gap: register colouring. Both residues are pure colouring ties of VC7.1's global allocator (ebx vs a scratch ecx + copy; ebx vs ebp for two equal-weight loop candidates); the instruction streams are identical. If anyone retries: find a source form in which `&*it` of the stock loop out-weighs the node (retail gives ebx to the heavier candidate; ours weighs the node heavier), e.g. a form whose IR keeps one element temp with four uses, and a form in which the participant pointer is two IR candidates (compare temp + call-spanning copy) that VC does not coalesce.
Dry vland run of the parked source (tmp/differ_payload.json, read-only, lv/ deleted): `DIFFER(2220v2220) PASS`, no
RELOC_IDENTITY or shared-header warnings (round 3, 2026-10-29).

## Round 3 (2026-10-29, fresh agent): the layout residue is closed
- What closed it: the wait loop's timeout branch is `if (timeout < CWorld::GetFrame()) { RemoveMessage(request); break; }` and the
  post-loop code is `if (reply) { RemoveMessage(reply); accepted = reply->ExtraData != 0; } else accepted = false;`. Retail's
  timeout path (`GetWorld ; RemoveMessage(request) ; xor bl,bl`, falling into the scratchpad GetTC) is then the in-loop break
  block followed by the else block, and the DoNothing exit moves to retail's place (after the GetTC has-path, before the no-has
  phi copy). Grade went 2217 -> 2220 bytes, 51 -> 38 diff lines (probe/m1f.cpp = differ/00919ae0.cpp).
- How it was found: a reduced model (exp/e13.cpp, ~2 s per compile: a 60-line Test() with the same loop skeleton; the
  `reply = 0; break;` hack makes the break edge thread like the full function) reproduced the 2217 layout, and 40 variants of it
  mapped VC7.1's rule. Rule, as far as the data goes (exp/r*, q*, u*, h*, w*, k*, i*, g*, m*): inside a `while (cond)` loop
  (front-end rotated), a then-block that ends in `return`/`break` whose fall-through alternative is the join is a deferred cold
  block; the walk follows fall-through edges, and at each chain break (an unconditional jmp or ret) emits ONE deferred block,
  the earliest one in source order; then continues with the next block. A `jmp` that only exists because a later block was
  inserted (the register allocator's phi-copy block on the GetTC no-has edge) is not a break at layout time. Inside a
  `for (;;)` / `while (true)` / `do {} while (true)` loop (optimizer-rotated), in-loop `return` blocks are instead sunk to the
  very end of the function (exp/ua, uw, ud, h3, h4, h5, w3-w6), and a `break` block behaves like the while-form (exp/w1).
  Goto labels, inline helpers, scopes and declaration order do not move a deferred block (exp/ga-gc, uag1-3, i1-i5, r1, r2).
- Tools added: tools/skel.py (block skeleton of a .obj or of the retail bytes: calls named, branches labelled), tools/qq.py
  (q.py with `--qfe`), exp/run2.sh (compile a reduced model and print its skeleton), pg.sh now keeps probe/<n>.obj + .skel.

## Skipped
None.

## Label corrections
- 0x00919AE0 is `?Update@CAIStateGroup_BuyFromSeller@@UAEXXZ`, proven two ways:
  - Vtable 0x0128FF0C (RTTI CAIStateGroup_BuyFromSeller) slot 12. In the same vtable, slot 11 is IsRunnable 0x0091A390 and slot 9 is Cleanup 0x00919A00.
  - Slot 12 of the sibling vtable 0x0128B78C is the landed `?Update@CAIStateGroup_BanditSitRoundCampfire@@UAEXXZ` 0x0093B1D0. The Ego_r.pdb name set holds `?Update@CAIStateGroup_BuyFromSeller@@UAEXXZ` (donor 0x0093D5D0).
- 0x00658B20 is CTCVillage::IsWorkTime, landed as `IsCookTime_00658b20` in IsInWorkHours 0x0091A6F0. 0x00658A80 is CTCVillage::IsStoryTime, bound as IsCookTime. Evidence:
  - Ego_r donor order IsSchoolTime / IsStoryTime / GetSchoolTimes / IsWorkTime / IsCookTime maps to retail 0x00658A35 / 0x00658A80 / 0x00658ACB / 0x00658B20 / 0x00658B6B.
  - 0x00658ACB is the only one of these with two arguments (`ret 8`, the float& pair), and it reads the same time vector as 0x00658A35 (+0x54/+0x60). The manifest labels all four as IsCookTime: propagated.
- 0x00919A70 GetSellersReply, 0x00919A30 BoughtSomething, 0x00919A10 ShouldBuyFromSeller and 0x0091A6F0 IsInWorkHours are landed with non-PDB spellings. The PDB spellings and their evidence are in NAME_FIXES.tsv.
- 0x009812C0 AIAction_SpeakAISpeechToTarget: all three bound spellings are wrong (QAEJJ / CIntelligentPointerCreature stand-ins). The correct spelling is the Ego_r IAE form in NAME_FIXES.tsv.
- 0x00919A30 carries the catalog module label "CCombatSequence". It is a CAIStateGroup_BuyFromSeller member.
- 0x01375550: this row and 568 use `?ConstantFPS@CMainGameComponent@@1JA`, read through the inline `CMainGameComponent::GetConstantFPS()` (FableWin.pdb `?GetConstantFPS@CMainGameComponent@@SIJXZ`). Only the getter is proven; the backing member name is not. The landed IsRunnable 0x0091A390 spells the same address `g_framesPerSecond` (one address, two names). The coordinator should pick one.

## Names used and their evidence
- Every callee uses its Ego_r.pdb spelling, checked with `objdump -t` on the built object:
  - AIAction_MoveToPosProximity, AIAction_SpeakAISpeechToTarget and AIAction_DoNothing (IAE)
  - GetWorld and GetPWorldSeed (IBE)
  - Terminate
  - GetSellersReply (AAE, `const CMessageEvent*` / `const CThingCreatureBase*`)
  - AddMessage (`W4EMessageEventType`) and RemoveMessage
  - GetShopTC, GetPVillage and IsWorkTime (QBE)
  - GetBuyersPhrase (QAEKXZ)
  - RemoveFromCreatureInteraction
  - GetPos
  - GFRandom (`YIKKPAK`)
  - CWorld::GetFrame (`SIJXZ`)
  - the IP ctor, dtor, GetPItem and SetPItem
  - STLport `clear` 0x004B8050 and `push_back` 0x004BA4E0 (bound spellings)
- Inline helpers inlined in this row. Each is named from FableWin.pdb and lines up with the retail field reads:
  - `CWorld::GetMessageEventManager()` (`?GetMessageEventManager@CWorld@@QAEAAVCMessageEventManager@@XZ`, a reference through `CCountedPointer::operator*`; field +0x60 per Ego_r CWorld layout)
  - `CThingCreatureBase::GetPCreatureInteraction()`
  - `CCreatureInteraction::GetParticipants() const`
  - `CTCThingOwner::GetObjectList()`
  - `CTCAIScratchpad::GetThankingPhrase()` / `GetIgnoringPhrase()`
  - `CMainGameComponent::GetConstantFPS()`
  - `IS_THING_ALIVE<CIntelligentPointer<T> >` (`YI_NABV...`, __fastcall template on a const reference)
- Descriptive (unproven) names:
  - `g_BuyFromSellerProximity`: the float 1.0 at 0x0138C690. Only this function reads it, and no PDB name exists.
  - the enum names EAIActionReturn, EAIActionRepeat, EMovementType and EMessageEventType are PDB names, but their enumerator values are used as casts. The enumerator names are unknown.
- RTTI-checked vtables (used through the IP templates):
  - 0x0123E7E0 CIntelligentPointer<CThingAICreature>
  - 0x01237F1C CIntelligentPointer<const CThing>
  - 0x0123E7D8 CIntelligentPointer<CThingCreatureBase>
  - 0x01238C6C CIntelligentPointer<CThing>
- TC interface numbers, from vtable slot 21 of each TC (tools/tctype.py): CTCVillageMember 0x23, CTCThingOwner 0x21, CTCStockItem 0x54, CTCShopKeeper 0x90, CTCAIScratchpad 0xAA.

## Header debt
- `engine/CAIStateGroup_BuyFromSeller.h` is flat (the vptr is a data member), so the class is a local model: CAIStateGroupBase : CBaseClass, then CAIStateGroup_WorkBase<CAIStateGroupBase>, FrameLastBought +0x14.
- `engine/CWorld.h` flattens CCountedPointer<CMessageEventManager> PMessageEventManager (+0x60) into `_Data`/`_Info` and lacks the inline GetMessageEventManager(). The row needs both, so CWorld is a local model (+0x60 counted pointer, static GetFrame). The flattened `_Data` field gives the wrong evaluation order (argument pushed before GetWorld).
- CThing (8-aligned by the `unsigned __int64 UniqueID`) and CThingCreatureBase (+0x12C PCreatureInteraction) are local retail models, the same as 566/568. CTCBase comes from engine/CTCBase.h. Also local: CTCVillage, CTCVillageMember, CTCShopKeeper, CTCThingOwner, CTCStockItem, CTCAIScratchpad, CMessageEvent, CMessageEventManager, CBaseIntelligentPointer/CIntelligentPointer, CCountedPointer, CVectorMap and the STLport list/bitset/vector pieces.

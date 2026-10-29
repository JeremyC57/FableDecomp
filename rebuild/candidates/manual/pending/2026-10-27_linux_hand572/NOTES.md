# Hand chunk 572 (CAIStateGroup_BuyFromSeller .. CAIStateGroup_BuyFromSeller): 0 of 1 byte-exact

## What matched (0)
None. The one row is parked 3 bytes short (below). Its best source is `differ/00919ae0.cpp` and its behaviour test is
`differ/00919ae0_test.cpp`. The test prints `00919ae0_TEST PASS` under g++ and under VC7.1 (`V.behaviour_of`).

## Parked for Fable 5.1 (1)
- 00919ae0 CAIStateGroup_BuyFromSeller::Update DIFFER(2217v2220): (1) the participant loop loads `*it` straight into ebx, where retail does `mov ecx,[esi+8] ; cmp ecx,eax ; je ; mov ebx,ecx`. That is the 3 missing bytes, and the loop-align pads after it (`nop` / `lea ecx,[ecx]` / `lea esp,[esp]`) move with them. (2) The stock-filter loop has the list node in ebx and `&node->_M_data` in ebp; retail has ebp and ebx. (3) The `AIAction_DoNothing()==1` exit (`GetWorld ; RemoveMessage(request) ; jmp` to the shared clear tail) is placed right after the reply path; retail places it after the scratchpad GetTC has-path, just before the no-has block `mov eax,[esp+1Ch]`. Everything else is identical: all 95 calls in retail order, the frame, the stack slots, the merged fail blocks and the shared `call free ; add esp,4 ; seller dtor ; epilogue` tail. | tried: about 70 probe grades (probe/v1..v10, t1..t29, w*, s1/s2, p1..p8, d1..d3, r1..r5, b1..b7, f1..f7, q1, c1..c3, g1/g2, i1/i2, m1, n1..n3, o1..o3, h1). For (1): `participant` local, `continue` form, `&GetCreature() != *it`, explicit IP temporary, static_cast, CThing* casts, the PDB accessors GetPCreatureInteraction()->GetParticipants() with const_iterator, and an IP ctor through a typed SetPItem. For (2): while form, reference local, `continue` form, a single bool for the whole test, GetObjectList() const / non-const, const owner, const_iterator. For (3): `if (!reply)` order, nested-if DoNothing, DoNothing-first branch order, no inner scope, timeout handled in the loop, `accepted = false` initialiser, `accepted` computed after the branch, explicit rotated do-while, reply handled inside `for (;;)`. Each one either changed nothing or broke more than it fixed. | next: (3) is a layout question. b1 (`if (!reply) {timeout} else {reply}`) and f2 (timeout handled inside the loop) both put the DoNothing exit in retail's place, but each then sinks the timeout block below it. Look for a form that keeps the timeout block falling into the scratchpad code while the DoNothing exit moves down. (1) and (2) look like register-preference artefacts of the global allocator. Try them again once (3) is fixed, since a layout change moves the allocation order.

Dry vland run of the parked source (tmp/differ_payload.json, read-only, lv/ deleted): `DIFFER(2217v2220) PASS`, no
RELOC_IDENTITY or shared-header warnings.

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

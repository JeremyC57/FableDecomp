# Hand chunk 2023 (CAIStateGroup_SellToBuyer .. CAIStateGroup_MoveGoodsBase): 1 of 2 byte-exact

0x0094C050 grades RELOCATION_MATCH under VC7.1 RTM from the exact file in src/ (gall.sh with the overlay, re-run after the last header
change), its behaviour test prints `0094c050_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py), and the read-only verifier (vland.py,
overlay on) reports 1 WIN with RELOC_NAME warnings only (catalog labels of 0x00798D10 / 0x00798D20 / 0x0094BEF0, see NAME_FIXES.tsv).
Every call target, vtable, global and literal was checked against retail (tmp/rel.py: all targets equal by address).
0x00915B60 is not matched (see "Not attempted / parked").

## Row extent (0x0094C050)
The scan's 1859 bytes are the whole function: int3 before 0x0094C050, last `ret` at 0x0094C792. The reservation's 1882 = 1859 + 13 bytes of
int3 padding (0x0094C793..0x0094C79F) + a separate 10-byte function at 0x0094C7A0 (`mov eax,[esp+4] ; mov [ecx+0Ch],eax ; ret 4`, a setter
of a +0x0C member, no caller in xref.py, no catalog row) that ends at 0x0094C7AA, where the next catalog row's padding starts
(0x0094C7B0 CAIStateGroup_GameMaster::OnActivate). True length 1859: no SPLITS.tsv entry needed; the 0x0094C7A0 setter is an uncatalogued
function for the coordinator (its owner is unproven; it sits between the SellToBuyer and GameMaster TUs).

## What matched (1)
- **0x0094C050 CAIStateGroup_SellToBuyer::Update** (1859 bytes; pass 1, 7 grades + 1 re-grade). The catalog name is right (vtable slot 12,
  Ego_r + FableWin `UAEXXZ`). Decoded flow: participants walk (PBuyer = the participant that is not the creature), wait while the buyer
  speaks (DoNothing == 1 -> refuse), walk back to the stall when further than GetMinDistFromStall (follow-me speech when also beyond
  radius + 0.5), 20 s wait for the buyer within GetMaxDistFromCustomer, then AddNewCustomer, reply speech, accept message (0x10, value 1),
  thanking speech, RemoveFromCreatureInteraction; refusals RefuseCustom + Terminate (Terminate + RefuseCustom after the walk). Part of it
  is a same-TU neighbour expanded inline: CAIStateGroup_WorkBase<CAIStateGroupBase>::StayInShop (0x0094BEF0) is expanded at three sites
  and called at the fourth. Levers, in the order found:
  1. **Inline split by static frequency** (c2 inline trace tmp/inl.sh): retail expands StayInShop in the walk-back and wait loops and in
     the reply loop but calls it in the thanking loop, and expands the inline LowerBound in the first two expansions only. The model's
     thresholds (measured from the traces): StayInShop called below f = 1.71, LowerBound below f = 1.86. Getting there needed the
     participant walk as a `for` (post-loop leak keeps the code after it at 2.5) and the wait loop as `bool waiting = true; while (waiting &&
     end_frame >= GetFrame()) { if (!StayInShop() || !alive) { refuse; return; } waiting = DoNothing() == 4 && dist2 > max*max; }` followed
     by `if (!waiting) { ...sale...; return; } RefuseCustom(); Terminate();` -- frequencies 12.5 / 13.9 / 3.65 (LowerBound score exactly 74:
     called) / 1.22.
  2. **Final refusal block**: with the sale inside `if (!waiting) {...; return;}` the trailing RefuseCustom/Terminate is the function's last
     block; every in-loop `{ RefuseCustom(); Terminate(); return; }` (speaking wait, wait loop) cross-jumps into it (0x0094C77D, falling
     into the one epilogue). A refusal written as an in-loop return alone is laid out after the next unconditional jump (0x0094C2DD /
     0x0094C556 in earlier builds).
  3. `end_frame >= CWorld::GetFrame()` gives retail's `cmp [esp+20h],eax ; jl`; `dist2 > max * max` inside the `&&` gives `test ah,5 ; jnp`.
  4. The radius is the member `GetCreature().GetPhysicsTC()->Radius` (header debt, see below). With it the follow-me test is
     `distance_squared > GetMinDistFromStall() * GetMinDistFromStall() + ...->Radius + 0.5f` in one expression and the phrase read into a
     named local before it: phrase (edi) and the TC pointer (ebx) load before the two calls, the float is read by `fadd [ebx+34h]`.
  5. `std::list<...>::const_iterator it; for (it = participants.begin(); ...)` (iterator declared before the loop) -- the extra
     frame node / candidate keeps the shop keeper pointer out of ebx before the speaking wait (retail stores it only, `mov [esp+18h],eax`,
     and loads ebx after the wait at 0x0094C137) and gives the participant compare in ecx with the `mov ebx,ecx` copy in the then-arm.
     This one change also fixed the frame slot order (shop_def 0x14 / keeper 0x18) and the two scheduling spots that remained
     (0x0094C150 shop position copy, 0x0094C1E1 far-block loads): RELOCATION_MATCH.
  6. `CIntelligentPointer` user copy constructor (FABLE_RETAIL_CINTELLIGENTPOINTER_COPY) for the by-value AddNewCustomer argument.
  7. The participants reference is read before the GetTC statement (retail loads [creature+12Ch] in the first block, 0x0094C067).
- Same-TU bonus: src/0094c050.cpp also emits ?StayInShop@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ; graded against 0x0094BEF0
  it is RELOCATION_MATCH (one probe), so the landed 0x0094BEF0 row (named IsInWorkHours) can be respelled from this TU.

## Names
Proven (two sources each, LABELS.tsv): the row (vtable slot + both PDBs), StayInShop (Ego_r donor order + matching body), CTCShopKeeper
GetMinDistFromStall / GetMaxDistFromCustomer / AddNewCustomer (both PDBs + donor order = retail order + bodies), NCreatureAI::IsSpeaking
(both PDBs, next to the landed StopSpeaking), AIAction_MoveToPosProximity and RemoveFromCreatureInteraction (both PDBs, already bound),
CThingCreatureBase::GetPCreatureInteraction and CCreatureInteraction::GetParticipants (FableWin / both PDBs, inline in retail).
No descriptive (made-up) function, class or member names were introduced. Enum values are passed as casts of their numbers
(EAIActionRepeat 2, EMovementType 3, EAIActionReturn 1 / 2 / 4, EMessageEventType 0x10): no PDB enumerator names are on this machine.
Local variable names (participants, shop_keeper, shop_pos, distance_squared, phrase, waiting, end_frame, reply_phrase, thanking_phrase)
are descriptive and do not mangle. Wrong landed / bound spellings: NAME_FIXES.tsv.

## Header debt
All header work is in hdr/ (hdr/README.md): 2 new headers (CAIStateGroup_SellToBuyer, CCreatureInteraction) and opt-in blocks in 6 copied
headers. Review at landing: FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS puts CTCPhysicsBase::Radius in a `public:` section -- byte evidence
only (retail's lvalue read at 0x0094C1F2 / 0x0094C216; the by-value inline GetRadius() cannot produce it, measured), the PDB access
attribute is not available here. The landed SellToBuyer siblings (Cleanup / RefuseCustom / IsRunnable / GetBuyersOrder) use local class
models; the new CAIStateGroup_SellToBuyer.h can replace them.

## Not attempted (1)
- **0x00915B60 CAIStateGroup_MoveGoodsBase::PlaceGoods** -- NOT_ATTEMPTED (0 grades): stopped for context after the first row (it took
  most of the session); no source was graded, nothing is in differ/. Decoded outline for the next agent:
  - **Name / extent (proven):** Ego_r.pdb ?PlaceGoods@CAIStateGroup_MoveGoodsBase@@IAE_NV?$CIntelligentPointer@VCThing@@@@MV?$vector@PBVCCharString@@V?$allocator@PBVCCharString@@@std@@@std@@ABVCAIFilterBase@CAIStateGroupBase@@PBVCThing@@@Z
    (protected; donor order OnActivate 0x009393D0 < FetchGoods 0x009393E0 < PlaceGoods 0x00939700 < Cleanup 0x00939FF0 = retail
    0x00915880 < 0x00915890 < 0x00915B60 < 0x009162F0). `sub esp,100h`, `ret 20h` (IP<CThing> 8 + float + vector 12 + filter& + const CThing*).
    Extent checked: last `ret 20h` at 0x009162E8, int3 to 0x009162F0 (Cleanup): 1931 bytes is right. No EH frame (the by-value vector is
    freed with `free` 0x00BFEA14 at both returns; the by-value IP<CThing> destination at [esp+114h] is destroyed with 0x00A01C10).
  - **Members:** DroppingGoods +0x14 (bool, `mov byte [edi+14h],1` at 0x0091610C), PickUpGoodsActionID +0x18 (Ego_r layout, header
    engine/CAIStateGroup_MoveGoodsBase.h is the flat generated one -- needs a retail class : CAIStateGroup_WorkBase<CAIStateGroupBase>).
  - **Flow:** (1) destination->HasTC(0xB5 crate stack, bit 21 of +0x34) else fail; GetTC 0x46 on the creature (bit 6 of +0x28; const
    LowerBound 0x004365B0) -> CTCCarrying::GetThingInPrimarySlot 0x006A1440 -> IP<CThing> local [esp+14h] (vtable 0x01238C6C) = the goods;
    fail if null; (2) the goods' TC 0x2D (bit 13 of +0x24) = CTCUserActionPointBase in ebp; timeout = GetFrame - (long)(ConstantFPS *
    float 0x01269960) via __ftol2; loop `GetFreeActionPoint(creature, &pos, &rhs, &point)` 0x007FEDE0 until it succeeds, else
    AIAction_MinionWander(3, 0) 0x0097F870 (==1 fail), timeout, filter vcall slot 0, destination / goods alive; (3) GetTC 0x02
    (bit 2 of +0x20) on the destination -> virtual +0x120 on that TC (its +0x0C/+0x10/+0x14 vector copied; role unproven) scaled by the 2D distance (fsqrt); when `ignore` != 0, the creature-to-ignore squared distance is
    compared with the float at 0x0125D824 (`test ah,41h ; jne` skips) before AIAction_MoveToPreCalcRoute 0x0097F690(ignore, &flag, 3, filter, 1); (4) walk:
    AIAction_MoveToPosProximity 0x0097E890 (2, dest pos + offset, 5.0f, &flag, 3, 1, 0, 1, 0, 1, 0) until != 4, fail on 1 / 3 / filter /
    dead; (5) TC 0x2F (bit 15 of +0x24) of the destination -> IP at +0x0C of it; if set: GetWorld 0x008FCFC0 -> GetWorldSeed 0x0049D810,
    LCG `*seed = *seed * 0x24A1 + 0x24DF; GFROR13 0x00497890`, % 50 + 50 frames of AIAction_MinionWander(3, 0) while the user is set;
    then CTCExclusiveUse::SetUser 0x00916640(creature) on it; (6) AIAction_MoveToPos 0x0097EC00 (2, pos, &flag, 3, 1, 1, 0, 1, 1.0f)
    until != 4 or filter fails; (7) AIAction_LookAtPos 0x0097D580 (2, goods pos, 0x11, 0.00278f) until != 4; on 2: DroppingGoods = 1;
    destination TC 0xB5 -> CTCCrateStack::GetNumCrates 0x007B6660 (index, 0 if none); goods TC 0x4D via GetThingInPrimarySlot, or
    `AddTC(CCharString("..." 0x0123B47C), true, 0)` 0x004C9D60 -> CTCSolidToThing::AddThingNotSolidTo 0x00770FA0(creature);
    `CCreatureAction_PutGoodsDown(creature, goods[index], 0)` 0x00915810 (local at [esp+68h], ~CCreatureActionBase 0x00693EF0) ->
    AIAction_PerformActionAndWait 0x0097DB70; on 2: SetUser(0) on the exclusive-use TC, SetActionPointAsFree 0x007FEF40(point), return
    true; every failure exit frees the action point when the goods are still alive (three cross-jumped copies 0x00916270 / 0x00916284 /
    0x00916298) and returns false.
  - **Models to build:** STLport `std::vector<const CCharString*>` by value (stlport/fable_containers.h), CAIStateGroupBase::CAIFilterBase
    (nested, vcall slot 0), CTCCarrying (GetThingInPrimarySlot), CTCCrateStack, CTCUserActionPointBase (GetFreeActionPoint signature from
    the PDB), CTCExclusiveUse::SetUser, CTCSolidToThing::AddThingNotSolidTo, CCreatureAction_PutGoodsDown (ctor 0x00915810, landed),
    CThing::AddTC. The landed sibling FetchGoods 0x00915890 is a local model with an invented signature (`vector<long>` / CLongVector,
    `CAIFilterBase*`): its catalog / leaf spelling should be respelled to the Ego_r one (same parameter list as PlaceGoods).

## Tools (tmp/)
cc.py (compile only), md.py (relocation-masked diff, `--regs --frame`), rel.py (call-target identity), ann.py (annotated disassembly)
-- copies from agent2026; fr.sh (fragment probe over probe/hdr_part.inc), lay.sh (compile-only layout markers), gr.sh (grade + diffs),
inl.sh + inl/ (c2 inline-decision trace, from agent2026), c2prio.py from lane_tools. Probe compiles on top of the 7 grades: ~30 fragment
probes (f1..f12, s1..s6, l1), ~12 compile-only layout probes, 9 inline traces, 3 c2prio traces, 1 StayInShop check against 0x0094BEF0.

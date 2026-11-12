# agentQ16 notes (wave 22)

Every grade and probe ran with `FABLE_HDR_OVERLAY=$A/hdr` exported (hdr/README.md lists each opt-in), one compile at a time,
wrapped in `timeout 240`. Probes (tools/pq.sh, same q.py, not logged) are separate from the official grades in ATTEMPTS.log.

## Matched (src/ + test/, RELOCATION_MATCH, tests PASS under g++ -std=c++03 and VC7.1 /Od via tools/vt.py)
- **0x00957DB0 CAIStateGroup_MinionBehaviourCrowdBase::Update** (Ego_r `?Update@CAIStateGroup_MinionBehaviourCrowdBase@@UAEXXZ`).
  Ported onto CAIStateGroupBase.h (MEMBERS / AIACTIONS / MEALACTIONS / GETCREATURE), CAIBrain.h, CThingAICreature.h and
  CTCPerceiveThing.h, plus the new header CAIStateGroup_MinionBehaviourCrowdBase.h (layout from Ego_r; pure slots 14-17 from the derived
  vtable 0x0128D16C). The Q15 local GetInterface model is gone.
  Levers:
  - the CTCPerceiveThing AWAREOFTHING opt-in (inline StatGetInterfaceType and IsAwareOfThing; out-of-line IsAwarenessOfThingDirect 0x00744430);
  - an uninitialised route flag;
  - FAILED + break, then an inline Terminate;
  - COPY_ORIGINAL_CHAIN.
- **0x00774730 CTCShop::SellItem**. Ported onto the HEAD CTCShop / CShopDef / CTCThingOwner / GetSubDef(NOINLINE) /
  STLP_TREE_FIND_MEMBER_TEMPLATE headers. Lever: a named const reference to `*it`.
  The map tree find 0x00777120 has no reloc binding yet (it binds through the STLport member template).
- **0x008378D0 CThingTrackNode::SetAsLinkedTo** (Ego_r / FableWin `?SetAsLinkedTo@CThingTrackNode@@QAEXPAV1@@Z`).
  - Header: CThingTrackNode.h opt-in LINK (declaration). PNextNode / PPreviousNode in the draft respelled to the HEAD members
    NextNode / PreviousNode.
  - Lever: the start-node arm's tail written in both branches (c2prio: other 210 -> above this).
  - Callees: Compare 0x00411570 (expanded CCharString ==), FlipTrackDirection 0x00836DE0, SetTrackName 0x00836EE0.

## Unmatched (differ/, line 2 = residue)
- **0x0090C900** (Q15 port): one jump target.
  - What happens: the factionless HasTC `return false` jumps to return-false copy 1 (0x0090C946); retail jumps to copy 2 (0x0090C994).
  - Probed about 30 restructurings (work/v, work/v9): every one either leaves the target alone or moves the village targets with it.
  - Next: trace c2's tail-duplication or cross-jump order for return blocks (tools8/xj.py in hand886), or try different spellings of
    the curfew tail. The tail is what owns copy 3 (`xor eax,eax`).
- **0x00738A80** (Q15 port): Data/Info esi/edi swap.
  - c2prio puts Info #11 at priority 80 and Data #9 at 76.
  - Next: a spelling that adds IL uses of Data. 0x008D49B0's inline filters do that.
- **0x00F3ABF0** (ported; new CRangeCompressor.h, CArray SIZE_CTOR_OUTOFLINE): one instruction is out of place.
  - The `remaining` init comes before the loop guard.
  - Best alternative: work/v10/g.cpp (guarded do-while). Its whole prologue and first loop match, but the const-0 esi piece then
    reaches the values-loop guard (`cmp ebx,esi` instead of retail's `test ebx,ebx`).
  - Next: combine g with a values-loop spelling that ends the const-0 piece before the CArray constructor call.
- **0x00A90260** (ported; C3DPrimitive2 INDEXBUFFER opt-in, scoped_ptr reset, CIndexBuffer): copy-loop colouring.
  - c2prio puts i at priority 84 and dest_index at 94.
  - The flag is read with `mov bl` where retail uses `cmp byte`.
  - The loop-2 alignment jmp is missing.
- **Regraded as they sit, not ported this wave** (each kept its earlier residue on line 2; full PDB leaves added): 0x00649E8B,
  0x0093C030, 0x009711D0, 0x008E16A0, 0x0096CD70, 0x00795090, 0x00555A90, 0x00966030, 0x00A87700.
  - Most are register swaps of this against an iterator or a pointer. The 0x008378D0 lever (more IL references to the value that should
    win) is the first thing to try on 0x009711D0, 0x00A87700, 0x00966030 and 0x00795090.
  - 0x00966030 needs a CTCUserActionPointBase header.
  - 0x00795090: the row is CTCUnholyPower::FinalCleanup (vtable 0x0126BFDC, Ego_r `?FinalCleanup@CTCUnholyPower@@AAEXXZ`). The
    catalog / oracle name CTCDivineWrath::FinalCleanup is propagated from the near-twin 0x0078FAF0.

## Not worked (no time)
- 0x0065C5BB: size-optimised TU, set<long> plus the deed log.
- 0x008BF500: CCombatActionBase / CCombatManager attackers map.
- 0x008CDD40.
- 0x0095B820: callees settled for the next author.
  - The area collector 0x008E3EB0 is the CThingSearchTools::GetAllThingsInRoughArea<CThingAICreature, vector<CThingAICreature*>,
    CThingFilter_HasDefGlobalIndex> instantiation (Ego_r 0x00826060). Its template body is in the shared CThingSearchTools.h.
  - IsStillRunnable is protected (IAE).

## Header debt
- New headers: CAIStateGroup_MinionBehaviourCrowdBase.h and CRangeCompressor.h.
- New opt-ins: CTCPERCEIVETHING_AWAREOFTHING, C3DPRIMITIVE2_INDEXBUFFER, CARRAY_SIZE_CTOR_OUTOFLINE and CTHINGTRACKNODE_LINK.
- NAME_FIXES: 0x00744430 is IsAwarenessOfThingDirect. Two landed files still spell it GetDirectAwareness.

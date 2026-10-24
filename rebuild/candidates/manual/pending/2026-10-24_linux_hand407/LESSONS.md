# Chunk 407 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **STLport /Gr sort over a by-value comparator whose operator() is out of line.** Compile operator() as a same-TU
  stand-in (`#pragma auto_inline(off)` + `__declspec(noinline)` body). With a declaration only, VC7.1 reloads each
  16-byte comparator copy one call later, where retail loads it two words ahead. `__partial_sort` also gets x87 float
  copies (0x00978C50, 0x00979650, 0x0097B320).
  - The exception is `__unguarded_insertion_sort_aux`: there the stand-in lets VC hoist the loop-invariant comparator
    copy out of the loop. Keep operator() declaration-only for that row (0x00978B00, 0x0097AEE0).
- **CThingCompare_Nearest passed straight to the STL** (0x0095CFE0..0x0095E010): the template operator() returns `T*`,
  and its result is the predicate (`test eax,eax`). Model it as `template <class T> T* operator()(T*, T*) const` and
  give it a same-TU stand-in body.
  - The landed 0x007A2F90's comment calling it bool is wrong: Ego_r.pdb gives T*.
  - Behaviour: the predicate is true for any non-null pair, so sort of 16 or fewer things reverses them.
- **CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest>** (Ego_r.pdb: one `Functor` pointer) is an inline
  `(*Functor)(a, b) == a`. It is reflexive (comp(x, x) is true), so behaviour tests must skip self-comparisons
  (0x0096D6F0, 0x0096D660).
- **`position += velocity * time; return position;`**: the result reuses the position's frame slot (retail stores
  the sum's Z back to that slot). `return position + velocity * time` gives the sum its own slot, at the same size
  (0x00961AB0).
- **An anonymous-namespace function calling its sibling**: declare the callee only (no body in the TU) to keep the
  call out of line. The predicted C3DVector must be a named local passed by hidden pointer (0x00961B90).
- **A list find_if returning its iterator by hidden pointer** (`lea ecx,[esp+..]`, iterators and predicate pushed):
  same-TU stand-ins for LowerBound and find_if give retail's single frame word shared by the GetInterface key and the
  result (0x00946AE0). The const-LowerBound stand-in alone is enough for 0x0094B840.
- **A named filter local built before a world call shares the GetInterface key's frame word** only when LowerBound is
  a same-TU stand-in (0x0096EB40, still parked on block layout).
- **The PDB shows a packed class** (CAIStateGroup_OpinionReactionRoot: an 8-byte CIntelligentPointer at +0x6b). Spell
  the unaligned members as byte arrays: same layout, no `#pragma pack` (0x0095F830).
- **The retail CCombatantInfo is 4 bytes shorter than Ego_r.pdb's**: the STLport vector is 12 bytes. The layout is
  AttackerInfo +4 (UsingCombatWheel +0x20) and DefenderInfo.Attackers header +0x34. A walk of the attacker map with the
  out-of-line `++` (0x006EF790) keeps the iterator in the `push ecx` frame word (0x00979910).
- **Scorpion king summon**: `AIAction_PerformActionAndWait(&CAction(*creature, *PEnemy.GetPItem()), false)` with a
  full-expression temporary gives retail's destroy-right-after-the-call with no EH frame (0x0097C580).

Review at landing:
- Two rows parked after 4 grades each: 0x00954220 (register allocation) and 0x0096EB40 (the early exit's return tail
  is not merged into the shrink-wrapped epilogue). Best sources are in differ/.
- Local models sit next to existing engine headers: CAIStateGroup_LightHomeCandle, _SitInPub, _OpinionReactionRoot,
  _ScorpionKing, _MazeRangedCombat, _MeleeCombatNearby, CTCPerceiveThing and CSnowTrollDef. Reasons:
  - The headers flatten the vptr and base into data words.
  - They use Ego_r.pdb's Dinkumware container sizes. For example, engine/CAIStateGroup_ScorpionKing.h puts PEnemy at
    +0x34 where retail reads +0x30.
  - The def classes need virtual slot 1 for their reference release.
- Already-landed sources to recheck:
  - 0x00961C50 declares `IsThingInRangeOfPound(CThing*, CThing*)`; Ego_r.pdb has `(const CThing&, const CThing&)`.
  - 0x0096EE90 calls 0x0096EB40 as `void Combat_ProcessMeleeAttackMessage()`. It is
    `bool CAIStateGroup_MeleeCombatNearby::AlertAlliesInVicinity()`.
  - 0x0093E5E0 calls its twin 0x0093DFC0 "AddCombatMessage". It is
    CAIStateGroup_MeleeCombatNearby2::AlertAlliesInVicinity.
  - Landed sources call 0x00662410 / 0x00662430 IsRegisteredCombatant / IsCombatant / IsCombatEligible. They are
    IsInMeleeAsAttacker / IsInMeleeAsAttacked (Ego_r.pdb names; the bodies test Attacked != 0 and the attacker count).
  - 0x0095F7D0 models CAIStateGroup_OpinionReactionRoot with `#pragma pack(push, 1)`; byte arrays do the same without
    the pragma.
- No catalogue row holds two functions. 0x0097B320's row (208) is the 194-byte body plus padding.

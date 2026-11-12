# 2026-11-11 landing db -- chunk Q16 (GetInterface-model reworks, Opus 5.5): 3 rows

vland --land: 3 wins (RELOCATION_MATCH), tests PASS. bindings_multi_check is at its NullCoord baseline.
- **0x00957DB0 CAIStateGroup_MinionBehaviourCrowdBase::Update.**
  - New header engine/CAIStateGroup_MinionBehaviourCrowdBase.h (Ego_r layout; pure slots 14-17 from the derived vtable 0x0128D16C).
  - New opt-in CTCPERCEIVETHING_AWAREOFTHING: IsAwareOfThing inline, IsAwarenessOfThingDirect out of line.
  - The `ret = AI_ACTION_FAILED; break;` / `if (ret != AI_ACTION_FINISHED)` tail, and an uninitialised route flag.
- **0x00774730 CTCShop::SellItem.** Ported onto the HEAD headers. A named `const CIntelligentPointer<CThing>& thing = *it;` in the owner
  loop removed the last byte.
- **0x008378D0 CThingTrackNode::SetAsLinkedTo.**
  - The start-node arm writes `other->NextNode = this; other->IsEnd = false;` in both branches. The extra IL references put `other`
    above `this` (211 v 210), so it lands in esi as in retail.
  - New opt-in CTHINGTRACKNODE_LINK.

**Headers**

Q16's overlay started as a copy of Q15's. Each changed header was 3-way merged with its own base:
- Q15's overlay copy for the headers Q16 inherited from Q15.
- f13d5086e for the others.

All merges were clean and every default is the same.
- New: CAIStateGroup_MinionBehaviourCrowdBase.h and CRangeCompressor.h.
- Differ-row opt-ins: CARRAY_SIZE_CTOR_OUTOFLINE and C3DPRIMITIVE2_INDEXBUFFER.

**Names**
- Queued as land1011db: 0x00744430 is `?IsAwarenessOfThingDirect@CTCPerceiveThing@@QBE_NXZ`. Two landed rows spell it
  GetDirectAwareness.
- 0x00795090 is CTCUnholyPower::FinalCleanup (vtable 0x0126BFDC), not the oracle's CTCDivineWrath name.
- Labels: ghidra_out/labels_landing_2026-11-11db.tsv.

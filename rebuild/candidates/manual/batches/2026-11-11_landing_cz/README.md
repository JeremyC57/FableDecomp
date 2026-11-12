# 2026-11-11 landing cz -- chunk Q15 (port-first near misses, Opus 5.5): 2 rows

vland --land: 2 wins (RELOCATION_MATCH), tests PASS. bindings_multi_check is at its NullCoord baseline.
- **0x0092E130 CAIStateGroup_MinionSleep::IsRunnable.**
  - Moving it onto the retail CAIStateGroup_GoToBed, CTCSleep and expanded GetTC headers removed the parked esi/edi swap.
  - Splitting `if (!ready || !base) return false;` into two ifs closed the last gap.
  - New opt-ins: CTCSLEEP_FASTTRACK and CAISTATEGROUP_MINIONSLEEP.
- **0x00713D50 NTCScriptedControl::CActionFollowCreature::SetAsSneaking.**
  - It matched on the first grade of the port: CActionBase::GetParentThing(), expanded GetTC<T>, and the inline
    StatGetInterfaceType / SetSneaking (opt-in CTCSTEALTH_SETSNEAKING).
  - New header engine/NTCScriptedControl_CActionFollowCreature.h.

**Headers**

Q15's overlay was 3-way merged with base f49ce227c. All merges were clean and every default is the same. QuadricOptimiserTypes.h
now carries both Q14's GetInverseMatrixPart and Q15's COptimisedFace.
- New: CThingFilter_IsValidGossipee.h and NTCScriptedControl_CActionFollowCreature.h.
- Differ-row opt-ins: COptimisedPrimitive, CRandomAppearanceMorph, CTCVillage, CVectorMap and the NEntityEvents event/filter
  headers.

**Names**
- Queued as land1011cz:
  - 0x00658D5D is `?IsCurfewTime@CTCVillage@@QBE_NXZ` (const; the bound QAE spelling and IsSitOutsideTime are wrong).
  - 0x00810FC0 is `?GetPHitBy@CEventHitBase@NEntityEvents@@QBEPAVCThing@@XZ`.
- 0x00ACD960 is COptimisedPrimitive::Optimise(float). The SimplifyByVertexPairs name in its draft was made up.
- Labels: ghidra_out/labels_landing_2026-11-11cz.tsv.

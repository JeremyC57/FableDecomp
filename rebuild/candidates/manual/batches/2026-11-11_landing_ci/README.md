# 2026-11-11 landing ci -- chunk Q6 (parked near misses, Opus 5.5): 2 rows on shared headers

Source: `pending/2026-11-11_linux_handQ6/`. vland --land: 2 wins (RELOCATION_MATCH), tests PASS.
- 0x00A66FD0 CMouse::ProcessMaintainedEventsFromEvent: list callees from stlport/fable_containers.h (new opt-ins FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT_NOINLINE and FABLE_RETAIL_STLP_LIST_ERASE_NOINLINE); CInputEvent.h opt-in FABLE_RETAIL_CINPUTEVENT_MOUSE_SETTERS.
- 0x00912840 CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath: deque push-back body from stlport/fable_deque.h (FABLE_STLPORT_DEQUE_PUSH_BACK_AUX_NOINLINE). New headers engine/CBezierCurve.h (PDB layout) and engine/CAIStateGroup_FlyingThingBase.h.
- Coordinator fix: the Middle / Right button names in 0x00A66FD0 were swapped. It was found by the RELOC_NAME warnings and settled by the type each retail SetAs*MouseButtonHeld body stores (Middle 8 at 0x00A03DC0, Right 0xB at 0x00A03E70). Rechecked RELOCATION_MATCH and PASS; bindings re-derived.
- Name fix queued as land1011ci: 0x009E4AA0 is list<CMaintainedEvent>::push_back(), not the catalog's CKeyedOwner_009e4aa0::Register. Q6's own name fixes are queued under the same tag, including 0x00912700 GetRandomFlightPos being protected const and taking references.

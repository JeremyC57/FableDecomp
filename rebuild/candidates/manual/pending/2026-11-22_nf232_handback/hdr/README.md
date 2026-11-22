# NF232 overlay: empty (no header change)

Every changed row uses opt-ins already landed in the repo tree:
- engine/CStartupDefinitionManager.h FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER(_SET, _PCLASS); engine/CCountedPointer.h
  FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL (0x004336B0);
- stlport/fable_containers.h FABLE_RETAIL_STLP_COPY_NOINLINE (0x00411B00), FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE (0x00412B70), the
  default path (0x00412450);
- fable_string.h FABLE_RETAIL_CCHARSTRING(_GETCRC_INLINE), FABLE_RETAIL_CWIDESTRING; engine/CBankFile.h
  FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC; engine/CMainGameComponent.h FABLE_RETAIL_CMAINGAMECOMPONENT(_TEXTBANK);
  engine/CNewFrontendGameComponent.h and engine/NUISystem_CText.h default paths (0x0054F410, 0x0054F370).
So no pin_asserts / assert_names_check / equiv_check run was needed (nothing to pin; no includer changes). orig/ is empty for the same
reason. Entry 2 (q12633) would need header opt-ins in engine/CNewFrontendGameComponent.h and engine/CGameComponent.h; it is blocked
before any edit (NOTES entry 2, Decision 1).

# 2026-11-10 landing af -- chunk 1011, CTCTavernGame and small CTCInventoryMap rows (8 rows)

Source: `pending/2026-11-10_linux_hand1011/`. vland --land: 8 RELOCATION_MATCH, tests PASS (VC7.1 and g++).

- CTCTavernGame: 0x005EA650 OnInitialActivate (same-TU GetSubDef<CTavernGameDef>, PFontManager->GetFont once per font; new CFontManager.h),
  0x005EB180 SetStateAfterInstructions (same-TU SetState body, static no_text_entry, FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER).
- CTCInventoryMap: 0x005FB4B0 TeleportHeroToBoastingArea (/GS), 0x005FBEA0 GetRegionTeleporter, 0x005FBFA0 TeleportToActiveRegion,
  0x005FCBE0 ConstructPastQuestsList / 0x005FDDD0 ...PC (new STLP_LIST_CLEAR_NOINLINE), 0x006061B0 FrameUpdate (vtable slot 9). Manifest
  names corrected (ConstructQuestsScreen_PastQuests(PC) / vfunc_9 placeholders).
- Coordinator at landing: the agent's CThingSearchTools.h predated chunk 1010's GetPFirstThing body and added a second body (without the
  type-mask test); 3-way merged, then the duplicate dropped -- 0x005FBEA0 / 0x005FBFA0 regraded RELOCATION_MATCH on chunk 1010's body
  (CThingSearchTools.h unchanged). Unguarded forward declarations in CTCInventoryMap.h / fable_game.h moved under their opt-ins.
- 0x005EADD6, 0x005EAF46, 0x005EB08D are cuts inside the landed SetStateAfterYesNoAlt / SetStateAfterYesNo / SetStateAfterOK
  (BOUNDS_FIX MIDSTART skip).
- vland notes: `_$E2` RELOC_CONFLICT is the per-TU atexit thunk name (benign); LowerBound 0x004365B0 is a byte-identical twin of 0x0040F020
  already bound by landed rows (queued).

Parked (differ/): 0x005EB590 GivePrize DIFFER(631v631) (register tie: def reload piece takes esi), 0x005FBAA0 GetRelativePosOnMiniMap
DIFFER(634v634) (scale_y homed in the loop-index slot). Unreached: PositionMesh, ProcessInput, DrawPreparePrimitivesForRendering.

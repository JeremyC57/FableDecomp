# hand4a header overlay (one line per opt-in)
Merged into the repo by the coordinator, and dropped from this overlay:
- b1: stlport/fable_containers.h
- b2: engine/CTCInGameMenu.h
- b3: engine/C3DVector.h and the three NCameraModes_CScriptModule*.h headers
- b4: engine/NCameraModes_CBase.h, engine/CGameCameraManager.h and engine/NCameraModes_CRotateAround.h
- b5: engine/CHeroCombatDef.h, engine/CTCCombat.h and engine/CTCHeroExperience.h
The history of each is in NOTES.md and batches/b1..b5/hdr.

Current:
- engine/C2DBoxI.h (copy of today's repo + 2 opt-ins; the default path is unchanged). Both are for row 0x00537776 NUISystem::CList::CanMove.
  - FABLE_RETAIL_C2DBOXI_SETBR: inline SetBRX / SetBRY (FableWin ?SetBRX / ?SetBRY@C2DBoxI@@QAEXJ@Z, debug 0x02185DB0 / 0x02185DD0).
  - FABLE_RETAIL_C2DBOXI_COPY_FORCEINLINE (an #elif beside the existing C2DBOXI_COPY): the same memberwise user copy constructor
    (FableWin ??0C2DBoxI@@QAE@ABV0@@Z 0x018B1BF0), marked FABLE_FORCEINLINE. INFERRED: forcing it is fitted. In the size TU c2 calls
    the plain inline out of line, while retail expands it.

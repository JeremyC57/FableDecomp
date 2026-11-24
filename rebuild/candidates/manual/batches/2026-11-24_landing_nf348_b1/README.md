# Landing nf348 b1 (Account A, 2026-11-24): the two PRIORITY owner ports from nf345 D1

Agent NF348's hand-back is in `pending/2026-11-24_nf348_handback/`. The nf345 b2 binders 0x0041E6D3 and 0x00E1A990 were respelled onto
PDB names whose owners still defined stand-ins. Their references were unresolved until these two owners landed; now both resolve.

## The two owners
- **0x0047D008 `?LoadGame@CTCInGameMenu@@QAEXJ@Z`** (was `?Set@T@@QAEXH@Z`).
  - Ported onto engine/CTCInGameMenu.h with the add-only opt-in `FABLE_RETAIL_CTCINGAMEMENU_LOADGAME`.
  - MATCH before and after; objsec ALL_EQ under the one rename.
  - The link resolves binder 0x0041E6D3 with no duplicates.
- **0x00E1AD30 `?InitialiseVariables@CQ_WizardBattleScript@NScript@@QAEXXZ`** (was `?Init@C@@QAEXXZ`).
  - Ported onto engine/CQ_WizardBattleScript.h with `FABLE_RETAIL_CQ_WIZARDBATTLESCRIPT_INITIALISEVARIABLES`. The header's size assert is
    pinned at its line.
  - MATCH before and after; objsec ALL_EQ.
  - The link resolves binder 0x00E1A990.
  - Name sources: FableWin.pdb, plus retail Main's first call (0x00E1A99F) and the object order Init < Main < InitialiseVariables.
    Ego_r.pdb is silent; it carries no Main, DoMission or EndMission for this class either, and no rival unplaced member fits a
    37-byte variable reset. Ruling NF348 D2 in BRIEF_NF_BATCH accepts this.

## Checks
- **Headers:** NF348's equiv_check over the 27 includers found 0 differences, and its planted control flagged. After landing,
  the coordinator rechecked all 24 compiled includers: 3 MATCH and 21 RELOCATION_MATCH PASS.
- **Pick-any:** neither object emits a copy.
- **Recheck from the repo after the catalog renames:** both rows MATCH PASS.
- **Catalog renames (RENAMES.tsv):**
  - files `CTCInGameMenu_LoadGame_0047d008` and `NScriptCQWizardBattleScript_InitialiseVariables_00e1ad30`;
  - Module `CTCInGameMenu` / `NScript::CQ_WizardBattleScript`;
  - the catalog and manifest names.

## Held / queued (T3 nf348, NAME_FIXES nf348)
- **D2 unit (0x00437990 + 0x00643D8C):** held. The forwarder's object gains the CCountedPointer `operator->` accessor COMDAT, which
  NF326 Q3 forbids (NF320 Q2). The retry is the local spelling.
- **0x007FC800 EndAttackBeam:** identity unit with the 0x0045C5F6 destructor port.
- **0x005EEDCF AddLearnedExpression:** identity unit with descriptive libpk respells of 0x00C22040 and 0x00C222B0.
- **0x00E1A990:** a self port; its object defines Main as QAE against a UAE leaf.

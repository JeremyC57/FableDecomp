# Chunk 831 header overlay

Both edits are opt-in `#ifdef` branches added after the existing lines; no existing line was changed or reflowed, so every current includer
compiles the default path byte-identically. Both files were diffed against rebuild/include at hand-back (2026-11-07): the repo copies
are unchanged apart from these branches.

## engine/CQ_HangingTreeEvilScript.h -- FABLE_RETAIL_CQ_HANGINGTREEEVILSCRIPT_INTRODUCTION (opt-in)
- Adds `void Introduction(); // 0x00D6D080` to NScript::CQ_HangingTreeEvilScript.
- Evidence: Ego_r.pdb / FableWin.pdb `?Introduction@CQ_HangingTreeEvilScript@NScript@@QAEXXZ`; the body plays CS_HANGINGTREE_EVIL_INTRO
  (0x012D2400) and sets IntroCutscenePlayed (+0x48); DoMission 0x00D6BFA0 calls it at 0x00D6C1F8 after SpawnParty.
- User: differ/00d6d080.cpp (parked). Landed includers: none (grep of rebuild/src and rebuild/include).

## engine/NScript_CQ_WaspBossScript.h -- FABLE_RETAIL_CQ_WASPBOSSSCRIPT_WASPHELPER (opt-in)
- Adds the nested forward declaration `class CWaspHelper;` inside NScript::CQ_WaspBossScript; the row defines the class body after the include.
- Evidence: Ego_r.pdb nested entity script CQ_WaspBossScript::CWaspHelper (0x20 bytes: ParentClass +0x14, PMasterData +0x18,
  PleadedToHero +0x1c, LeadToRegion +0x1d, WavedOver +0x1e); vtable 0x012E0284 stored by the landed Alloc 0x00E0F7F6, slot 1 = Main 0x00E0F8A0.
- User: src/00e0f8a0.cpp (RELOCATION_MATCH).
- Landed includers rechecked read-only with the overlay (vland --recheck-landed): 0x00E13310, 0x00E137B0, 0x00E11670 -- all
  RELOCATION_MATCH + PASS (SIBLINGS.tsv).

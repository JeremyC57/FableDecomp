# Chunk 817 notes

Every name below is the exact PDB spelling (Ego_r.pdb + FableWin.pdb name sets) unless marked *descriptive*. The only
descriptive names are locals and macros (DW_T, TY_SPEAK, GM_LINE, counter, lastHits, ...), which have no retail symbol.

## 0x00E07640 NScript::CQ_TraderEscortScript::CDarkwoodTrader::Main: RELOCATION_MATCH (src/00e07640.cpp)
- Shape: intro, then the infected trader lies unconscious until the parent releases it; it waits at the cut-scene trigger
  (fading out at the infected exit when the incubation runs out, or turning into a balverine through the TurnToBalv spawn);
  it follows the hero with banter (a 4-entry switch table) until it dies.
- Model: the landed sibling CEndTrader::Main 0x00E055B0 (defines, includes, move / speak blocks).
- Key spellings:
  - `CTimer::operator<(long)` for the incubation tests.
  - `!(health <= 0)` for the alive test (jnp).
  - The hero-behind distance as one x87 expression `me < hero`.
  - `SetEntityAsRegionFollowing(GetHero(), Me, b)` (hero first).
  - The not-alive wait as `DW_T; for (;;) { Game->NewScriptFrame(); DW_T; }`. VC7.1 tail-merges this into the exit test;
    `while (!T) NSF;` duplicates the test.
- Callees (bound spellings checked one symbol, one address):
  - CDarkwoodTrader::SetBrainState 0x00E0A510 (needs the header opt-in, see hdr/README.md).
  - CQ_TraderEscortScript::MakeTraderComment 0x00E01900.
  - CQ_TraderEscortScript::TurnToBalv 0x00E0A820, reached through CSpawnedFunc1<CQ_TraderEscortScript,CScriptThing> (vtable 0x012DF764).
  - pGlobalScriptDef 0x0143E90C, spelled `CDefPointer<const CScriptDef>` (bound at 00cef3b0). The PDB has a dynamic
    initializer and an atexit dtor for it, so it is a class object.
- Labels to correct:
  - 0x00E0A510 is labelled CTraderScript::HandleTrader; it is SetBrainState.
  - 0x00E0A820 carries the Ghidra autoname Script_Darkwood_Balverine_Trader; it is TurnToBalv.
  - Both are in LABELS.tsv; 0x00E0A510 is also in NAME_FIXES.tsv.

## 0x00E87030 NScript::CV_FisticuffsClubScript::CTyler::Main: RELOCATION_MATCH (src/00e87030.cpp)
- Shape: runs a fight already in progress, counting the hero's hits on the quest bar.
  - On a time-out the hero loses, with the outside or the round-lost cut scene.
  - Winning the final round pays the region reward item, experience and renown.
  - Otherwise it gates entry on the minimum fee and the entry item, then runs the warm-up rounds.
- Model:
  - The chunk 813 CFisticuffsMember::Main draft (preamble, CPredicate_And, the CV_FisticuffsClubScript.h overlay).
  - DarkwoodTrader (map / movie / pause).
- Key spellings:
  - A declared-only `template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` before the movie
    include. With it the first MovieBase dtors expand around the 0x006E7AB0 call and the rest call 0x006E7B80, as retail.
  - `CDefPointer<const CScriptDef> pGlobalScriptDef`, so the def pointer is preloaded into esi.
  - The ring-cleared arm written first as `if (!TimeOut) { ... no return ... } else { lost }`. VC7.1 moves the no-return
    arm out of line, and the lost arm's exit pads follow it.
  - `SetPredicate(CIsThingAlive(Me) && CIsVariableFalse(ParentClass.EverybodyAttack))`.
- CIsVariableFalse is a row-local class with the PDB name (`??0CIsVariableFalse@NScript@@QAE@AB_N@Z`, `??RCIsVariableFalse@NScript@@UBE_NXZ`):
  operator() 0x00E7DDD0, Clone 0x00E7DDE0, dtor 0x00E7DE10. *Header debt:* move it to an engine/CIsVariableFalse.h once a
  second includer needs it.
- Callees:
  - CTyler::CleanupAfterFight 0x00E83680 and CTyler::FightWarmupRounds, both declared in the repo's NScript_CV_FisticuffsClubScript.h.
  - CV_FisticuffsClubScript::BeenAttacked.
  - RunCutsceneMacro 0x00CBFB7D.
  - KillAllThingsInVector 0x00CBED82 `(CGameScriptInterfaceBase&, const std::vector<CScriptThing>&, bool)`, the PDB spelling.
  - FadeOutAndKillEntity via fable_script_fade.h: `?FadeOutAndKillEntity@@YIXABVCScriptThing@@M_N1@Z`, i.e. (thing,
    float, bool, bool), called `(Me, 1.0f, true, true)`.
  - All emitted Main / CleanupAfterFight / FightWarmupRounds / BeenAttacked symbols equal the PDB.
- Labels to correct:
  - 0x00E83680 has module CTyler; it should be NScript::CV_FisticuffsClubScript::CTyler.
  - The landed binding is the global `?CleanupAfterFight@CTyler@@QAEXXZ`.
  - Both are in LABELS.tsv / NAME_FIXES.tsv.

## 0x00D52E90 NScript::CQ_GuildTrainingPreMeleeScript::CTheRealGuildmaster::Main: RELOCATION_MATCH (src/00d52e90.cpp)
- Shape: waits for the teleport and comments while the hero walks up (a 4-entry comment switch).
  - Then the punch and stick cut scenes, each with seven counted dummy hits.
  - Then the smashed-dummy end, a help timer and the beetle toggle.
- Model: the landed melee twin CQ_GuildTrainingMeleeScript::CTheRealGuildmaster::Main 0x00D58490.
- Key spellings:
  - A `__forceinline` CScriptThing::GetPos (the dummy position expands).
  - Each dummy phase's quest counter is block-scoped; retail reuses its slot for the last loop's line temporary.
  - One function-scope `long lastHits;`, declared after `CTimer HelpTimer`, and assigned 0 in each phase.
- Callees are the twin's bound spellings; nothing new to bind.
- MANIFEST: 0x00D52E90 is in neither rebuild/manifest/functions.tsv nor the gap list. vland reports OUTSIDE_MANIFEST (it skips the
  row; the p.sh grade is RELOCATION_MATCH), so the coordinator adds the manifest start before landing: 00d52e90, Main,
  NScript::CQ_GuildTrainingPreMeleeScript::CTheRealGuildmaster, __thiscall, void. Code runs to 0x00D55CF4; the 4-entry comment
  jump table follows, to 0x00D55D04.

## 0x00DFE0F0 NScript::CQ_TraderConflictGoodScript::CTraderToRescue::Main: parked DIFFER(9501v9501), 59 lines
- Residue: retail cross-jumps the whole TRADERB arm, its T-check included, into the TRADERC arm. Ours merges only the
  AddQuestInfoBarHealth tail and keeps B's own T-check, plus 3 alignment pads. Everything else is relocation-only.
- Tried: any spelling that joins B and C before frame allocation gives retail's exact branch structure but shifts every
  temporary slot by 4..16. This covers:
  - a `__forceinline` helper `||`;
  - two literal helpers;
  - a goto into C's T-check.
- So retail's join is VC7.1's late cross-jump, which this lane does not reproduce for the T-check. That makes 20 attempts
  over chunks 815 + 817.

## 0x00E6BC50 NScript::CV_ChapelOfEvilScript::CCOE_AcolyteEvil::Main: parked DIFFER(7692v7655), 100 lines (best a5)
- a5 adds a row-local `~CCountedPointer<CScriptGameResourceObjectScriptedThingBase>` specialization (Info = 0 before Data = 0),
  which fixes the PImp zero-store order.
- Left:
  - The teleport loop's register pair (retail i = ebx, begin = ebp).
  - The natural-exit GROBase vptr store order.
  - The T-exit pad tail: retail keeps a separate pad, +37 bytes. This is GOTCHAS chunk 801 epilogue cross-jumping.

## 0x00D647F0 NScript::CQ_GuildTrainingWoodsDepartureScript::CFinalMaze::Main: parked DIFFER(7405v7389), 316 lines (chunk 814 base)
- Residue: in the phase-2 lightning branch, retail evicts ebx (the flag word of the `||` temporaries) to cache the Game
  vtable across GetHero / AddPersonToConversation. VC7.1 spills the vtable instead, and the frame-slot order follows.
- 10 attempts were inert or worse.
- A `CScriptThing& hero` local drops the spill but moves GetHero ahead of the vtable load, so it was not kept.

## Name / binding observations (no action this chunk)
- 0x0143E90C is bound under six spellings in reloc_bindings.tsv (pGlobalScriptDef as CDefPointer<const CScriptDef>,
  CDefPointer<CScriptDef> in NGlobalScriptDefs, CScriptDef*, g_ScriptDef, g_pScriptDef, X_00e54990::ScriptDef).
- The PDB shows a dynamic initializer / atexit dtor for 'pGlobalScriptDef', so it is a class object. That rules out the
  plain pointer spellings; the namespace and const-ness are not proven.
- These rows use the global `CDefPointer<const CScriptDef>` spelling already bound by landed row 0x00CEF3B0.

## Verifier (read-only vland, tmp/vland.log)
- 0x00E07640 and 0x00E87030: RELOCATION_MATCH + PASS. 0x00D52E90: OUTSIDE_MANIFEST (see above). No RELOC_IDENTITY.
- RELOC_NAME warnings:
  - SetBrainState -> 0x00E0A510 'CTraderScript::HandleTrader' is the label fix in LABELS.tsv.
  - The others are pre-existing label drift: the map dtor 0x00CDBFB0 'CRetailTree_erase00cd344a::~...', and
    `operator char const*` / `CDefString::operator CCharString` underscore spellings.

## Header debt
- None beyond the CIsVariableFalse note above.

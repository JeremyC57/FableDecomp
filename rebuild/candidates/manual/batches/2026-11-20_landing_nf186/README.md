# Landing nf186 (Account A, 2026-11-20): quest / script function names (14 rows)

Agent NF186 (round 10) took the quest / script function entries and finished its slice: 49 entries, 14 done, 32 done-already, 2
blocked and 1 deferred. Its files are in `naming/2026-11-20_agentNF186/`. Most done-already entries are q48xx Anniversary-wave lines
that naming wave 35 had already re-modelled; HEAD-compile probes confirm the binders of the rest. q4860 goes back to the queue: its
row was locked by NF184, which has since landed.

- 14 rows land: 3 MATCH + 11 RELOCATION_MATCH, every test PASS.
- Twelve self renames onto the shared script headers:
  - Five ??_G stand-ins become the compiler-generated PDB ??_G on the 0x00E016F0 placement-new scaffolding: CCROWDBANDITS 0x00D0B3C0,
    CSpeedFriend 0x00D40890, CBeggarBully 0x00E5B390, CDF_SummonsCreatures 0x00E70370, CManWithDoorName 0x00ED17B0.
  - CSP_UndeadTrigger GetParentScript / OnPredicateFail 0x00DEF700 / 0x00DEF720.
  - CQ_SecretPassageScript::InitialiseVariables 0x00DED0A0, with its test rewritten on the header (it linked the old free function).
  - ProcessMission 0x00DA8510, GetVillagerSpeechIndex 0x00DAEBF0, WatchForTermination 0x00E15500, Should_I_Flee 0x00E4DF20.
- Two more rows are respelled in place with no symbol change (0x00E64DF0, 0x00E95F10).
- 0x00DA8510 now carries `// Flags: /d2inlS22`, with an `// Inline-budget:` line, in place of a FABLE_FORCEINLINE in its old local
  model. The catalog CompilerFlags are synced. /d2inlS20 and /d2inlS21 differ; 22 is the smallest value that matches.
- Headers: seven add-only, declaration-only opt-ins. `orig` equals HEAD, default paths are unchanged, and no landed includer defines
  them; the asserts pinned are listed in `naming/.../hdr/README.md`.
  - CQ_BanditCampBossBattleScript.h CROWDBANDITS_CTOR
  - CQ_GuildTrainingScript.h SPEEDFRIEND
  - CQ_MinionClifftopChaseScript.h GUARDCOMMENT
  - CV_BeggarAndChildScript.h BEGGARBULLY_CTOR
  - CV_DemonDoorsScript.h SUMMONSCREATURES_CTOR
  - CV_SingingStonesScript.h MANWITHDOORNAME_CTOR
  - NScript_CQ_SecretPassageScript.h UNDEADTRIGGER
- Pick-any check: the new copies of ??_GCEntityScriptBase, ??_7 / ??0 CEntityScriptBase and placement new equal their owners
  (0x00CDEBF0 and the landed 0x00E016F0; agent cmpbody, bytes and relocations).
  - The table binds placement new `??2` to 0x00405C10, but that row defines only a folded CFourierAnalysis stand-in, so
    pickany_check reports MISSING. The NO-OWNER-OBJ lines are the self rows' own vtables.
- Held as blocked:
  - q4871 0x00DEF730 / q4879 0x00EE1BE0, the ??_G rows of CSP_UndeadTrigger and CTH_SnowTroll. Their ctor owners define the PDB ??0
    strongly, plus a one-slot stand-in ??_7. Porting the ctor rows comes first (queued).
  - Landed Alloc 0x00E702C0 still emits its own ??_GCDF_SummonsCreatures copy. Its bytes are identical, but it binds the never-defined
    ??1CDF_SummonsCreatures of a local model, so it is queued as debt.
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created; includers and referencing rows 64 good, 0 bad.
- tablecheck (`logs/tablecheck.txt`): 0x00CB7940 10 -> 9 and 0x012ECBA8 2 -> 1; none grows. 0x00DA8510 moves GuardComment 0x00DA8970
  onto its NScript leaf (its only referencer). 0x00E15500 moves the CCharString ctor PBDH -> PBDJ, and IsActiveThreadTerminating onto
  the CScriptBase member.
- Link (`logs/link_landing.log`): 2 LNK2005 against 5 at HEAD (`logs/link_control_head.log`). The CLandscapeBackgroundPatch / `VecDel@T`
  stand-in clashes are gone, and no new one appears.

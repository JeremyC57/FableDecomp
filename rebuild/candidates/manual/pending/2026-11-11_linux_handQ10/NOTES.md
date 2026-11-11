# Chunk Q10 (agentQ10) -- 40 parked near misses

All grades with `FABLE_HDR_OVERLAY=$A/hdr` (two new opt-ins, see hdr/README.md). Every draft was regraded once as it sat
(prior/ with `// Flags: none` stripped and a full `// Leaf:` added where it lacked one): none of the 40 changed grade by itself.
35 of the 40 drafts are local models (padded / stand-in classes) and are not landable as written; that, not the residue, is
the main debt of this pool.

## Matched (4, all RELOCATION_MATCH, tests PASS under g++ -std=c++03 and VC7.1 /Od)
| row | identity | lever |
|---|---|---|
| 0x0078F8F0 | CTCDivineWrath::CancelForCutscene() | shared headers (CThingCreatureBase / CTCDivineWrath / CTCSpecialAbilities / CWorld / CEnvironment / ThingAlive) spelled exactly as the landed byte-twin CTCUnholyPower::CancelForCutscene 0x00794E60 (GetParentThing() as a CThingCreatureBase&, `GetTC(&ptc_spec_abils)` + UnLockAbilitiyMutex, StopSound inline); new opt-in FABLE_RETAIL_CTCDIVINEWRATH_STOPSOUND_INLINE (the StopSound body in class). The local-model draft was 506v505. |
| 0x00D43190 | NScript::CQ_GuildTrainingScript::CKillBird::Main() | identity corrected (the draft said CQ_AwakeningTheOracleScript::CPretendGuard::Main = landed 0x00CFCCA0); new opt-in FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_KILLBIRD (Ego_r layout); predicate vtable 0x012C2FE8 = NScript::CIsThingConscious; spelled as 0x00CFCCA0: `if (IsActiveThreadTerminating()) return; do { NewScriptFrame(); } while (!IsActiveThreadTerminating());` gives retail's jump into the bottom test. |
| 0x00E6AD20 | NScript::CV_ChapelOfEvilScript::WatchForAcolyteDeath(CScriptThing) | shared headers; the landed WatchForDeputy1Killed 0x00D16570 loop shape (`for (;;)` with the terminate return first, `bool killed` / `bool hit` locals, `break` to one trailing terminate check) + FABLE_FORCEINLINE ~CScriptThing (retail's zero-in-edi needs the forced dtor) + scoped `#pragma inline_depth(0)` around the out-of-line exits, the last region ending after the closing brace (the scope-end destructor). Line-2 Inline-budget note. |
| 0x00EDC980 | NScript::CV_TempleOfLightScript::WatchForAcolyteDeath(CScriptThing) | byte-twin of 0x00E6AD20, same source on engine/CV_TempleOfLightScript.h. |

## Closest near miss
- 0x00E4AA00 NScript::CV_BanditTollScript::CBanditLeader::Main: migrated onto the shared headers (CV_BanditTollScript.h nested
  CBanditLeader, CIsThingAlive, CScriptGameResourceObjectScriptedThingBase, CTimer with FABLE_FORCEINLINE operator= / GetTime
  through pDSTGame, fable_script_distance.h, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations). /d2inlT200 + scoped
  inline_depth(0) on the in-loop returns move the handle dtor's inline site to the loop exit as retail. 1356v1356, ONE register:
  the inline release's reload of Info before operator delete is eax (ours) v ecx (retail 0x00E4AE44). c2prio --rotation: plain
  rotation with the pointer at eax; the codegen order lists the angered block (lines 55-69) between the loop bottom and the
  dtor. Moving the angered block to the loop end (negated condition + continue) is byte-identical. 7 grades.

## Parked as they sat (differ/<addr>.cpp line 2 = measured residue + the PARKED_INDEX diagnosis)
- 0x00ED9BB0 (298v306): the only gap is retail's entry `mov [esp+4],0` (a conditional-temporary flag home, GOTCHAS chunk 845);
  which conditional class temporary retail wrote is open. Draft still has a local CCharString / vector<CCharString> stand-in.
- 0x00666310 (915v908): retail's RMW `and byte ptr [esi+15Ch],7Dh` (Blinking/HeadPosCached); chained or reordered clears are
  byte-identical on the local model (compile probes). engine/CThingCreatureBase.h now has the real bitfields.
- 0x00435190 (783v774): the ENGINE def release -- the landed 0x0042E204 fixed the same shape by defining GetDef<CEngineDef>
  (0x00430096) in the TU under auto_inline(off).
- 0x00E37E40 / 0x00D90290: same families as 0x00E4AA00 (dtor inline site) and 0x00E6AD20 (zero register from a forced
  ~CScriptThing); both need the shared-header migration first (CV_AssassinAttacksScript.h, CQ_HobbeCaveScript.h + CCounter.h +
  CScriptDef.h: HC_BanditTimeBeforeComment +0xCAC, HC_BanditCloseToMarkerDistance +0xCC4 -- the draft's names/offsets differ).
- 0x0062B070: q.py grades it against the 1866-byte auto-re-candidates oracle (two functions); needs SPLITS.tsv (891).
- The rest: regraded unchanged; see each differ line 2.

## Names
LABELS.tsv: the four matched rows + 0x00E4AA00. No landed spelling proven wrong (NAME_FIXES.tsv empty): the 0x00D43190
CPretendGuard mislabel is only in the old draft (ghidra_out/labels_anniversary_2026-11-04_waves23.tsv already has CKillBird).
Callee spellings of the matched rows all agree with rebuild/oracles/reloc_bindings.tsv (checked symbol by symbol).

## Header debt / notes for the lander
- 0x0078F8F0 turns on FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE (it gates CTCSpecialAbilities::UnLockAbilitiyMutex /
  StatGetInterfaceType / FadeOrKillEffect and CTCSoundPlayer::StopAttachedSound, all needed here too); a neutral name for that
  opt-in would read better.
- 0x00E6AD20 / 0x00EDC980 use the same pragma device as the landed 0x00D16570 (which carries a "Revisit: inline-depth" note).

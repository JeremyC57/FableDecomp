# Landing 2026-11-05x: chunk 796 (3 of 6 rows landed, 3 parked)

One expert agent authored the rows (sharing the lane with the chunk 795, then 797 and 798, agents). All three landed rows are plain
C++ and grade `genuine`; the agent paired every relocation with its retail target.

| Address | Name | Grade |
|---|---|---|
| 0x008315C0 | CThingAICreature::ConstructFromLevelScript | RELOCATION_MATCH PASS (13 grades; the ledger label `vfunc_4` was vtable 0x0127293C slot 4) |
| 0x00E80F10 | NScript::CV_FishingCompetitionScript::CFishCompOrganiser::AskForFish | RELOCATION_MATCH PASS (10 grades) |
| 0x00E55CE0 | NScript::CV_BookCollectingScript::CBS_Teacher::AskForBook | RELOCATION_MATCH PASS (8 grades) |

- **Headers** (`hdr_README.md`): nine additive opt-in blocks in eight headers (CThingAICreature / CThingCreatureBase LEVELSCRIPT,
  CThingCreatureBase ANIMATION, CCreatureActionBase INTERRUPTION, CTCWeapon LOADING, CVectorMap INSERT, stlport/fable_containers.h
  STLP_SORT_FASTCALL, CCombatActionBase SOUNDCTOR, CTCDPhysicalObstruction CREATE). The overlay was additive against the current tree
  (no merge needed); hdrcheck2 reads `default=same`, and every DIFF macro is new.
- **vland warnings:** 8 RELOC_NAME, all wrong landed labels the agent proved (PlayCutscene 0x00E80D60, CheckForRod 0x00E81A60,
  RefreshCompetition 0x00E7FD50, the map<CCharString,CCharString> ctor / dtor 0x009AC2D0 / 0x009AC310, the cast-map dtor 0x00CDBFB0,
  ~_List_base 0x004046B0): queued.
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-05x.tsv` (the six rows, with the real names ConstructFromLevelScript and
    CModeStrafe::AnimUpdate, and unlanded callees).
  - 43 queue lines (tag `land0511x`), including 0x0122DEDC: the shared 0.0f is the compiler literal `__real@00000000`, not a named
    global (five landed spellings: g_CombatMinDistance, _kZero, _kMinGain, _g_FloatZero, _HEALTH_UNSET). This answers HANDOFF's
    open 0x0122DEDC decision with evidence; it waits for the next naming pass.
  - 0x00662930 ConstructFromLevelScript: the PDB spelling is public virtual (`UAE`); 0x006A9960 binds a non-virtual stand-in (queued).
  - **Open decision:** `pGlobalScriptDef` 0x0143E90C is spelled two ways in landed rows (148 as `CScriptDef*`, 15 as
    `CDefPointer<const CScriptDef>`); an older audit line (`cg10`) records the same conflict. The new rows use the majority spelling.
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0.

## Parked (`pending/2026-11-05_linux_hand796/`)
| Row | Grade | Residue |
|---|---|---|
| 0x008E18D0 NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams | DIFFER(1833v2073), 12 attempts | retail inlines CVectorMap::LowerBound, the vector overflow copy and ~CParticleUpdateParameters only in the sky-beam block; making LowerBound inlinable inlines it at all three sites |
| 0x008E9B70 NCreatureMode::CModeStrafe::AnimUpdate | DIFFER(1699v1705), 18 attempts | eax/edx/ecx swapped at three CAnimInit + AddCombinationAnim sites; retail keeps PullBackAmount with `fst` |
| 0x00951FB0 CAIStateGroup_JackDragon::UpdateHover | DIFFER(2229v2272), 13 attempts | VC7.1 sinks the distance math below later calls (retail computes and spills it first: the 12-byte frame gap); the first route push_back inline; exit-block order |

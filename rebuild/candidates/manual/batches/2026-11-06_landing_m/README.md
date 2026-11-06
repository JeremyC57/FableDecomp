# Landing 2026-11-06m: chunk 812 (2 of 6 rows landed, 4 parked)

One expert agent authored the rows (sharing the lane with the chunk 811, then 813, agents; a container restart stopped it once and
it was resumed from its transcript). The six rows were unnamed gap-scan Mains: two quest-own Mains and four entity Mains, five
ending in a jump table. The agent proved every name from the vtable slot and both PDB name sets. The landed rows are plain C++ and
grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00D2AA40 | NScript::CQ_EndGameFocalSitesScript::Main | RELOCATION_MATCH PASS (6 attempts; a 4-entry jump table; the battle offset summed in place, the random angle its own statement; chunk 805's `_JACKOFBLADES` opt-in, no header change) |
| 0x00EA59E0 | NScript::CV_LostTraderScript::CLostTraderBrother::Main | RELOCATION_MATCH PASS (3 attempts; a 4-entry jump table; the state-4 body in its own block so two locals share a frame slot; the distance compare loads its left operand first) |

- **Headers:** only engine/CV_LostTraderScript.h is copied in (FABLE_RETAIL_CV_LOSTTRADERSCRIPT_BROTHER, additive, needed by
  0x00EA59E0; no landed source defines it). The parked rows' opt-ins (FANMARKER, GATE2GUARD1, CAMPHOSTAGEGUARD, the BountyHunt `MAIN`
  additions) stay in `pending/2026-11-06_linux_hand812/hdr/` (as for chunks 810 and 811).
- **vland warnings:** RELOC_ALIAS on the `rand` thunk; RELOC_NAME on 0x00CDBFB0 (the queued map-dtor placeholder).
- **Names.** Labels: `ghidra_out/labels_landing_2026-11-06m.tsv` (the six Mains plus the unlabelled 0x00D0EBC0
  `CQ_BanditCampBossBattleScript::OpenGate`). 4 queue lines (tag `land0611m`): 0x00D15140 is `TeleportData::TeleportData` (catalog
  `CMainGameComponentInit::CMainGameComponentInit`; the body builds four CCharStrings at 0 / 0xC / 0x10 / 0x14, the 24-byte PDB
  layout), 0x00D15170 `TeleportData::operator=` lacks its `NScript::CQ_BountyHuntScript` scope, and the landed 0x00EA7460 binds
  `WatchForQuestFailureState` without NScript (0x00EA59E0 binds the PDB spelling).
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0; bindings_multi_check prints only
  the known NullCoord case; the 11 new bindings add no second symbol at a target and no symbol at a second address.

## Parked (`pending/2026-11-06_linux_hand812/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00D0D910 NScript::CQ_BanditCampBossBattleScript::CGate2Guard1::Main | DIFFER(4928v4752), 10 attempts | the movie-dtor budget; `~CTimer` expanded at every exit (retail 2 of 3); exit pads |
| 0x00D09010 NScript::CQ_BanditCampBossBattleScript::CCampHostageGuard::Main | DIFFER(5880v5864), 10 attempts | the movie-dtor vptr store; one exit copy shared with the epilogue; `TalkedTo` at frame byte 0x1F (retail 0x23) |
| 0x00CF4EC0 NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main | DIFFER(6244v6164), 10 attempts | `~CScriptThing` called inside the inlined `~CIsThingAlive` but expanded at plain temporaries; the movie dtor |
| 0x00D13580 NScript::CQ_BountyHuntScript::Main | DIFFER(6831v6917), 11 attempts | the movie dtor; VC7.1 cross-jumps three identical post-loop thread checks into one (retail 39 checks, ours 37) |

**The movie-dtor residue** (three of four parks, and chunk 810's 0x00D1B200): retail expands the implicit
CScriptGameResourceObjectMovieBase dtor at each site as `call 0x6E7AB0`, the GROBase vptr store 0x0126008C, `call 0x0099A430`; our
header calls the COMDAT 0x006E7B80, and MOVIEBASE_FORCEINLINE expands every site but adds a MovieBase vptr store retail lacks. No
plain C++ spelling found yet: header debt (agent_NOTES.md). Chunk 814's brief asks for it.

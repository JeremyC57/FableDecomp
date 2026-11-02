# Hand chunk 717 (NScript::CIsThingAlive .. the late-.text dynamic initializers): 10 of 10 byte-exact

- 1 MATCH, 9 RELOCATION_MATCH. All ten tests PASS under g++ (tall.sh) and VC7.1 (tall.py).
- Read-only vland with `FABLE_HDR_OVERLAY=hdr`: 10/10 wins, behaviour PASS (`tmp/vland.log`; `lv/` deleted).
  - Its warnings are the expected `_$E2` RELOC_CONFLICTs (TU-local names).
  - The RELOC_NAME warnings come from the three wrong atexit-thunk labels below.
  - SHARED_HEADER_AVAILABLE for CList is a false positive (see Header debt).
- Every `// Leaf:` was confirmed with `objdump -t`, and every call target against reloc_bindings.
- Float constants were checked with const_check (0.5f, 0.1f: 0 mismatches). String literals were checked against
  Fable.exe by hand, because the grader masks them.
- Extents: all ten end in `ret` / `jmp` followed by int3 padding (or the next initializer). No extent fixes.

## What each row really is
Nine of the ten are **whole compiler-generated dynamic initializers** (`.text$yc`, one `.CRT$XCU` entry each, found
with `xref.py`). None is an EH funclet or a library (CRT / STLport) object. Each is a Fable static's `_$E` initializer,
authored the house way (the object defined in its own TU, Leaf `_$E1`), like the landed 0x0121E260 / 0x0121F074 /
0x0121A670. The tenth, 0x00CDF120, is a virtual member function.

| Address | Identity | Global | Shape |
|---|---|---|---|
| 0x0121F220 | `anonymous namespace'::VALUE_DISPLAY_DURATION (CTCOverheadDisplay TU) | 0x013BAE68 `const float` | `0.5f * CMainGameComponent::GetConstantFPS()`: `fild ; fmul` |
| 0x0121F260 | CTCDrunkenness::DrunkennessDeltaPerFrameOfDrinking | 0x013BAE70 `static const float` | `0.1f / GetConstantFPS()`: `fild ; fdivr` |
| 0x0121F780 | `anonymous namespace'::JACK_TOTAL_DURATION (Jack of Blades flash mode TU) | 0x013BAFD8 `long` | `g_01388a7c + g_01388a80` |
| 0x0121F7A0 | `anonymous namespace'::JACK_GLOW_START_FRAME | 0x013BAFDC `long` | `JACK_TOTAL_DURATION - g_01388a80`; TOTAL defined first, so Leaf `_$E3` |
| 0x0121F7E0 | CTCCreatureModeManager::CModeBase::NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS | 0x013BAFE8 `static const long` | `GetConstantFPS() * 12` |
| 0x01221B60 | `anonymous namespace'::MIN_TIME_TO_SIT (seat state-group TU) | 0x013BB6F4 `const long` | `GetConstantFPS() * 20` |
| 0x0121A6AC | NUISystem::CList::GPreviousSelected | 0x013B8A98 `std::list<NUISystem::CList*>` | STLport 4.6 list ctor; `#pragma optimize("s", on)` |
| 0x0121E3A0 | CTCTrophy::InstructionsText[2] | 0x013BAB84 `CCharString[2]` | braced literals through the implicit `CCharString(const char*, long = -1)` |
| 0x0121EF70 | NFire::OPINION_SOURCE_DEF_NAME | 0x013BADE0 `const CCharString&` (temporary 0x013BADD8) | reference bound to a literal temporary |
| 0x00CDF120 | NScript::CIsThingAlive::operator()() const | - | `return Thing.IsAlive();` (CScriptThing slot 75, virtual tail call) |

## Names and their evidence (two sources each; full detail in each source header)

### The dynamic initializers
The method was the same for each row:
1. Read retail's `.CRT$XCU` order (the table at 0x01374300 .. 0x0137470C) and each neighbour's literal / object.
2. Align it with the `dynamic initializer for 'X'` runs of Ego_r.pdb and FableWin.pdb, which are TU-contiguous.
3. Cross-check against the Ego_r body sizes (`naming_stragglers/ego_r.exe.signatures.tsv`) and the retail readers of
   the global.

Rows:
- **VALUE_DISPLAY_DURATION** -> REACTION_MANAGER -> DrunkennessDeltaPerFrameOfDrinking -> CTCOracleMinigame::SymbolStrings
  -> CTCFireheartMinigame::BlockNames.
  - Both PDBs list this order. Retail runs 0x0121F220 / F240 ("OPINION_REACTION_MANAGER_INSTANCE") / F260 / F280 / F2D0.
  - Ego_r sizes are 19 / 19, matching retail.
  - Readers: CTCOverheadDisplay 0x007B7xxx, and the drunkenness adders 0x0093A4E0 / 0x0093AA07.
- **JACK_TOTAL_DURATION / JACK_GLOW_START_FRAME / JACK_FADE_DOWN_TIME**, then CModeBase::NO_FRAMES_BEFORE_PLAYING_COMB_IDLE /
  _IDLE / _WAVE_ANIMS.
  - Both PDBs list this order. Retail runs 0x0121F780 / 7A0 / 7C0 / 7D0 / 7E0 / 800.
  - Ego_r sizes are 17 / 11 / 16 / - / 14, matching retail.
  - The algebra fits the names. CModeJackOfBladesFlash's activation 0x008D7F10 rewrites the JACK trio at run time with
    the same relations, which proves them non-const and in the same TU.
  - GetNoFramesBeforePlayingIdleAnims 0x0073A760 (PDB IAEJ) reads 0x013BAFE8.
- **SIT_ON_CHAIR .. GET_UP_FROM_FLOOR, then MIN_TIME_TO_SIT** (both PDBs).
  - Retail's six literals "ST_SIT_ON_SEAT" .. "ST_STAND_UP_FROM_FLOOR" precede 0x01221B60.
  - The following run (fps*120, *10, *50, "ST_SIT_ON_FLOOR", "ST_STAND_UP_FROM_FLOOR", *10, *50, *5) matches the PDB
    names RANGE_TIME_TO_SIT .. TIME_TO_LEAVE_SHOUT_MESSAGE one for one.
- **GPreviousSelected**, then GAlreadyHovered.
  - Source 1: Ego_r order, 29 bytes and unaligned, as retail.
  - Source 2: FableWin's `GetPreviousSelected` return type proves `list<NUISystem::CList*>`.
  - The data spelling is the landed 0x005E41E0 binding, kept so the address keeps one name. Its access is unproven.
- **CTCTrophy::InstructionsText**.
  - Source 1: Ego_r order, 46-byte initializer and 32-byte atexit, as retail; then MIN_SPEECH_FRAMES at 14 bytes,
    as retail's 0x0121E3D0.
  - Source 2: the trophy-rules literals, and the CTCTrophy reader 0x006DF1F3 indexes the table.
- **NFire::OPINION_SOURCE_DEF_NAME**.
  - Source 1: FableWin.pdb is the only PDB with it. It lists REACTION_MANAGER_DEF_INSTANTIATION_NAME -> NFire::OPINION_SOURCE_DEF_NAME,
    which is retail's 0x0121EF50 -> 0x0121EF70.
  - Source 2: CTCFlammable 0x00780EA6 passes `[0x013BADE0]` as AttachOpinionSource's `const CCharString&`.
  - The reference-to-temporary model reproduces retail's unusual order (atexit before construction, then the
    address store) byte for byte.

### NScript::CIsThingAlive::operator()
- Vtable 0x012C32F0, slot 0. Its slot 2 is the landed ??_GCIsThingAlive 0x00CDF160.
- Ego_r.pdb 0x00CC4A70 and FableWin.pdb name it `??RCIsThingAlive@NScript@@UBE_NXZ`.
- The callee is ?IsAlive@CScriptThing@@UBE_NXZ (slot 75, +0x12C), reached through the vtable.

### Types, const and access
Retail bytes cannot settle these, and there is no PDB data-symbol dump. What I chose:
- `const` on VALUE_DISPLAY_DURATION, MIN_TIME_TO_SIT, DrunkennessDeltaPerFrameOfDrinking and NO_FRAMES_*: never
  written after init.
- Not const on JACK_*: proven written at run time.
- Public on the class statics, which is house style.

### Descriptive (unproven)
- `g_01388a7c` (new) and `g_01388a80` (the existing landed spelling) are two statically initialised longs (60 / 40)
  of the Jack-flash TU.
- No PDB names them, because they have no dynamic initializer.

## Parked for Fable 5.1 (0)

## Skipped (0)

## Label corrections
The catalog row labels (all `_gapscan::sub_*`) become the identities in the table above. Other labels:
- **0x012294D0** `CFontBankStateBlock::CreateCRC` is `dynamic atexit destructor for 'CTCTrophy::InstructionsText'`.
- **0x01228E44** `_global::Static_OnReadFinished_on_013b8a98_01228e44` is `dynamic atexit destructor for
  'NUISystem::CList::GPreviousSelected'`.
- **0x01229B40** `_global::AtexitDtor_CCharString_at_013badd8_01229b40` is the destructor thunk of the
  OPINION_SOURCE_DEF_NAME temporary.
- Each of these three was graded RELOCATION_MATCH from this chunk's row TU at the thunk's own address (`thunks/`).
  Their landed sources are invented `AutoTiny*` stand-ins; two of them have Leaf lines that match no symbol.
- **Neighbouring landed initializers** with descriptive names. The class-static spellings are in NAME_FIXES.tsv.
  - 0x0121F7C0 = JACK_FADE_DOWN_TIME
  - 0x0121F7D0 = CModeBase::NO_FRAMES_BEFORE_PLAYING_COMB_IDLE_ANIMS
  - 0x0121F800 = CModeBase::NO_FRAMES_BEFORE_PLAYING_WAVE_ANIMS
  - 0x0121E3D0 = MIN_SPEECH_FRAMES
  - 0x01221B80 / B90 / BA0 = RANGE_TIME_TO_SIT / MIN_TIME_TO_LEAVE_BETWEEN_SITTINGS / RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS
  - 0x01221BF0 / C00 / C10 = the next TU's MIN_ / RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS / TIME_TO_LEAVE_SHOUT_MESSAGE
- **Uncatalogued initializers** seen in passing, which no catalog row covers:
  - 0x0121F280 (CTCOracleMinigame::SymbolStrings, by the PDB order)
  - 0x0121E210, 0x0121F590, 0x0121F5C0, 0x0121F850, 0x0121F86F, 0x01221D20, 0x01223CB0

## New spellings: collisions with landed bindings (said explicitly)
The new names below give an address a second spelling next to a landed stand-in, so the coordinator must respell
the landed readers in the same landing:
- 0x013BAE68: new `_VALUE_DISPLAY_DURATION` (internal), landed `?g_OverheadValueExpiryFrames@@3MA` (0x007B7A70).
- 0x013BAE70: new `?DrunkennessDeltaPerFrameOfDrinking@CTCDrunkenness@@2MB`, landed `?g_DrunkennessPerFrame@@3MA` (0x0093A8C0).
- 0x013BAFE8: new `?NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS@CModeBase@CTCCreatureModeManager@@2JB`, landed
  `?g_DefaultFrames_013bafe8@@3JA` (0x0073A760).
- 0x013BAFD8 (JACK_TOTAL_DURATION) gets two anonymous-namespace hashes, one per source file: `?A0x968ebc88` from row
  0x0121F780 and `?A0x35e1d656` from row 0x0121F7A0 on this lane. The hash comes from the file path, so this is
  unavoidable for one anonymous object that two rows touch.
- New, with no landed binding: 0x013BAFDC (JACK_GLOW_START_FRAME), 0x013BB6F4 `_MIN_TIME_TO_SIT`, 0x013BAB84
  `?InstructionsText@CTCTrophy@@2PAVCCharString@@A`, 0x013BADE0 `?OPINION_SOURCE_DEF_NAME@NFire@@3ABVCCharString@@B`,
  and 0x01388A7C `?g_01388a7c@@3JA` (descriptive).
- Unchanged: 0x013B8A98 uses the landed GPreviousSelected spelling, and 0x01388A80 the landed `g_01388a80`. Note that
  13 more stand-in spellings of 0x013B8A98 are in reloc_bindings (NAME_FIXES.tsv).

## Header debt
- **fable_string.h**: fixed in `hdr/` with the opt-in FABLE_RETAIL_CCHARSTRING (default length -1); see `hdr/README.md`.
  Not fixed: the private member is `storage_`, where Ego_r.pdb names it `PStringData` (struct_layouts_egor.tsv).
  Renaming it changes no bytes, but it is a default-path edit (1,309 landed includers).
- **engine/CList.h**: the generated header is a flattened global `struct CList`, with the Ego_r 0x240 size and retail's
  0x1FC. The real class is `NUISystem::CList` (class key V, namespace NUISystem).
  - vland's SHARED_HEADER_AVAILABLE warning on 0x0121A6AC is a false positive: that row only defines the class
    static and touches no layout.
  - The header should be regenerated into `namespace NUISystem` with the class key V.
- **No CMainGameComponent header exists.** The rows use the established minimal declaration (protected static
  ConstantFPS + inline `GetConstantFPS()`), the same as the landed 0x0121F7D0 / 0x0121F800.

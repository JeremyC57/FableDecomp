# Chunk 811 notes: six unnamed gap-scan Mains (6.2 to 6.8 KB)

Result: 1 matched (RELOCATION_MATCH), 5 parked. Every attempt is in STATUS.tsv. Each address was graded sequentially, with at
most two grades running at once.

All six names are proven from two sources: the slot in the RTTI-anchored vtable plus the Ego_r.pdb and FableWin.pdb name sets.
The repo labels were already right (anniversary naming wave 3). Every callee spelling comes from rebuild/oracles/reloc_bindings.tsv
or the PDB name sets (cited in each source header). One symbol per address was checked; the conflicts found are in
NAME_FIXES.tsv.

## 0x00DCA6D0 NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::Main: RELOCATION_MATCH (attempt 1)
- **Shape:** generated entity script with plain `return;`, a T-check before every statement, and the CIsThingConscious predicate
  (row-local inline ctor/dtor).
- **Defines:** STLP_MAP; out-of-line ScriptedThingBase dtor/assign; out-of-line MovieBase ctor/dtor; the forceinline
  CWideScreenMagicPauseEntities ctor.
- **Key spellings:**
  - The KEEPERMEET block (seh_hero, cast map, movie, pause, RunCutsceneMacro) is its own nested `{}` inside
    `if (!doors_open)`, so `doors_open = true` comes after ~seh_hero.
  - The skeleton_visited talk chain mixes the order of `talk_count = N` and the speech:
    - `<2`: speak then set 2.
    - `==2` / `==3` / `==4`: set first, then speak.
    - PRE_SKELETON: `==0` speaks then sets 1; the else branch sets 1, then speaks.
  - The talk tag is a by-value `CScriptThing talk_tag`, and its text is `CCharString text = talk_tag.GetDataString();`, used
    inside `if (!DoorChange)`.
- **Strings:** 36/36 in order; every relocation is bound-same.
- **Descriptive local names (no retail source):** talk_count, talk_tag, text, conversation. retail_asm: no.

## 0x00EDD170 NScript::CV_TempleOfLightScript::CTOL_AcolyteGood::Main: parked, DIFFER(6597v6594), 130 lines (10 attempts)
- **Shape:**
  - TRetVoid `return _Fail_Val.Return();`, `for (;!T;NSF)` main loop.
  - A row-local inline `~CScriptThing` (the natural loop exit expands ~altar, as retail does).
  - Prayer timer as `Timer + rand()%Var` (retail reloads pGlobalScriptDef after rand).
  - SetHeroAge as `Min + rand()%Var`.
- **Residue:** epilogue cross-jumping only (GOTCHAS chunk 801). VC7.1 keeps the shared `[~altar, ~seh_me, pops, ret]` tail after
  the FIRST pad (exit_point), while retail keeps it after the LAST (vector) pad. All earlier body instructions match, apart from
  relocation zeros.
- **Tried without effect:** while-form loop, mixed / plain returns, extra blocks.
- **Descriptive names:** exit_point, altar, acolytes, count, morality_change, donation.

## 0x00DD6A60 NScript::CQ_PrisonEscapeRescueMotherScript::CMotherTorture::Main: parked, DIFFER(6478v6494), 69 lines (7 attempts)
- **Shape:** TRetVoid returns, the CIsThingAlive predicate, and the row-local inline ~CScriptThing, so the GetHealth temporaries
  expand at the first site per block (742 → 69 lines).
- **Key spelling:** the OBJECTIVE_06 4th argument is a plain `""`, not `CCharString("")` (temporary order).
- **Residue:** the shared GOTCHAS chunk 799 / 810 blocker. Retail expands ~CIsThingAlive one level (vtable from ebx, nested
  ~CScriptThing called). Under VC7.1 it is all-or-nothing, so ebx/ebp colouring and the pop-ebp placement at three return pads
  differ.
- **Tried:** `__forceinline` (234 / 3115 lines), named predicate, definition after Main, plain returns. All identical or worse.
- **Descriptive names:** nag_timer, comment_timer, lever, door, run_point, entrance, conversation.

## 0x00DBB310 NScript::CQ_NewOakValeIntroScript::CNOVI_Bully::Main: parked, DIFFER(6527v6639), 203 lines (7 attempts)
- **Shape:** the CNOVI_Victim model (TRetVoid, CIsThingConscious predicate, cast map + text map BULLYRUN cut-scenes, Sprintf
  intimidation line).
- **Key spelling:** `home_pos` and its walk loop in their own `{}` give frame 0x13c as retail (368 → 203 lines).
- **Residue:**
  - The home loop's GetScriptThing temporary is called and the loop is rotated, where retail expands it and does not rotate. This
    is the same inline ~CScriptThing / ~CIsThingConscious conflict as MT.
  - The AddQuestInfoBar CRGBColour temporaries sit at 0x70 (retail 0x7c).
  - One block at 0xDBC71F is laid out differently.
- **Descriptive names:** the globals `g_NOVIBullyIntimidateRange` (0x013AC85C, 15.0f) and `g_NOVIBullyIntimidateRepeatChance`
  (0x013AC860, 75). Neither PDB name set holds data symbols, so they stay descriptive. Also the locals home_pos, item, answer,
  line, mymap, textmap.

## 0x00DE4CB0 NScript::CQ_RansomVictimScript::CVictimsFather::Main: parked, DIFFER(6714v6778), 141 lines (6 attempts)
- **Shape:**
  - The CIsThingAlive predicate.
  - The parent-reference seh release `{ CQ_RansomVictimScript& parent = ParentClass; ... }`.
  - Row-local CTimer ctor/dtor, and forceinline GetTime / operator=.
- **Key spelling:** the bodyguard's IsAlive at sites 2-3 goes through `const CScriptThing* p_guard = &guard;`, which keeps virtual
  slot 75 (GOTCHAS 2147) and fixes the block order (390 → 141 lines).
- **Residue:**
  - The first hit block's GetHealth temporary dtor is called (retail expands it). This is the same blocker.
  - The movie / pause / temporary frame slots are a permutation of retail's.
- **Descriptive names:** over_here_timer, facing_timer, guard, p_guard, answer, conversation.

## 0x00DF6010 NScript::CQ_TraderConflictEvilScript::Main: parked, DIFFER(6247v6493), 2010 lines (5 attempts)
- **Shape:**
  - Quest Main modelled on the Good twin, with five `CEntityScriptBinding<CQ_TraderConflictEvilScript>` bindings.
  - **Flags 0:** retail stores ebp=0 at +0x18 in all five bindings; Flags 1 was wrong.
  - The STLport AllCreatures vector, an inline `CScriptThing::operator==` (slot 78 IsEqualTo) for the hand-written erase loop, and
    an inline GetDataString.
- **Residue:**
  - Whole-function colouring: retail has ebx=this, ebp=0 and the conditional-temporary flag word in memory, with frame 0x80. Ours
    has ebp=this and ebx=0, with frame 0x7c.
  - The destructor split. Retail calls 6 plain ~CScriptThing temporaries and expands ~CScriptThing plus the vector element loops at
    17 loop-exit return pads (0xDF6D58..0xDF7948), sharing the vector pads.
  - Ours calls all of them (no inline ~CScriptThing) or expands all 36 (with it: 8215 bytes).
- **Descriptive names:** beds, bed_count, intro_pos, fakes_created, fake, follower, fighter, unused_name (retail keeps a
  default-constructed CCharString at [esp+0x54]).

## Labels to correct (LABELS.tsv)
- 0x00DBCD00: should be `NScript::CQ_NewOakValeIntroScript::CNOVI_Bully::GivenTeddy`. The catalog name lacks the namespace.
- 0x00CDBFB0: the placeholder `CRetailTree_erase00cd344a::~CRetailTree_erase00cd344a` should be
  `std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map` (vland RELOC_NAME on the GK row).

## One-symbol conflicts (NAME_FIXES.tsv)
These addresses are bound under several spellings: 0x00CDBFB0 / 0x00CDBF70 (cast map dtor/ctor), 0x01375748, 0x0099F1F0
(Sprintf v Format) and 0x0143E8EC (CScriptThing::NullString). The rows use the PDB / decided spelling.

## Header debt
None. All header work is opt-in or a new file (hdr/README.md).

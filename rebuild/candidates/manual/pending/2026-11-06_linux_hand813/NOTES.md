# Hand chunk 813 (CPreMeleeWhisper .. CFisticuffsMember): 2 of 6 byte-exact

All six names are proven from two sources and use the exact PDB spelling. For each row:
- Its only .rdata reference is slot 1 of an entity vtable, read from the Alloc or ctor that stores it.
- The same mangled name is in both `ghidra_out/egor_pdb_names.tsv` and `fablewin_pdb_names.tsv`.

The proofs are in LABELS.tsv. Every grade used the overlay in hdr/ (see hdr/README.md).

## What matched (2)
- **0x00D524A0 `NScript::CQ_GuildTrainingPreMeleeScript::CPreMeleeWhisper::Main`** (2540 bytes, 19-entry jump table): RELOCATION_MATCH.
  - What it does: the pre-melee guide Whisper. She waits for her cut-scene and follows the hero. When he talks to her, or when both
    stand within 7 units of a numbered PreMeleeChatMarker, she removes that marker and says its guide line.
  - The lesson: the marker number of each vector element is a named local, compared after its CCharString dies. Writing
    `if (GFCharStringToInt(...) == n)` sets the flag before the dtor (245 lines).
  - Case order = source order = descending literal addresses.
  - vland: RELOCATION_MATCH + PASS.
- **0x00F17C70 `NScript::CQ_ArenaScript::CArenaCellDoorGuard::Main`** (7740 bytes, 8-entry ArenaRound jump table): RELOCATION_MATCH.
  - What it does: the arena cell door guard.
    - He answers by arena state: early chat, the enter-arena question, and the per-round line pairs with the second guard.
    - A yes fades the screen and starts the arena.
    - He heals off hero hits and walks back to / faces GuardingDoorMarkerRight.
  - The lesson: the main loop is `for (;; NSF) { if (T) return ...; ... }`.
    - Its loop-top return is retail's natural exit 0x00F1994C, with ~seh_me expanded.
    - The first StartScriptingEntity wait pad 0x00F17EBC joins that expansion.
    - Every other return cross-jumps to the one [~movie; ~seh_me; ret] tail 0x00F19A72.
    - `for (; !T; NSF)` expanded ~seh_me only at the natural exit and duplicated the epilogues (193 lines).
  - relpair: every binding is the same as the existing ones; strings 48/48.
  - vland: RELOCATION_MATCH + PASS.

## Parked for Fable 5.1 (4)
- 00E5B3B0 CV_BeggarAndChildScript::CBeggarBully::Main DIFFER(6996v7012): 138 lines, all from one blocker.
  - Retail expands the SetIsPushableByHero by-value Me copy (0x00E5B484..A4: two vtable stores, PImp copy, refcount inc) and calls
    the CScriptThing copy 0x004ABE90 inside the expanded CIsThingAlive ctor. Ours calls both, which then recolours the vtable
    registers up to 0x00E5B578 and moves one alignment pad.
  - Tried, 14 attempts:
    - The declared-only CCountedPointer<MovieBase> dtor: fixes the movie dtor split.
    - The same-TU noinline Compare body: fixes the esi data pointer and the frame slots.
    - IMPLICIT_COPY (644).
    - A reference-bound predicate temporary.
    - Bool declaration order.
    - Inverted talk branches.
    - The MOVIEBASE OUTOFLINE_DTOR / FORCEINLINE_CTOR / FORCEINLINE opt-ins.
  - next: lane gap: the chunk 810 CIsThingAlive copy blocker. It is the only residue.
- 00E84B80 CV_FisticuffsClubScript::CFisticuffsMember::Main DIFFER(8968v8836): 190 lines.
  - VC7.1 expands the implicit ~CScriptGameResourceObjectScriptedThingBase at the natural loop exit. Retail calls it there and
    cross-jumps to the shared [~destination; ~ring; ~seh_me] tail 0x00E86DC6. The case-1 block and the pads follow.
  - Tried, 10 attempts: plain returns; SCRIPTEDTHINGBASE_OUTOFLINE_DTOR (265); CTimer in the for-init; an explicit return after the
    loop; `for (;; NSF) { T_RET; }` (203: matches every byte to the loop exit, then expands the loop-top pad); a declared-only
    ~CTimer.
  - The diagnostic /d2inlT72 is identical (190), so this is not an inline-budget match. No /d2 flag is in the row.
  - next: lane gap: the per-exit implicit-dtor expansion (GOTCHA chunk 809 class).
- 00DB97A0 CQ_NewOakValeIntroScript::CNOVI_Theresa::Main DIFFER(6954v7013): 873 raw / 181 masked.
  - Retail calls the CScriptThing copy 0x004ABE90 inside the expanded CIsThingConscious ctor, and ~CScriptThing inside its expanded
    dtor, while expanding ~CScriptThing at the exits. With the row-local inline ~CScriptThing the predicate dtor is called out of
    line instead.
  - Tried, 6 attempts: inline ~CScriptThing; __forceinline ~CIsThingConscious (195); a named predicate local in a block (new,
    identical); `for (;; NSF) { T; }` (539); the implicit inline movie ctor (identical).
  - next: lane gap: the chunk 810 CIsThingConscious copy/dtor blocker.
- 00DB5330 CQ_NewOakValeIntroScript::CNOVI_BarrelMan::Main DIFFER(6228v6148): 769 raw / 317 masked. The same predicate blocker.
  - Retail calls 0x004ABE90 inside the expanded predicate ctor while the four by-value Me copies expand
    (SetWanderCentrePoint / Min / Max, SetScriptingStateGroup). With IMPLICIT_COPY the predicate copy expands too, and
    ~CIsThingConscious is called out of line.
  - Tried, 5 attempts:
    - The declared copy (839).
    - IMPLICIT_COPY (789).
    - No MOVIEBASE_OUTOFLINE_CTOR (426), because retail inlines the movie ctor at the hit site.
    - `for (;; NSF) { T; }` (677).
    - Both together (317), which is the best.
  - next: lane gap: the chunk 810 blocker, then recheck the exit ~CScriptThing expansions.

## Skipped
None.

## Label corrections
- 0x00E83920: correct name `NScript::CV_FisticuffsClubScript::CFisticuffsMember::CFisticuffsMember`.
  - FableWin.pdb has `??0CFisticuffsMember@CV_FisticuffsClubScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z`,
    and the landed ctor row stores vtable 0x012EB0F8.
  - The autoname labels were 'CScriptThing_Constructor_Helper' / 'ScriptThingContext_Construct_A'.
- NAME_FIXES.tsv has two bound spellings that are wrong:
  - FadeOutAndKillEntity 0x00CBE716 is bound with `_NM1` (bool and float swapped) by row 00e0f4e0. Both PDBs spell it `M_N1`.
  - The CPredicate_And<CIsThingAlive, CIsVariableFalse> ctor 0x00E7E020 is bound as `??0CAndPredicate@@...CIsFlagFalse` by row
    00e8b020. Neither class is in either PDB.
- 0x013AC854 / 0x013AC858 (BarrelMan's overhear modulus 100 and notice distance 10.0f) have no PDB name. I named them descriptively
  (BarrelManOverhearChance / BarrelManNoticeDistance).

# Chunk 814 notes

**Result:** 2 RELOCATION_MATCH (0x00EA8F40, 0x00E44EA0) and 4 parked DIFFER. All six names are proven from two sources: the entity vtable
slot 1, stored by a landed Alloc, plus `?Main@...@NScript@@EAEXXZ` in both the Ego_r.pdb and FableWin.pdb name sets. No row name is
descriptive. Every attempt is in STATUS.tsv and ATTEMPTS.log.

## Blocker-bonus finding: implicit MovieBase dtor expansion
- **Spelling:** plain C++ with no special dtor trick reaches retail's mix of expanded and called `~CScriptGameResourceObjectMovieBase` sites.
  1. Keep the implicit dtor.
  2. Declare only `template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` (declared, not defined).
- **Grades that prove it:**
  - **0x00EA8F40, RELOCATION_MATCH.** The expanded form (`call 0x006E7AB0`, GROBase vptr 0x0126008C, `call 0x0099A430`) sits at the shared
    T-exit pad 0xEA9B82, and the first movie ctor is expanded (0x99A380 + vptr 0x01260EF4). Every later site calls 0x006E7B60 / 0x006E7B80.
    The function-wide inline budget alone decides which sites expand.
  - **0x00E44EA0, RELOCATION_MATCH.** Two expanded dtor sites, the rest called. Same spelling.
- **Where it still fails:** rows where retail expands at *every* site (EntranceGuard 0x00CF68C0).
  - The gate is frame-slot sharing. Retail gives both talk-block movies slot +0x14 with seh_me at +0x24 (frame 0x15C). The T-exit pads can then
    be cross-jumped, which leaves fewer dtor sites (6) within the budget.
  - VC7.1 here puts seh_me at +0x14 and the movies at +0x44 / +0x34 (frame 0x160). The result is 14 sites, the budget runs out, and 11 sites call.
- **Next lever:** declaration order and scoping that make the two talk movies share one slot.
- **Ordering trap:** when the row also defines `FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR` (or uses the movie header's
  inline ctor), the specialisation must be declared before the movie header is included. Declare it right after `engine/CCountedPointer.h`
  with a forward `class CScriptGameResourceObjectMovieBase;`. Otherwise you get C2908 (explicit specialisation after instantiation).

## 0x00EA8F40 NScript::CV_MurderTwistScript::CMurderTwistLeader::Main (RELOCATION_MATCH, 1 attempt)
- **Model:** the landed CMurderTwistBrotherYounger::Main 0x00EADBB0. It uses the same defines, includes, movie dtor specialisation, inline
  `~CScriptThing`, and inline CIsThingAlive ctor.
- **Body:**
  - Predicate, then the seh_me START.
  - Four `while (ParentClass.MurderTwistState==MTS_x)` talk loops, each with a speech movie and literal English SAY lines
    (`GROUP_SELECT_NONE,false,true,false`).
  - The yes/no answer loop (answer==1: State 2, Event 2).
  - The `ItemsFound` 0/1/2 branches with GiveHeroGold(200) / (500).
  - The `for(;;){NSF; T}` tail.
- **Bindings:** relpair matched all 48 strings, and every symbol is bound-same in reloc_bindings.tsv.

## 0x00E44EA0 NScript::CV_BordelloScript::CBordelloClient::Main (RELOCATION_MATCH, 2 attempts)
- **Model:** the landed CBordelloGuard::Main 0x00E40420 opening.
- **Parent calls:** IsHeroDressedAsLady 0x00E3E320, PlayCutscene 0x00E3E720, seh_Whore +0xA4, csargs +0xB4.
- **Spellings:**
  - `Me.GetDefName()=="..."` needs `FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ` + `FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE`.
  - Line text is built as `WHO+GFIntToCharString(ClientID+1)+TAG+GFIntToCharString(NextLine)` (fastcall operator+ / GFIntToCharString,
    `FABLE_RETAIL_CCHARSTRING_CONCAT_STRING`).
  - Row-local CTimer ctor / dtor / GetTime are plain inline. `operator=` is `__forceinline`: attempt 1 called it at the one `timer=120` site,
    and that was the only diff (35 lines).
  - Row-local `CScriptThing::GetPos` (NullCoord when PImp is null).
  - fastcall `IsDistanceBetweenThingsUnder(stand,Me,5.0f)` and `FadeOutAndKillEntity(Me,2.0f,true,true)`.
- **Bindings:** relpair matched 44 strings, and every symbol is bound-same. The only unbound symbol is the compiler literal `__real@00000000`.
- **vland:** raised a RELOC_NAME warning only for the 0x0099E4C0 placeholder label (see Labels).

## 0x00CF68C0 NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard::Main (PARKED, DIFFER(8210v8070), 1299 lines, 4 attempts)
- **Parked early on blocker 2.** Two ideas not in GOTCHAS were tried first:
  - **a2:** a declared-only `template<> CCountedPointer<CScriptThing>::CCountedPointer(const CCountedPointer<CScriptThing>&);` makes the by-value
    CScriptThing copy at 0x00CF6982..0x00CF699C call 0x00704430 inside an otherwise inline copy, as retail does. 1308 -> 1299 lines.
  - **a3:** MOVIEBASE / CWIDESCREEN FORCEINLINE_CTOR expands all 4 movie ctors as retail does, but the frame grows (1416 lines).
- **Residue:** the frame-slot / dtor-expansion analysis above.
- **Header:** new opt-in CEntranceGuard overlay (hdr/README.md).

## 0x00D647F0 NScript::CQ_GuildTrainingWoodsDepartureScript::CFinalMaze::Main (PARKED, DIFFER(7405v7389), 316 lines, 10 attempts)
- **Shape:**
  - Opening cut-scene (`FABLE_RETAIL_STLP_MAP` cast map, map ctor/dtor 0x00CDBF70 / 0x00CDBFB0 declared-only).
  - Three rounds (melee / bow / lightning) with per-phase hit counters. Hits are checked with `MsgIsHitByWithWeapon` and
    `MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_LIGHTNING_SPELL)`.
  - Forced CTimer operator= / GetTime, then the win cut-scene.
- **Residue:** register-masked SDIFF is only 10. In the phase-2 lightning branch retail evicts ebx (the counter, reloaded from [esp+0x1c]) to
  cache the Game vtable, while VC7.1 here spills the conversation to [esp+0x14]. Frame-slot order of the timer and text temporaries follows
  from that. Every call, string and relocation matches.
- **Header:** CFinalMaze added to chunk 810's new CQ_GuildTrainingWoodsDepartureScript.h.

## 0x00E6BC50 NScript::CV_ChapelOfEvilScript::CCOE_AcolyteEvil::Main (PARKED, DIFFER(7692v7655), 102 lines, 10 attempts)
- **Model:** chunk 811's CTOL_AcolyteGood twin.
- **Key spelling:** `VictimHealth*(rand()%3+1)+50` (112 -> 102 lines).
- **Residue:**
  - The follower-loop register pair: retail has i = ebx and begin = ebp; ours are swapped.
  - A separate T-exit pad that retail keeps for the size()>1 check. VC7.1 here cross-jumps it (GOTCHAS chunk 801).
  - The natural-exit ~seh_me store order.

## 0x00E2FF00 NScript::CV_AmbushScamScript::CAS_Buddy::Main (PARKED, DIFFER(8551v8567), 387 lines, 10 attempts)
- **Residue:** the CScriptThing copy split.
  - Retail calls the Par1 copy 0x004ABE90 and the by-value param dtor 0x004AA840 inside each of the 3 forced CSpawnedFunc1 ctors. MSVC is
    callee-destroys, so the param dtor sits in the callee.
  - Yet retail expands the same copy and dtor at depth 1 elsewhere.
  - Best: a `__forceinline` row-local copy.
- **Diagnostics (not candidates):**
  - inline_depth(1) as a probe: 2542 lines.
  - Explicit CSpawnedFunc1 instantiation: inert.
- **Note:** the landed CUndeadRisingZombieAttacker::Init 0x00E0DAA0 spawn relies on a now-banned inline pragma.

## Labels to correct (LABELS.tsv / NAME_FIXES.tsv)
- **0x00CF0C00:** label `sub_00cf0c00`; should be `NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard::GetParentScript`.
  - FableWin.pdb `?GetParentScript@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@UBEPAVCScriptBase@@XZ`.
  - vtable 0x012C6158 slot 3; body `8B 41 14 C3`.
- **0x0099E4C0:** labels `operator_char_const*` (manifest) and `CCharString_ToConstChar_API` (FSE); should be `CCharString::operator const char*`.
  - `??BCCharString@@QBEPBDXZ` is in both PDBs and is already bound to this address, so this is a label fix only.
  - It causes the vland RELOC_NAME warning.
- **0x00704430:** landed as `CCountedPointer<CUnprovenPointee_00704430>` copy ctor (file named ...CBankFileAsyncEntry...). Proposed:
  `??0?$CCountedPointer@VCScriptThing@@@@QAE@ABV0@@Z`.
  - FableWin.pdb name set, plus the EntranceGuard caller typed through CScriptThing::PImp.
  - Not yet in reloc_bindings.tsv, so no one-symbol/two-address conflict. The draft 00cf68c0 binds it via the declared-only specialisation.
  - Caveat: the identical 25-byte body may be folded with other instantiations.
- **One symbol, one address:** no symbol bound in either matched row is bound elsewhere at a different address.

## Header debt
None beyond the overlays in hdr/. For CEntranceGuard, the repo header lacked the nested class; it is now an opt-in.

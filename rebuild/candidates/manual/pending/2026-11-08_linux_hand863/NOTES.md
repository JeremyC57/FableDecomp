# Chunk 863 notes

The chunk has 8 rows. 1 is matched (RELOCATION_MATCH) and 7 are parked in differ/. Every row has a standalone behaviour test that
PASSes (`<addr>_TEST PASS`). All names are proven from two sources: Ego_r.pdb / FableWin.pdb plus a vtable slot or spawn literal
(LABELS.tsv). No descriptive names are used.

## 0x0043B570 CPlayerGui::CPlayerGui -- RELOCATION_MATCH (src/)
- **Shape:**
  - The five reference parameters are stored first, in retail order. The members are then built in declaration order, which is
    the order chunk 853's ~CPlayerGui destroys them.
  - CPlayerGui::PeekGuiDef (0x00438570) is defined in the TU and expanded in the ctor.
- **Key spelling:** the one-argument `resize(n)` on the three ability-button vectors. v5 used a two-argument resize and placed the
  temporary store differently (md 2).
- **Headers:** opt-in overlays in hdr/ (CPlayerGui, CPlayerGuiDef, CSubtitleRenderer, NHeroInformationScreens_CManager,
  NPlayerGui_CDrawBase, NPlayerGui_CDrawNPCName). See hdr/README.md.
- **Labels:** landed row 0x00438570 binds wrong callee names (InitGuard, Builder::Build, AcquireBuilder). NAME_FIXES.tsv has the
  correct spellings.
- **Header debt:** CPlayerGuiDef.h flattens its CDefClassBase base. PeekGuiDef therefore needs a `reinterpret_cast` from
  `PGuiDef.InternalGetObject()`. Fix: give CPlayerGuiDef its CDefClassBase base under the CDEF opt-in.

## 0x00ECE7F0 NScript::CV_SickChildScript::CWitch::Main -- DIFFER md 4 (differ/)
- **Shape:**
  - SetPredicate(CIsThingAlive(Me)), then the parent-reference seh_Witch release and the priority-4 StartScriptingEntity loop.
  - The ingredient loop runs `while (ThingsForPotionGot < 4)` with:
    - the talk block (CS_SICK_WITCH once, objective 02);
    - the TakeObject loop written as `bool more = true; while (more) { if (...) {...} else more = false; }` so it is not rotated;
    - CS_SICK_POTION with objective 03, then `break`;
    - the hit block, using a switch on TimesHit for ONHIT_10/20/30.
  - Afterwards, `for (; !T; NSF)` handles GOT_CURE and the hit block.
- **Levers that worked:**
  - a declared out-of-line `CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()` (0x006E7AB0), which gives
    retail's count of movie-dtor expansions;
  - a row-local `inline ~CScriptThing() {}`, which expands the one hit-1 temporary dtor.
- **Residue:** at 0x00ECF317 retail emits `mov eax,esp; push eax` where ours emits `push esp`. This is the hidden return slot of
  CDefString::operator CCharString 0x00415D70 for `pGlobalScriptDef->EndPotion`. A byte scan finds that pattern before a call
  only here and in libvorbis.
- **Tried, no effect:** 40 spellings, including casts, explicit / static_cast / functional temporaries, operator* / ->, a named
  const ref, GetString, `this->`, a parent reference, and inline flags.

## 0x00ECBDF0 NScript::CV_SickChildScript::CTalkingTrader1::Main -- DIFFER md 24 (differ/)
- **Shape:** the twin of the landed CTalkingTrader2::Main, plus a CTimer that paces four chats. The CTimer ctor, dtor, GetTime
  and operator= are defined inline in the row.
- **Residue:** retail expands ~CTimer (DeregisterTimer, slot 88) exactly once, at the outer loop exit. The talk-block thread check
  at 0x00ECC25C cross-jumps there.
  - Plain source expands it instead at the hit block's speech-wait return (md 30).
  - The line-93 `do { } while (0)` lever (Inline-split note on line 4) gets the loop-exit expansion, but the hit-exit one stays
    (md 24).
- **Probes:** removing return sites shows VC7.1 only ever expands ~CTimer at a pause exit or at a lever site.
- **Inline device tried (owner rule 2026-11-08):**
  - `/d2inlT38..40` removes every expansion; T41 and above keeps both.
  - `#pragma inline_depth(0)` around the hit exits also outlines the CWideScreenMagicPauseEntities dtor there, even when it is
    `__forceinline`, and grows the frame.
  - So no device lands it yet.

## 0x00DCFA60 NScript::CQ_OrchardFarmRaidScript::DoCutsceneIfRequired -- DIFFER md 41 (differ/)
- **Shape:** the main path sits inside `for (; !IsActiveThreadTerminating(); Game->NewScriptFrame())`, with the evil and good
  cut-scene blocks in `{}` scopes.
- **Header:** a new opt-in header engine/CQ_OrchardFarmRaidScript.h (hdr/) with the retail layout. Evidence is in its comment and
  in hdr/README.md.
- **Residue A (inline split):** VC7.1 expands the inline CScriptThing release at three exits; retail expands it at two.
  - Retail calls 0x004AA840 from the post-wait thread check at 0x00DCFC14.
  - Because of the extra copy, the shared operator-delete tail lands in a different copy.
- **Residue B (schedule):** at DeactivateQuest("Q_OrchardFarm_Barricade", 0), retail schedules `lea ecx` between the two
  CCharString ctor pushes and `lea eax` before `push ebp`.
  - The landed rows 0x00DCC140 (0x00DCC613) and 0x00E58B8B show the same interleave, and it arises naturally there.
  - No argument spelling reproduced it here.

## 0x00D11930 NScript::CQ_BanditCampBossBattleScript::CAssassinMarker::Main -- DIFFER md 263 (differ/)
- **Shape:**
  - The three assassins are made unkillable.
  - The loop runs `while (!AssassinCutsceneTriggered && !attacked)`, with:
    - under attack: three SAY blocks, best-enemy targets, then release;
    - otherwise: the overheard conversation switch over six lines.
  - The three assassins are then made killable, and the cut scene runs if it was triggered.
- **Header:** the CAssassinMarker class declaration is the opt-in FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSINMARKER in
  hdr/.
- **Residue:** retail expands ~CIsThingAlive at the SetPredicate temporary.
  - It keeps the vtable 0x012C32F0 in edi and re-stores it, then calls ~CScriptThing 0x004AA840 out of line.
  - Because of that expansion, retail packs the temporary at +0x3C. Ours calls 0x00E241B0 and the temporary sits at +0xBC, so every
    later slot shifts.
- **Variants tried:**
  - a plain inline definition: unchanged;
  - `__forceinline` or an implicit dtor: the nested release also expands (md 403-404);
  - inline_depth 0/1/2: no help;
  - definition order and a scoped named predicate: no help;
  - `/d2inlT80`: md 170 (diagnostic only).
- **Contrast:** landed GUARD2 0x00D6A360 expands ~CIsThingAlive because its TU never makes ~CScriptThing inline.

## 0x00D45060 NScript::CQ_GuildTrainingScript::CheckFriendlyAttacks -- DIFFER md 375 (differ/)
- **Shape:**
  - The "make everything but the sparrow / Whisper apprentices / maze friendly" loop runs once, then again inside the level-load
    wait.
  - The maze hit check follows: three warnings, or the guild-seal cut scene.
- **Residue:** frame slot order. Retail has maze at +0x18, the first creatures vector at +0x24, and the name temporary at +0x30.
  Ours puts the vector lowest. Moving the vector to function scope grows the frame to 0xB4.
- **Unproven idea:** a helper for the repeated loop. No PDB member name was found for it.

## 0x007A1B20 CTCWillLightning::CLightningBeam::FrameUpdate -- DIFFER md 562 (differ/)
- **Model:** a local-model draft. No shared header fits CLightningBeam, or CThing slot 65 OnHit / GetTC / AddTC.
- **Constants:**
  - TCI values: SOUND_PLAYER 16, ENTITY_EVENTS 66, PARTICLE_EMITTER 75, ELECTROCUTED_EFFECT 138, WILL_LIGHTNING 148,
    DECAPITATION 209;
  - CHitParameters fields: Decapitate +0x1B, MoveBack +0x22, CreateDustParticleEffectOnHit +0x24, Blocked +0x26,
    HitNegated +0x27;
  - CMainGameComponent::ConstantFPS is at 0x01375550.
- **Def members:** CSpecialAbilitiesLightningSpellDef has InitSound +0x110, LoopingSound +0x114, EndHeroSound +0x118 and
  EndAttackeeSound +0x11C. The landed Deactivate row calls +0x11C "HitSound"; NAME_FIXES.tsv corrects it.
- **Header debt:**
  - engine/CThing.h needs a retail branch exposing GetTC / AddTC / OnHit (slot 65) and the TC interface map.
  - CTCWillLightning.h needs to declare CLightningBeam (0x7C bytes).

## 0x00A2F740 C3DMeshStats::InitialiseFromFileData -- DIFFER md 1333 (differ/)
- **Model:** a local-model draft.
  - NameCRC uses g_crc32Table 0x0129A168 with init 0 and no final xor.
  - A dummy whose name starts with "createmesh " (via GetAsLowerCase 0x0099ED10) becomes a C3DGeneratorObject.
- **Status:** 438 bytes short of retail. The STLport vector and sort instantiation splits are not reproduced.
- **Header debt:**
  - C3DMeshFileHelpersChunk needs FindFirst/NextPoint, Dummy and Volume.
  - C3DMeshFileRootChunk needs HasHelpers / GetHelpers.
  - C3DMeshStats needs the retail layout.
  - fable_string.h needs GetAsLowerCase / StartsWith. A root-level header cannot be overlaid; see LESSONS.md.

## Inline-budget rule (2026-11-08)
No parked row lands under the new inline control:
- 0x00ECBDF0 and 0x00DCFA60 A are inline splits, but the devices also move a neighbouring inline (the pause dtor / the copy
  merge).
- 0x00DCFA60 B and 0x00ECE7F0 are scheduling / codegen residues, which the rule does not cover.

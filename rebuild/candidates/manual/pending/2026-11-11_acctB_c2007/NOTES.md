# Hand chunk 2007 (CTCTavernGameShoveHaPenny .. CThingCreatureBase): 4 of 4 byte-exact

All four rows grade RELOCATION_MATCH under VC7.1 RTM with the overlay (`FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2007/hdr`), their
behaviour tests print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py), and the read-only vland run passed all four
(RELOCATION_MATCH PASS; only RELOC_NAME label warnings, all listed in NAME_FIXES.tsv / LABELS.tsv). Every call target was checked
against retail with rel.py (one wrong target found and fixed: the const LowerBound in 0x00665860's look lookup).

## What matched (4)
- **CTCTavernGameShoveHaPenny::ProcessInput 0x007CF9A0** (vtable 0x0126F2F4 slot 28). Positioning slides the held coin
  (`Held->Pos.X += movement.X * PositionCoinSpeedPC * frame_time`, clamped with STLport std::max / std::min to Minimum / Maximum x
  BoardSize.X); yes starts aiming (MousePosEffect / AverageJoystickPos reset to (0, -0.2), the coin's start velocity); aiming blends the
  normalised mouse effect into AverageJoystickPos (C2DVector operator* / operator+), sets the coin velocity from it while the action
  button is held and flicks the coin (sound, WATCH_COINS) when it is let go; yes with enough velocity drops the coin; the quit buttons ask
  SetStateAfterYesNo while a coin state runs and stop the coin sounds. Shapes: the y impulse `-(AverageJoystickPos.Y * JoystickScalePCY)`
  as a named float (retail fchs + fadd; one expression folds to fsubr); `AverageJoystickPos.Y != 0.0f` (constant loaded first for fucompp);
  GetGameState() as the last SetStateAfterYesNo argument (the sibling's lever). **Device:** `// Flags: /d2inlT116` (owner inline-budget
  decision, preference 2): the c2 inline trace (hook c2.dll 0x1078D1AA) gives the third std::max<float> site (0x007CFB45) c1 19 at static
  frequency 0.0273 (switch case 1/4 x `(A && B) || (C && D)` 0.4375 x two if/else halvings), score 115 >= 74; T115 still calls it, T116
  expands it and the whole row matches (the call also flipped the def / input_process registers). GetSubDef<CTavernGameShoveHaPennyDef>
  (score 174) stays out of line. Plain-C++ alternatives traced (named moving bool: f 0.039, score 86; else-if spellings: 0.0137) never
  reach f 0.047.
- **CTCTrophy::GlobalUpdate 0x006DEEE0** (catalog `vfunc_26`). The dialog answers of the trophy minigame: yes (ExtraData 1) starts the
  minigame, first unsheathing the trophy for its inventory owner (CCreatureAction_UnsheatheItemFromInventory temporary) unless a creature
  carries it; no (0) shows the trophy (CCreatureAction_UnsheatheToShowToSheatheTrophy) and posts the two opposite show-trophy deeds with the
  bandit faction; the third answer (2) shows instructions page 0; click-past ends the game, clears the score wait, pages the instructions
  (DONE / NEXT button) and then shows the play-game text with the available-witness count and the best score, or unpauses. Shapes: an
  if / else-if chain on ExtraData with RemoveMessage after it; StartMinigame in both arms of the carrier test (cross-jumped); the
  play-game block is the `else` of the page test (frame 0xD8 -> 0xD4); GetPFaction(name.GetCRC()); **device:**
  FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION (`// Inline-budget:` line) for the animation-name conversion temporary: retail expands
  the ??B forwarder and calls GetString 0x00415DD0 (the plain inline header calls the ??B COMDAT 0x00415D70, a different target with
  identical bytes; an explicit `.GetString()` changes the frame).
- **CTCTrophy::OnShowTrophyPerformAction 0x006DE8C0**. Gained witnesses who notice the shown trophy get bored (deed 57, personality
  override, opinion-reaction state group); available ones move to the gained list (erase + push_back, emote cleared, time bonus, pending
  gained-witness sound, a good / evil deed by EvilTrophy == creature group 4, a personality override unless married to / divorced from
  the hero, opinion reaction); none left: the all-witnesses sound (PlaySoundW expanded), dialog with the starting count, wait for
  terminate. Shapes: `for (it = begin(), end = end(), next = it; it != end; it = next) { ++next; ... }` (end in ebx, next in memory, jump
  into the aligned head, as retail; `*it++`, `current = it++` and `next = it; ++next` all colour it / end the other way round); the tail as
  a block (its text shares the loops' frame word; frame 0x38 -> 0x30); FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (the creature def handle
  released `dec [ecx+4]`); CTCTrophy::PlaySoundW's own body (landed 0x006DDCD0) defined in the TU so it expands at the tail.
- **CThingCreatureBase::UpdateAnimation 0x00665860**. Builds the CAnimateThingUpdate (time x speed multiplier, xy distance x multiplier),
  clears IdleCounter, queries the current mode's type; a gap past 9999980 x rate resets to the DEFAULT pose (speed 0, interruptable); else
  when the main animation is finished / interruptable it lets the action's non-combination animation keep it (fading a type-25
  combination over 3 frames) or holds a type-7 main animation, then flies / glides a hovering creature or lets the mode AnimUpdate;
  refreshes DEFAULT_FACE (type 20), drops the head layers (11, 12) when the look is out of range, gates the expression (18) and talk
  (19) layers by the gap unless DemonDoor / Oracle, updates the action's animation and finally the animator. Shapes: in NOTES of the
  source; the levers were MovementVector.GetSquaredMagnitude2D(), the update before `IdleCounter = 0`, `!(rate * N > time)`.

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Lengths
- 0x00665860: the scanned 1405 bytes are the whole function (`ret 4` at 0x00665DDA). The 1416-byte reservation estimate ran over
  3 int3 and into the unlisted 8-byte forwarder 0x00665DE0 (`push 0 ; call 0x00665860 ; ret`, UpdateAnimationBulletTime by the Ego_r
  donor order). The oracle already holds 1405; no SPLITS.tsv entry needed. The other three rows' ends were checked (`ret 0Ch` /
  `ret 4` at the last bytes of their extents).

## Names (two sources each)
- Rows: 0x007CF9A0 ProcessInput (vtable slot 28 + PDB name sets); 0x006DEEE0 **GlobalUpdate(bool)**, not `vfunc_26`: CTCTrophy vtable
  0x0125FC54 slot 26 is CTCBase's slot 26 GlobalUpdate(bool) (engine/CTCBase.h, the CTCBase vtable order) and
  `?GlobalUpdate@CTCTrophy@@UAEX_N@Z` is in both name sets (the only `(bool)` virtual of the class; `mov al,[esp+4]` / `ret 4`);
  0x006DE8C0 OnShowTrophyPerformAction (PDB name sets + sole caller CCreatureActionShowTrophy::PerformAction); 0x00665860 UpdateAnimation
  (PDB + donor order with UpdateAnimationBulletTime).
- Callees: every spelling is from the Ego_r / FableWin name sets (see LABELS.tsv / NAME_FIXES.tsv for the proofs); notable ones:
  IsGameActionEventInQueue(EGameAction, EInputButtonEventType, float&) (body forwards to CGamePlayerInterface's 4-argument overload);
  UpdateCoinSounds(bool); SetStateGroupEnabled (the PBrain forwarder 0x008302D0); IsMainAnimFinished vs CAnimSet::IsFinished
  (IsAnimationFinished is private in both PDBs, so neither call can be it; the bodies test the main set's anim indices / Finished bit).
- **Descriptive (no PDB data names available):** the globals 0x013B8774 `g_primitiveDrawDistanceScale` (a float added to 1.0 as the
  animation gap rate here; the name is the only established spelling and looks semantically wrong, review at a data-symbol pass),
  0x0129CB44 `FlyingNavigatorType` (the hover navigator type compared with GetNavigatorType; two made-up spellings exist,
  `?FlyingNavigatorType@@3KA` and `?g_NavigatorTypeHover@@3JA`, I used the unsigned one matching GetNavigatorType's return type) and
  0x0129D25C `g_InvalidSoundHandle` (established). Enumerator values without PDB names stay numeric: EInputButtonEventType 2, creature
  group 4, CAnimInit types 6 / 9 / 10 / 20 / 25, combination types 11 / 12 / 18 / 19, EOpinionPostDeedType 1 / 2, EEmotePriority 2.
- Variable names inside the rows are descriptive.

## Header debt / header notes
- Seven new shared headers and opt-ins in fourteen existing ones (hdr/README.md). CTCTavernGame.h, CRightHandedSet.h and
  CInputProcessTavernGame.h carry chunk 2002's not-yet-landed opt-ins unchanged.
- The access route of the protected CInputProcessGameBase members used by the tavern games (IsEventButtonPressed,
  IsGameActionEventInQueue) is still a using-declaration in CInputProcessTavernGame (unproven: a friend declaration would do the same).
- CTCPerceiveThing's and CTCEmoteIcon's StatGetInterfaceType are declared out of line in their headers, so the rows use the
  `GetTC(id, &p)` / `PeekTC(id, &p)` forms with the TCI constant.
- CThingCreatureBase needs FABLE_RETAIL_CTHING_ORIGINAL_VTABLE for AnimExists at slot 73 (0x124).

## Label corrections
NAME_FIXES.tsv: 19 retail addresses whose landed / bound / catalog spelling is wrong (wrong access AAE vs QAE on four CTCTrophy
members, wrong parameter types on IsGameActionEventInQueue / UpdateCoinSounds / Coin::UpdateSound / the CAnimInit ctor / the unsheathe
ctor, made-up names on 0x008302D0 / 0x006DF860 / 0x0065F17B / 0x00668A80 / 0x0070C0E0 / 0x006CB6A0 / 0x006655A0 / 0x0071AAB0 /
0x00864700 / 0x00751BC0). The landed row 0x008302D0 (leaf `?Run@T@@QAEXXZ`) and 0x008EF910 (`UpdateCoinSounds(float)`) need respelling.

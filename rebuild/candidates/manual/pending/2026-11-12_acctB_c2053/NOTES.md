# Hand chunk 2053 (CCombatAction_KickThingOnGround): 1 of 1 byte-exact

## What matched (1)
- **0x0087F3A0 CCombatAction_KickThingOnGround::PerformAction** (2422 bytes; src/0087f3a0.cpp, test/0087f3a0.cpp).
  q.py RELOCATION_MATCH 2422v2422 for the exact file in src/ (grades.log lines 60 and 66), no flags, Leaf
  `?PerformAction@CCombatAction_KickThingOnGround@@UAEXXZ`. Read-only vland: RELOCATION_MATCH, behaviour PASS, one RELOC_NAME
  warning (a stale label, see Label corrections). Behaviour test prints `0087f3a0_TEST PASS` under g++ (tall.sh) and under
  VC7.1 in Wine (tall.py).
  What the function does: the kick lands when the kicker stands within 0.2 (ground plane) and 1.0 (height) of MoveToPos and
  the target is alive. The target's CTCEntityEvents gets a CEventKicked. A target whose def has a CKickableDef with a shot is
  launched: the shot def's class is added as a TC and initialised to fly along the kicker's facing, scaled by a random term, the
  def's ranges, the hero's physique and speed levels and the part of Movement along the kick direction; the target gets creature
  mode 0x39, a spinning CTCRotate and the shot's particle effect. Otherwise a creature target takes a 1.0 unarmed melee hit and
  a dead creature plays RESPONSE_GET_KICKED_ON_GROUND. A type-5 object has its die-off timer reset. Last, the def's opinion
  deed is posted from the kicker's deed log.

## What closed the row (grades.log, 66 lines)
Three sessions: the interrupted author (lines 1-46), the scout (47-55), this finishing session (56-66).
1. First draft, md.py 364 equal instructions of 716 (line 4). Making the shared GetSubDef body visible and using the original
   CThing vtable: 421. Implicit `const char*` -> CCharString conversions at the five string arguments instead of explicit
   `CCharString("...")`: 434.
2. `FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY` (the visible GetDef body: in-place `dec [p+4]` release of the shot def
   handle and the allocation that follows from it): 637 (line 7). The largest single step.
3. The shot class name as a named block local (`{ CCharString shot_class_name = ...GetString(); shot = AddTC(shot_class_name,
   true, 0); }`): 668. `ToC2DVector().GetSquaredMagnitude()` for the flat distance (instead of GetSquaredMagnitude2D): 670.
   `direction.Dot(movement)` (not `movement.Dot(direction)`): no count change, kept (retail's operand roles).
4. `C3DVector effect_pos = PTarget->GetPos() + C3DVector(0, 0, 0.3f);` as a NAMED local passed to CTCDParticleEmitter::Create:
   size 2413 -> 2422 = retail's length, 708 equal (line 22). From here every candidate has the right length and only x87 operand
   orders in the projection block (Dot 0x0087F9B6..0x0087F9D2, GetMagnitude 0x0087FA0A..0x0087FA2E) differ.
5. The residue was the TEXT of two shared C3DVector inlines. A grid of existing and hand-written texts (lines 23-46) moved the
   count between 706 and 714; `Dot` as `float dot=(X*other.X+Y*other.Y);dot+=Z*other.Z;return dot;` with `GetMagnitude` as a
   named squared sum closed it (line 45, RELOCATION_MATCH). The neighbouring Dot texts all DIFFER (with the same magnitude
   text): the same text without the parentheses 714 (line 42), with `return (dot);` 714 (46), the one-line
   `return (X*other.X+Y*other.Y)+Z*other.Z;` 714 (43), `float dot=X*other.X+Y*other.Y;return dot+Z*other.Z;` 2424v2422 (44).
6. The scout replaced the fitted GetMagnitude text (27i/70b at /Od) by the one that equals the FableWin debug copy exactly,
   `float squared=(X*X+Y*Y)+Z*Z;return (float)sqrt(squared);` (26i/66b), which keeps the match (lines 51, 55).
7. This session: regraded both (56, 57); pruned hdr/ to the 13 headers the row owns and graded under it (58); final text with
   the header comment and the enumerator (59); src/ file (60); diagnostics: each opt-in alone DIFFERs (61: 710, 62: 711), so
   both are needed; the match holds with 1 / 2 / 4 extra named locals in the row (63-65: not on an edge); final regrade of src/
   after the documents (66).

## FITTED v PROVEN
- PROVEN (second source besides the grade): the row's name and vtable slot; every callee name (LABELS.tsv); CEventKicked by RTTI
  vtable 0x01277B5C; the conditional-destruction flags 2 / 4 / 8 of the one `&&` that holds the GetPBaseDef() / CCharString /
  CAnimFilterNoGroup temporaries; the member offsets (struct_layouts_egor.tsv minus the 0x18 retail shift, as the landed
  siblings read them); `GetMagnitude` text (FableWin debug size 26i/66b exact); the inline helper texts fwsize reports SAME
  (CEventKicked ctor / dtor, CTCRotate::SetRotating, CThingDeadCreature::GetAnimationFilter, the StatGetInterfaceType set,
  IS_THING_ALIVE, CWorld::PeekDefinitionManager, GFFastOneOverSqrt 30i/96b v 30i/95b).
- FITTED (the row grade only): the `Dot` text (23i/60b v FableWin 23i/59b; one byte off like the landed running-sum
  GetSquaredMagnitude2D); the named block local `shot_class_name`; the named `effect_pos`; `movement` copied from the member and
  reassigned; `base_speed_scale = speed_scale = ...` as one chained assignment; `ToC2DVector().GetSquaredMagnitude()` for the
  flat distance; the implicit string conversions; `is_kickable` as a bool set in the first arm; local variable names (all
  descriptive: the PDB name sets carry no locals).
- Not on an edge: 1, 2 and 4 dead named locals added to the row keep RELOCATION_MATCH (diagnostics, never a hand-in).

## Names and evidence
- Row: `?PerformAction@CCombatAction_KickThingOnGround@@UAEXXZ` in ghidra_out/egor_pdb_names.tsv and fablewin_pdb_names.tsv;
  vtable 0x01277D3C slot 3 (rebuild/manifest/rtti-vtables.tsv).
- Callees and data: LABELS.tsv (39 rows; each with its sources). Relocation pairing of the graded object with retail is one
  to one (44 symbols on 44 targets, scout/tools/pair_out.tsv) and vland reports no RELOC_IDENTITY problem.
- NAME_FIXES.tsv (4 landed spellings proven wrong): 0x006624F0 is CThingCreatureBase::SetAttackedByCreature (landed 0x00911C20
  declares it on CThing); 0x007BF790 is NThingShot::CTCShotDefinitionBase::Init, the `const C3DVector&` overload with
  CDefPointer parameters (landed 0x008F3630 has no namespace, CDefPointerByValue and pointer parameters); 0x0087E760 is
  NEntityEvents::CEventKicked::GetType (landed as the placeholder gapscan_sub0087e760, `T::Get`); 0x0087FEA0 is the scalar
  deleting destructor of NEntityEvents::CEventKicked (landed as a global CEventBase destructor).
- prior/2051/NOTES.md step 2 says the event is CEventKillLocked: wrong. Vtable 0x01277B5C is CEventKicked by RTTI
  (CEventKillLocked is 0x01277CA8).
- Descriptive (original not found anywhere): every local variable name in the row (`entity_events`, `is_kickable`,
  `kickable_def`, `hit_params`, `appearance`, `dead_creature`, `no_group`, `filter`, `anim`, `animating`, `shot_def`, `shot`,
  `shot_class_name`, `facing`, `strength_scale`, `speed_scale`, `base_speed_scale`, `experience`, `movement`, `direction`,
  `velocity`, `start`, `mode_manager`, `rotate`, `effect_pos`, `deed_log`); the parameter names in the header declarations
  (`rotating`, `axis`, `speed`, `num_rotations`, `effect`, `position`, `independant`, `location`, `thing_hitting`, `weapon`,
  `attribute`, `damage`, `anim`, `priority`, `loop`, `creature`); the two opt-in macro names.
- `OPINION_POST_DEED_TYPE_ALL = 7`: landed spelling, two landed rows (0x006D00A0 CTCWife::Murdered; 0x00ECB0E0, whose enum
  list gives RECIPIENT 1, WITNESSES 2, VILLAGE 4, ALL 7). The PDB name sets on this machine carry no enumerators, so this is
  not PDB-proven; the row defines only that one enumerator, as 0x006D00A0 does.
- Raw numbers kept as the landed siblings spell them: thing types 13 (dead creature) and 5 (object), type flag 8 (creature),
  creature mode 0x39, SetAsRenderedThisFrame(2). No shared enum header names them.

## Header overlay
hdr/README.md lists all 13 files: 2 new headers, 11 repo copies with opt-in blocks. Two opt-ins change a shared inline body
text (C3DVector Dot and GetMagnitude); no default text changed, no landed row needs a regrade. The chunk-2051 headers that this
directory inherited were removed from hdr/ (the row does not use them; they are in the pending c2051 hand-back).
Header debt seen (not needed by this row): hdr/README.md last section; FableWin lists `GetType` and `StatGetType` of CEventKicked
as inline class members (debug copies right after the constructor), the new header only declares GetType (the row never calls
it and no shared header defines the EEventType enumerators).

## Evidence lines the row does not need
No `// Flags:` (plain /O2 defaults), no `// Inline-budget:` (no forced inline, no /d2inl, no inline pragma; GFFastOneOverSqrt and
FABLE_VECTOR_INLINE are the repo defaults), no `// Retail-asm:` (retail has no fistp, no ftol call and no SSE in this function;
the x87 sites are fabs 0x0087F412, four fild, fsqrt 0x0087FA2E, all plain C++; the TU includes none of the shared
float-to-long helper headers).

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
- 0x005DCC00: vland's RELOC_NAME warning quotes a stale catalog label `AnimDef_FilterAnimationList`; the function is
  `?GetRandomAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@AAKABVCAnimFilterBase@@J@Z` (Ego_r.pdb + FableWin.pdb,
  functions.tsv CAnimationSet::GetRandomAnim, landed bound spelling).
- NAME_FIXES.tsv rows above.

## Not covered by the behaviour test
The exact rounding of the fast inverse square root (the model divides by sqrt, tolerance 1e-3); the def-handle reference counts;
AddTC returning 0 for the shot TC (retail calls Init on the result unchecked, as the source does).

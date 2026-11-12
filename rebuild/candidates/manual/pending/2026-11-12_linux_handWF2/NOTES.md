## 0x00868CE0

## 0x00868CE0 CHitParameters::SetWeapon(CThing&) -- RELOCATION_MATCH (2 grades + 1 strict regrade)
- What made it match: the port onto the shared headers (`PWeapon = &weapon;`, `PWeapon->PeekTC(&ptc)` with FABLE_RETAIL_CTHING_GETTC_EXPANDED, std::vector<long>
  iterator loop, the shared visible noinline GetDef<CThingBaseDef> / GetSubDef<CAugmentationDef> bodies, inline push_back(paug->Type)) gave 360v360 with one
  2-hunk residue (index load order); `long index = *it;` declared BEFORE `CDefPointer<const CThingBaseDef> pdef;` fixed it (retail `mov eax,[ebx]` ahead of the
  out-pointer push). Variants v3 (manager reference local) / v4 (index local after pdef) were not graded (fg.py stops at the first match).
- Headers / flags: no compiler flags. Overlay opt-ins (hdr/README.md): FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS, FABLE_RETAIL_CHITPARAMETERS_SETWEAPON
  (engine/CHitParameters.h), FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_GETAUGMENTATIONS (engine/CTCObjectAugmentations.h). Macros used: see the source's #define block.
- No made-up names. Name fixes (NAME_FIXES.tsv): landed caller 0x00838BC0 GetMeleeHitParameters declares SetWeapon(CThing*) -- PDB says CThing&
  (?SetWeapon@CHitParameters@@QAEXAAVCThing@@@Z); landed 0x008685F0 "unattributed AssignTarget008685f0" is ?SetThingHitting@CHitParameters@@QAEXAAVCThing@@@Z
  (dossier section 6; CHitParameters.h FABLE_RETAIL_CHITPARAMETERS_CTOR already binds that address).

## 0x00E2EAC0


## 0x00E2EAC0 CV_AmbushScamScript::TricksterApproach -- author pass 1 (2026-11-12): DIFFER, not matched
Best source: differ/00e2eac0.cpp. Grade DIFFER(363v363) with `--flags /d2inlT130` (2 hunks = ONE swapped instruction pair), DIFFER(266v363) at
default flags. Ported onto the shared headers (CV_AmbushScamScript / CScriptThing / CGameScriptInterfaceBase / fable_script_distance.h), no
header opt-in added (hdr/ is empty), names as the dossier proved them; no NAME_FIXES.

### What is proven (each by a grade on the row)
1. FRAME / ZERO REGISTER -- solved in mechanism, unproven in spelling. Retail's dead `mov [esp+0Ch],edi` before the GetTrickster call
   (frame 10h, edi = zero register for the whole body) is a compiler TEMPORARY FLAG: VC7.1 allocates and zeroes it when a `?:` whose arm
   builds a destructible temporary is in the function, and leaves only the zero store when the conditional folds away. Probe pa
   (`CScriptThing trickster = true ? GetTrickster() : GetTrickster();`) gives the exact retail entry sequence but a copy; the stand-in
   statement `false ? (void)GetTrickster().IsAlive() : (void)0;` gives the exact entry sequence with the elided initialisation and no other
   byte (d1: every hunk before 0x00E2EB81 gone). `false && ...` / `if (false && ...)` do NOT leave the flag (folded before flag allocation)
   and an `if (...) return;` form changes the site frequencies. Retail's real construct is unknown (some compile-time-constant conditional
   with a CScriptThing / CCharString temporary in an arm: a debug or platform branch). SpringAmbush 0x00E2F170 zeroes the same slot but
   really uses its flag later, so it does not tell.
2. CONTROL FLOW -- the old draft's flow is right: pre-loop check, `for (;;)` { dead test -> check, flag, break; near test -> check, flag,
   break; NewScriptFrame; check }, post-loop check. With the stand-in and `/d2inlT130` this is byte-exact except item 4 (block order,
   register choices of the three merged dtor copies, unaligned loop top all match).
3. INLINE SPLIT -- retail expands ~CScriptThing at the initial, in-loop, dead-arm, post-loop and scope-end exits and calls 0x004AA840 only at
   the near-arm exit. The all-macro form (`do { if (IsActiveThreadTerminating()) return; } while (0);` x5, as Main 0x00E2B090) expands only
   the initial / in-loop exits at the default threshold. Site scores measured with /d2inlT sweeps on the v1 form: one site flips in
   (100,110], dead-arm + post-loop + end in (110,130], near-arm in (150,200]; T130..T150 give retail's split. The plain-`if` form gives the
   same bytes at T180. So retail's dead-arm / post-loop blocks are about 2x hotter for c2's estimator than a `break` exit path of `for(;;)`.
4. LAST RESIDUE (at T130) -- the shared dtor tail at 0x00E2EB7D: retail `lea ecx ; mov [Info],edi ; mov [Data],edi ; call ~CBaseClass`,
   ours Data then Info. The order is a function-wide property of our builds, not of the header: Info-first came out for the forms
   whose loop exits through a loop condition (`while (!done)` with both arms setting `done`: j3, Fw1 at T>=90; c3) and for the forms that
   compute the dead test into a `bool` first (S3 / S6), Data-first for every `for(;;)`+`break` form, for the mixed form (dead arm sets
   done, near arm breaks: M2) and with FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA. In my variants "Info-first" and "retail block layout" never
   came together: the Info-first forms lay the near arm out falling into the post-loop check with the dead arm last (retail: near arm
   jumps, dead arm falls through) and pad the loop top.
5. The `while (!done) { CHECK; if (dead) {CHECK; flag; done = true;} else if (near) {CHECK; flag; done = true;} else NewScriptFrame(); } CHECK`
   form (v4 / j3 with the stand-in) reproduces retail's hot/cold split AND the Info-first stores at the DEFAULT threshold (347v363, 3 hunks),
   but its loop is not rotated (head check + `jmp`, exit copy at the function end) -- the strongest hint that retail's loop has a real
   condition at inlining time (post-loop "leak" + arms as body blocks, not exit paths).

### Ideas tried (all graded; see ATTEMPTS.log, ~145 lines) -- do not repeat
- Check placement: all-macro, loop-head macro, `while (!IsActiveThreadTerminating())`, near-arm plain, all plain (+T sweep).
- Loop forms: for(;;)+break (v1), while(true), do-while(true), else-chain, nested alive/near, arms ending `CHECK; return;` (s1-s3),
  dead = loop condition (c2, c3), post-loop dispatch with a `killed` bool (b1-b4), single shared `break` (R1-R3), in-loop tail check
  (N4-N8), body wrapped in do-while(0) / for(;;) / counted for (k1, x1-x3: all-hot / all-cold / loop kept).
- `done` flag forms: head check (v4, z1-z3, j3: right split, unrotated), pre + else check (w1, y2, Fw1, Fa-Fc, Q1, Q2: post-loop cold by a
  small margin, flips at T90, wrong arm layout), common bottom check (v3, n2: flag not threaded), `continue` arms (y6: not threaded),
  per-arm trailing checks (n1, n3), `while (!done && !IsActiveThreadTerminating())` (P2: 363 bytes, unrotated), mixed done/break (M1-M3).
- Dead-flag probes: unused locals, const-ref temporary (virtual IsAlive, wrong), `?:` initialiser (copy), comma, object-expression `?:`
  (folded, no flag), `false && temp`, for-init declaration, `?:` in the dead test.
- Store-order probes at T130: IsAlive if-form, definition order, Game as reference member (_ORIGINAL_MEMBERS), const local, include
  order, RELEASEDATA dtor, flag stand-in after the declaration / dtor-only arm, plain vs wrapped checks.
- By-value inline helper (probe q1): not expanded (too big) -- dropped; no helper invented.

### Best next ideas
1. Find the natural loop spelling that the estimator sees as a conditional loop (arms = body blocks) but c2 lays out like
   `for(;;)`+break: e.g. the dead arm leaving through the loop condition and the near arm through `break` with the checks pre-loop +
   after NewScriptFrame (M2 matched the loop and EXIT_A bytes exactly at the default threshold but kept Data-first and a cold post-loop;
   try M2 with the dead test formed into a bool first, and `do { } while (!dead)` spellings of it).
2. Use the store order as the oracle: it flips with how PImp.Data's uses are structured (bool-first dead test S3/S6 gave Info-first with
   retail's loop + EXIT_A + near-arm bytes at the default threshold, 297v363, dead-arm / post-loop sites still cold) -- sweep S6 with
   the dead arm as a body block (`done`), and with only the near test as a bool.
3. Recover retail's real conditional temporary (item 1): if it sits in the loop (a `?:` in the dead / near test or in a flag
   assignment) it changes the estimator's branch structure and may be the single cause of items 3 and 4. SpringAmbush 0x00E2F170 opens
   with the same loop and the same split, so one answer serves both rows; GetTrickster 0x00E2EC30 (same TU) is the place to look for
   the constant conditional the scripter used.
4. Not acceptable as a landing form without the owner's word: `/d2inlT130` plus the `?:` stand-in plus a reordered zero pair.

## 0x00E2EAC0 -- author pass 2 (2026-11-12): RELOCATION_MATCH at default flags
Source: 00e2eac0.cpp (copy in differ/00e2eac0.cpp), test 00e2eac0_test.cpp (vt.py PASS), strict-header regrade RELOCATION_MATCH,
payload.json / oracle.tsv written by addmatch.py. No header overlay (hdr/ holds only README.md), no flags, no pragma.

### What made it match
1. The loop is Main 0x00E2B090's generated form, not the draft's: `for (;;) { CHECK; if (dead) { CHECK; flag; break; }
   if (near) { CHECK; flag; break; } NewScriptFrame(); } CHECK;` with ONE check at the loop head. VC7.1 inverts the loop itself
   (check copy before the loop and after NewScriptFrame). This form is byte-exact -- store order included -- under the
   diagnostic /d2inlT86../d2inlT100, so the earlier "Info-first zero pair" residue belonged to the pre-loop + bottom-check
   spelling (two source checks), not to the header.
2. Site scores of that form (T sweep, c2 model of INLINE_DEPTH_REVISIT: callee cost 103; scope end f=4, dead-arm and post-loop
   checks f=2, near-arm check f=1; a site expands when (c/f + 2c)/3 < 74, i.e. f > 6.4). Retail has the dead-arm, post-loop
   and scope-end sites hot and the near arm cold, so the dead arm's exit is hotter than a plain loop exit by about x4 and the
   boost carries on to the code after it. Writing the dead arm's check one `do { } while (0)` deeper does exactly that at default
   flags (16-way probe x1211: only "dead arm double" matches; head or post double changes nothing useful, near double is 341).
3. The unused flag word (`mov [esp+0Ch],edi`, frame 10h) still needs one folded `?:` statement with a class temporary; any
   position works (top, inside the dead arm). Spelled `true ? (void)0 : (void)GetTrickster();` (the shape of MFC's release TRACE).

### CAUTION FOR THE COORDINATOR (decide before landing)
Items 2 and 3 reconstruct code that emitted no instructions of its own (same category as 0x00EB3F40's two lines). The bytes
prove (a) a flag word whose uses folded in the front end and (b) a dead-arm exit about x4 hotter than this loop form gives, with
the heat carried to the post-loop check and the scope end; they do not give the script text. Supporting witness for (b):
SpringAmbush 0x00E2F170 opens with the same loop and expands ~CScriptThing at its dead arm's check and return while the near-arm
and post-loop checks call 0x004AA840 -- the split this construct predicts there too. Not the cause: the callee's spelling (three
dtor spellings and the ReleaseData form leave the scores unchanged), the check macro's shape (while / for / switch / TRetVoid
`return _Fail_Val.Return()` forms graded), a preceding deleted `for` loop (leaks too much: near arm turns hot, 341).

### Ideas tried in pass 2 (all graded, a2/ holds the sources; ATTEMPTS.log has one line each)
- 128-variant sweep: five loop families x every wrapped / plain check combination (wrapping moves nothing but the NSF check).
- 628-variant grammar sweep: entry style (pre + NSF checks / loop-head check / check in the loop condition) x loop kind
  (for(;;), while(!done), do-while(!done), while(!term), while(!done && !term), do-while(!term)) x if-if / else-if chain x arm
  endings (break / done / done+continue / own check + return) x near plain or wrapped x post check. Best: head check +
  while(!done) (347v363, right split, loop not inverted).
- Loop-head for(;;) form T sweeps (66..108): match window T86..T100; J form window T70..T78.
- for(;;) + head check + `done` tested in the loop (t1, u*, w*): right split at default (359v363) but the dead arm is laid
  out after the post-loop code in every arrangement (the layout is CFG-driven, not source-order-driven).
- Deleted leading loops (p*, q*): only `for (init; cond; inc)` leaks, and too strongly.
- Check macro shapes (m*), TRetVoid style (b*), dtor spellings via a probe overlay (removed again), per-site double wrap (x*),
  dead-arm constructs (y*, s*, f*).

## 0x005590C0


## 0x005590C0 NUISystem::CScrollBarOutside::OnLeftClicked -- RELOCATION_MATCH (18 grades, first authoring attempt)
- What made it match: (1) the port onto the shared headers (CScrollBarOutside / CManager / CUIDef / CDefPointer; the default CDefPointer
  Release gives the `mov edx,[eax+4] ; dec edx ; test ; mov [eax],edx` temporary release by itself); (2) the 12 "unused" frame bytes
  were a `C2DBoxI box` local (16 bytes at esp+0x34: only TLY is stored; TLX / BRX are dead stores, so the first GetUIScale / GetSize
  pair is left with no arithmetic); (3) each bottom-right coordinate is `(PScrollBar->GetSize() * GetUIScale().<axis>).<axis> * zoom +
  pos` through C2DVector::operator*(float) (the by-value scalar is why the scale's Y is copied to a frame word before the second
  GetSize call; a plain `GetUIScale().Y * GetSize().Y` keeps the pointer in ebx and pushes edi/ebx); (4) the tests read the corners
  through C2DBoxI::GetBRPos() / GetTLPos() (the C2DCoordI temporary is the `mov [esp+8],eax ; fild [esp+8]` slot; reading box.BRY
  directly spills to esp+0x40); (5) the control flow is `if (under) ... else if (above) ...` followed by ONE ObserveEvent: VC7.1
  tail-duplicates it into retail's two ObserveEvent / epilogue copies, while an explicit `ObserveEvent(); return;` in the under branch
  is cross-jumped into a single tail (`jmp`).
- Headers / flags: no flags (base /O2). Opt-ins used: FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_C2DVECTOR (operator*(float)),
  FABLE_RETAIL_C2DBOXI (GetTLPos / GetBRPos), FABLE_RETAIL_CMANAGER (GetUIScale), plus two NEW overlay opt-ins in $W/hdr
  (hdr/README.md): FABLE_RETAIL_CSCROLLBAROUTSIDE_ONLEFTCLICKED (override declaration) and FABLE_RETAIL_CMANAGER_GETMOUSECURSOR
  (inline GetMouseCursor). Strict-headers regrade: RELOCATION_MATCH. Test: 005590c0_test.cpp (vt.py PASS).
- Names: every callee / member is the PDB spelling from the dossier. GetMouseCursor is FableWin.pdb
  ?GetMouseCursor@CManager@NUISystem@@QAEPAVCComponent@2@XZ (fablewin_pdb_names.tsv; not in Ego_r because it is inlined there), the
  same spelling the landed rows 0x0055A5D0 / 0x0055A5B0 use. Event 0x1C = UI_EVENT_LEFT_MOUSE_RELEASE (engine/NUISystem_enums.h).
  Local variable names (pmouse_cursor, mouse_position, position, zoom, box) are descriptive, not from a PDB.
- Landed spelling to recheck (not a symbol, a local-model member): 0x0055AF60 CClickable::OnLeftClicked calls the word at +0x148
  "CurrentState"; the shared header / Ego_r.pdb have CurrentState at +0x144 and TargetState at +0x148 (this row uses TargetState).
- Ideas tried before the match (ATTEMPTS.log): size.X / size.Y member products (372, edi/ebx pushed); float scale read first (344,
  frame 0x2C, merged tail); operator*(float) with a C2DVector size (340, frame 0x34); C2DBoxI with direct field tests and early
  return (340, 3 hunks); else-if with direct fields (365, bottom spilled at esp+0x40); else-if + corner accessors = match.

## 0x00541FD0


## 0x00541FD0 NUISystem::CIconText::GetDataFromDef -- RELOCATION_MATCH (2 grades: 1 SRC_FAIL on enum spelling, 1 match)
- What made it match: port onto the shared headers and the twin's (CText::GetDataFromDef 0x0054ED90) one-expression spelling
  `PFont = GFGetSystemManager()->GetFontManager()->GetFont(def->Font.GetString());` (inline CDefString::GetString + inline
  CSystemManager::GetFontManager), `CDefPointer<const CUIDef> def = LoadAndReturnDef();`, Window / PositionOffset struct copies,
  SetValue(def->TextValue), switch on LEFT / CENTER / RIGHT. The old draft's residue was wrong identities + a split font statement.
- Needs: defines FABLE_RETAIL_DEF_CLASS_BASE, _CENGINEPERSISTENTPRIMITIVEBASE, _CCHARSTRING, _CWIDESTRING, _CDEFSTRINGTABLE,
  _C2DVECTOR, _C2DBOXF, _CUIDEF, _CSYSTEMMANAGER, _CSYSTEMMANAGER_FONTMANAGER (all existing repo opt-ins; no header overlay, hdr/ empty).
  No pragma, no flags. Strict-header regrade also RELOCATION_MATCH. No made-up names.
- Name fix (NAME_FIXES.tsv): landed callers spell 0x00541FD0 `UpdateFont` (CIconText_CIconText_00542330.cpp,
  NUISystemCIconText_vectordeletingdestructor_00543b30.cpp); Ego_r.pdb 0x005BB900 and FableWin.pdb 0x02262EA0 both name it
  ?GetDataFromDef@CIconText@NUISystem@@QAEXXZ.

## 0x008C2AD0


## 0x008C2AD0 CCombatAction_WillHealInto::FinishAction -- RELOCATION_MATCH (strict headers), 11 grades
- What made it match: the port onto the shared headers (CThing chain, CDefPointer / CDefClassBase_GetSubDef.h, CSpecialEffectsDef.h,
  CTCDParticleEmitter.h FULL, CCreatureActionBase / CCombatActionBase.h MELEE + FINISHACTION, CIntelligentPointer.h). The old
  residue (`mov eax,[def] ; cmp ; mov esi,eax`) was the local model: with `PCreature->GetPBaseDef()->GetSubDef(&pspecial_effects)`
  (PDB spelling: const, pointer out-param, CDefPointer<const T>) and `!pspecial_effects.IsNull()` the def loads straight into esi.
  Last 2-byte residue after the port: the AddTC result must be a named local (`CTCHealLife* p = GFDebugPDynamicCast(AddTC(...))`
  then `p->SetAICast(SpellLevel)` as its own statement) so the "CTCHealLife" temporary dies before SetAICast.
- Ablations graded: FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE and FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY are NOT needed
  (final source has neither). `long effect` must be block-scoped (frame word [esp+10h] shared with the later strings); at function
  scope it only matches when the GetEffectFromString body opt-in is on (f1/f2 DIFFER: slots 0x10/0x18 swapped).
- Headers / flags: no overlay header, no opt-in added ($W/hdr is empty), no `// Flags` (default flags, no /EHsc).
- Local declarations (no shared header exists): CCombatAction_WillHealInto (SpellLevel +0x114), CCombatAction_WillHealRelease
  (ctor 0x008C3930, 0x114 bytes, implicit dtor -> ~CCombatActionBase 0x0062E900), CTCHealLife (SetAICast only) -- all PDB names.
- Landed names proved wrong (from the DOSSIER, relied on here; see NAME_FIXES.tsv): 0x007C3EA0 is CTCHealLife::SetAICast(long)
  (landed as C_007c3ea0::M / AddExclusiveObserver); 0x00669D30 is CSpecialEffectsDef::GetEffectFromString (landed leaf CCRC::CalcAndLookup).
- No made-up names.

## Repair (verifier finding)
- NAME_FIXES.tsv 0x007C3EA0: added `SetState` to the wrong landed spellings (callers 008ddd70.cpp:200, 008e0060.cpp:190 declare `CTCHealLife::SetState(long)`; PDB `?SetAICast@CTCHealLife@@QAEXJ@Z`). Source/test unchanged; regraded RELOCATION_MATCH (strict headers), test PASS.

## 0x005F11A9

# 0x005F11A9 CTCInventoryAbilities::UnAssignAbility -- RELOCATION_MATCH (375 B)

Files: 005f11a9.cpp (source), 005f11a9_test.cpp (test, vt.py PASS), payload.json, oracle.tsv. Strict-header regrade: RELOCATION_MATCH.
Flags: none beyond `#pragma optimize("s", on)` in the source. Header overlay: empty (existing repo opt-ins only, see hdr/README.md).
differ/005f11a9.cpp and draft_original.cpp are the OLD parked draft (local models, wrong names); they are superseded, not updated.

## What made it match
Port onto the shared headers with the body of the landed sibling 0x005F1320 (flag/break loop, icon block, find/erase tail), plus
this head:

    EHeroAbility current = AssignedAbilities[slot];
    EHeroAbility saved = AssignedAbilitiesSave[slot];
    EHeroAbility removed = AssignedAbilities[slot];   // second read of the slot; the map::operator[] key, homed at [ebp-4]
    EHeroAbility ability = removed;                   // the value the loop and the `!= 0` test use (edx)
    if (current != saved) AssignedAbilities[slot] = saved; else { both = 0; }

How this spelling was found: NOT derived from evidence about the original source. After about 150 hand-written ideas left the
same head residue, I enumerated local-variable arrangements mechanically (three generated families, about 1,250 variants graded
with fg.py; lists in bulk/INDEX.txt, bulk2/INDEX.txt, bulk3/INDEX.txt) and this one (bulk3/d0097) graded RELOCATION_MATCH. It is
plain C++ and byte-exact, but four locals for one value is unlikely to be what the programmer wrote; the original may be an
equivalent form (for example an inlined accessor or by-value parameter) that I did not find. The local names `current`, `saved`,
`removed`, `ability` are descriptive (locals have no PDB names here).

## What the residue was, for the next row with this shape
- One read of `v[slot]` followed by a write: VC7.1 keeps `&v[slot]` in a register (ours 358 B, retail re-reads `begin`).
  Two reads before the branch: it keeps the VALUE and re-reads `begin` for each store, as retail does.
- A store to the address-taken key local makes a later re-read of `v[slot]` come from memory again, so both reads must come
  before that store (hence `removed = AssignedAbilities[slot]`, not a re-read in the `if`).
- `ability = removed` (a register local assigned FROM the address-taken local) is what leaves retail's `mov edx,ecx`; assigning
  the key local from the register local (`removed = ability`) coalesces and loses the copy.
- Declaration order of the locals, `long` vs `EHeroAbility`, `const`, /O1 and /Oa made no difference or made it worse.

## Ideas tried that did not match (all logged in ATTEMPTS.log)
Single address-taken local; `removed` copy before / after the if; saved local or re-read; `==` first; assignment in the
condition; chained assignment; removed assigned in both arms (hoist); bool for the comparison; const local; const-path and
signed-index reads; first local reused as loop counter or redefined later; inline getter / loop helpers; /O1; /Oa.

## Names
No landed name proved wrong by this row. The DOSSIER's remark that 0x0057926B is landed under a behavioural name
(`ListNode_RemoveByData`) while siblings reach it as `_Rb_tree::erase(iterator)` is unverified by me; I did not look into it.

## Process slips (mine)
- One Bash call showed a Python traceback (BrokenPipeError): I piped fg.py into `head -4`. Grades were unaffected.
- ATTEMPTS.log holds ERROR lines that are not compile results: bulk/b001-b003 once (wrong cwd, regraded) and bulk2/c0313-c0330
  (files that were never generated).
- Bulk variants were regraded singly to keep their listings, so ATTEMPTS.log counts some variants two or three times.

## 0x006978D0


## 0x006978D0 CGameCameraManager::GetInterpolatedCamera -- RELOCATION_MATCH (376/376), 37 grades, 17 probes
- What made it match: (1) port onto the shared headers with GFGetInterpolatedValue(old, new, interp) and the CCamera accessors
  (PeekWorldPos / PeekUpDirection / PeekViewDirection / MoveTo / GetFOV / SetFOV / SetView); (2) the up and view vectors of both
  cameras copied BY VALUE into locals (`const C3DVector old_up = old_camera.PeekUpDirection(); const C3DVector new_up = camera...`,
  old before new) and blended memberwise into named `up` then `forward` -- that alone gave every retail instruction (373v376, a
  pure int/x87 interleaving residue); (3) every GFGetInterpolatedValue call in an assignment PARENTHESISED, `v.X = (GFGet...(...));`,
  exactly as the landed sibling CGameCameraBase::GetRenderCamera 0x006FD5E0 spells it. The parentheses change no instruction, only
  VC7.1's scheduling: the `mov [cam+N],eax` lands after the `fstp st(0)`, the forward copies are loaded inside the up.Z blend and
  the two SetView pushes move before the forward blend. Parenthesising only the position (376v376, 9 hunks) or only up/forward is not enough.
- Headers / flags: no flags, no pragmas. Overlay hdr/engine/CCamera.h (FABLE_RETAIL_CCAMERA_FOV: GetFOV / SetFOV declarations) and
  hdr/engine/CGameCameraManager.h (FABLE_RETAIL_CGAMECAMERAMANAGER_INTERPOLATEDCAMERA: the private declaration in the LOOKAROUND branch).
  Strict-headers regrade: RELOCATION_MATCH. Test 006978d0_test.cpp: vt.py PASS (repair 2026-11-12: the test model SetFOV now sets FOVFlags 1/3 like retail 0x00A0BED0; expects FOVFlags == 1).
- Names: function / callees exact PDB spellings (two sources: Ego_r.pdb + FableWin.pdb). Descriptive (made-up, no PDB local names):
  parameters interpolate / camera / old_camera, locals passcam / vecman (the sibling row's spellings) / old_up / new_up / up /
  old_forward / new_forward / forward.
- Landed names proved wrong: the landed caller rebuild/src/compiled/00/69/CGameCameraManager_GetRenderCamera_00697f30.cpp calls this
  function `Helper6978d0`; the old draft called 0x00A0BEB0 GetHorizontalFOV (it is ?GetFOV@CCamera@@QBEMXZ; GetHorizontalFOV is 0x00A0BE90).
  See NAME_FIXES.tsv.
- Ideas tried that did NOT matter (same 373v376 residue): copy order new-before-old (adds an fxch), copies by assignment (421),
  all four copies up front (426), C3DVector(x,y,z) ctor spellings, MoveTo(C3DVector(..)) (Z operand loaded first), C3DVector::Interpolate
  (vector-op code, 349), helper inlines with by-value or by-reference vectors, reused copy variables, the compiler-generated CCamera
  copy ctor, the default header branch, /Oa, /G7, the QFE toolset. Probes showed the residue followed the source TEXT of the sibling
  (its parenthesised calls), not the class, the signature, member-vs-parameter loads or the include set.
- differ/006978d0.cpp is still the old parked draft (untouched); the matched source is 006978d0.cpp, the probes are in scratch/.

## 0x00EB3F40


## 0x00EB3F40 CMurderTwistAssassin::Main -- RELOCATION_MATCH (author pass 1, 2026-11-12)
- What made it match: (1) port onto engine/CV_MurderTwistFinalScript.h + CIsThingAlive.h + TRetVoid.h with the PDB callee names
  (SetPredicate / IsActiveThreadTerminating / IsValid / Release / AttackHero / AssassinState); that alone left 378v381: no flag
  word at L+0, no `mov bl,al`, &Me/&MyEntityHandle in swapped registers. (2) loop A's body as `if (trust) {check; DoWatchingForTrouble();}
  else {check; DoWatchingForTrouble();}`: VC7.1 merges the identical arms after register allocation, which leaves the dead
  `mov bl,al` and gives the second terminate check. (3) one statement `if (trust && false && Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) { }`
  after the trust line: the front end allocates a conditional-destruction flag word for the CCharString temporary, the optimiser
  deletes the statement, and only `mov dword ptr [esp+0Ch],0` stays; with it the register assignment and the late `push ebx` fall out.
- CAUTION FOR THE COORDINATOR (please decide before landing): (2) and (3) reconstruct code that emitted no instructions. The bytes
  prove a flag word (same form as 0x00EB03B3 / 0x00E9312C) and a trust value kept live into loop A, in this row and in the twin
  CMurderTwistAssassinGuard::Main 0x00EB55D0, but not the wording of the script lines. `trust && false && <call with a string>` is my
  spelling, not recovered text; the string and the callee in it are arbitrary (borrowed from DoWatchingForTrouble). A front-end
  constant (`false && ...`, grade f1/f2) gives NO flag word, so the original condition was folded by the optimiser, e.g. an inline
  predicate returning a constant. If such an inline is ever found in the script headers, respell line (3) with it.
- Headers / flags: no overlay, no extra flags, no pragma. Opt-ins used (all already in the repo): FABLE_RETAIL_NSCRIPT, _CLEVELSCRIPTBASE,
  _BASE_OBJECT, _CCHARSTRING, _DEF_CLASS_BASE, _CDEFSTRINGTABLE, _CV_MURDERTWISTFINALSCRIPT_ASSASSIN. Strict-header regrade: RELOCATION_MATCH.
- Ideas graded (do not repeat): plain port with `bool trust` local / ORIGINAL_MEMBERS + NOVTABLE set / `long unused = 0` (all 378v381,
  16 hunks); `if (trust || !trust)` (no bl); `if (trust==true) .. else if (trust==false)` (384); `false && temp` before the loop or in
  the else arm (no flag word); `trust && false && temp` without the if/else arms (381, only `mov bl,al` missing).
- Twin 0x00EB55D0 (unlanded Guard Main): same two devices should match it; it has no terminate check before the final idle loop.
- Made-up names: enumerators ASSASSIN_STATE_0..3 (already in the shared header, descriptive); local `trust`.
- Name fixes proven (NAME_FIXES.tsv): 0x00EB53A0 is CMurderTwistAssassin::AttackHero (landed as CMurderTwistSheriff::Init);
  0x00CD2770 is CScriptGameResourceObjectScriptedThingBase::Release (landed file named CMemoryDataOutputStream::Clear).

## 0x008D5300


## 0x008D5300 CCreatureAction_ReloadMissileWeapon::PerformAction -- RELOCATION_MATCH (grade 1 of 2; strict headers pass)
- What made it match: the port onto the shared headers with the dossier's identities, nothing else. `PCreature->` written per access
  (GetTC, GetPos, GetPlayerNumber: three GetPItem calls, `this` kept spilled), `PAmmo = CThingObject::Create(ammo, PCreature->GetPos(),
  PCreature->GetPlayerNumber(), 0, 0, "")` (six-argument static __fastcall; the "" CCharString temporary and the CIntelligentPointer
  temporary of the assignment die in retail's order), `pweapon->PeekBaseDef().GetSubDef(&weapon_def)` with CDefPointer<const CWeaponDef>,
  `long ammo = def->Ammo; long slot = def->MissileLoadCarrySlot; if (ammo > 0 && slot > 0)`, RemoveThing / PutThingInCarrySlot by
  reference. The old draft's "register tie" (379 v 383, &PCreature hoisted into ebp) was the model error (cached creature pointer,
  2-arg Create + invented 4-arg position query).
- Headers / flags: no flags, no pragma, NO header overlay (hdr/ is empty). Opt-ins used, all already in the repo headers:
  FABLE_RETAIL_CTHING + _GETTC_EXPANDED, _CTHINGPHYSICAL, _CTHINGGAMEOBJECT, _CTHINGCREATUREBASE, _CTHINGOBJECT_CREATE,
  _CINTELLIGENTPOINTER + _COMPARE_NULL_INT + _CTOR_OUTOFLINE + _ASSIGN_OUTOFLINE, _IS_THING_ALIVE + _NOINLINE,
  _CCREATUREACTIONBASE + _PCREATURE, _CTCCARRYING_PUTTHING / _REMOVETHING / _CARRYSLOT, _CDEFCLASSBASE_GETSUBDEF_NOINLINE,
  _DEF_CLASS_BASE, _CCLASS, _CDEFINITIONMANAGER, _CRESOURCELIST, _CCHARSTRING, _CTCBASE.
- Local declaration: class CCreatureAction_ReloadMissileWeapon (no shared header exists) over engine/CCreatureActionBase.h with the one
  Ego_r member `CIntelligentPointer<CThingObject> PAmmo` (+0xA8). A shared engine/CCreatureAction_ReloadMissileWeapon.h would let the
  sibling FinishAction 0x008D5480 (still on local stand-ins) move onto it.
- Made-up names: locals only (ptc_carry, pweapon, weapon_def, ammo, slot, existing). All callee / member names are PDB spellings.
- Name candidates for landed rows (NAME_FIXES.tsv): 0x0078B2A0 is landed as the made-up `IsThingAlive_0078b2a0(CIntelligentPointer<CThing>&)`;
  this row calls it as IS_THING_ALIVE<CIntelligentPointer<CThingObject> >(const CIntelligentPointer<CThingObject>&) (FableWin
  ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingObject@@@@@@YI_NABV?$CIntelligentPointer@VCThingObject@@@@@Z; PAmmo's Ego_r type).
  Caveat: the body is pointee-independent, so identical instantiations may be folded onto this address (8 call sites); the template
  name IS_THING_ALIVE and the `const T&` parameter are certain, the pointee is proven for this call site only. 0x007B82C0 (landed
  file name "...CThingCreatureBase_operator") is CIntelligentPointer<CThingObject>::operator= at this call site, same caveat.
- differ/008d5300.cpp is the OLD parked draft (superseded by 008d5300.cpp in this directory / payload.json).

## 0x009ADC90 (not landed)


## 0x009ADC90 CDefinitionManager::InstantiateDef -- RELOCATION_MATCH (5 grades, first authoring attempt)
- What made it match: port onto the shared headers with the dossier's identities (parameter roles class name / instance name / is_template; `info->Name` instead of
  the class-name argument after the lookup; inline `CDefString(const CCharString&) { SetString(); }`), then three source facts: (1) `if (info) { ... return true; }
  return false;` (an early `return false` shrink-wraps the prologue); (2) the two indices are signed `long` -- pushing the long into `CArray<unsigned long>` makes the
  conversion temporary retail stores in the dead is_template slot; (3) the record is built in its OWN statement (`CInstantiatedDefInfo* def_info = new ...;` then
  `push_back(def_info)`): with the `new` inside the push_back call the class-name CDefString temporary cannot share the is_template slot and the frame grows by 4.
  SetInstantiationName takes the name by implicit conversion (built in the argument slot, `push ecx ; mov ecx,esp`).
- Headers / flags: no compiler flag. Overlay $W/hdr (4 headers, see hdr/README.md): FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF, FABLE_RETAIL_CDEFSTRING_CTOR_SETSTRING,
  FABLE_RETAIL_CSYMBOLMAP_ADDSYMBOLDIRTY, FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR; also FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES for the final Release.
- Unproven detail: the body of FableWin's `CDefPointer(T*)` ctor is modelled from this one expansion (store + AddRef, no null test); CInstantiatedDefInfo's ctor
  (by-value CDefString class_name / instantiation_name, class_index, pdef) is not in any PDB list (fully inline) -- parameter names are descriptive.
- Names: no made-up function names. Landed spellings to fix: stdvector_Minsertoverflow_009b4bd0.cpp uses the element placeholder `CUnprovenElement_009b4bd0` (exact landed spelling: forward declaration line 17, `// Leaf:` mangled name `PAVCUnprovenElement_009b4bd0@@`, explicit instantiation line 92); this row's call
  site 0x009ADD90 passes InstantiatedDefs, so the element is `CDefinitionManager::CInstantiatedDefInfo*` (NAME_FIXES.tsv). The old draft's FindClassInfo / Create /
  NeedsName / SetName / Destroy / SInstantiatedDefInfo / InsertGrow are gone (GetPDefClassInfo / AllocFunc / IsParentDef / SetInstantiationName / Delete /
  CInstantiatedDefInfo / _M_insert_overflow).
- Repair (verifier finding 1): NAME_FIXES.tsv wrong_spellings_seen column now holds exactly the landed spelling `CUnprovenElement_009b4bd0` (the parenthetical moved to the evidence column); source unchanged, regraded RELOCATION_MATCH (also strict headers), test PASS.


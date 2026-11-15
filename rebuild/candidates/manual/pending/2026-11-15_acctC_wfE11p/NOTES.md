# wfE11 NOTES

## 0x004736C4 CPlayerGuiDef::Transfer -- RELOCATION_MATCH, test PASS, addmatch done (1 grade)
- Name proven: vtable 0x012352DC slot 18 (Transfer slot of every def), Ego_r.pdb + FableWin.pdb. Extent 0x004736C4..0x00475023 (`ret 4`) = 6498 bytes, confirmed.
- What matched: the acctB outline body (463 statements, retail order incl. the out-of-order minimap colours) on the repo's
  existing whole-class opt-in FABLE_RETAIL_CPLAYERGUIDEF_CLASS + FABLE_RETAIL_DEF_CLASS_BASE + FABLE_RETAIL_CPERSISTCONTEXT +
  _NAMED_DEFAULTS (3-arg TransferMap; defaults `0L` / `GAME_ACTION_NULL`). TU pragmas y-off + s-on (CSoundThemeDef::Transfer twin). No flags, no overlay change.
- vland read-only: 0 RELOC_IDENTITY; 5 RELOC_NAME warnings (TransferVector<T> callees landed as TransferBinary_* / a wrong label) -> NAME_FIXES.tsv.
- Test: model of the body (offset + helper kind per statement) vs the retail call sequence table; mutant (one offset swapped) FAILs.

## 0x0044C72B CGameDefinitionManager::InitAndCompile -- RELOCATION_MATCH, test PASS, addmatch done, strict-header vland PASS (5 grades + fix-up)
- Name proven (vtable 0x01232C24 slot 2; Ego_r + FableWin). Extent 0x0044C72B..0x0044EA4D (`ret`) = 8995, confirmed by acctB dossier.
- Body = acctB outline (paths, 138 AddDefClass(CDefClassInit("X", X::Alloc)) in retail order, two StatRegisterClass, path-list
  assignment, DisplayProgress / Compile / DisplayProgress, four GetDef). TU y-off + s-on, no flags.
- What closed it: (1) FABLE_RETAIL_STLP_VECTOR_ASSIGN (vector<CWideString>::operator= out of line 0x00433DF0; without it the
  assignment was a bitwise 12-byte copy); (2) the string arguments of DisplayProgress / GetDef written as plain literals (implicit
  CCharString conversion): retail builds the temporary at [ebp-4] BEFORE pushing the other args; explicit `CCharString("..")` args
  are built last (after the pushes) into another slot. EVIDENCED by bytes 0x0044E92E / 0x0044E98D.
- New overlay opt-ins (hdr/README.md): FABLE_RETAIL_CDEFINITIONMANAGER_COMPILE (Compile decl), _PATHLISTS (SymbolPathList /
  CompilePathList typed std::vector<CWideString>), FABLE_RETAIL_CGAMEDEFINITIONMANAGER_DEFPOINTERS (four CDefPointer<const T> d0..dc,
  descriptive names PGlobalDef / PEngineDef / PEnvironmentDef / PHeroStatsDef).
- The 138 Alloc holders are local `class X { public: DEF_CLASS_REGISTRATION_INTERFACE(X) };` declarations (only the address of
  ?Alloc@X@@SIPAVCDefClassBase@@XZ is taken), the idiom of landed CTCBase::RegisterTCs 0x004EE23F. vland warns
  SHARED_HEADER_AVAILABLE for the 81 that have engine headers (strict mode would reject): coordinator decision.
- vland read-only: 0 RELOC_IDENTITY (all 138 Alloc symbols + every callee bind as named).
- FIX-UP 2026-11-15 (strict headers): the strict check (verify_and_land.py ~l.522) is purely textual: `(struct|class) Name {` with
  rebuild/include/engine/Name.h present and no `engine/Name.h` in the source. The cited precedent 0x004EE23F does NOT pass strict
  mode either (75 of its `class XDef {` lines hit; it was landed non-strict; its TC classes escape the regex only via `: public CTCBase`).
  No sanctioned allow-list exists, so the row now INCLUDES the headers: all 81 repo headers + overlay-new engine/CCarrySlotDef.h +
  the three NUISystem_CUIDef / _CUILocaleGraphicsDef / _CUIMiscThingsDef.h (85; not flagged by the regex, but they are shared
  headers). All 85 compose in one TU (no conflicting pair found). Alloc: 8 headers declare it by default, 8 under existing opt-ins
  (CDEGRADABLEDEF_ORIGINAL_FUNCTORS, SKYBEAM_ORIGINAL, CTHINGOBJECTDEF_VALIDATE, CSIMBUILDINGDEF, CENVIRONMENTDEF_PARENT,
  CENVIRONMENTTHEMEDAYSETDEF, CSPECIALABILITIESFIREBALLSPELLDEF_MEMBERS, CJACKDRAGONDEF); CTHINGBUILDINGDEF selects the retail
  CThingBuildingDef branch (default is a _quarantine include); CCOMBATABILITYDATA / _BLOCKINGDATA / _PROJECTILEATTACKDATA satisfy
  the layout asserts of the combat-ability headers. 69 headers get a new additive FABLE_RETAIL_<CLASS>_ALLOC opt-in (hdr/README.md):
  `public: static class CDefClassBase* FABLE_FASTCALL Alloc();` before each class end (elaborated `class` so it needs no include;
  VC7.1 mangles it ?Alloc@X@@SIPAVCDefClassBase@@XZ, measured); single-line classes get an #ifdef twin with the original line in
  #else; asserts pinned (pin_asserts.py; CThingHolySiteDef l.20 pinned by hand). Multi-header opt-ins FIRE_RANGED_WEAPON /
  BRIAR_ROSE / AUGMENTATION_PARTICLES were NOT enabled (they reach NCreatureMode / stlport containers): CHeroCombatDef, CBriarRoseDef,
  CAugmentationDef got their own _ALLOC in the branches lacking Alloc. 52 registered classes have no header anywhere: they keep the
  registration-interface-only declaration (RegisterTCs idiom).
- Trap found: a class whose header lacks Alloc silently binds an inherited base Alloc (CThingObjectDef::Alloc -> CThingBaseDef::Alloc
  until CTHINGOBJECTDEF_VALIDATE was enabled: DIFFER 8992v8995, ebx reuse). Guard used: extern symbol set of the object identical to
  the all-local model (162 symbols, objdump -t).
- Result: RELOCATION_MATCH (both alphabetical and registration include order; alphabetical kept), vt.py PASS, addmatch redone with the
  method name `InitAndCompile` (clears the STANDIN_LEAF warning the mangled name caused), strict read-only vland
  (LANDVERIFY_STRICT_HEADERS=1): `0044c72b RELOCATION_MATCH PASS`, no WARN of any kind. Compile ~2 s (no cost from 85 includes).
  Regraded 004736c4 (includes the changed CPlayerGuiDef.h) and 004804de: both still RELOCATION_MATCH / vland PASS. No FITTED item.

## 0x006F5C10 CCreatureActionBase::StatRegisterClass -- PARKED DIFFER(9096v8717) (differ/006f5c10.cpp; ~25 grades/probes)
- Name proven: Ego_r.pdb 0x0071BCF0 + FableWin.pdb 0x023ED150 ?StatRegisterClass@CCreatureActionBase@@SIXXZ; the repo header
  CCreatureActionBase.h already documents it (STATREGISTER opt-in, caller CWorld::Init 0x004A7601). Catalog `_global` label is wrong.
  Extent 0x006F5C10..0x006F7E1C (`ret`) = 8717 confirmed. The map 0x013BA8C8 is CCreatureActionBase::RegisteredActions (Ego_r/FableWin
  dynamic initializer names); reloc_bindings calls it `Registry` (NAME_FIXES.tsv).
- Body (EVIDENCED by the 55 insert blocks: key immediates, new sizes, ctor calls, vtable stores -> rtti-vtables.tsv classes):
  RegisterActionEventTypes(); RegisteredActions.insert(std::make_pair(ACTION_NULL, CCountedPointer<CCreatureActionBase>())); then 54x
  RegisteredActions.insert(std::make_pair(ACTION_X, CCountedPointer<CCreatureActionBase>(new CX))) in retail order (tools/gen5.py table).
  3 counted-pointer objects per block (temp, make_pair pair, converted value_type) = retail's 2 increments + 3 releases.
- Headers: FABLE_RETAIL_BASE_OBJECT (without it CCreatureActionBase came out 0xAC), retail CCreatureActionBase branch + new opt-in
  FABLE_RETAIL_CCREATUREACTIONBASE_REGISTEREDACTIONS (RegisteredActions + StatRegisterClass, placed at the class end),
  FABLE_RETAIL_CCOMBATACTIONBASE_PRIORITYCTOR (protected CCombatActionBase(long) 0x00857E40, Ego_r IAE@J@Z),
  FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_DEFAULTCTOR (0x00867950). The 54 action classes are LOCAL declarations
  (inline `: CCombatActionBase(50/150/250) {}` or `{}` ctors where retail stores the vtable after the base ctor; declaration-only
  ctors where retail calls 0x008xxxxx; FlameSlice inline: its COMDAT 0x006F5B50 is in this TU). Members from Ego_r tails.
- KEY FINDING: with FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (dtor = `{ ReleaseData(); }`, one more inline level) the inline-budget
  transition lands on the retail block (block 20, 130 v 125 instr) and blocks 21..54 equal retail's instruction/call counts exactly
  (pair dtor 0x0048C5C0, CCountedPointer(T*) 0x00695880, pair dtors 0x00695D90 / 0x006F5260 called out of line as in retail). Without
  it the transition moves to block 27.
- RESIDUE (blocks 1..19, +5 instr each): after insert_unique, ours reloads pair.second.Info after `dec [Info]` and after the
  DeleteFunc call, keeps the zero stores, and caches 0 in ebx; retail: `mov edi,[esp+1Ch]; test; dec [edi]; jne; mov ecx,[edi+8];
  call [edi+4]; push edi; call free` (no reloads, no zero stores, test reg,reg), frame 0x2C v ours 0x30 + push ebx.
  Tried (no effect on the reload): /Gr (sibling RegisterActionEventTypes flag), braces-scoped named local `action`, value_type direct
  construction, FABLE_RETAIL_STLP_MAP (visible insert_unique body) + explicit instantiation before the caller, /Oa, /Ow,
  #pragma optimize a/w, truncating to 6 blocks (so not a tuple-limit effect), FABLE_RETAIL_CCOUNTEDPOINTER (ctor test of member).
  Next levels: the pair / make_pair texts in stlport/fable_containers.h (by-value make_pair args? pair ctor spelled with
  member-init), CCPPointerInfo ctor visibility, whether retail's pair temp is const (c2 const-object alias rule), STLport 4.6
  map::insert(const value_type&) wrapper text.
- Also tried after the notes above: pair without the user-declared copy ctor (implicit copy) -- no effect (opt-in not kept).

## 0x004A3740 CWorld::UpdateRegionLoad -- SCOUTED, not drafted (untouched for grading)
- Name proven: Ego_r.pdb 0x004ACFB0 + FableWin.pdb 0x01B47E70 ?UpdateRegionLoad@CWorld@@IAEXXZ (protected, void, no args); one
  caller 0x004A5BFB.
- Extent proven = 8948 (0x004A3740..0x004A5A33): code ends `ret` 0x004A5A0E, `nop` 0x004A5A0F, then the 9-entry switch table
  0x004A5A10 (targets 0x004A3786 0x004A3F05 0x004A3FDD 0x004A40AC 0x004A4763 0x004A4CEC 0x004A5A07(default) 0x004A5095
  0x004A5877) for `switch (this->[+0x104])` cases 1..9 (`dec eax; cmp eax,8; ja default`). int3 pad from 0x004A5A34. The extent
  scan's "last instruction dec edx" is the table data. Oracle row length is right.
- Shape: `and esp,-8` frame 0xF8, ~140 distinct callees, 284 jumps, x87 present; state machine over the region-load state.
  Siblings: matched CWorld::LoadQuestsFromFile 0x004A0D90 ($S/w22/wfA3/src) for the CWorld macro set.

## 0x004804DE CTCInGameMenu::OpenInGameMenuPC -- SCOUTED, not drafted (untouched for grading)
- Name proven: Ego_r.pdb 0x0048CD2B + FableWin.pdb 0x01AB1A2F ?OpenInGameMenuPC@CTCInGameMenu@@QAEXPAVCComponent@NUISystem@@0@Z
  (`ret 8` = the two CComponent* args); one caller 0x0058B638.
- Extent proven = 11203 (0x004804DE..0x0048309E `ret 8`; next function 0x004830A1 `push ebp; lea ebp,[esp-78h]`). No SEH frame
  (`lea ebp,[esp-70h]; sub esp,2C4h` VC7.1 /O2 large-frame idiom), 506 calls (88 CCharString(const char*), 50 operator new, 28
  CWideString dtors, CCountedPointer<CComponent>/<CAction> COMDATs 0x429C15 / 0x42AA29 / 0x4291DE / 0x429160 as in the landed
  NUISystem::CManager::Action 0x0041E6D3 -- FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR / RELEASEDATA idioms apply).

### 0x004804DE successor session -- RELOCATION_MATCH, test PASS, addmatch done (~35 grades; src/004804de.cpp)
- Hand-authored from the disassembly on the landed console twin CTCInGameMenu::OpenInGameMenu 0x004830A1 (same TU; its defines,
  `#pragma optimize("s", on)` alone, local CTCFishing model, signature-check / description idioms copied), plus VIDEODIALOGS
  (VideoChangesTimer = -1.0 at +0xC0), OPENPC, CYESNO, CACTIONPARAMULONG. Extra PC blocks read from the bytes: save / load frames
  (PC_UI_FRAME_FOR_SAVE_LOAD_LISTS), position dummies, 8 scroll-arrow AddActionsListenIgnoreOnList calls, accept / back containers
  with six actions (OBSERVE_EVENT x2 AddActionOnSelected, IGNORE_EVENT x2 AddActionOnInactive, SAVE/LOAD + SEND_EVENT(UI_EVENT_BACK)
  AddActionOnLeftUnclicked) in the overwrite, confirm and load-confirm sub-lists, PC_DIALOG_YESNO_GAME (SEND_EVENT SELECT / BACK),
  centred by GetBackBufferWidth/Height*0.5 - GetUIScale()*104/52, Components.push_back, PYesNo.
- Confirm block: the six actions sit in a nested `{ }` (retail releases them 0x0048213B..0x0048216F BEFORE the PC_CONFIRM_DESCRIPTION
  lookup); there the containers / description / subtitle go to the LOAD frame (+0x30) and the title to the save frame (bytes, sic).
- InitialiseOffsets: the twin's if/else (cross-jumped call) gives retail's `jne; dec edx` (a ternary does not).
- Residue after 3 grades: with frame offsets masked only `lea ecx,[ebp-254h]` scheduling at 0x00482FE0 differs; frame 0x2BC v
  retail 0x2C4 and ~390 frame-offset hunks (slot order). Retail puts the ChangePosition C2DVector temp at the frame bottom
  (-0x254, its own slot); ours at -0x40. Next: derive slot membership (packer: size asc / weight desc, density sort).
- CLOSED: (1) the yes / no position as a named FUNCTION-SCOPE `C2DVector position(xexpr, yexpr)` with both components inline
  (retail's C2DVector sits alone at the frame bottom [ebp-254h]; a call-argument temporary, named floats, or a `{ }`-scoped local
  share a slot -> frame 0x2BC; member assignments drop the y spill); (2) `(GetBackBufferWidth() * 0.5f) - scale.X * 104.0f`
  parenthesised (PAREN tuple) puts `fsubp` before `lea ecx,[ebp-254h]` (0x00482FDE). Tried without effect on that one swap:
  packed C2DVector, forceinline ctor, const / reference-bound / copy-init position, int 0, k=1..3 unused locals, operand-order
  swaps, parens on the Y expression. Both marked FITTED in the source header. No flags, no header change.

### 0x004A3740 successor session -- still UNTOUCHED (no draft; context spent closing 0x004804DE)
- Machine outline by switch case written to ref/004a3740/notes/CASE_OUTLINE.txt (outl2.py: callee labels from reloc_bindings are
  UNPROVEN, e.g. "HeroMap::lower_bound" x26 is likely the inline PeekTC's CVectorMap::LowerBound, "D::Do" the bitset test).
  Outline lines per case: 1:135 2:18 3:15 4:109 5:120 6:61 8:158 9:32 (case 7 = default/exit 0x004A5A07). Prologue: GSI_GetWorld
  -> CPlayer::GetPControlledCreature, then `switch (state at +0x104)` cases 1..9. ~14 CCharString literals (TEXT_GUI_STAY /
  TEXT_GUI_RELOAD ... in case 1), CWorldMap::GetRegionNumberMapIsIn x13, CThing::GetPos x16, IsDisplayingMiniMapOnRegionLoad x6,
  screen fade calls, particle emitters (CTCDParticleEmitter::Create x4), player modes. Start from the matched CWorld sibling
  0x004A0D90 ($S/w22/wfA3/src) for the CWorld macro set; author case by case.

### 0x004A3740 third session -- full first DRAFT written (v/ur1.cpp, ungraded at time of writing)
- Read the whole function; compact listing tool tools/cdis.py (GETTC/PEEKTC idioms folded, callee names) -> work/cdis.txt.
- Local names from the Anniversary 4k PDB (S_REGREL32 of CWorld::UpdateRegionLoad): pregion_entrance, pteleport_out_particle,
  ptcd_particle_emitter, followers, pfollower_teleport_out_particle, pthing_carried, precall_teleporter, precall_thing,
  precall_region_entrance, regionIndex, majorIndex, visitedRegion, villages, pvillage, opinion, people, direction_to_head_in,
  pcamera_manager, pcreature, pfollower, nearby_pos, pcam_man, pcam_man_def (tmp/anniv_url.txt).
- Facts: no EH frame (CCharString temps without SEH); prologue `mov [esp+14h],0` = the conditional-destruction flag of the
  CCharString("Prison") temporary in case 4's `||` expression; HasTC is the inline bitset test up to ~0x004A5000 and the
  out-of-line CBitset test 0x00410DE0 afterwards (inline budget of the row spent, natural). Same-TU callees: SetKickPos 0x0049FFD0,
  SetGuildSealRecallPos 0x0049C9B0 (called once, expanded once), SetSaveGameMarkerPos 0x0049E590, AutoLoadRegionLeaving 0x004A34F0,
  IsDisplayingMiniMapOnRegionLoad 0x0049E9F0, DisplayCollectableRentInformation 0x004A0370, AutoSaveQuestStart 0x004A0CD0.
- Many callee / member names in the draft are PLACEHOLDERS (IsInWorld, IsFollowingHero, SetRegionLoading, TCI_TCBE ...): each
  needs the two-source proof; see the draft's call sites and work/cdis.txt.
- STATE at hand-back (third session): differ/004a3740.cpp = the full draft (v/ur1.cpp + grade line), SRC_FAIL: cl stops at 100
  errors. tools/ce.py prints every cl error by line (q.py shows only the last 3000 chars); tools/optin.py maps each
  "X is not a member of Y" to the header opt-in that declares it (work/ur_missing_members.txt = the list after the 2nd pass).
  Opt-ins already added in the draft: CQUESTMANAGER_REGIONLOCK, CSUBTITLERENDERER_SETTEXT_CHAR, CMAINGAMECOMPONENT_DISPLAYENGINE /
  _MAINFONT, CMESSAGEEVENTMANAGER_FINDRECENT / _REMOVE, PLAYER_REMOVEMODE, CTHINGMANAGER_ALLPAUSE, C2DVECTOR(+_SQUAREDMAGNITUDE_YX:
  retail squares Y first, 0x004A44F1), CCHARSTRING_STARTSWITH. Still needed: CDISPLAYENGINE_STARTSCREENFADEOUT; CWorld overlay
  opt-in typing RegionLoadStartPos (C3DVector) + RegionLoadFollowers (vector<CIntelligentPointer<CThing> >) + WeatherMasking
  handles (the pinned asserts name RegionLoadStartPos_X: put the typed members under a NEW opt-in with the float names kept in the
  #else and pin the moved asserts); CWorld decls of AutoLoadRegionLeaving / SetGuildSealRecallPos 0x0049C9B0 /
  SetGuildSealRecallAngleXY 0x0049C9D0 (Ego_r QAE ABV / M) / SetSaveGameMarkerPos / IsDisplayingMiniMapOnRegionLoad /
  DisplayCollectableRentInformation / SetAsStarting3DRendering; class headers missing: CTCFadeOutAndIn, CTCRegionDisplay; TCI
  enum names for 0x10 0x13 0x51 0x68 0x7 0x8e 0xbc 0xbe 0xd7 0xe0 (ETCInterfaceType.h); and the PLACEHOLDER member names listed in
  work/ur_missing_members.txt (each to be proven from the callee body / PDB before use).
- Placeholder identities to prove (callee address -> draft name): 0x004378B0 CPlayerGui "SetRegionLoadRegion"(CRegion),
  0x00437830 / 0x00437840 CPlayerGui bool queries, 0x006BD830 CMusicManager(bool), CASoundManager slots 0xC8 / 0xB8, CWorldMap
  slot 0x40 (pos -> map number) / 0x6C / +0x4C byte, CTCPhysicsBase slots 0x7C (SetPos?) 0x8C 0x108 0xFC(GetFacingAngleXY) 0x120,
  CAIGameCameraBase slot 0x24 (out CGameCameraManager*), CIEngine slots 0x130 (bool) / 0xDC StartNewScene, CTCHero 0x004AB270(CThing*),
  CTCFadeOutAndIn 0x006C3DF0 (AddThingToFade), CThing bytes +0x10 / +0x15C&0x40 / +0x1C9&2 / +0x91 / +0x93&4, 0x006AA100 on the hero,
  0x0066E985 on the hero-info manager, 0x006C6010 / 0x006D7570 / 0x006C6E20 / 0x006CED30 / 0x005FAA30 TC methods, CDisplayEngine
  +0xBC / +0xC8 / +0xD8 bytes, CPlayerGui +0xF4 / +0xF6 / +0xFD bytes, the subtitle event type 0x10 and message types 0x28 / 0x3C.
- Retail structure facts to keep: case 9's `test cl,cl; jne` at 0x004A5981 re-tests DoneExtraFrameUpdate... (source likely
  `if (done && !(fading && !locked)) {finish} if (!done) {...} else if (!AllPause) {...}`); case 8 releases two CDefPointers
  (region def [esp+18h], camera-manager-set def [esp+38h]) inline at the end; case 5/8 CThing lists are function-case scoped.

### 0x006F5C10 successor session -- RELOCATION_MATCH, test PASS, addmatch done, read-only vland PASS (0 RELOC_IDENTITY) (~30 grades + 4 inline traces)
- Step 1 (9096 -> 8735): FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (visible STLport _M_insert). With the whole insert chain visible
  c2 sees the value_type temporary is not captured: the +5 instr/block reloads, dead zero stores and the ebx zero all vanish (lesson wfB5).
- Step 2: the rest was the INLINE BUDGET only (call-target list per block, tools/cmpcalls.py + g5.sh: q.py masks call targets).
  c2 caller-size penalty read from c2.dll 0x10742BA5: pen = floor(floor(sz/2)*(a+g)/9000) once a+g > 9000 (NOT sz*(a+g)/18000
  as MATCHING_PLAYBOOK says: the sz/2 is truncated first). Retail transitions: value_type/pair ~CCountedPointer called from block 21,
  temp ~CCountedPointer from block 23, CCountedPointer(T*) from block 21, pair destructors from block 45. Ours: block 21 a+g 8995
  (needs > 9000) and, with a ~pair(){} probe, block 44 a+g 11720 (needs < 11625). Window = a few front-end tuples.
- Closer: ONE dead release assert, FABLE_RETAIL_ASSERT(RegisteredActions.empty()) at the top (device d, `// Inferred:` in the row
  header). size()==0 and either placement (before/after RegisterActionEventTypes) also match; ASSERT(true) does not. Needs
  FABLE_RETAIL_STLP_MAP_EMPTY + FABLE_RETAIL_STLP_SET_EMPTY (existing opt-ins) and fable_assert.h (agentC6 header, copied to hdr/).
- Strict headers: the 41 action classes that have engine/<Class>.h moved from local models into NEW FABLE_RETAIL_<CLASS>_PROTOTYPE
  opt-ins (first branch, existing text under #else, asserts pinned; tools/mkproto.py did it mechanically); still RELOCATION_MATCH.
  15 classes without a shared header stay local (CastWillDrainLifeSpell, RockTrollFallToDeath, WaspQueenDeath, MazeDoForcePush,
  BanditKingStuckGetHit, BreakIntoMelee, SummonCreatures, MidAttackKnockdown, InterruptableNearAttack, TurnStrikeLeft/Right,
  ChargeStrike, HobbeSpellcasterAim/Fire, ScreamerDie). ScreamerDrainAttack's opt-in needs the local CastWillDrainLifeSpell first.
- Test: test/006f5c10.cpp (keys + new sizes extracted from retail, tools/keys5.py; refcount model); a one-key mutant FAILs.
- Name fix kept: map 0x013BA8C8 = CCreatureActionBase::RegisteredActions (reloc_bindings `Registry`, NAME_FIXES.tsv).

### 0x004A3740 fourth session (wfE11 successor) -- still SRC_FAIL, parked (context spent closing 0x006F5C10)
- differ/004a3740.cpp now also defines FABLE_RETAIL_C2DVECTOR_FROM3D, CRGBCOLOUR + CRGBCOLOUR_CTOR4_INITIALISE, CMESSAGEEVENT and
  includes engine/CMessageEvent.h / CRGBColour.h (fixes the C2440 C2DVector(C3DVector), C2514 CRGBColour, C2027 CMessageEvent errors).
- Remaining (tools/ce.py; cl still stops at 100): C2118 negative subscript at the engine/CTCSoundPlayer.h include (an opt-in combo
  breaks one of its static asserts: find which define); CSubtitleRenderer::SetText 9-arg overload missing (row 0x00492973's wide
  overload is the 4-caption one: check retail's call at the case-1 site for the real overload and arg count); CTCFadeOutAndIn /
  CTCRegionDisplay have no shared header (declare locally: Fade(float,bool) / AddThingToFade 0x006C3DF0 / IsFading,
  SetCurrentAreaName); CTCRegionFollower.h has only IsRegionFollower: GetFollowers / IsCreatureFollowingHeroAcrossRegions /
  IsCreatureAllowedInRegionAt / RemoveFollower need a NEW opt-in with the Ego_r names (prove each from the call addresses);
  HasTC<CTCTargetingPlayer> spelling (use the header's HasTC form); CTCDParticleEmitter::AttachToThing takes CThing& (pass *phero);
  then the 26 member placeholders of work/ur_missing_members.txt (several are inline byte reads or VIRTUAL calls, e.g. CWorldMap
  slot 0x40 = a GetMapNumberAt(const C3DVector&) overload per engine/CWorldMap.h virtual list -- count slots, do not invent names).
- For the inline budget of this row later: the c2 caller-size penalty is pen = floor(floor(sz/2)*(a+g)/9000) past a+g 9000
  (see 0x006F5C10 notes); the HasTC inline -> out-of-line switch at ~0x004A5000 is that budget.

### 0x004A3740 fifth session (wfE11 successor) -- SRC_FAIL -> DIFFER(8624v8948): the draft COMPILES
- Work file v/ur3.cpp == differ/004a3740.cpp. tools/ce.py now prints header-file errors with their file name and the x.cpp line of
  template instantiations ("ref"); tools/addopt.py <Class> <OPT> - [anchor-regex] inserts an opt-in block into hdr/engine/<Class>.h
  (copies the repo header first, anchors searched from the class line) and pins moved asserts; tools/rep.py does batch replacements.
- Identities proven this session (two sources each: retail call/inline bytes + Ego_r/FableWin name set; see the opt-in comments):
  CSubtitleRenderer::IsInCutscene (+0xDE, was "IsInventoryOpen"), SetText 4-caption char overload 0x00492FB4, IsGamePausedFlagSet (+0xDC);
  CMainGameComponent::PeekMainFont (+0x1614C); CWorldMap::GetMapNumberAt(const C3DVector&) slot 16, SetSoundThemesDisabled (+0x4C),
  StartMovePlayerPos 0x00502500 / EndMovePlayerPos 0x005064B0 / IsLoadingAnyMap 0x004FB4A0, Update (slot 27, +0x6C);
  CDisplayEngine IsScreenFadeOut (+0xBC), IsScreenFullyFadedOut 0x004A9210 (out of line), IsScreenFadeOutClosing (+0xC8),
  IsScreenFadeOutLockFlagSet (+0xD8), EndScreenFadeOut 0x00434C90, Get3DEngine; CIEngine::Enable3DSceneRendering (slot 76 +0x130);
  message types 0x10 CONVERSATION_ANSWER_YES_OR_NO, 0x28 REGION_PREUNLOAD, 0x3C TELEPORTER_USED; CTCFadeOutAndIn (new header:
  FadeOut 0x006C3B70, FadeIn 0x006C3BD0, AddThingToFade 0x006C3DF0, IsFading COMDAT 0x0049D310); CTCRegionDisplay (new header);
  CTCRegionFollower GetFollowers/IsCreatureFollowingHeroAcrossRegions/IsCreatureAllowedInRegionAt(const CThingAICreature&,...)/RemoveFollower;
  CTCTargetingPlayer::StatGetClassTypeName + ForceToHaveNoTarget (+0x3D) -- the "HasTC<CTCTargetingPlayer>" site is HasTC(8) + GetTC(8) +
  class-name compare (GETTC non-const LowerBound, not PeekTC); CThingCreatureBase::IsFollowingHeroThroughTeleporters (+0x15C&0x40);
  CClass::GetType()==1 (was "IsInWorld"); CThing::GetCurrentMapNumber (+0x8E), IsToKillOnLevelUnload (+0x93&4), StatGetType()=15;
  CTCHero::SetTeleportParticle 0x004AB270, SetWeaponsAsUsable 0x006CA380, IsWeaponUseOverridden/IsWillUseOverridden/SetWillAsUsable;
  CTCTrophy::IsMiniGameInProgress (+0x14); CTCInventoryMap::IsOpenedViaGuildSeal (+0x241), ResetWorldMapDisplay 0x005FAA30;
  CQuestManager::IsAnyQuestActive 0x004B1080 / IsRegionStartOfRegionLockingMode 0x004B11A0 (Ego_r donor order); CMusicManager::PauseMusic
  0x006BD830 (was "SetRegionLoading"); CASoundManager FreezeAtmosMaps (slot 50) / SetGainFader (slot 46); CPlayerGui SetMiniMapRegion
  0x004378B0 / IsMiniMapFinishedOpening 0x00437830 / IsMiniMapFinishedClosing 0x00437840; NProgressDisplay::StopProgress 0x009E9F20;
  NHeroInformationScreens::CManager RemoveAllPlayerModes 0x0066E985 / Close 0x0066E73D / IsOpen (+0x34); CTCTrespasser::LeavingRegion
  (new header, slot 28 +0x70); CTCHeroExperience::OnRegionLoad 0x006D7570; CThingAICreature ForceAIReset 0x006D5AB0 /
  IsRegionFollowingOverriddenFromScript (+0x1C9&2); CTCVillage PeekVillagePeople / ClearCrimes; CTCPhysicsBase ResetPosition (slot 31 +0x7C,
  was "SetPos"), ResetMovementState (slot 35 +0x8C), SetFacingAngleXY, GetRHSet; CTCCreatureNavigation::GetNearbyDirectedPosition 0x006CB780;
  CTCWife::OnHeroReturnToRegion 0x006CED30; CThingPlayerCreature::OnRegionChange 0x006AA100; CGameCameraManager::ResetCamera 0x0069A990;
  CTCAppearanceMorphBase::UploadTextureMorphsToEngine 0x006CA7F0 (new header, TCI_APPEARANCE_MORPH); CTCHeroRememberCrimes Get/SetHasPaidTheFine
  (+0x18, new header); CRegion::GetPRegionDef (COMDAT 0x00419650 called) + CRegionDef PreventHeroWeaponUse/PreventHeroWillUse/CameraManagerSetIndex;
  CAsyncLoadingMemoryPool::ProcessLoading 0x00A14900 (new header); CTCSoundPlayer::PlayUnattachedCriteria 0x0066BDF0.
- INFERRED (one source only, say so in hand-back): CTCHeroOpinionDeedLog::PlayerPutWeaponAway = 0x006C6010 and PlayerTakenOutWeapon = 0x006C6E20
  (the deed log's only two QAEXXZ methods; put-away at region unload, taken-out when IsThingCarryingWeapon(hero) after load).
  CTCRegionFollower::GetFollowers(std::vector<CThing*>&) 0x006AEE40: in no PDB name set (landed row's name; signature as CTCFollowed's).
- NAME_FIXES candidates: landed 0x006AEBF0 spells IsCreatureAllowedInRegionAt(const CThing*, ...) -- Ego_r has (const CThingAICreature&, ...);
  landed 0x00434C90 leaf CDisplayEngine_Methods::EndScreenFadeOut -- PDB CDisplayEngine::EndScreenFadeOut.
- NEXT: work the 324-byte residue hunk by hunk in retail address order (tools: q.py output in tmp/q_ur3.txt; compare call TARGETS per case).
- Residue measured after the first grades (v/ur4.cpp == differ/004a3740.cpp; ur4 = ur3 + same-TU bodies GetFrame / SetGuildSealRecallPos /
  SetGuildSealRecallAngleXY defined above the row, with FABLE_RETAIL_CWORLD_TYPED_SAVE + FABLE_RETAIL_C3DVECTOR_PACKED for the typed
  GuildSealRecallPos; both grade DIFFER(8624v8948)):
  * Case byte sizes retail v ours (jump table v the obj's $L labels): prologue 0x46/0x44, case1 1919/1857, case2 216/216, case3 207/199,
    case4 1719/1635, case5 1417/1413, case6 937/934, case8 2018/1879, case9 400/377. Frame 0xF8 retail v 0x100 ours.
  * Call sequences (tools/calls2.py, our obj via ce.py + objdump): structurally equal except (a) case 8/9: retail calls the out-of-line
    std::bitset<274>::test 0x00410DE0 (7 sites from 0x004A5305) and IS_THING_ALIVE 0x0040DCE0 (0x004A56F2, 0x004A57AE) where ours still
    expands them -- the row's own inline budget (re-measure once the length gap closes; device b /d2inlT is allowed for a pure inline residue);
    (b) case 6: retail has two ResetPosition (slot 31) calls 0x004A4F2E / 0x004A4F6E, ours tail-merges them into one; (c) retail calls
    SetGuildSealRecallPos at 0x004A45E7 but expands it at the first site; ours (with the body visible) expands both.
  * Dominant residue: GLOBAL register allocation. Ours: this in esi, phero in ebp, the constant 0 kept in ebx (`xor ebx,ebx`, then
    push ebx / mov [m],ebx everywhere -- 1 byte shorter per use, most of the -324). Retail: this in ebp, phero in edi + [esp+24h], immediates
    (`push 0`, `mov [esp+14h],0`), ebx used for case-local values (bl = the confirmed flag in case 1, loop indexes). Next idea: find which
    retail case-local variable owns ebx (the case-1 `bool confirmed` is bl at 0x004A38DB; loop counters `xor ebx,ebx` at 0x004A3CC5 /
    0x004A4E51) and make their use counts / scopes match (named locals at function scope v case scope), then re-grade; the c2 priority of a
    callee-saved register for the zero constant falls once real variables outrank it.
- Grades this session: 3 full q.py grades of the row (+ ~35 ce.py compiles); all four matched rows re-graded after the overlay changes:
  still RELOCATION_MATCH (004736c4, 0044c72b, 006f5c10, 004804de).

### 0x004A3740 sixth session (wfE11 successor #2) -- DIFFER(8624v8948) -> DIFFER(8804v8948), hunks 205 -> 158
- Work dir v6/ (a0 = old differ). Each step graded; differ/004a3740.cpp = v6/f1.cpp. Tools: tools/g.sh (one-line grade + hunk
  count), tools/md.py (register-normalised difflib of q.py output), tools/bd.py (byte delta per hunk, largest first), tools/win.py
  (window of retail/ours by address), tools/al.py, tools/cc.sh (calls2 on our obj).
- Steps that moved (all EVIDENCED by retail bytes):
  a1 FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ: retail expands CCharString::operator==(const CCharString&) at the CTCTargetingPlayer
     class-name compare (0x004A3E53..0x004A3E8A: data pointers, null tests, length +4, Compare 0x00411570) (+96 bytes).
  a2 FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ: retail expands operator==(const char*) at "HeroGuildComplexInside" (0x004A4252..
     0x004A4287: null arm = strcmp(text,"") as repe cmpsb, else CBasicString==(const char*) 0x004115A0) (+48).
  b1 case 4 distance test is `to_entrance.GetSquaredMagnitude() > to_teleporter.GetSquaredMagnitude()` (retail test ah,5 / jp).
  b10 positions as four named C2DVector copies (entrance_pos, hero_pos, teleporter_pos, hero_pos2) then the two differences:
     retail calls GetPos e,h,t,h with each copied before the next call and subtracts after the 4th (0x004A4479..0x004A44F1). FITTED
     spelling (a natural one; the unnamed temporaries evaluate the argument first: h,e,h,t). This flipped GLOBAL allocation:
     this -> ebp, phero -> edi, frame 0xF8 (all as retail).
  c2 case 1: the four CCharString locals of SetText in their own { } block: retail destroys them BEFORE RegionLoadWaitingForConfirmation=true
     (0x004A38A7..0x004A38CE). Temporaries in the call (c1) were worse (frame 0x108).
  d1 case 6: `if (pfollower->PeekTC(TCI_NAVIGATION,&pnavigation) && (GetNearbyDirected(..) || GetNearbyWander(..)))`: retail's
     HasTC jns goes straight to the else ResetPosition (0x004A4E8E -> 0x004A4F38); loop counter i then lives in memory as retail.
  d3 case 6: `CTCAppearanceMorphBase* p; if (phero->GetTC(TCI_APPEARANCE_MORPH,&p)) p->UploadTextureMorphsToEngine(...)`
     (retail takes the TC before the engine argument, 0x004A4FFF..0x004A5040).
  e1 the two descriptive globals are CWorld.cpp FILE STATICS (only this function references them; xref): 0x013B89D2 =
     `static bool GAutoSaveQuestStartOnScreenFade` (Anniversary PDB S_LDATA32, same neighbours GDoNotCallStartAutoSaveProgress /
     GDisplaySaveGameLaunchFailed as TLC 0x013B89D1 / D3), 0x01378880 = `static long GExtraFramesToWaitBeforeProgressRender = 2`
     (Anniversary S_LDATA32 long; TLC .data initial value 2). Static -> c2 schedules the store as retail (0x004A4737).
  f1 GetMessageEventManager().AddMessage/FindMostRecentMessage/RemoveMessage (FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER, existing opt-in:
     retail loads [this+60h] before the argument pushes, 0x004A404E). GetMusicManager / GetGameScriptInterface: no change (not used).
- Tried, no gain: C3DVector named-result / -= forms, C3DVector difference then C2DVector, /d2inlT 30..150 (default = T65..70; T60
  8836 but case 8/9 calls still expanded), /d2inlS (no effect at any value), GETASCHARARRAY_SELECT/NULLRETURN (StartsWith still two
  null tests), C3DVector(0,0,0) int literals / Initialise at the case-1 zero site, removing C3DVECTOR_PACKED (SRC_FAIL).
- c2prio (tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py, 65 s): const 0 is candidate #1 (prio -697, 213 refs), split
  into pieces; pieces #290 (prio 8) and #109 (prio -55) take ebx -> ours `push ebx` (14 sites: SetText 0,0 in case 1, case 3
  AddMessage, case 4 PSoundManager, case 5, case 9) and `cmp [m],ebx`. Retail has no zero piece. Output tmp/prio_f1t.txt (--trace).
- Later steps (sixth session, continued; base templates v6/*.tpl with `//@ASSERTS@` = N copies of FABLE_RETAIL_ASSERT(phero), tools/sw.sh):
  g1 `StartsWith("Prison")` (implicit conversion, not CCharString("Prison")): retail one null test of the prefix (0x004A4299..0x004A42B7).
  A92 INLINE BUDGET: c2 caller-size penalty never fires in ours (a+g max 8874 < 9000; tools/inl.sh + an.py now prints ag). Retail calls
     bitset::test 0x00410DE0 at the last 7 HasTC sites and IS_THING_ALIVE at 0x004A543B/0x004A56F2/0x004A57AE, i.e. a+g crosses 9000
     between our decisions #511 and #538 => ~1560..1800 more IL tuples in retail before case 8. Our /Od build is 11.9 KB / 3302 instr v
     FableWin UpdateRegionLoad 23130 b / 5206 instr (FableWin.exe.signatures.tsv): ~1900 instructions of debug-only code = ASSERTs. So
     device d (dead release asserts) is the evidenced closer. One trivial FABLE_RETAIL_ASSERT(phero) = 18 tuples. N=96..100 at the top
     now gives retail's exact call list in cases 8/9. PLACEHOLDER: the asserts must be spread as plausible pointer asserts before case 8
     with the same total (re-tune with sw.sh; N also moves the frame map: N=84/90 -> fm 380 v 202).
  perm08 case-1 confirmed path: `RegionLoadWaitingForConfirmation=false; RegionLoadStatus=0; RegionLoadStartPos=C3DVector(0,0,0);
     RegionLoadStartAngleXY=0;` (retail's zero piece in eax covers the temporary: `xor eax,eax; mov ecx,eax`, 0x004A39BD).
  h2/h3 implicit CCharString conversions at PlayUnattachedCriteria("BIG_DOOR_SLAM_SHUT", ...) and DefinitionManager->GetDef(
     "CAMERA_MANAGER_SET_DEFAULT", ...): retail builds the string before the other arguments (0x004A5938, case 8).
  h4 case 9 `if (!phero->GetTC(TCI_FADE_OUT_AND_IN, &pfade)) pfade = AddTC(...)` (one out-of-line bitset::test, 0x004A5642); the case-2
     site keeps HasTC + GetTC (changing both: worse).
  k6 FableWin COMDAT order after UpdateRegionLoad (FableWin.exe.signatures.tsv 0x01B4EF90..0x01B4F520 = inline helpers first used by the
     row): SetGuildSealRecallPos, SetGuildSealRecallAngleXY, IsTeleportingEnabled, IsRegionLoadToDisplayRegionName,
     IsScreenToFadeInOnNextRegionChange, SetScreenToFadeInOnNextRegionChange, IsAllEntitesPauseMode, GFGet2DDistanceSquared,
     IsFollowingHeroThroughTeleporters, IsNonZero(C3DVector)(not in TLC row bytes), IsScreenFadeOutClosing, ... ForceToHaveNoTarget.
     The case-4 distance test is `GFGet2DDistanceSquared(phero->GetPos(), precall_region_entrance->GetPos()) >
     GFGet2DDistanceSquared(phero->GetPos(), precall_teleporter->GetPos())` (implicit C2DVector temporaries; retail evaluates e,h,t,h and
     keeps dx/dy as separate 4-byte frame slots [esp+14h]/[esp+10h]); new overlay opt-in FABLE_RETAIL_C2DVECTOR_GFGET2DDISTANCESQUARED
     (body FITTED: pos2-pos1, dy*dy first; /Od 24i/62b v FableWin 25i/64b). Replaces the fitted four named C2DVector copies.
  l1 new overlay opt-in FABLE_RETAIL_CWORLD_REGIONLOAD_ACCESSORS (the four FableWin accessors above; no byte change, evidenced spelling).
- Current best: differ/004a3740.cpp = v6/l2_100.cpp = DIFFER(8856v8948), 107 hunks. cc.sh call list = retail's except case 6.
- Remaining residue: (1) FRAME LAYOUT: ~200 instructions with different esp offsets (tools/fm.py; tools/fit.sh prints fm=); retail
  keeps the PeekTC key temporaries at [esp+10h]/[14h]/[18h], ours elsewhere; the high region is 8 bytes lower in retail. (2) the
  const-0 pieces: ours keeps 0 in ebx across cases 1/3/4/5/9 (c2prio: pieces of candidate #1 take ebx; a callee-saved constant is given
  back after codegen only when its benefit <= 2, C2_ALLOCATOR_PROBE.md); retail has local zero pieces (case 9 `xor ebx,ebx` 0x004A58B0).
  (3) case 6 ResetPosition arms merged in ours (register choice inside the arms differs in retail). (4) CQuestManager::Get() object
  loaded before the GetRegion argument chain in ours (0x004A37E2), after it in retail (0x004A3810, 0x004A4666).
- c2prio on l2_100 (tmp/prio_l2t.txt): const 0 (#1, prio -692, 209 refs) still splits into pieces; #177 (prio 8, allowed only ebx)
  and #23 (prio -55) take ebx; phero is also split (pieces in esi/ebx/eax). Retail keeps phero in edi with home [esp+24h] and has no
  global zero piece. Next idea for the zero piece: find what makes retail's phero a single web (edi) -- our phero pieces take the
  registers that leave ebx free for the constant; candidates: phero_tc / pplayer named locals, the case-local `phero` re-reads.
- Next idea for the frame: tools/fm.py shows retail sharing [esp+10h] for several PeekTC key temporaries (cases 4/6/8) and the high
  half 8 bytes lower; the frame packer rules are in MATCHING_PLAYBOOK "Frame packer" (size asc, IL weight desc, join latest slot,
  density sort); the hand886 pack.py tracer would name the first differing node.
- Grades this session: ~130 full q.py grades + 4 c2prio runs + 3 inline traces. Matched rows regraded after the two overlay header
  additions (CWorld.h, C2DVector.h): 004736c4 / 0044c72b / 006f5c10 / 004804de still RELOCATION_MATCH.

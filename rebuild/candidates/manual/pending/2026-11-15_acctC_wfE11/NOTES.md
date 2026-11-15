# wfE11 NOTES

## 0x004736C4 CPlayerGuiDef::Transfer -- RELOCATION_MATCH, test PASS, addmatch done (1 grade)
- Name proven: vtable 0x012352DC slot 18 (Transfer slot of every def), Ego_r.pdb + FableWin.pdb. Extent 0x004736C4..0x00475023 (`ret 4`) = 6498 bytes, confirmed.
- What matched: the acctB outline body (463 statements, retail order incl. the out-of-order minimap colours) on the repo's
  existing whole-class opt-in FABLE_RETAIL_CPLAYERGUIDEF_CLASS + FABLE_RETAIL_DEF_CLASS_BASE + FABLE_RETAIL_CPERSISTCONTEXT +
  _NAMED_DEFAULTS (3-arg TransferMap; defaults `0L` / `GAME_ACTION_NULL`). TU pragmas y-off + s-on (CSoundThemeDef::Transfer twin). No flags, no overlay change.
- vland read-only: 0 RELOC_IDENTITY; 5 RELOC_NAME warnings (TransferVector<T> callees landed as TransferBinary_* / a wrong label) -> NAME_FIXES.tsv.
- Test: model of the body (offset + helper kind per statement) vs the retail call sequence table; mutant (one offset swapped) FAILs.

## 0x0044C72B CGameDefinitionManager::InitAndCompile -- RELOCATION_MATCH, test PASS, addmatch done (5 grades)
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

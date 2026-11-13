# Hand chunk 2051 (CWorld .. CMap): 2 of 5 byte-exact

## What matched (2)
- **0x004A6E30 CWorld::Init** (src/004a6e30.cpp, test/004a6e30.cpp; RELOCATION_MATCH at the FIRST full grade, vland
  RELOCATION_MATCH, tests PASS under g++ and VC7.1). `// Flags: /Gr` (the TU's free functions and boost::checked_delete<T> are
  __fastcall). Shape: the landed PostInit 0x004A6550 pattern (StartProfile / DisplayProgress per subsystem) plus
  `P<Member>.reset(new T(...))` on the Ego_r.pdb-typed members (hdr CWorld.h opt-in FABLE_RETAIL_CWORLD_TYPED_MEMBERS). What made
  it match with no iteration: (1) real Boost 1.30 scoped_ptr::reset (swap, then checked_delete) from the shared boost/scoped_ptr.h;
  (2) REAL implicit destructors in the TU for the classes whose checked_delete retail expands (CFactionManager vector,
  CThingSearchTools vector) and for the three it calls out of line (CGameTimeManager set, CBulletTimeManager map,
  CScriptConversationManager map): c2 makes the same inline / out-of-line split by itself; (3) CTBaseSingleton<T>::Get from the
  shared header for the skeletal-morph singleton; (4) CClassTypeInit's inline ctor (TypeID byte from the long navigator-type global);
  (5) the stack CGameNavigatorManagerExternalInterface temporary with an inline ctor and empty inline virtual dtor.

- **0x0042EC7C CNewFrontendGameComponent::Run** (src/0042ec7c.cpp, test/0042ec7c.cpp; RELOCATION_MATCH 2191v2191, tests PASS
  under g++ and VC7.1; rel.py: every call / data target equals retail, 11 name-only differences against stale bound spellings ->
  NAME_FIXES.tsv). `#pragma optimize("s", on)` alone (biased `lea ebp,[esp-74h]` frame), `// Flags: /Gr` (the TU's own free
  functions GPlayMusic / InstallationComplete are YI in both PDBs), CRGBColour copy forced (opt-in, `// Inline-budget:` line:
  c2 size 31 savings 5 -> 26 > the size-TU limit 15, refused with no score). The last residue (12 bytes, the bank manager address
  hoisted into the bank-if test block, no skip-path load, display / timers / Preload vtable in edi, return-false block misplaced)
  was ONE statement shape: the menu-font glyph string is a NAMED block-scoped local --
  `{ CWideString glyphs(L"ABC...()"); m_pMenuFont->Preload(glyphs, true); }`. With the unnamed temporary the font's vtable is
  loaded ahead of the string constructor into a callee-saved register, and that candidate pushes &GBankFileManager out of edi for
  the init block and main loop (c2 then splits the constant into a bank-block piece and a leave-block piece). FITTED (one source
  shape among several that compile alike; proven only by the full-row grade).
  Continuation measurements kept in tmp/*.trace (c2 colouring traces) and probe/k/ (structural probes): VC7.1 places a constant
  shared by an if-block and a later block at the if-test block (idom of the join) whatever the else arm holds; what retail shows
  (first-use load + skip-path load + reload after the attract block) is the allocator's split of that constant when edi is free
  in the init block -- i.e. decided by the init block's other candidates, not by the bank code.

## Not started / in progress (one row at a time)
- 0x0087F3A0 CCombatAction_KickThingOnGround::PerformAction (2422 B; virtual, `sub esp,0C4h`, plain `ret`, single exit at
  0x0087FD0B) -- STARTED by the continuation agent (decoded outline below, no graded draft yet). Retail disassembly:
  tmp/pa_retail.txt. Models: landed siblings Initialise 0x0087E860 / FinishAction 0x0087EBD0 / GetNextPos 0x0087E770 (local
  CCombatActionBase model: PCreature +0x08, PTarget +0xA8, MoveToPos +0x114, Movement +0x120, MaxRunningSpeed +0x12C; retail
  sits 0x18 below the PDB offsets) -- the hand-in must use the shared headers (engine/CCombatActionBase.h has opt-ins,
  engine/CThing.h with FABLE_RETAIL_CTHING_GETTC_EXPANDED, engine/CIntelligentPointer.h, engine/CKickableDef.h,
  engine/CHitParameters.h, engine/CTCHeroExperience.h). Decoded flow (this in [esp+3Ch], &PCreature in [esp+24h], ebx = 0):
  1. `pos = PCreature->GetPos()`; `dy = pos.Y - MoveToPos.Y; dx = pos.X - MoveToPos.X; if (dx*dx + dy*dy > C[0x01277D30]) return;`
     `if (fabs(PCreature->GetPos().Z - MoveToPos.Z) > C[0x0122DED8]) return; if (!IS_THING_ALIVE(PTarget)) return;`
  2. bool [esp+1Bh] = target HasTC 0x42 (+0x28 bit 2); if so GetTC(0x42) (LowerBound 0x0040F020) ->
     CTCEntityEvents::AddEvent 0x008113C0 (`new NEntityEvents::CEventKillLocked` = new(8), +4 = 0, vtable 0x01277B5C; landed ctor
     0x0087FED0 expanded).
  3. `CDefPointer<CKickableDef> kickable_def` ([esp+38h] = 0, ebp after): target def handle +0x70 (inc/dec [p+4] refcount) ->
     GetSubDef<CKickableDef> 0x006AE260; `kickable = got && kickable_def->+0x40 (shot def id) != 0`.
  4. kickable: [esp+1Bh] = 1; if (target +0x6C & 8) target->SetAttackedByCreature(PCreature) 0x006624F0.
     not kickable: if (target +0x6C & 8) { CHitParameters hit(0, PCreature, 0) (ctor 0x0087FF60); hit.AddDamageWithAttribute(1,
     1.0f) 0x00868CA0; target->virtual slot 65 (+0x104)(hit); ~CHitParameters 0x00789358 } else if (target type byte +0x10 == 13)
     { CDefPointer<CAppearanceDef> via +0x70 handle GetSubDef 0x0042B0A2 + conditional-destruction flags ([esp+1Ch] = 2 / 4 / 8:
     a `&&` with temporaries: `GetSubDef(&appearance) && appearance->AnimSet(+0x34).AnimExists(CCharString(str 0x01277D10),
     CAnimFilter... (vtable 0x0122FFDC, field 0xE))` 0x005DC2E0); if it exists: second AnimExists with the target's own filter
     (+0xD0) picks the filter, `GetRandomAnim(CCharString(same), *GetWorld()->GetWorldSeed(), filter, -1)` 0x005DCC00, the
     target's animating TC (0x83: +0x30 bit 3, else AddTC(CCharString(str 0x0123C260), 0, 0)) ->PlayAnimation(anim[0], 0, 0)
     0x007F9010, target->SetAsRenderedThisFrame(2) 0x004C7590; release appearance }.
  5. 0x0087F79D: if (target type == 5) target->ResetDieOffTimer() 0x00702CE0; if (!kickable_def) return;
  6. if (kickable) { CDefPointer<CThingShotDef> shot_def: GetWorld()->DefinitionManager(+0x10)->GetDefShot(kickable_def->+0x40,
     &shot_def) 0x0078B230; shot TC = target->AddTC(shot_def->+0x60 CDefString::GetString() 0x00415DD0, true, 0);
     facing = PCreature physics(+0x60)->virtual +0x118 (GetFacing, hidden C3DVector); three floats = 0; if PCreature HasTC
     0x68 (+0x2C bit 8: CTCHeroExperience) and !PCreature->IsChild() 0x00661F70: strength = (float)GetTrainableStatLevel(0) /
     (float)GetTrainableStatMaxLevel(0) (0x006D6520 / 0x006D6770); movement copy (this +0x120); delta = target pos - creature pos;
     fast inverse sqrt through the 0x013CE558 table + one Newton step (constants 0x0122DED4, 0x0122F59C) -> unit direction; the
     movement projected onto it (dot X,Y,Z; x87 order Y, X, Z); speed = |projected| / MaxRunningSpeed; skill = ((float)
     level(3) + 1) / ((float)max(3) + 1) * speed }; r = GFFloatRandom(def +0x3C, seed) 0x004978A0 + def +0x38; velocity =
     facing * r, height = a*def+0x34 + b*def+0x30 + c*def+0x2C + def+0x28, start = target pos + (vx, vy, vz*height...);
     CTCShotDefinitionBase::Init(CDefPointerByValue<CThingShotDef>(shot_def), PCreature, 0, &start, 50.0f, 1.0f, 0, &null
     ability) 0x007BF790; target HasTC 0x31 (+0x24 bit 17) -> CTCCreatureModeManager::AddMode(0x39) 0x0073AE10;
     tc = target->AddTC(CCharString(str 0x0123A014), false, 0); if (tc) { tc +0x28 = 1; SetRotationInfo(C3DVector(0,0,1),
     0.15f, 5.0f) 0x0072A530 }; CTCDParticleEmitter::Create(shot_def +0x40, target pos + (0, 0, C[0x01238170]), false)
     0x006E0880; release shot_def }
  7. 0x0087FC74: if (def +0x44 || def +0x4C) and PCreature HasTC 0x51 (+0x28 bit 17: CTCHeroOpinionDeedLog) and !def +0x48 and
     def +0x44: PostDeed(def +0x44, PCreature->GetPos(), PTarget, 0, 7, 0, 0, 0, 0, 0) 0x006C6990; release kickable_def.
  Spelling hints found while decoding: the def-handle copies with `inc [p+4]` before GetSubDef are the by-value
  `CThing::GetPBaseDef()` temporary (engine/CThing.h); `test byte [thing+6Ch],8` is `GetTypeFlags() & 8` (ThingTypeFlags +0x6C);
  the step-4 anim block's flags 2/4/8 are conditional destruction of `GetPBaseDef()` / `CCharString(...)` / `CAnimFilterNoGroup()`
  temporaries in one `&&` -- copy the include / define set of the landed CThingAICreature::AnimExists 0x00830F20
  (FABLE_RETAIL_CANIMATIONSET_ANIMEXISTS, FABLE_RETAIL_CANIMFILTER, engine/CAnimFilterNoGroup.h, CAppearanceDef Animation
  +0x34); the inverse square root is engine/C3DVector.h's FABLE_FORCEINLINE GFFastOneOverSqrt (FABLE_RETAIL_C3DVECTOR, table
  0x013CE558, `(3.0f - r*r*v)*r*0.5f`), probably through C3DVector::Normalise; CKickableDef members (engine/CKickableDef.h):
  BaseDistanceToTravel +0x28, StrengthScaleRange +0x2C, SpeedScaleRange +0x30, BaseSpeedScaleRange +0x34, RandomBase +0x38,
  RandomRange +0x3C, ShotToAttach +0x40, OpinionDeedType +0x44, OpinionDeedFaction +0x48, OpinionDeedTypeToEnemies +0x4C.
  Next: name every callee (two sources), write the full draft over the shared headers, grade, iterate.
- 0x00A7AF60 CNavQuadTree::LoadFromFile (2503 B): `and esp,-8` frame, CCachedFile reads, STLport vector inserts; landed CNavQuadTree
  ctor 0x00A7A760 and many CNavQuadTree / CANavQuadTreeNode rows in 0x00A7xxxx for the model.
- 0x00821A80 CMap::LoadFromFile (2648 B): /GS; same TU as the landed CMap::LoadBrushesFromFile 0x00821010 (copy its defines and
  includes: engine/CMap.h retail model works); CReplaceSerialise<...>::ReadFromFile helpers, CFileLargeThemePalette x2 in a 0x10822
  allocation, theme-index tables at +0x1A6C.

## Names (two sources each; LABELS.tsv / NAME_FIXES.tsv)
- Proven: all CWorld::Init callees (Ego_r.pdb ctor spellings + argument shapes), CCombatManager::Init 0x006E8300 (catalog said
  ResetDialogue), CCreatureActionBase::StatRegisterClass 0x006F5C10, boost::checked_delete<T> x3 (landed stand-in names wrong),
  NUISystem::CManager::SetInput (EInputType nested: @12@), CTBaseSingleton<CSkeletalMorphResourceManager>::PClass 0x013BAB10.
- Context-proven (PDB name + use): CFrontEndManager::CanStartAttractMode 0x00595A03, CXMVPlayer::SetSkipOnAnyButton 0x0042DB90.
- Descriptive (no PDB data symbols): CXMVPlayer::ClearColour 0x013961E0, CXMVPlayer::SkipOnAnyButton 0x013CA7E9 (Run only);
  reused bound spellings g_NavigatorTypeDefault 0x0129CBA4 / g_NavigatorTypeHover 0x0129CB44 (three different landed names exist for
  0x0129CBA4: g_NavigatorTypeDefault, g_groundWanderMode, g_flyingMode -- needs one name), g_bFrontEndFlag_01375448 /
  g_bFrontEndFlag_137544A / g_bRuntimeLevelFiles / g_pRetiredGameComponent (Run).
- FITTED bodies: CSystemManager::GetForceMusicPlay / ResetForceMusicPlay (store order from Run), CFrontendGameComponentInit ctor,
  CMainGameComponent::PeekEngineGraphics (reads PMainGraphicBank).

## Header debt
- See hdr/README.md. CWorld.h's retail branch flattens the scoped_ptr / CCountedPointer members to raw words; the typed opt-in should
  become the retail default after the 16 landed CWorld-retail includers are respelled (their accessors use the _ptr / _Data names).

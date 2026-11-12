# Hand chunk 2051 (CWorld .. CNewFrontendGameComponent): 1 of 5 byte-exact (hand-over in place on 0x0042EC7C)

## What matched (1)
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

## In progress -- 0x0042EC7C CNewFrontendGameComponent::Run (best: differ/0042ec7c.cpp, DIFFER(2179v2191), 587/624 frame-masked)
- TU: `#pragma optimize("s", on)` only (adding `("y", off)` loses the biased `lea ebp,[esp-74h]` frame), `// Flags: /Gr`.
- Matched so far: the whole movie table (CMovieInfo aggregate, Ego_r.pdb), the movie loop body (needed the CRGBColour copy ctor
  forced: hdr CRGBColour.h FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_COPY -- in a size TU the plain copy ctor is refused with no score;
  `// Inline-budget:` line still to be added to the source when it matches), SetClearColour / SetSkipOnAnyButton statics, bank
  open / close blocks, the init / main loop / attract-movie block / language switch, the three exit paths
  (CFrontendGameComponentInit's inline ctor ??0CFrontendGameComponentInit@@QAE@XZ fixed the zero store; a separate
  `long index = rand() % (long)size();` statement fixed the def-pointer reload).
- Exact remaining difference (relocation + frame masked):
  retail 0x0042EE43 `je L189` / 0x0042EE7D `mov edi,offset GBankFileManager` (inside the bank block, after the first temporaries)
  / 0x0042EF20 `jmp L190` + return-false block (0x0042EF22 call ~CWideString; xor al,al; jmp epilogue) / 0x0042EF2E `mov edi,offset
  GBankFileManager` (skip path) -- ours: `mov edi,R` hoisted before the `je` in the test block, no skip-path block, the return-false
  block laid out after the CXMVPlayerBuffers allocation (ours 0x0042EF93). Then in the init-frontend block ours keeps the Preload
  vtable, `display`, &m_attractModeTimer, &m_inputDelayTimer in edi where retail has the constant in edi (display in [ebp+70h], two
  `lea ecx,[esi+100h]` / `lea ecx,[esi+120h]`), and retail re-sets edi after the attract block (0x0042F20A); ours re-materialises
  it only in the leave block (0x0042F308). Plus a frame swap in the movie loop: i [ebp+6Ch] / error_string [ebp+68h] in retail,
  the other way round in ours.
- Measured (c2 traces, tools/c2p, tools/c2pf, tools/pk/tc.py): Run is 702 tuples (regime 0, every block w=1 in the size TU); the
  constant (#20) is "referenced, cost 0" in block 10 (the bank-if test) together with L"pc\\" and L"frontend.big" -- c2 hoists the
  constants that the first bank block shares with the leave-frontend bank block to their common dominator; removing the
  leave-path bank code removes the hoist (diagnostic g12). Forcing the constant's priority to 300 gives it edi unsplit but
  steals edi from the movie-loop pointer (worse, 528 equal). So the lever is the hoist / def placement, not the priority.
- Tried with no effect: reference / pointer local for the bank manager (function scope, block scope), `for(;;)` + breaks for the main
  loop, named `const CMovieInfo&`, `sizeof` loop bound, display as reference, `== true` test, bool guard local, direct calls.
- Next ideas: (1) find what makes VC7.1 sink a shared constant instead of hoisting it (try the first bank block's three calls
  written through a temporary CWideString path local, or OpenRetailBank's first argument built in a named local, so the block's
  first constant use moves after a call; try the leave block's bank code before `CTBaseSingleton<CFileInstaller>::Release()` vs
  after; try `NBankFileManager::GBankFileManager` reached through an inline static accessor if one turns up in FableWin's COMDAT
  list for this TU); (2) a short c2 trace of where the global-CSE pass (not the allocator) inserts the constant's def; (3) the
  i / error_string frame swap usually follows from the same live-range change -- recheck after (1).

## Not started (one-row-at-a-time rule; Run is still open)
- 0x0087F3A0 CCombatAction_KickThingOnGround::PerformAction (2422 B): landed siblings Initialise 0x0087E860, GetNextPos 0x0087E770,
  FinishAction 0x0087EBD0 (models); 34 CBaseIntelligentPointer::GetPItem calls, x87 maths with a fast inverse-sqrt table, CHitParameters.
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

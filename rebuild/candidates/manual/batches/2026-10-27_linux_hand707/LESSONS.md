# Chunk 707 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `T* p = static_cast<T*>(Ptr.GetPItem()); p->F(a, b);` calls the out-of-line GetPItem BEFORE pushing a and b. Written inline as
  `static_cast<T*>(Ptr.GetPItem())->F(a, b)`, the arguments are pushed first (63v63 with three instructions moved;
  CCreatureAction_RiseUp::FinishAction 0x008B6A90).
- A void function whose two exits both tail-jump to the same call (two `mov ecx,esi ; pop esi ; jmp Terminate`) is a guard clause:
  `if (!A() || B()) { Terminate(); return; } ...; Terminate();`. The `if (A() && !B()) {...} Terminate();` form shares one exit and
  keeps `this` in edi (52v60; CAIStateGroup_SkillPractice::Update 0x0092B3A0).
- `Member = C2DVector(x * a, y * b)` with a two-float inline ctor gives retail's 8-byte frame temporary (`fstp [esp]` / `[esp+4]`),
  copied with two dword moves. Assigning `.X` and `.Y` directly stores straight from the FPU (48v68;
  CGameCameraManager::SetRightStickLookAroundVector 0x00697B10).
- Under `FABLE_RETAIL_BASE_OBJECT` (engine/CBaseObject.h), CBaseObject already owns slot 1 (Validate). A creature-action model
  therefore declares only slot 2 `EditUpdate(float)` before PerformAction (slot 3) and FinishAction (slot 4). Adding filler slots
  shifts PerformAction by one.
- `new T` where T has an inline ctor, followed by `return p->Init(sameArgs...)`, reuses the caller's argument block as a tail `jmp`,
  even after operator new (CGraphicFrame::InitialiseAsSurface 0x009FCB20). The `push 1Bh` size proves a pack(1) class of 27 bytes.
- When a non-polymorphic class's first member is a CWideString, its destructor ends `mov ecx,esi ; pop esi ; jmp ~CWideString`.
  Passing an edit box to a `CObserver*` parameter (second base at +4) gives the null-tested `test ; lea edi,[eax+4] ; jmp ;
  xor edi,edi`, evaluated before GetInstance (CVirtualKeyboard::~CVirtualKeyboard 0x00851720).

Review at landing:
- Landed callee spellings proven wrong (NAME_FIXES.tsv), to respell at landing (name-only, bytes unchanged):
  - 0x0088B6F0 in CThingAICreature_OnLevelUnload_00833ba0.cpp should be CAIBrain::OnKill.
  - 0x00594F78 (CLandscapeBackgroundPatch vector deleting dtor) should be `??_GCVirtualKeyboard@@QAEPAXI@Z`, and its callee
    `AutoTinyScalarDestructor_00851720` should be `??1CVirtualKeyboard@@QAE@XZ`.
  - 0x0084CF40 (CEditControlCentre::OnEngineSurveyLMBHeld) should be NCameraModes::CBase::SetRightStickLookAround.
- 0x006925E0 is CCreatureActionBase::GetPercentageComplete (landed as CInputTypeMouseMovementEvent::GetMovementRatio). Four landed callers
  call it GetProgress, GetProgressRatio or GetAnimationFraction (see NAME_FIXES.tsv).
- vland SHARED_HEADER_AVAILABLE warnings (CAIBrain, CPlayerGui, CTCPhysicsBase) are flattened headers without member functions,
  so the local models remain until those headers get a retail branch.
- Local base models next to existing (flattened) engine headers:
  - CThing / CThingCreatureBase / CThingAICreature in 0x00830D80, 0x008B6A90, 0x008D68A0, 0x008CC8A0 and 0x008B8B80.
  - CCombatActionBase in the three creature-action rows.
  - CGameCameraManager in 0x00697B10.
  - The NUISystem UI classes in 0x00851720.
  - CPlayerGui and CTBaseSingletonNoDefaultAlloc in 0x00830D80.
  - CWorld and CMainGameComponent in 0x0092B3A0.
  - See "Header debt" in NOTES.md.
- Four header overlays in hdr/ (all opt-in, default text unchanged): CGraphicFrame.h, CVirtualKeyboard.h, CAIStateGroupBase.h
  (new _MEMBERS sub-branch inside FABLE_RETAIL_CAISTATEGROUPBASE) and CAIStateGroup_ApprenticeHero.h. Agent 706/708 may touch the same files.
- 0x009FCB20 / 0x009FCB60 are byte twins whose relocation targets differ (0x009FC950 vs 0x009FCA40). The catalog had one name for both.
- 0x0068D590 is an uncatalogued `__fastcall` function that calls 0x00697B10 (from 0x0068D5AE).
- No row needed more than 4 grades.

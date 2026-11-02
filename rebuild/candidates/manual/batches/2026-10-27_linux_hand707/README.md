# Chunk 707 (NA050) -- 10 of 10 byte-exact (RELOCATION_MATCH), four headers given opt-in retail branches (2026-11-02)

Rows (`verify.log`: WINS 10, RELOCATION_MATCH + PASS; 17 probes in all; no asm, volatile or flags): CCreatureAction_RiseUp::FinishAction
0x008B6A90 (vtable 0x01268C9C slot 4; byte twin of chunk 706's GoUnderground::FinishAction 0x008B6A20, two symbols), CCreatureAction_ScreamerDie::
FrameUpdate 0x008B8B80, CCreatureAction_BalverineLungeAttack::FinishAction 0x008CC8A0, NCreatureMode::CModeDrained::OnActivate 0x008D68A0 (catalog
lacked the namespace), CAIStateGroup_SkillPractice::Update 0x0092B3A0 (guard-clause early return), CGraphicFrame::InitialiseAsSurface 0x009FCB20 and
its byte twin CGraphicFrame::InitialiseAsSurfaceSection 0x009FCB60 (the catalog gave both the first name; different callees 0x009FC950 / 0x009FCA40),
CGameCameraManager::SetRightStickLookAroundVector 0x00697B10 (a C2DVector temporary), CThingAICreature::OnKill 0x00830D80 and
CVirtualKeyboard::~CVirtualKeyboard 0x00851720 (non-polymorphic; ends in ~CWideString).

Headers (hdr/README.md): opt-in branches FABLE_RETAIL_CGRAPHICFRAME (engine/CGraphicFrame.h), FABLE_RETAIL_CVIRTUALKEYBOARD (engine/CVirtualKeyboard.h),
FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO (engine/CAIStateGroup_ApprenticeHero.h) and FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS nested in the existing
FABLE_RETAIL_CAISTATEGROUPBASE branch. Coordinator check: `cpp` of each header under the default macros and under FABLE_RETAIL_CAISTATEGROUPBASE
gives the same text as before apart from the __LINE__-numbered static-assert typedef names; all 179 landed includers (and the 10 rows) rechecked:
2 MATCH + 177 RELOCATION_MATCH, all PASS (includer_recheck_grades.tsv).

Second spellings at landing (owner 2026-11-02 rule): 0x00851720 had the stand-in `?AutoTinyScalarDestructor_00851720@@YIXPAX@Z` in the landed
0x00594F78 row; that caller was respelled in this commit to `??1CVirtualKeyboard@@QAE@XZ` through engine/CVirtualKeyboard.h (binding updated,
recheck RELOCATION_MATCH + PASS). 0x006925A0 `?HasPerformedAction@CCreatureActionBase@@QBE_NXZ` (both PDBs) joins eight pre-existing stand-in
spellings of that address; queued as a PRIORITY row in NAME_FIXES_QUEUE.tsv (c707) with all eight. The other NAME_FIXES rows (0x009FCB60,
0x008D68A0, 0x0088B6F0 CAIBrain::OnKill, 0x00594F78 ??_GCVirtualKeyboard, 0x0084CF40 NCameraModes::CBase::SetRightStickLookAround, 0x006925E0
CCreatureActionBase::GetPercentageComplete) are queued as c707; labels in ghidra_out/labels_chunk707_2026-10-27.tsv.

Header debt and local models: NOTES.md. Lessons: LESSONS.md (folded into GOTCHAS).

# Chunk 713 (NA056) -- 10 of 10 byte-exact (RELOCATION_MATCH), three opt-in branches, one retail-branch declaration (2026-11-02)

Rows (`verify.log`: WINS 10, all PASS; all ten labels wrong): CFrameRateSmoother::Uninitialise 0x0049BA20, CTCCreatureOpinionOfHero::
IsFactionEnemyToHeroOverriddenOnOpinion 0x00681850 (a 12-way jump table; retail's `sub eax,6` prologue comes only from a named unsigned index),
~CTCQuestCard 0x00702A50, NCreatureMode::~CModeJackOfBladesFlash 0x008E3720 and ~CModeScorpionKingTailPlunge 0x008E37C0 (byte twins named from
their own vtables), the STLport `vector<pair<CCharString,CScriptInfo>>::_M_clear` 0x00CB6E60, and the compiler-generated village-script dtors
NScript::~CV_AmbushScamScript 0x00E32310, ~CV_AssassinAttacksScript 0x00E398A0, ~CV_BeardyBaldyScript 0x00E542B0 and ~CV_SingingStonesScript
0x00ED39F0 (vtable -> ctor -> factory -> tag). vland cannot check 0x00681850's relocations (multi-block TU); the agent checked the table entries by
hand with `objdump -r`.

Headers (hdr/README.md): opt-in FABLE_RETAIL_CTCQUESTCARD (engine/CTCQuestCard.h: : CTCBase, vtable 0x01264C44, sizeof 0x34),
FABLE_RETAIL_CTCCREATUREOPINIONOFHERO (engine/CTCCreatureOpinionOfHero.h: : CTCBase, eight virtual slots, containers as opaque storage at their
retail offsets, sizeof 0x350) and FABLE_RETAIL_CLEVELSCRIPTBASE (engine/CLevelScriptBase.h: retail's ~CLevelScriptBase is compiler-generated, so
implicit level-script dtors tail-jump to ~CScriptBase 0x00CBD510), plus `class CWorld;` + protected `const CWorld& PeekWorld() const;` (0x00686A80,
IBE in both PDBs) merged into the FABLE_RETAIL_CTCBASE branch of engine/CTCBase.h beside chunk 714's PeekWorldMap. Default text of each
unchanged under `cpp`.

Second spellings at landing: the landed CMainGameComponent::Shutdown 0x004175E5 modelled its +0x16178 member as `CEditWorld EditWorld` with
`Shutdown()` 0x0049BA20; Ego_r's CMainGameComponent has CFrameRateSmoother FrameRateSmoother at 90488 = 0x16178, so it was respelled
`?Uninitialise@CFrameRateSmoother@@QAEXXZ` in this commit (binding updated). Recheck of the 68 landed FABLE_RETAIL_CTCBASE / new-header includers
plus 0x004175E5: 69 of 69 = 4 MATCH + 65 RELOCATION_MATCH, all PASS (includer_recheck_grades.tsv). Queued PRIORITY (NAME_FIXES_QUEUE.tsv, c713):
0x00CB6E60 bound through 0x00CB6FD0's ScriptVector-namespace model, GetThingAttitudeToPlayer 0x00701840 under seven caller forms, the four
village-script ??_G rows named CLandscapeBackgroundPatch_vectordeletingdestructor, and CInitBaseClass::IsValid 0x0099A330 bound as
CPlayer::IsInitialised. Header debt (NOTES.md): flipping the CLevelScriptBase default needs a 1,062-includer recheck; CWorld's retail branch
flattens POpinionReactionManager; the opinion types have no shared header. The agent's 20 NAME_FIXES rows are queued as c713; labels in
ghidra_out/labels_chunk713_2026-10-27.tsv.

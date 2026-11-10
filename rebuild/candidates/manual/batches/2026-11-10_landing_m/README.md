# 2026-11-10 landing m -- Codex pass 15 return (20 reserved rows), 17 landed

Source: the ChatGPT Codex return for the 20 rows reserved in `pending/2026-11-09_codex_pass15/RESERVED.tsv` (5 never-attempted, 15
parked near misses). Notes, per-row naming evidence and probes: `pending/2026-11-09_codex_pass15/return_2026-11-10/` (history/ and
verification/ JSON not committed).

Coordinator checks before landing:
- Header overlay: 72 headers (31 new). Two (engine/CPlayerGui.h, engine/CPlayerGuiDef.h) had changed in the repo since the package was
  cut (chunk 898's opt-ins); both were three-way merged against the shipped base, clean. hdrcheck2: 39 default=same; CTCInventoryClothing.h
  reports DIFF only because the class head's `{` moved to its own line (token-identical apart from generated static-assert names).
- One physical include tree (the overlay applied to rebuild/include): as a second include path fable_string.h is defined twice
  (C2011) and two rows fail to compile.
- Fixes: two tests' bare `#define FABLE_VECTOR_INLINE __forceinline` -> plain `inline` (0x00A22CA0, 0x00A56670); house line 1 on
  seven rows (0x00B801F0, 0x006D6CB0, 0x006D68D0, 0x0074E140, 0x00868E50, 0x0099BB80, 0x00C3D030, 0x00BA48F0); 0x00868E50's
  anonymous-namespace Leaf cut at `?A0x` (house convention: the hash after it is the TU path's); four comment spacing slips.
- vland --land, no overlay: 17 wins (2 MATCH: 0x00A358F0, 0x00C3D030; 15 RELOCATION_MATCH), tests PASS. bindings_multi_check 1.

Landed: CTCInventoryClothing::CalculateArmorValues 0x005B4EBA, CMainGameComponent::InitSound 0x00417A58,
CTCInventoryAbilities::ConstructScreen 0x005F519F, vector<CTVertexSeaBackground>::_M_fill_insert 0x00B801F0,
CTCHeroExperience::IsToAutoBlock 0x006D6CB0 / IsBlockRecoilAnimToConserveAnimationHandedness 0x006D68D0 /
AddSuccessfulMultistrikeHit 0x006D8830, CTCContainerRewardHero::GeneratePopoutVectorFromVars 0x0074E140,
GFGetHermiteSplinePosition 0x00A56670, CWideString::SprintfDynamic 0x0099BB80, C3DMeshObject::GetPositionMatrixFromRHSet 0x00A358F0,
CRightHandedSet::GetAngleXZ 0x00A22CA0, CCodaContext::GetResourceCounts 0x00C3D030 (descriptive GameCODA name),
NThingShot::CTCShotDefinitionProjectilePredictive::InitMovement 0x007BC6FA, GetArmourWeightsFromThing 0x00868E50,
Mass::BackplaneFriction 0x008EEBE0, CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksNoMorph 0x00BA48F0.
Parked (3): 0x0064FDB4 / 0x00651273 CQuestInfoElementBar / MiniGameBar::DrawAt, 0x00E5E350 CV_BodyGuardScript::HenchmanCommentary
(sources + FINAL_UNFINISHED_GUIDANCE.md in the return folder).
Names: `ghidra_out/labels_landing_2026-11-10m.tsv`; Codex's 22 NAME_FIXES queued as `land1011m` (to verify with two sources before
applying).

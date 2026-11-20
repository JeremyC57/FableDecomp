# 2026-11-20 landing nf127: PRIORITY slice 9 (41 rows)

Records: `naming/2026-11-20_agentNF127/` (selfcheck 0 problems). 31 entries: 11 done, 4 done-already, 3 rejected, 1 deferred (q5020),
1 held (q3965, below), 11 open (hand-back on context; NF131 took them).

## Rows (41): 6 MATCH, 35 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- Shader family: 0x00987E50 CPixelShader::operator=, 0x00988630 CShaderRenderManager::SetTextureTransformCount, 0x00988730
  CShaderRenderManager::SetUVCompressionVertexShaderConstant (both re-authored on engine/CShaderRenderManager.h), 0x00989A60 / 0x0098A7F0
  leaf respells, 0x0098A8E0 UpdateBoneMatrices -> UpdateBoneTransform; their binders (ApplyVertexShader, SetBoneTransform,
  SetUserVertexShaderConstant, UpdateBoneTransform) respelled in place.
- AI state groups: GetPWorldSeed / PeekThingSearchTools / FindAlertEvent / FindFreeBanditBed / AcquireSeller / IsInWorkHours binders respelled;
  0x00946380 renamed to CAIStateGroup_PrepareMeal::GetFreeCookingPot; 0x00910A90 ported onto engine/CThingSearchTools.h.
- 0x009A5A50: SoundInterface stand-in removed, binds CASoundManager::EnableMusic; test rewritten on engine/CASoundManager.h.
- Files renamed with their self renames (build_candidates.ps1 updated).

## Held (2): 0x0090AC50 + 0x0090B8E0
Their object defines the real STLport `std::list<LHNetworkLib::LSocket*>` members, which clash with the STLport inline every includer
emits. Re-port debt; queued in NAME_FIXES_QUEUE as held (advisor decision, owner delegated).

## Headers (add-only opt-ins, default path token-identical; assert_names_check 0 mismatches)
CThingSearchTools.h FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY_CASTARG, CAIStateGroupBase.h FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED,
CAIStateGroup_PrepareMeal.h FABLE_RETAIL_CAISTATEGROUP_PREPAREMEAL_GETFREECOOKINGPOT.

## Advisor decisions (owner delegated)
- In-place respells accepted for 0x0091A390 and 0x00946F30 (the template CAIStateGroup_WorkBase the row already defined at HEAD; token
  check passes, the object defines no symbol owned by another row).
- 0x00910A90: the GetAllLeafThings instance was defined in both the HEAD and the landing object (pre-existing, not new).
- CIsSeatWithSpaceOwnedBy local model accepted under the narrow rule: no header anywhere, siblings 0x009109B0 / 0x009104D0 / 0x00945E70
  model it the same way, no base.
- q4308..q4310 rejected: already bound to EnableMusic / EnableSound / EnableAtmos.

## Checks
bindings_multi_check 1; funclet_includes_check 0 includers of a changed row. 813 rows rechecked: 811 good, 2 bad. The two bad rows are
pre-existing: 0x00BE6F70 CLandscapeBackgroundPatch::Render DIFFER(198v2654) and 0x00BF4570 CEngineLandscapePatch::RenderForeground
DIFFER(316v4431). Both are old no-Leaf rows (not NF127 rows) and grade the same with HEAD headers (`logs/differ_head_check.txt`).
No-new-split gate 0. Table spelling sets grew only by witness (p_before / p_land pairs SAME): 0x01396E34 0 -> 1, 0x01436E8C 2 -> 3. Shrank:
0x008FCFD0 11 -> 6, 0x008FD000 8 -> 7, 0x00988020 3 -> 1, and 0x0090F110, 0x0091A6F0, 0x0092C8A0, 0x00989A60, 0x0098A7F0, 0x0098A8E0, 0x009915E0.
NAME_FIXES queued: 5 from the agent (0x0093D3A0, 0x0093DB10, 0x0093DC10, 0x0091B170, 0x0091B010) plus the held pair.

# NF166 header overlay (round 10)

Three repo headers are overlaid. Each base copy is in `orig/engine/` and is byte-equal to the repo working tree at hand-back.
Each overlay adds one `#ifdef` opt-in block with a member-specific macro and nothing else. The default path (macro undefined) is
token-identical to the repo header.

| Header | Opt-in macro | Adds | Rows that define it |
|---|---|---|---|
| engine/CParticleProgram.h | `FABLE_RETAIL_CPARTICLEPROGRAM_UPDATE` | `void Update();` (public; 0x00AE34D0) | 0x00AE34D0 |
| engine/CQuestManager.h | `FABLE_RETAIL_CQUESTMANAGER_FAILALLACTIVEQUESTS` | `void FailAllActiveQuests();` (0x004B2AD0) | 0x00891520 |
| engine/CGameTimeManager.h | `FABLE_RETAIL_CGAMETIMEMANAGER_SETTIMEOFDAY` | `void SetTimeOfDay(float time);` (narrow `FABLE_RETAIL_CGAMETIMEMANAGER` model; 0x006BB810) | 0x006BB810, 0x0088FDC0 |

Placement:
- CParticleProgram.h: right after the `FABLE_RETAIL_CPARTICLEPROGRAM_PASS22_CLEARGROUPS` block, in the public section.
- CQuestManager.h: right after the `FABLE_RETAIL_CQUESTMANAGER_PREPAREQUESTSWHENFINALQUESTISCOMPLETED` block, in the public section.
- CGameTimeManager.h: right after the `FABLE_RETAIL_CGAMETIMEMANAGER_ISDAY` block, in the narrow model's public section. The
  `_FULL` model already declares SetTimeOfDay.

Declarations only. No layout change and no `FABLE_FORCEINLINE`.

pin_asserts.py ran on each header and pinned 0 asserts (all three use `FABLE_STATIC_ASSERT_AT(<tag>, ...)`). No assert line moved.

Header users (`grep -rlE 'engine/<header>' rebuild/src/compiled`): CParticleProgram.h 31, CQuestManager.h 134, CGameTimeManager.h 31.
No landed row or shared header defines any of the three macros, so these opt-ins change none of them.

Sibling overlays read before adding (read-only):
- NF164 has only CCreatureAction_ReleasePhysicalShieldSpell.h.
- NF165 has CCodaObjectBase.h, CEAXListener.h, CodaMemory.h, DSound3D.h, EAX40.h and EAXNativeTypes.h.
- Neither touches these three headers.

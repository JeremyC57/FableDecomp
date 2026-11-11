# Chunk Q8 header overlay (grade with `FABLE_HDR_OVERLAY=$A/hdr`)

Every file is a copy of the committed `rebuild/include/engine/<name>.h` (HEAD, 2026-11-11) with one new opt-in block added.
The default path of each header is byte-identical to the repo copy. Scripts that made the edits: `tools/patch_*.py`.

| header | new opt-in | used by | what it adds |
|---|---|---|---|
| engine/C3DVector.h | `FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED` | 0x006A4FB0 (src) | a GFFastOneOverSqrt body with the Newton step fully grouped, `((3.0f-((r*r)*value))*r)*0.5f`. Same instructions; the extra PAREN tuples move the scheduling-region cut so that the epilogue pops interleave with the last Normalise multiplies as retail does (0x006A51F1..0x006A51F6). It is an `#ifdef` ahead of the existing `FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN` (now `#elif`). |
| engine/CThingPhysical.h | `FABLE_RETAIL_CTHINGPHYSICAL_FIRSTCOLLISION` | 0x006A4FB0 (src) | declares Ego_r `?GetFirstCollision@CThingPhysical@@QBE_NMABVC3DVector@@0AAV2@1@Z` (public const). |
| engine/CPhysicsMeshObject.h | `FABLE_RETAIL_CPHYSICSMESHOBJECT_SPHEREINTERSECTION` | 0x006A4FB0 (src) | declares `GetSphereIntersectionFromTo(float,const C3DVector&,const C3DVector&,CMeshIntersectionData&) const` (0x00A120D0). |
| engine/CAIStateGroup_RangedCombatUsePreferredFirePoint.h | `FABLE_RETAIL_CAISTATEGROUP_RANGEDCOMBATUSEPREFERREDFIREPOINT_STILLRUNNABLE` (nested in the retail branch) | 0x0095D0E0 (src) | declares Ego_r `?IsStillRunnable@...@@IAE_NXZ` (protected, non-virtual). |
| engine/C3DBoundingSphere.h | `FABLE_RETAIL_C3DBOUNDINGSPHERE_FASTBUILD` | 0x00ADD690 (differ) | declares `FastBuildFromPoints(const C3DVector*, long)`. |
| engine/CTCHeroExperience.h | `FABLE_RETAIL_CTCHEROEXPERIENCE_UPGRADESTAT` | 0x006D8980 (differ) | declares UpgradeTrainableStat (public) and the private UpdateAbilitiesForStat / OnStatLevelChange. |
| engine/CTCHeroStats.h | `FABLE_RETAIL_CTCHEROSTATS_AGE` (inside the `FABLE_RETAIL_CTCHEROSTATS_SERIALISE` layout) | 0x006D8980 (differ) | declares `SetAge(float)` (0x0057B05F) and the inline FableWin `GetAge() const`. |

Only the first four are needed by the src/ rows.

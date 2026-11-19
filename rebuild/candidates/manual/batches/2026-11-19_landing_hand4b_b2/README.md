# Landing hand4b b2 (2026-11-19): two CThingSearchTools rough-area instances

vland --land, strict headers: RELOCATION_MATCH + PASS; no new headers, no flags.
- 0x0087A7C0: IDENTIFIED as ??$GetAllThingsInRoughArea<const CThingCreatureBase, vector<const CThing*>, CThingFilter_IsNot> (Ego_r 0x00864EC0 /
  FableWin 0x029CECB0, cm_projectile_weapon.obj; retail proof: overflow callee 0x0049C370 = vector<const CThing*>, conversion temporary stored
  after the filter call). The catalog label GetPBestThingInRoughArea<..> was propagated. Plain shared ROUGHAREA_EGOD body.
- 0x009871C0 GetPFirstThingInRoughArea<CFilter_Bind4<..>>: Ego_d's own record of the instance over the declaration-only FIRSTROUGH opt-in; first grade.
Landed slug of 0x0087A7C0 dropped the function name (respell queued, `NAME_FIXES.tsv`).

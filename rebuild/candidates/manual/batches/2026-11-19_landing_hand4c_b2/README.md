# Landing hand4c b2 (2026-11-19): GetAllThingsInRoughArea<CThing, list<CThing*>, CFilter_Bind2<CThingFilter_OverlappingCircle, CWorldMap::CThingFilterBase> > (0x0063FD40)

vland --land, strict headers: RELOCATION_MATCH + PASS; no flags. Identity from Ego_r (world_map_tools.obj) + FableWin; the catalog label belongs to
0x0066A1E0. The Ego_d rough-area body + the circle filter's real body as new opt-in FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGCIRCLE_BODY
(engine/CThingFilter_OverlappingCircle.h, FableWin text, Ego_d local) + existing SQUARED2D_ONE_EXPRESSION_PASS23. All 4 header users rechecked.
Coordinator decision on the helper self-grade: the same body at its own out-of-line address 0x0063EF50 is DIFFER(95v95), only the square order at
0x0063EF7E..84 -- retail itself splits (out-of-line copies 0x0063EF50 / 0x0063F2C0 square dx first; expanded copies 0x0063FD40 / 0x0066A1E0 dy first)
and every text flips both together, so it is c2 context, not a wrong body. Accepted; 0x0063EF50 stays a separate open item.

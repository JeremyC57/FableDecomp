# Landing hand3a (2026-11-19): 9 of 10 parked same-size rows

vland --land, strict headers: RELOCATION_MATCH + PASS for all 9 (`logs/vland_land.log`). Header overlays merged as additive opt-ins
(`hdr_README.md`): CIVCountedPointer.h FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_EARLY_RETURN (FableWin operator= 0x01FAD9C0),
NEntityEvents_EventFilters.h FABLE_RETAIL_NENTITYEVENTS_CISHITEVENTHITBY_LOCAL (Ego_d locals pevent / phit_by, FableWin 0x0250E970),
NShape_CClosedShapeBase.h FABLE_RETAIL_NSHAPE_CSHAPE_UPDATESELECTEDPOINT and CTCShop.h FABLE_RETAIL_CTCSHOP_ADDSALEITEMPLACES (declarations).

| row | closed by |
|---|---|
| 0x004C10E0 CTCGraphicAppearance::GetStaticFocalPos | two early-return arms, each setting CachedStaticFocalPos first |
| 0x005E1210 std::__adjust_heap<pair<EAnimComponent,CIVCountedPointer<..>>*,..> (/Gr) | CIVCountedPointer operator= with InternalGetObject compare + early return |
| 0x00738A80 CTCEntityEvents::GetEvent<const CEventHitBy, ..> | CIsHitEventHitBy inline body with its named local (c2 priorities Data 80 / Info 78) |
| 0x00746DB0 NStealth::GetStabPosOnThing | shared headers, Ego_d locals in FableWin construction order |
| 0x007621C0 NShape::CShape::UpdateSelectedPoint | FableWin body structure + STLP_FIND_RANDOM_DISPATCH / NOINLINE_BODY |
| 0x00774C60 CTCShop::StockDisplay | same-TU real bodies of GetSaleItemsToPlace / GetFreeSaleItemPlaces (both grade RELOCATION_MATCH there) |
| 0x00789CE0 anon::GetDamageMultiplierFromThingAtBuildUpTime | FableWin order, Ego_d locals, grouped GFSqr, no (float) cast on comp1 |
| 0x0080DC90 anon::CThingFilter_IsEvadeExplosion::operator() | same-TU real body of IsTravellingAway (0x0080C140) defined first; without it DIFFER(245v242) |
| 0x00C6EBE0 CVtable_012bedb8::RebuildGain | same-TU ComputeGainRamp body defined first so VC7.1 inlines it; retail_asm via libpk/FtoL.h (FABLE_PORTABLE twin) |

Lesson (LESSONS.md, GOTCHAS): three long-parked rows closed only once the same-TU callee's real body was visible to c2.

**0x006B5890 held -- advisor decision (owner delegated):** it matches only through a FITTED opt-in taking pair operator<'s right operand by
value (STLport 4.6.2 and the donor mangled name are const&). Not landed and its fable_containers.h overlay not merged; reserved for the next
parked-row author with ~10 grades to test sourced readings (val type differing from the element type -> converting-ctor temporary; the
caller's construction of val; sibling pair<..,float> comparisons). If none matches, land the fitted text as Inferred with its note (006E89B0
precedent). Queued: re-land landed 0x00774B40 on the shared-header text with its own test. Name / label fixes: `NAME_FIXES.tsv`, `LABELS.tsv`.

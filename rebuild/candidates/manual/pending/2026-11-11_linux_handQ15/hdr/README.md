# agentQ15 header overlay (grade with FABLE_HDR_OVERLAY=$A/hdr)

Every change is behind a new `#ifdef FABLE_RETAIL_*` opt-in; the default path of each header is byte-identical to HEAD.

| header | opt-in | what | row |
|---|---|---|---|
| engine/CTCSleep.h | FABLE_RETAIL_CTCSLEEP_FASTTRACK | `GetFastTrack()` decl (0x0092C3B0) + inline `GetIsReadyToSleep()` (Ego_r.pdb names) | 0x0092E130 |
| engine/CAIStateGroup_MinionSleep.h | FABLE_RETAIL_CAISTATEGROUP_MINIONSLEEP | retail class deriving CAIStateGroup_GoToBed (0x64, WasCarryingWeapon +0x60) | 0x0092E130 |
| engine/NEntityEvents_CEventHitBy.h | FABLE_RETAIL_NENTITYEVENTS_CEVENTHITBY_TYPE | inline `StatGetType()` (EVENT_HIT_BY = 1) + `GetType()` slot decl (Ego_r / FableWin names); needs _CEVENTBASE_VSLOTS3 | 0x00738A80 |
| engine/NEntityEvents_EventFilters.h | FABLE_RETAIL_NENTITYEVENTS_CISHITEVENTHITBY | new filter class CIsHitEventHitBy (Ego_r layout HitBy +0; ctor `(const CThing&)` and inline operator() from the name set) | 0x00738A80 |
| engine/COptimisedPrimitive.h | FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_METHODS | declarations of GetFaceNormal, FindTextureTangent, FindFaceTextureTangents, PerformContraction, DetectFoldOver, PopFrontContraction, UpdateCost, Optimise (Ego_r name set) | 0x00AC7CB0, 0x00AC7300, 0x00ACD960 |
| engine/QuadricOptimiserTypes.h | FABLE_RETAIL_QUADRIC_COPTIMISEDFACE | COptimisedFace (Ego_r, 0x48) | 0x00AC7CB0, 0x00AC7300 |
| engine/CVectorMap.h | FABLE_RETAIL_CVECTORMAP_GETKEYLIST | `GetKeyList(std::vector<K>&) const` decl (Ego_r name; 0x0086D240) | 0x0086B9D0 |
| engine/CRandomAppearanceMorph.h | FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_SKELETAL | GetRandomSkeletalMorph / AddSkeletalMorphToMorphSet decls (Ego_r, protected const) | 0x0086B9D0 |
| engine/CThingFilter_IsValidGossipee.h | (new header, no default path) | CThingFilter_IsValidGossipee from Ego_r layout + name set | 0x0090C900 |
| engine/CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_CURFEW | IsCurfewTime() const decl (Ego_r; 0x00658D5D) in the FABLE_RETAIL_CTCVILLAGE class | 0x0090C900 |
| engine/NTCScriptedControl_CActionFollowCreature.h | (new header) | CActionFollowCreature : CTCScriptedControl::CActionBase (Ego_r layout, name set) | 0x00713D50 |
| engine/CTCStealth.h | FABLE_RETAIL_CTCSTEALTH_SETSNEAKING | inline StatGetInterfaceType (TCI_STEALTH) and SetSneaking (FableWin names; expanded in retail) | 0x00713D50 |

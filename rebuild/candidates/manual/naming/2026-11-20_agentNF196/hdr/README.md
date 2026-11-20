# NF196 header overlay

Four shared headers carry NF196 opt-ins. The originals are in `../orig/engine/`. Every addition is an `#ifdef FABLE_RETAIL_<CLASS>_<MEMBER>`
declaration (one inline body: IsGuard). None changes a layout. With no NF196 macro defined, each header's default path is token-identical to
HEAD; the one exception is the line pin that pin_asserts.py wrote. No landed row in rebuild/ defines any of these macros, so the landed
includers (counted with grep below) compile the default path unchanged:
CTCVillage.h 276, CTCCreatureOpinionOfHero.h 117, CTCDummyVillager.h 6, CThingCreatureBase.h 1008.

| Header | Opt-in macro | Declaration | Used by row |
|---|---|---|---|
| CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_TRYTORESPAWNDEFNAMED | `CThingAICreature* TryToRespawnDefNamed(const CCharString&, const C3DVector&);` | 0x0065B349 |
| CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_ENABLEGUARDS | `void EnableGuards(bool);` | 0x0065C45A |
| CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_ISREPORTEDORUNREPORTEDCRIMEKNOWN | `bool IsReportedOrUnreportedCrimeKnown();` | 0x0065BFFA |
| CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_DETACHFROMDUMMYVILLAGER (the existing guard `#ifdef FABLE_RETAIL_CTCWIFE_MARRY` became `#if defined(FABLE_RETAIL_CTCWIFE_MARRY) \|\| defined(FABLE_RETAIL_CTCVILLAGE_DETACHFROMDUMMYVILLAGER)`; both defaults skip the declaration) | `void DetachFromDummyVillager(CThing*);` | 0x0065B2C4 |
| CTCCreatureOpinionOfHero.h | FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DECRIMINALISEALLKNOWNCRIMES | `void DecriminaliseAllKnownCrimes();` | 0x0065C45A |
| CTCCreatureOpinionOfHero.h | FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_FINDMOSTRECENTDEED_FILTER | `template<class Filter> const COpinionDeed* FindMostRecentDeed(const Filter&,long*,bool) const;` | 0x0065BFFA |
| CTCDummyVillager.h | FABLE_RETAIL_CTCDUMMYVILLAGER_ISGUARD_INLINE | `public: bool IsGuard() { return Guard; }` (the `#else` keeps the HEAD out-of-line declaration `public: bool IsGuard();`) | 0x0065C45A |
| CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_FLAT_GETRANDOMANIMFROMNAME (flat branch, before FLAT_ANIMSPEED) | `const class CAnimationEntry& GetRandomAnimFromName(const CCharString& name, long index) const;` | 0x0093C6D0 |

Both CTCCreatureOpinionOfHero.h opt-ins sit inside the retail-branch class, after the MINION_TRESPASS block.

pin_asserts.py results (run after each edit):
- CTCVillage.h: pinned 0 (all asserts were already `_AT`).
- CTCCreatureOpinionOfHero.h: pinned 0.
- CThingCreatureBase.h: pinned 0.
- CTCDummyVillager.h: pinned 1. `FABLE_STATIC_ASSERT(sizeof(CTCDummyVillager)==0x50)` became `FABLE_STATIC_ASSERT_AT(45, ...)`, pinned to its HEAD line 45. The assert itself did not move.

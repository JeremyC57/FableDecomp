# Chunk 794 header overlay (hdr/engine)

Every change is an opt-in (`#ifdef FABLE_RETAIL_...`) or a new header; the default path of every existing repo header is byte-for-byte
the repo text, except one forward declaration (CThingAICreature.h, below). So no landed includer can change bytes: none needed a
recheck. The coordinator should still run its usual includer recheck at landing. Evidence for each opt-in is the comment above it in
the header (PDB spelling + the retail bytes that read it).

| Header | Opt-in macro(s) | What it adds | Evidence | Used by |
|---|---|---|---|---|
| C3DVector.h | FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ | GetSquaredMagnitude as `(X*X+Y*Y+Z*Z)` (the retail branch keeps `(X*X+Z*Z+Y*Y)`) | 0x0079ACC0 inlined Normalise loads Z, Y, X (0x0079AE93 / 0x0079AEA0 / 0x0079AEAA); the X,Z,Y spelling loads Y, Z, X in that TU | 0079acc0 |
| CDefString.h | FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING | GetString forced inline (same body) | 0x00703F4B..58 expand it in place; plain inline calls 0x00415DD0 | 007039a0 |
| CGameDefinitionManager.h | FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF | PHeroStatsDef at +0xDC (descriptive name: no PDB layout for this class) | OnHit `mov edx,[eax+0DCh]`; landed 0x0057DE14 reads the same slot | 007039a0 |
| CHitParameters.h | FABLE_RETAIL_CHITPARAMETERS_BLOCKABLE, FABLE_RETAIL_CHITPARAMETERS_CTOR | IsBlockable() inline; ctor / dtor / SetThingHitting declarations (0x007A3BC0 / 0x00789358 / 0x008685F0) | FableWin / Ego_r spellings; landed callers bind the same addresses | 007039a0, 007c4430 (parked) |
| CMatrix3x4.h | FABLE_RETAIL_CMATRIX3X4_POSTMULTIPLY | forceinline PostMultiply (this = this * m, the landed 0x00C1CF20 sums) and SetInverseOfUniformScaleRotationTranslation (returns the scale) | FableWin ?PostMultiply / ?SetInverseOfUniformScaleRotationTranslation@CMatrix3x4; 0x007DE710 expands both (`fdivr [1.0f]`, the 12-dword `rep movsd` copy) | 007de710 (parked) |
| CRightHandedSet.h | FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE | the (up, forward) ctor body visible | CreateMinion builds the set in place (COMDAT 0x00487050) | 0079acc0, 007de710 (parked) |
| CTCBuyableHouse.h | FABLE_RETAIL_CTCBUYABLEHOUSE | real base + members to Rented, inline accessors | RTTI; OnHit reads +0x11 / +0x12 | 007039a0 |
| CTCCreatureOpinionOfHero.h | FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM | StatGetInterfaceType, KnowsOfDeed(long, EOpinionDeedType, bool) 0x00681AE0, InformOfDeed 0x00682810 | Ego_r spellings; the landed 0x00683650 binds 0x00681AE0 | 006c6040 (parked) |
| CTCDoor.h | FABLE_RETAIL_CTCDOOR_VANDALISM | PostVandalismCrimeIfDestroyed 0x007305F0 + interface key | Ego_r; landed row | 007039a0 |
| CTCHeroExperience.h | FABLE_RETAIL_CTCHEROEXPERIENCE_BREAKDOORS | IsAbleToBreakDownDoors 0x006D6370 | Ego_r / FableWin; landed row | 007039a0 |
| CTCHeroOpinionDeedLog.h | FABLE_RETAIL_CTCHEROOPINIONDEEDLOG (+ nested _DEEDS) | NEW header (no repo copy): PostDeed 0x006C6990, InformRelevantThingsOfDeed; _DEEDS: PeekPDeedEffectsDef 0x006C5630, GetDeedFromHandle 0x006C58F0, OpinionDeedMap +0x0C | RTTI base CTCBase; Ego_r / FableWin spellings; retail `lea esi,[edi+0Ch]` + find 0x006C7830 | 007039a0, 006c6040 (parked) |
| CTCHeroStats.h | FABLE_RETAIL_CTCHEROSTATS_MORALITY | Morality at retail +0x28 (public), GetMorality, SetMorality 0x0057C2E6 | Ego_r member; OnHit `[esi+28h]` | 007039a0 |
| CTCShop.h | FABLE_RETAIL_CTCSHOP_SHOPKEEPER | GetShopKeeper inline (+0x10) + interface key | FableWin; `lea ebp,[esi+10h]` | 007039a0 |
| CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_LIMBO | GetVillageLimbo() returning the +0xB3 byte | FableWin ?GetVillageLimbo; `mov cl,[eax+0B3h]` | 007039a0 |
| CThing.h | FABLE_RETAIL_CTHING_BULLETTIMEOVERRIDE | SetIsAffectedByBulletTimeOverride inline | FableWin; the +0x93 bit RMW | 0079acc0 |
| CThingAICreature.h | FABLE_RETAIL_CTHINGAICREATURE_SAVE (+ nested _WORKBUILDING), FABLE_RETAIL_CTHINGAICREATURE_SETNEWBRAIN | Save override + the relation members / OverridingBrainName at their retail offsets; GetWorkBuilding (body in includer); SetNewBrain 0x00833010 | Ego_r / FableWin; Save reads +0x168..+0x190 | 008304a0, 0079acc0, 006c6040 (parked) |
| CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_FLAGSETTERS, FABLE_RETAIL_CTHINGCREATUREBASE_SAVE | three inline +0x15C flag setters; Save declaration (0x00661A60) | FableWin / Ego_r | 0079acc0, 008304a0 |
| CThingGameObject.h | FABLE_RETAIL_CTHINGGAMEOBJECT_COMBOMULTIPLIER | SetAddToComboMultiplierOnHit 0x008389A0 | Ego_r | 0079acc0 |
| CThingObject.h | FABLE_RETAIL_CTHINGOBJECT_ONHIT | OnHit protected virtual declaration | Ego_r / FableWin | 007039a0 |
| NGraphicAppearance_CTCGraphicAppearance.h | FABLE_RETAIL_CTCGRAPHICAPPEARANCE_TINT, FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT | HasDummyObject 0x004BE2C0, SetTintColour 0x004C00B0; GetDummyObject 0x004C08C0 | Ego_r spellings; retail pushes match | 0079acc0, 007de710 (parked) |
| NThingShot_CTCShotDefinitionBase.h | FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE | the first two virtuals after CTCBase's 28 (OnCollision slot 28, OnHitByMelee slot 29) + MS forward enum ECollideType | vtable family 0x0126E1A4; Ego_r spellings | 007039a0 |

Default-path change (no bytes): CThingAICreature.h adds `class CBrainDef;` to its forward-declaration line (needed by the SETNEWBRAIN
declaration; a forward declaration cannot change any includer's code).

Includers rechecked: none needed (no default path changed). The three matched rows were regraded with this final overlay
(RELOCATION_MATCH each, relocation identity checked symbol by symbol against retail call/data targets).

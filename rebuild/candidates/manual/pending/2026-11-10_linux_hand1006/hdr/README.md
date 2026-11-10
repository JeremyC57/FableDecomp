# agent1006 header overlay (chunk 1006)

Every grade in this hand-back ran with `FABLE_HDR_OVERLAY=$A/hdr`. Each changed header is a copy of HEAD's
`rebuild/include/engine/<file>` with additions only: the default path (no new `FABLE_RETAIL_*` define) stays byte-identical.
Only row 0x005B3E4E (CTCInventoryClothing::GlobalUpdate) needs the overlay; the other three src rows build against HEAD.
NInventory_CTCInventoryBase.h was rebased on the current HEAD copy (which already has chunk 1007's HIGHLIGHTEDITEM /
CAPTUREBACKBUFFER opt-ins), so nothing of 1007's is dropped.

## Changed headers (opt-in blocks)
| header | opt-in | adds |
|---|---|---|
| CAnimInit.h | FABLE_RETAIL_CANIMINIT_CTORS | inline CAnimFrameInfo() / (float,float) and CAnimInit() / argument ctors (FableWin.pdb); out-of-line `Init` 0x0070C050 |
| CAnimSet.h | FABLE_RETAIL_CANIMSET_FRAME | inline `GetAnimIndex()` (Anims[0]) and `GetFrame()` |
| CAnimateThing.h | FABLE_RETAIL_CANIMATETHING_MAINANIM | inline `GetMainAnim()` / `PeekMainAnim()`; `Update(const CAnimateThingUpdate&)` 0x0070E8B0; includes CAnimateThingUpdate.h |
| CAnimFilterNoGroup.h | FABLE_RETAIL_CANIMFILTER | polymorphic class over CAnimFilterBase (vtable 0x0122FFDC) |
| CAnimationEntry.h | FABLE_RETAIL_CANIMATIONENTRY_ACCESSORS | `GetAnimateThingFlags() const` 0x005DC390, `GetTransitionTime() const` 0x005DC1D0 |
| CTCHeroAttachableAppearanceModifiers.h | FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_COVERS (needs _LIST) | StatGetInterfaceType (94), `GetAppearanceModifierListWithCoversBodyAreaFlags` 0x00706B50, non-virtual `EditFrameUpdate` 0x00707930 (retail calls it directly) |
| NInventory_CTCInventoryBase.h | FABLE_RETAIL_CTCINVENTORYBASE_GLOBALUPDATE | `virtual void GlobalUpdate(bool)` slot 26, 0x005BDD39 |
| CTCInventoryClothing.h | FABLE_RETAIL_CTCINVENTORYCLOTHING_GLOBALUPDATE | `virtual void GlobalUpdate(bool)` |

## New headers (no default-path impact; nothing included them before)
| header | content |
|---|---|
| CAnimFilterBase.h | abstract filter: virtual `operator()`, `Clone`, inline virtual dtor (vtable 0x0122FFCC) |
| CAnimateThingUpdate.h | TimePassed / DistanceMoved / PercentageComplete (Ego_r.pdb, 0xC bytes) and its three ctors |
| CAnimationSet.h | opaque 0x20-byte layout; `GetAnim(const CCharString&, long, const CAnimFilterBase&) const` 0x005DC340 |
| CAppearanceDef.h | CSubDefClassBase-derived (pack 1), Graphic[0xF], Animation at +0x34, inline StatGetTypeName "CAppearanceDef" |
| CTCAnimationComplex.h | CTCBase-derived, TCI_ANIMATION (90), `GetAnimation()` 0x0070B460 |

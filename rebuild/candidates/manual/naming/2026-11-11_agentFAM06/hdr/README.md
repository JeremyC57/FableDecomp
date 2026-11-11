# agentFAM06 header overlay (`FABLE_HDR_OVERLAY=$S/w22/agentFAM06/hdr`)

Full copies of the current repo headers with `#ifdef` opt-in additions only; with none of the new macros defined each file
compiles exactly as the repo copy (the default and the existing opt-in paths are untouched).

| header | new opt-in | used by | what it adds |
|---|---|---|---|
| engine/C3DAnimationBlendState.h | `FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA` (inside the existing `_TYPED_DATA` block; needs `FABLE_RETAIL_DEF_CLASS_BASE`) | 0x00A3C2D0 | the nested Ego_r.pdb `CSharedData : CIVCountedPointeeBase` (BaseAnimation `CIVCountedPointer<C3DAnimationStateBase>` +0x08, CombinationBlends `CArray<C3DAnimationStateAlpha>` +0x0C, SkeletalMorph `CCountedPointer<const C3DSkeletalMorph>` +0x18; retail 0x20 bytes) and the private `MakeOwnCopyOfData()` (0x00A3C1A0); includes CIVCountedPointeeBase/CArray/CCountedPointer/C3DAnimationStateAlpha under the opt-in only |
| engine/CTCInterestingToVillagers.h | `FABLE_RETAIL_CTCINTERESTINGTOVILLAGERS_IMPLICIT_DTOR` | 0x004DC916 | drops the user-declared `virtual ~CTCInterestingToVillagers()`: retail's destructor is compiler-generated (no vtable store; a user-declared one stores it, graded 38 v 32) |
| engine/CTCInventoryItem.h | `FABLE_RETAIL_CTCINVENTORYITEM_PDEF` (nested in `FABLE_RETAIL_CTCINVENTORYITEM`; needs `FABLE_RETAIL_DEF_CLASS_BASE`) | 0x004DC600 | the +0x0C member typed as Ego_r.pdb's `CDefPointer<const CInventoryItemDef> PDef` instead of the raw `CDefPointeeBase* PDef_Object` (same offset and size) |

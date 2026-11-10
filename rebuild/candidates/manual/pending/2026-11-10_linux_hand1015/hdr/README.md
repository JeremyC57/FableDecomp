# Chunk 1015 header overlay (grade with `FABLE_HDR_OVERLAY=$A/hdr`)

Every change is behind a new opt-in; the default text of each existing header is unchanged (the existing body sits in the
`#else` branch or outside the new `#ifdef`). New files are retail def models that `#error` unless FABLE_RETAIL_DEF_CLASS_BASE is set.

| file | change | opt-in |
|---|---|---|
| engine/CPersistContext.h | the shared body of `template<class T> void CPersistContext::TransferVectorOfSubComponents(std::vector<T>&)` (after the class; includes the retail stream models and stlport/fable_containers.h; `#error` without FABLE_RETAIL_CDATAINPUTSTREAM / _CMEMORYDATAINPUTSTREAM / FABLE_RETAIL_CDATAOUTPUTSTREAM) | FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS |
| engine/CShopItemDef.h | retail polymorphic class (CSubDefClassBase base, Type +0x28, CShopItem Item +0x2C, out-of-line ctor 0x004D405A, slot 18-20 overrides); the generated flat class is the `#else` branch | FABLE_RETAIL_CSHOPITEMDEF |
| engine/CMultiStaticMeshEntryDef.h | retail polymorphic class (CSubDefClassBase base, Ego_r members, implicit ctor, slot 18-20 overrides; ELightingChannel as an MS forward enum / empty enum elsewhere); the generated flat class is the `#else` branch | FABLE_RETAIL_CMULTISTATICMESHENTRYDEF |
| engine/CAbilityTrackLevelDef.h | slot 18-20 overrides (the class is otherwise abstract under the retail def chain, so no default element can be built); `class CPersistContext;` forward declaration | FABLE_RETAIL_CABILITYTRACKLEVELDEF_VIRTUALS |
| engine/CAbilityTrackDef.h | out-of-line ctor 0x0047A3EA, virtual dtor 0x00478722, slot 18-20 overrides | FABLE_RETAIL_CABILITYTRACKDEF_MEMBERS |
| engine/CQuickAccessItemButtonGuiDef.h (new) | retail def model, 0x50, implicit ctor/dtor | needs FABLE_RETAIL_DEF_CLASS_BASE |
| engine/CAbilityButtonGuiDef.h (new) | retail def model, 0x5C, implicit ctor/dtor (EControllerType / EXboxControllerButton / EMouseButtonControl as MS forward enums) | needs FABLE_RETAIL_DEF_CLASS_BASE |
| engine/CLensFlareElementDef.h (new) | retail def model, 0x4C, implicit ctor/dtor | needs FABLE_RETAIL_DEF_CLASS_BASE |
| engine/CAppearanceModifierScalingDef.h (new) | retail def model, 0x48, implicit ctor/dtor | needs FABLE_RETAIL_DEF_CLASS_BASE |
| engine/CCardDef.h (new) | retail def model, 0x44, implicit ctor/dtor | needs FABLE_RETAIL_DEF_CLASS_BASE |
| engine/CPrizeScoreDef.h (new) | retail def model (CSubDefClassBase), 0x30, implicit ctor | needs FABLE_RETAIL_DEF_CLASS_BASE |
| engine/COpinionDeedReactionDef.h (new) | retail def model, 0x50, out-of-line ctor 0x00479574 / virtual dtor 0x0047955B (needs FABLE_RETAIL_CDEFSTRINGTABLE for CDefString) | needs FABLE_RETAIL_DEF_CLASS_BASE |
| engine/CEngineLocalDetailLayerDef.h (new) | retail def model, 0x54, out-of-line ctor 0x004799A0 / virtual dtor 0x0047747C | needs FABLE_RETAIL_DEF_CLASS_BASE |
| engine/CCardPositionsDef.h (new) | retail def model, 0x48, out-of-line ctor 0x004F9D15 / virtual dtor 0x004F9CFC | needs FABLE_RETAIL_DEF_CLASS_BASE |

Member offsets: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) minus 4 after the 0x39-byte retail CParentDefClassBase and minus 4 per
STLport vector (12 bytes, Ego_r 16); every sizeof is checked against the row's element stride with FABLE_STATIC_ASSERT.

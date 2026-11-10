# 2026-11-10 landing at -- chunk 1015 (13 rows, all matched)

Source: `pending/2026-11-10_linux_hand1015/`. vland --land 13 RELOCATION_MATCH, tests PASS. All 13 are the TLC one-argument
CPersistContext::TransferVectorOfSubComponents<T>(std::vector<T>&) on ONE shared template body (new opt-in
FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS in engine/CPersistContext.h). Element types settled from the `idiv` stride
(sizeof T), the RTTI of the vtable stored into the default element, the element ctor/dtor and resize COMDAT called, and the owning
def's Transfer caller (LABELS.tsv). The catalog called 12 of the 13 `<CAbilityButtonGuiDef>`; catalog + manifest respelled.
Row flags are inline budgets only: the one-argument resize(n) costs 39 with an implicit element ctor (255-byte rows, /d2inlS40) and
29 with out-of-line ctor/dtor (245-byte rows, /d2inlS30); the 223-byte rows call resize out of line (no flag). The `if (count > 0)`
guard around the load loop is the register lever.
Headers (3-way merged onto HEAD, base aa0f44807; hdrcheck2 default=same): CPersistContext, CShopItemDef, CMultiStaticMeshEntryDef,
CAbilityTrackLevelDef, CAbilityTrackDef; new CQuickAccessItemButtonGuiDef, CAbilityButtonGuiDef, CLensFlareElementDef,
CAppearanceModifierScalingDef, CCardDef, CPrizeScoreDef, COpinionDeedReactionDef, CEngineLocalDetailLayerDef, CCardPositionsDef
(hand-built from struct_layouts_egor.tsv; enum members are placeholders -- regenerate with gen_class_headers.py later).
Follow-ups (NOTES.md): landed 0x00472A8D / 0x0067D6B0 reach RELOCATION_MATCH on the shared body in probes (0x00632E00 / 0x007D2740
likely); landed callers spell 0x00993EB0 WriteLong (PDB ?WriteSLONG@CDataOutputStream@@QAEXJ@Z) and the resize COMDATs 0x004ED04D /
0x004EDA32 / 0x004EDF0A with invented *DefVector structs -- queued land1011ar.

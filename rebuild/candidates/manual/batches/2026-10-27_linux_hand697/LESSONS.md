# Chunk 697 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 697) In a twin-labelled destructor band, settle each copy by what it stores and who calls it: a body with no own
  vtable store whose tail is a base dtor belongs to the class whose CTOR stores the vtable next to it (0x006E7B60 ->
  0x006E7B80 MovieBase, 0x007E72A0 -> 0x007E74D0 ScriptedThingBase); a body called only by sibling ??_Gs is the base's
  (0x00723F90 ~CTCPhysicsStandard under CTCPhysicsControlled / CTCPhysicsNavigator).
- (chunk 697) A non-polymorphic dtor whose first call is a member function is `{ Reset(); }` / `{ Close(); }`; Ego_r donor
  order (X::Close then ??1X, ??0Y then Y::Clear) pins the callee name when the catalog label is propagated (0x00A60CE0,
  0x00B81E20). An inline user dtor of a member class with its own `Reset()` expands inside the outer dtor even when the TU
  also emits its out-of-line copy (LHHttpHeaders 0x00569C30 vs inline in 0x00569D30).
- (chunk 697) STLport 4.6 map destruction is ~_Rb_tree's clear() (leftmost / root / rightmost / count stores in that order)
  plus ~_Rb_tree_base's null-tested header deallocate; modelling the two levels separately gives retail's bytes with the
  _M_erase declared private and defined elsewhere (0x00A60CE0, first probe).
- (chunk 697) `if (!p.IsNull()) p->Clear();` over a CIVCountedPointer gives the load / vtable store / branch interleave
  (0x00BEBC50); the call needs a complete pointee with CIVCountedPointeeBase at offset 0 so the static_cast adds no adjust.

Review at landing:
- Three rows (0x006E7B80, 0x007E74D0, 0x004C2E90) carry the ungraded `DestroyOne` emission helper (`p->T::~T()`), the
  accepted chunk 656/657/696 pattern for out-of-line implicit `??1` rows.
- Respell the landed callers in NAME_FIXES.tsv: 0x00B81E20 leaf ClearSimulations -> Clear; 0x00B9C660 QAE/free stand-in ->
  UAE; 0x00BDDFB0 ??1QuadTreeElement -> ??1CQuadTreeElement@CLocalDetailCacheMap; 0x0083FBC0 / 0x00569940 invented
  CBlockVectorOwner / LHStringVector / CRetailStringVector / CBlockVector spellings; 0x00569C30 (landed as
  CBlockVectorOwner_00569c30) is ??1LHHttpHeaders@LHNetworkLib@@QAE@XZ; 0x006E7B80 / 0x007E74D0 / 0x004C2E90 / 0x00A60CE0 wrong
  bound spellings; 0x0099A430 / 0x0099A300 stand-ins.
- The landed 0x00B9C660 (~CEngineInternalPrimitiveOcclusionBase) source models the class as a `struct` with a non-virtual
  dtor and a non-template CIVCountedPointer; retail's dtor is UAE (Ego_r).
- The landed LHNetworkLib rows (0x0083FB60, 0x0083FBC0, ...) use invented CRetailString / CRetailStringVector types; 0x00569D30
  shows the real STLport basic_string / vector templates reproduce this TU.
- No row needed more than 2 probes; no flags or pragmas; no retail_asm, no volatile.

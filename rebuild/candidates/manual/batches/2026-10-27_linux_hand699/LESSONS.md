# Chunk 699 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 699) Propagated "OnDie" destructor band: eight rows sharing one label were eight different implicit or user dtors;
  each named in minutes from its own ??_G (vtable slot 0 -> RTTI) or, for the RTTI-less vtables (0x012CCC50, 0x012EA478,
  0x012BF0C8), from the landed ctor that stores the same vtable. All ten rows matched on the first probe by reusing the
  landed sibling models (0x00740540, 0x00AA7320, 0x00B0A060, 0x00E7E110, 0x00B33AE0, 0x00C774F0).
- (chunk 699) An implicit derived dtor whose implicit BASE dtor is large (CPSCSplineBase, 111 bytes) still tail-jumps to
  the out-of-line base ??1: model the whole chain implicit with real member types and VC7.1 keeps the base out of line on
  its own (0x00B0A160 -> 0x00B0A060); no `virtual ~Base();` declaration is needed.
- (chunk 699) A def class's implicit dtor over engine/CDefClassBase.h (FABLE_RETAIL_DEF_CLASS_BASE) gives exactly retail's
  "member dtors, CDefPointeeBase vtable store, jmp ~CResource" tail: the inline ~CDefClassBase store is dropped as dead
  (0x00D34F80 CFireHeartPatternDef).
- (chunk 699) Cross-grade a callee's COMDAT from your own object against its catalog row (`q.py <callee> <src> <leaf>`):
  it proves the callee's spelling (0x00B09E40 ~_List_base<CSpriteTrailEntry>, 0x00C77470 ReleaseBlock).
- (chunk 699) A protected callee (Ego_r IAE) reached from another class (CTCInventoryItem -> CTCInventoryBase::
  RemoveItemFromInventory) needs `friend class` in the callee's class model to keep the IAE mangling.

Review at landing:
- Five rows (0x008CC330, 0x00E81F30, 0x00B0A160, 0x00A9D3C0, 0x00D34F80) carry the ungraded `DestroyOne` emission helper
  (`p->T::~T()`), the accepted chunk 656/657/696-698 pattern for out-of-line implicit `??1`.
- Respell in the same landing (NAME_FIXES.tsv): the landed 0x00A9D3A0 row (?OnDie@CTCInventoryItem@@QAEXXZ is really
  ??1C3DMeshFilePrimitiveChunk@@UAE@XZ; it is NOT the same symbol as this chunk's ?OnDie@CTCInventoryItem@@UAEXXZ 0x005D8EC0,
  but carries the same class::name and must go), its ?OnDie@BaseItem@@QAEXXZ binding of 0x00AA8490, the landed
  CStopWatch::GetTicks 0x0051ED60 (PeekPThingWithUID), ?Release@COwner@@QAEXH@Z (0x005BC499) in the landed 0x005D8E20,
  ?vfunc_13@CTCBase@@UAEXXZ (0x006868E0), the catalog StdList_Clear_00b09e40 / ~CBuffer_00c77470 rows.
- libpk 0x00C74450 has three landed spellings: its own row ??1CVtable_012BF050 (non-virtual struct), the reverb sources'
  ??1CDspObject@@UAE@XZ (used here, as the most recent landed callers spell it), and older CBase_/CChunkBase_/CCounted_ bindings.
  Same for the 0x012BF0C8 class: landed ctor 0x00C77440 is `struct CVtable_012bf0c8` (U) over `CVtable_012bf050`, the
  reverb sources `class` (V) over CDspObject. Pick one in the libpk header pass.
- No row needed more than two probes; no retail_asm, no volatile, no pragmas, no flags.

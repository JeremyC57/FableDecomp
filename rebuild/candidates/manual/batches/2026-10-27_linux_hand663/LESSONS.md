# Chunk 663 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Never-authored destructor pools resolve from the tail jump + the stored vtable + RTTI** (10/10 here, first try for 7):
  the `jmp` at the end is the direct base's (or the last member's) dtor, the vtable stores give the class, the slot-0 ??_G is
  the sole caller; Ego_r members re-laid at STLport sizes (map/set/vector 12, list 4; Ego_r is Dinkumware 16/16/16/12)
  reconcile every offset. Labels were wrong for all ten (CPatchTesselationEdgeStrip propagated over different classes).
- **The real STLport 4.6 container text reproduces both tree-clear shapes with no tuning**: `~_Rb_tree() { clear(); }` +
  `~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }` gives the `xor edi,edi ; cmp eax,edi ; mov [x],edi`
  shared-zero form when a function clears two or more trees (0x006BA6D0, 0x0082E780, 0x006ED480, 0x00B94B50) and the
  `test eax,eax ; mov dword ptr [x],0` immediate form with one tree (0x0051A0A0, 0x00550D50, 0x007783F0). Do not hand-model
  the clear when the STLport text is available.
- **STLport list destructor inlining is VC7.1's own decision, given the real text** (0x00850360): out-of-class
  `_List_base::clear()` + `~_List_base() { clear(); deallocate; }` + `~list() {}` makes VC call ~_List_base for a
  trivially-destroyed element (clear inlined into it) and expand ~_List_base around an out-of-line clear() when the element
  owns a list. A `jmp <~_List_base>` thunk right after a list dtor body is the matching `~list` (0x00850220 -> 0x008501E0,
  0x0048CF80 -> 0x0048CA60), so the dtor a caller reaches is `??1?$_List_base@...`, not `??1?$list@...`.
- **A class with virtuals but a non-virtual, compiler-generated dtor** (CPhysicsForceModelBase, deleted through
  boost::scoped_ptr): no vtable store, QAE spelling, non-virtual ??_G. A real retail TU function (`boost::checked_delete<T>`,
  __fastcall under /Gr, explicitly instantiated) emits the ??1 COMDAT without a fake harness.
- **Member alignment of an opaque stand-in shifts later offsets**: an `unsigned char _body[0x24]` model of a member class put
  CCombatManager::DialogueManager at +0x35 instead of +0x38 (0x006ED480); give the member class its real members (or at
  least a pointer-aligned first member).
- **vector<bool> in STLport is `_Bvector_base`** (two 8-byte bit iterators + end-of-storage proxy, 0x14 bytes): its dtor is
  `if (_M_start._M_p) free(...)` reading +0 of the member (0x007B9D30).
- **Explicit virtual element destructor in `_Destroy(first,last)`**: an STLport vector of polymorphic elements
  (CIntelligentPointer<T>) destroys each with `push 0 ; mov ecx,esi ; call [eax]` (scalar-deleting slot with flag 0) and caches
  the end in a callee-saved register because `_Destroy` takes the bounds by value (0x007783F0, 0x007B9D30).

Review at landing:
- NAME_FIXES.tsv (24 rows): the own-row respells for 0x006F0DD0 (CUnproven placeholders -> the RegisteredCombatSequences
  instantiation), 0x0048CA60 (`??1?$list@VC3DVector..` -> `??1?$_List_base@VC3DVector..`; the list spelling moves to the thunk
  0x0048CF80); caller respells for 0x0082E780 (QAE -> UAE in CMap::CreateScriptedMapBrush 0x00820F80), 0x00B8FEE0 (global ->
  static member, 0x00B4AAA0), 0x009F2B20 (0x00A5C6E0), 0x008FB170 (CPhysicsForceSubmodel -> CPhysicsCollisionPointsObject,
  0x00850410), 0x00553690 (`?_M_erase@?$map@...` in CTable::Initialise 0x00550C60), 0x006BA6D0 (descriptive spellings in
  0x006BAB80 / 0x006BAED0 / 0x006BAA70), and the descriptive families on the base dtors 0x0099A300 / 0x0099A430 / 0x00686830
  / 0x0052CCF0 / 0x00A01C10 and the tree erasers 0x004ABA70 / 0x004A9CB0 / 0x00776AA0 / 0x00824830 / 0x00824870 / 0x00B95EE0
  / 0x00B98350 / 0x006F4C20 (CCombatantMap / CRetailTree stand-ins).
- 0x0052CCF0 is bound as `??1CDraggableInto@NUISystem@@QAE@XZ` by landed 0x00549A00 (CTextSlider dtor): wrong class.
- TU harnesses (`TUHarness_<Class>`, not retail) in 0x0051A0A0, 0x006BA6D0, 0x007783F0, 0x007B9D30, as in chunks 657/658/660.
- Every source repeats the STLport text; a shared `rebuild/include/stlport46/` header set would remove the duplication.
- Each row needed 1-3 probes; none was parked, so the 12-probe rule never applied.

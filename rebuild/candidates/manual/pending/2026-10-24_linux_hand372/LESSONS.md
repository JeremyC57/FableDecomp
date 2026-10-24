# Chunk 372 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport map iterator `++` (inline) must be `_M_node = _Rb_global<bool>::_M_increment(_M_node);`, a static by-value
  helper. The walk runs in a scratch register and is stored back. An in-place member walk grades 127v138 (0x4C5F10).
- STLport `pair` has a user `pair(const pair& o) : first(o.first), second(o.second) {}`. Declare it. Without it,
  for_each over a by-value pair keeps `first` instead of `second` (0x4CB9F0, 0x4CBA30), and the insertion sort
  shrink-wraps its callee-saved pushes behind the early exit (0x4CBE80).
- `_Rb_tree::_M_insert` can inline `_M_create_node` at the left site and call the out-of-line copy at the right site
  (inline budget). Model it with a `__forceinline` copy for the left site and the declared member for the right
  (0x4C4890, 0x4C4F70). A smaller node inlines both (0x4C6960).
- CRGBColour copy goes through `Set(R,G,B,A)` (a byte shuffle through a frame byte). A memberwise copy grades 188v194
  (0x4C4890).
- An STLport map ctor must be split: `_Rb_tree_base(const A&)` allocates the header in its body, and the derived
  `_Rb_tree(comp)` runs `_M_node_count(0)` then `_M_empty_initialize()`. A flat ctor stores the count before the
  malloc (0x4C60F0).
- `if (x != 0) return f(x); return 0;` puts the computation on the fall-through path. `if (x == 0) return 0;` lays out
  the zero branch first (0x4CDAE0).
- No /Zc:wchar_t in the lane: an `L""` parameter is `const unsigned short*`. `wchar_t` is SRC_FAIL (0x4CDB10).
- A declared `_List_iterator(const _List_iterator&)` copy ctor fixes the list `insert(end(), x)` register/push schedule
  (0x4C0050).
- C3DVector arithmetic must be value-constructing (`return C3DVector(a.X - b.X, ...)`) for the x87 schedule (0x4BBF00).
- An identical flag store in both branches of an `if` is hoisted above it (0x4C10E0).
- A comparator that reads only its left operand gives an exit-less unguarded insert loop (0x4C1EC0, 0x4C1F10, 0x4C2B90).
- A forward-declared parameter type must be `class`, not `struct`, or the leaf mangles U instead of V and the grader
  falls back to the largest symbol (0x4BE4E0).
- An out-of-line prefix `operator++()` forces the loop iterator into the frame. It is copied from a local, and the
  inline postfix walks a register (0x4BCF50).

Review at landing:
- The landed 0x4BE490 "PrepareForRenderIfNeeded" is really CTCGraphicAppearance::Draw(info, posInterp, orientInterp).
- The landed 0x4C6010 calls 0x4C5FA0 "GetTimeScale"; its PDB name is GetBulletTimeSlowdownMultiplier.
- Local base models sit next to existing engine headers:
  - CBulletTimeManager (rows 0x4C5F10, 0x4C5FA0, 0x4C60F0): the header's map is 16 bytes, retail's STLport map is 12.
  - CTCPhysicsBase (row 0x4BBF00): the header declares no virtuals.
  - CTCGraphicAppearance (rows 0x4C0050, 0x4BCF50, 0x4C10E0): the maps are 12 bytes, not the PDB's 16.
- 0x4BBF00's Leaf is the prefix `?GetRenderPositionForThing@?A0x`, because the anonymous-namespace hash depends on the
  file path.
- Inferred identities:
  - 0x4CCA80 is Transfer<unsigned __int64>(name, v).
  - 0x4CF450 / 0x4CFC50 are swap vs iter_swap; their bytes are identical.
- Rows that needed more than 4 grades:
  - 0x4C5F10 and 0x4C4890 matched after more than 4 forms.
  - The three parked rows each used their 4.

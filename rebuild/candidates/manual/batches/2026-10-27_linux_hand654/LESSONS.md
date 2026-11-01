# Chunk 654 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Implicit string conversion orders conditional temporaries** (0x00670D50): `set.AnimExists("DEFAULT", CAnimFilterNoGroup())`
  builds the const char* -> CCharString conversion temporary BEFORE the argument pushes (cleanup bit 1, frame slot above the
  filter; the string is pushed by `lea`, not from the ctor's eax), so the filter's inline base dtor keeps its vtable store
  ahead of the string dtor call. An explicit `CCharString("DEFAULT")` argument is evaluated right-to-left (filter first) and
  the dead filter store disappears. Try the implicit conversion whenever a retail CCharString temp is built before a sibling
  argument temporary.
- **Shared refcount header + real operator= stops a by-value copy reload** (0x00A6BCD0): `engine/CIVCountedPointeeBase.h`
  (retail opt-in) with an inline `operator=` lets VC7.1 prove `__x_copy` is not clobbered in `_M_fill_insert`.
- **PDB out-parameter GetTC<T>(T**)** (0x0065A8EE matched; 0x0095EF80 residue 92 -> 57): `long type = T::StatGetInterfaceType();
  if (HasTC(type)) { key = type; ... *out = ...; return true; } return false;` gives `mov eax,[eax+4] ; mov ecx,eax` and the
  retail frame-slot sharing of the key; the pointer-returning `GetTC(T*, type)` behind HasTCWord loads into ecx and
  separates the slots.
- **Block-scope out locals share dead argument slots** (0x0065A8EE): start/end in an inner block let the IS_THING_ALIVE
  temporary, the GetTC key and `start` share the dead `child` slot; at function scope they push the temporary into its own
  slot (frame 0x34 v 0x30).
- **if/else of block-scope CCharStrings** (0x0065A8EE): `push -1 ; je ; push A ; jmp ; push B ; call ctor` is two block-scope
  CCharString locals whose identical tails VC7.1 merges; `if (a > b) return false; return true;` gives `test ah,41h ; jne`
  (`return a <= b` gives the `jp` form).
- **FableWin inline GetTexture(out, mip, flags)** (0x00BC7660, residue 86 -> 39): the font texture fetch is
  `bool CGraphicInfoFrame::GetTexture(const CTexture** out, unsigned long mip, unsigned long flags) const` with ONE return
  after the if/else (the caller zeroes on failure); a returned pointer or one return per arm duplicates the frame-copy tail.
- **Seed a vector from two re-fetched statements; add through operator+** (0x00533D30, frame 0x9c -> 0x94): retail calls
  BaseState() once per component (`minimum.X = ...; minimum.Y = ...;`) and adds the child size through an inline
  `C2DVector::operator+` (fadd through the returned pointer, result temporary copied).
- **VC7.1 normalises**: the operand order of a three-term squared-distance sum (all six orders identical, 0x0095EF80), the
  polarity of a final `if (Term()) return;` (0x00D38D20), and an inverted `if/else` (0x00533D30) -- do not spend probes there.
- In /Os TUs the STLport bitset test stays out of line and C3DVector operators need `__forceinline` (0x0065A8EE).
- A same-TU plain private member (StopSound 0x0078EC80) is inlined in CancelForCutscene (0x0078F8F0) as retail.

Review at landing:
- Every row needed more than 4 grades (probe counts 2..30, logged in probe/<addr>.log).
- 0x00670D50 carries a same-TU stand-in body of CDefClassBase::GetSubDef<CAppearanceDef> (0x0042B0A2, the landed body, as
  0x004BFC50) and local CCharString/CAnimFilterBase/CAnimFilterNoGroup/C3DMeshStats/CMeshDataBank models next to the generated
  stub headers (engine/CAnimFilterNoGroup.h is flattened; engine/CMeshDataBank.h / C3DMeshInfo.h have no methods).
- 0x0065A8EE emits out-of-line IS_THING_ALIVE<CThingBuilding*> and bitset::test bodies in its TU (only the target is graded);
  engine/CThingAICreature.h has Ego_r offsets (WorkBuilding retail +0x170).
- 0x00A2EA20 is bound twice (HasDummyObject via 0x00A36100 and HasHelperPoint via 0x004BE170): respell 0x00A36100's caller.
- 0x005DC2E0 has five competing bound spellings (HasAnimation x2, HasAnim, FindAnimationByName, AnimExists): respell to AnimExists.
- 0x00686A70 has many per-row helper spellings in reloc_bindings (`_helper_prep`, `_helperA`, GetDisplayEngine, ...).
- 0x013BAC30/34/48 have competing descriptive spellings bound (g_CCharString_*, g_RightHandAttachmentName,
  g_LeftHandAttachmentName, g_BurntHelperA, g_FocalPointHelperName, g_helperFocal_013bac48); none is PDB-proven.
- 0x00935E40's sibling 0x009363D0 (MinionWatchFires::Update) has identical code: land/park them together.

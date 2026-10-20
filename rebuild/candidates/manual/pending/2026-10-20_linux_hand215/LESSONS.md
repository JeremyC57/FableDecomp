# Chunk 215 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A CCharString temporary in a `while` condition (`while (GSI->IsRegionLoaded(CCharString("X", -1)))`) lets VC fetch the
  interface vtable before building the string; retail's order is an inline helper with a NAMED string local returning
  `GSI->IsRegionLoaded(name) ? true : false` (the plain return gives `mov bl,al` where retail has `test al,al ; setne bl`) (0x00E6A9E0).
- `while (!term) { if (flag) { ...; return; } NewScriptFrame(); }` keeps the flag test at the loop head; the
  `while (!flag) { frame; if (term) return; }` wait is rotated and duplicates the flag test (0x00E32F30).
- VC7.1 evaluates the right operand of a commutative float `+` first: a source sum `a + b + c` loads c, b, a. Write the
  terms in the REVERSE of retail's load order (x-first source gives retail's z-first dot products, 0x00875040/0x00875120).
- A value retail keeps on the FPU through a "take the higher" step is a ternary over an inline by-value helper
  (`includeWater ? HigherOf(water, ground) : ground`); an if/assign stores and reloads it (0x00875550).
- Two call arms that differ in one pushed argument with the rest shared are two complete calls VC7.1 tail-merges
  (`if (flag) hit = F(..., A); else hit = F(..., B);`); a ternary argument gives one call with a select (0x008754A0).
- A `__forceinline` helper expanded in two arms gives retail's cross-jumped compare/store tails, the stored arm
  forwarding the value it just stored (0x004068D0); with constant `return true` arms it threads straight into the shared
  block (0x00408820).
- The GetInterface wrong-kind fallback that stays in the component's own register is a reassignment
  (`if (comp->GetKind() != 3) comp = fallback;`); the CFixedDolly ternary selects into a second register (0x0087B910).
- Defining a callee's real body above the caller under `auto_inline(off)` also lets VC7.1 keep a LOCAL container's head
  pointer in a register across the call (0x006AC140: retail never reloads `things._M_node` after GetAllLeafThings).
- `const T& p = *it++;` puts the iterator step at the top of the loop body (`mov eax,ecx ; add ecx,0Ch`); `++it` in the
  for header puts it at the bottom (0x00761F40).
- Map lookups repeated in a loop share ONE frame word only through a `__forceinline` helper that declares the iterator;
  `it = map.find(k)` on an existing iterator local needs a second word for the hidden-pointer result (0x0098F1E0).
- A uid kept in a register AND passed by address as a map key is a copy (`unsigned long key = uid;`); a
  `CPendingMap& queue = DeviceQueue[d][p].Objects;` reference forms the address before the key temporary (0x0098F0F0).
- A def object read out of a GetDef out pointer and released by hand at the end (`T* scheme = 0; ... scheme = def.Object;`)
  shares the zero register with the out pointer's initialiser (`xor esi,esi ; mov [esp+8],esi`) (0x00408820).
- Block-scoped file objects (CDiskFileWin32 / CMemoryFile) with an inline `virtual ~T() { Close(); }` reproduce retail's
  vtable store + direct Close + member/base destructors (0x0040D350, 0x0040A740).
- Lane gap (3 rows): retail inlines a local's destructor on an early `return` inside a loop and calls the out-of-line
  copy at the normal end (0x004AA840 / 0x008AC970); the lane inlines both, tail-merges both exits, or out-lines both
  (0x00E503D0, 0x00E9D800, 0x00D1CAC0).
- Lane gap: CUIState's colour conversion is the inline-asm `fistp` FtoL (store/reload through a dead slot), the
  PackVertexNormal family (0x0052F440).

Review at landing:
- 0x0040D350 / 0x0040A740 model CDiskFileWin32 / CMemoryFile locally although engine/CDiskFileWin32.h and
  engine/CMemoryFile.h exist (retail CMemoryFile is 4 bytes narrower than the PDB: 12-byte CArray, Opened +0x1c, Pathname
  +0x20; the landed CDiskFileWin32 ctor 0x0099AD80 is a local model too).
- 0x006698D0 / 0x00965E80 include engine/CThingSearchTools.h but model CThing locally (retail CThing sits 0x10 below the PDB).
- 0x006AC140 (parked) needs the GetAllLeafThings body 0x006AE830 in the TU; the source emits it under `auto_inline(off)`.
- 0x00EDC640's line-1 class (CV_TempleOfLightScript) differs from the catalog (CV_ChapelOfEvilScript); same leaf EndMission.
- The CScriptThing / CCountedPointer / GSI slot models are local copies of landed ones (0x00E82010, 0x00D506B0, 0x00D2D830).

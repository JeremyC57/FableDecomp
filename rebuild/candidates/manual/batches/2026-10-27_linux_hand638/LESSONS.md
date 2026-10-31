# Chunk 638 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Callee-first mod-ref moves a loop-invariant store into the preheader** (0x004FF920). A function pointer passed to a
  `const F&` template parameter is a temporary per call (`ForEachInRoughArea<T>(*area, CThingFilter_None(), &Fn)`). When the
  template's body is compiled noinline in the same TU, VC7.1 knows the callee only reads that word. It then hoists the store
  past the loop guard, as retail does (`je ; mov [esp+10h],offset Fn`). Without the body, a named local stores before the guard
  (between `cmp` and `je`) and a body-scope temporary stores every pass from a register. Template members defined in a header
  were instantiated in the caller's TU in retail, so the same-TU body is the real situation.
- **A filter whose address the stand-in takes is stored before the out-vector's lea** (0x008A8890). The stand-in for a search
  template must take `&filter` (as the landed 0x008A9C40 does). A stand-in that reads the filter by value lets VC7.1 schedule
  `lea ecx,[esp+18h]` (&found) above the filter store into the dead argument slot. Check the landed sibling's stand-in before
  inventing one.
- **A leaf that matches no symbol hides a wrong virtual/const spelling** (0x004FF920). The parked source declared
  `void Update();` non-virtual, so its `// Leaf: ?Update@CWorldMap@@UAEXXZ` matched nothing and the grader graded the largest
  function in the TU. Grep each regraded log for `WARN leaf`.
- **Explicit `std::vector<T, std::allocator<T> >` specialisations** (declared after `engine/fable_nscript_forward.h`'s forward
  declaration) give the real STLport mangled names for reserve / _M_insert_overflow / template arguments without changing a
  byte. They replace the CScriptThingVector / CThingVector stand-ins of the GetAllThings* family (0x008A8890).
- **Register colouring ties stay**: the "list node edi / item pointer esi" swap is shared by 0x007729A0 and 0x006061B0. Neither
  declaration order nor a same-TU GetPItem body changes it. The `lea ecx` v `lea eax` key-address choice in 0x00773950 survived
  six forms, among them the 0x007729A0 ternary GetTC that gets the key-first order there. Park both kinds on sight.
- The shared headers clear vland's SHARED_HEADER_AVAILABLE warnings without changing a byte (0x008A8890, 0x004FF920):
  - engine/CCountedPointer.h for CScriptThing's PImp, in place of the local CCountedPointer model;
  - engine/CSuspendableProcessMicrothread.h as the CLevelLoader base;
  - the `#define X XLayout` include pattern for CThingSearchTools / CDefinitionManager.

Review at landing:
- The matches need the NAME_FIXES.tsv respells for their callers' bindings: 0x008ADBA0, 0x004C7E50, 0x008ACD30, 0x008AD5F0,
  0x009AD410 and 0x0044C6B0. The landed GetAllThings* / GetAllCreatures* siblings (0x008A8570, 0x008A86C0, 0x008A89D0,
  0x008A9C40) still spell them with the CScriptThingVector / CThingVector / CThing_Retail stand-ins.
- Several landed / parked sources spell 0x00A01B50 GetPItem as returning `CThing*`. The PDB returns `CBaseObject*`.
- The parked differ sources carry PDB respells that changed no bytes, so they can be adopted as they are:
  - 0x00773950: GetPVillage, IsOpeningHours const;
  - 0x007175D0: IsModeActive, GetCurrentMode;
  - 0x006D8830: access specifiers;
  - 0x00A1ABF0: the agent633 GetNodeAt set;
  - 0x006061B0: CWorld::GetFrame.
- Grades per row this session (each one lever):
  - 0x00A112F0: 9;
  - 0x00773950: 7;
  - 0x00A1ABF0: 6;
  - 0x004FF920: 7 before the match;
  - 0x006D8830: 5, and 0x007175D0: 5;
  - the rest: 4 or fewer.
- 0x004FF920 uses a local CWorldMap next to the flattened engine/CWorldMap.h (header debt in NOTES.md).
- Landed 0x008A9C40 / 0x008A89D0 use `CWorld_Retail` with a ThingSearchTools member. engine/CWorld.h has `PThingSearchTools_ptr`.

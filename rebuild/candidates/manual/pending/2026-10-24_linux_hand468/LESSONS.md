# Chunk 468 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Retail CWorldMap layout (12-byte STLport containers, PDB +4..+0x14 high).**
  - Members:
    - vptr +0, CIDrawWorldMap vptr +4, World +8, Component +0xC, PThingMapManager +0x10
    - LoadedScriptedMapBrushes map +0x14 (12 bytes), Maps CArray<CMapInfo> +0x20, Regions vector<CRegion> +0x2C
    - PRegionGraph +0x44, GranularityShift +0x54, MapPlacementPitch +0x58, Area +0x5C, MapPlacement +0x6C
    - PlayerPositionInfo +0x78 (13-byte packed {bool Set; C3DVector Pos}), LoadedRegion +0x9C
  - CMapInfo is 0x48 bytes: PMap +0x10, Used +0x24, Placed +0x25, Activated +0x26.
  - CRegion is 0x58 bytes: SoundThemesEnabled +0x56.
  - Primary vtable 0x01244AEC slots: 2 DeleteAllRegions, 4 SaveToFile, 5 RemoveLevel, 7 PostLoadTopology, 10 IsMapLoaded,
    16/17 GetMapNumberAt (C3DVector / C2DCoordI), 18 GetMap, 19 PeekMap, 25 IsPosChangeable, 29 LoadScriptBrushesFromFile.
  - Secondary vtable 0x01244AB4 slots: 6 DrawConvertWorldMapCoordsToLocalCoords, 7 DrawGetMyPlayerMapNumber.
  - Retail CWorld equals the PDB up to +0xA8. From there it is 0xC lower: MiniMapEnabled +0xDD, PAtmosProcessor +0x24,
    PeekWorldMap is slot 13.
- **`operator[]` as `*(begin() + n)`** fixes the SIB base/index order (`[edi+eax+10h]`) where `Start[n]` swaps it
  (0x004FF440).
- **A box result built as a named copy.** Write `C2DBoxI result(box)` with a user memberwise copy constructor, take the
  `area` reference first, then apply the `+=` in retail's store order. This reproduces 0x004FC560's load order; the
  constructor form and the aggregate form do not.
- **A bool read under /GS lands in the dead `event` argument slot** when `event` is not used after `GetData`
  (0x006292D0, 0x00629330: `x == true` compares).
  - The landed DIFFER 0x00629390 EASetFreeCameraMode still uses `event->Player` after the read, which keeps that slot
    live.
  - Worth one retry there: read `event->Player`, or call GetPlayer, before `ReadBool()`.
- **A `__fastcall` template declaration reproduces the /Gr SGI `for_each` convention.**
  - Convention: hidden result in ecx, first in edx, last and the 4-byte `mem_fun_ref_t` on the stack.
  - The /O2 TU loads begin/end into registers before the pushes, where the /Os twin 0x00644660 does `push [ecx+20h]`.
  - Two dropped results share one `push ecx` frame word (0x0082A1D0).
- **The entity-script forwarder `GetParentScript()->SetPredicate(pred)`** pushes the argument before the slot-3 virtual
  call that produces the callee's `this` (0x00F35B10). CEntityScriptBase vtable 0x012C3224:
  `[dtor, Main(pure), Init, GetParentScript(pure), ...]`, and derived bodies are `mov eax,[ecx+14h]` (ParentClass).
- **Non-polymorphic destructor pairs with no image references.**
  - ??1 = an empty out-of-line user destructor.
  - ??_G = an implicit destructor plus a `delete p` helper.
  - The ??_G keeps `this` in esi, so its last member buffer goes through eax; the ??1 reuses esi for the last buffer
    (0x00B22EE0 / 0x00B23110).
  - Identify the class from the constructor's member stores (0x00B21DD0).
- **STLport list destructor** = out-of-line `clear()` + inline `if (_M_node) free(_M_node)`, with `lea edi,[member]`
  held across the clear (0x0070B930). STLport list is 4 bytes and vector 12, so PDB CAnimSet 92 -> 0x54 and
  CCompleteAnimation 120 -> 0x60 in retail.
- **Same-TU stand-in to keep ecx.** A same-TU `noinline` stand-in of a callee that touches only eax (an empty `switch`),
  under `auto_inline(off)`, lets VC7.1 keep ecx = this across the call (0x00C216D0).
- **Hidden-pointer local before argument pushes.** A named C2DCoordI local filled by a hidden-pointer call is built before
  the next call's argument pushes (0x0063A910, 0x0063AE70).
- **Leaf class keys and access must match the object symbol.**
  - A `struct` parameter type mangles `U` and a PDB `class` mangles `V`. Protected members mangle `IAE`.
  - STLport `_List_iterator` is a struct (`U`); the PDB's Dinkumware iterator is `V`.
  - With a wrong letter, q.py silently grades the only/largest function and still says MATCH. Only calls.py's
    "(0 blocks)" shows it. Five rows here needed the fix: 0049e5e0, 006292d0, 00629330, 007573c0, 00bbd630.

Review at landing:
- **Local models next to existing engine headers.**
  - CWorldMap (all CWorldMap rows): `engine/CWorldMap.h` has the PDB 16-byte containers.
  - CWorld in 0049ea10 / 0049e5e0: offsets past +0xA8 are 0xC lower than the header. 006292d0 / 00629330 use the header
    as a donor base.
  - CMiniMapDisplay (0082a1d0): the header has MarkerPrimitiveHandles at +0x14; retail has it at +0x10.
  - CEngineInternalPrimitiveParticleGroup (00bbd600, 00bbd630): the header has PDB offsets.
  - CRegion (0049e5e0, 00507610): a local 0x58-byte model.
- **Same-TU stand-ins and conventions.**
  - 00c216d0 carries a same-TU stand-in CheckState body. It is not retail's body; only YieldPhysicalPlayer is graded.
  - 0082a1d0 declares the SGI for_each as a `__fastcall` template (declaration only), as the landed 0x00644660 does.
- **Grades.**
  - 004fc760 is parked after 4 grades (lane gap).
  - 004fc560, 006292d0 and 00629330 took 3 grades. The third grade of the CWorld EA pair was the leaf-key fix only.
- **Name evidence.** 00b22ee0 and 00b23110 have no reference anywhere in the image; the PDB name set (`??_G...`) and
  the constructor layout are the evidence.
- **Already-landed sources.**
  - `CWorldMap_ApplyScriptedMapBrush_00c0dd40.cpp` is libpk CString::Append(const char*) with a wrong stdcall model.
  - Landed callers name 0x004378C0 `Refresh(long)`; it is `UpdateMiniMapMarkers(bool)`.
  - `CActiveFile_OnReadFinished_0070b8f0.cpp` and `_0070b910.cpp` are STLport `list<CAnimSet>` destructors, and their
    free is declared `free_func`.
  - The libpk sound class is named CSoundEvent (agent434) and CPanelItem (agent435). Pick one.

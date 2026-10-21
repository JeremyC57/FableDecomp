# Chunk 256 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Callee stand-ins for local vectors.** Retail keeps a local STLport vector's begin/end in registers across later calls, and keeps a zero register unchanged. The fix is a same-TU `#pragma auto_inline(off)` stand-in body for the callee that fills the vector, so VC7.1 knows the vector's address does not escape. The stand-in should touch only the vector's words, not take its address.
  - 0x0063E480: the 0x49C490 rough-area search.
  - 0x00502170: GetLevelFilesFromWorldFile 0x501D30.
  - 0x004A0370: the 0x4AD780 house search.
  - A stand-in that reads the bound filter objects makes things worse: VC drops their stores (0x0063E590).
- **Def release null test.** A same-TU GetDef stand-in (the landed 0x63F3A0 body) folds the `if (Object)` null test of the CDefPointer release, giving the short `mov ecx,[def] ; dec [ecx+4] ; jne` form (0x0063EB20).
- **8-aligned frame.** The `and esp,-8` frame of 0x0063EB20 comes out by itself from a C2DCoordI local that is pushed into a vector. No double is needed.
- **Labelled gotos.** `goto fail;` / `goto avail;` exits reproduce retail's block order and its tail-duplicated `return false` epilogues (0x00861670).
- **Bool event payload.** It goes through an inline template `Add(const T&)`, so the temporary lands in the dead argument slot (0x0049F180). A `long` second argument on the loading-stage marker gives `xor edx,edx`.
- **Vector indexing.** Write `Maps[i]` twice. A reference local moves the address add (0x00502850).
- **PeekMap ordering.** A named `const CMap& m = PeekMap(map);` makes PeekMap run before the local result's address is pushed (0x0063A420, 0x0063E590's map branch).
- **Loop body ordering.** Naming the physics pointer before building the segment end moves the `[edi]`/`[edx+60h]` loads to the top of the loop body (0x0063E480).
- **Search box as a call temporary.** It shares its slot with a later per-iteration local, which gives retail's frame size (0x0063E480).
- **Filter temporaries.** Empty-filter and bounding-box filter temporaries written inline in both branches' Bind2/Bind3 calls are hoisted above the branch with retail's reversed store order and slots. The empty filters are homed in the dead parameter slots (0x0063E590).
- **Release zero-store order.** Swap with an empty vector gives retail's start/finish/end zero-store order for the release (0x005022F0, still parked on the dead finish store).
- **C2DCoordI copies.** A user copy ctor on C2DCoordI fixes the order of the box copy stores (0x004FEC80).
- **CThingFindNavigationLayerAt::GetLayer() const returns float** (PDB `QBEMXZ`). That is retail's `fild ; call __ftol2` return.
- **Retail CWorldMap layout.** Retail CWorldMap uses 12-byte STLport vectors:

  | Member | Retail offset |
  |---|---|
  | World | +0x08 |
  | Maps | +0x20 |
  | Regions | +0x2c |
  | MapPlacement | +0x6c |
  | LoadedMaps | +0x90 |
  | DestroyedThemePositions | +0xb0 |
  | PLevelLoader | +0xbc |

  The CWorldMap vtable is at 0x01244AEC:

  | Slot | Function |
  |---|---|
  | 10 (+0x28) | IsMapLoaded |
  | 16 (+0x40) | GetMapNumberAt(C3DVector) |
  | 17 (+0x44) | GetMapNumberAt(C2DCoordI) |
  | 18 (+0x48) | GetMap |
  | 19 (+0x4c) | PeekMap |
  | 25 (+0x64) | IsPosChangeable(C2DCoordI) |
  | 28 (+0x70) | HeightAreaChanged |

  The two GetMapNumberAt overloads sit in reverse declaration order.
- **CEngineThemeDef.** DestructionThemeDef is at retail +0x98 (PDB +0x9c).
- **Quest scripts.**
  - DTOR_OUTLINE: declare `~CScriptThing` out of line in TUs where retail never expands it.
  - WAIT_UNTIL as `!(!x)`.
  - A const-reference local for virtual dispatch.
  - Script members sit at PDB minus 0x14 and CScriptThing members must be 4-aligned.
  - The unrotated `while (!IsDone()) { if (!alive) {...; return;} ... }` loop form.
- **Lane gaps (parked):**
  - Inline ~CScriptThing expansion sites (00e2eac0, 00e32080, 00e504c0).
  - eax/ecx choice in a counted release (00ead9f0).
  - Register-pair swap (004fec80, 00beab40).
  - Retail keeps a dead finish store (005022f0).
  - A materialised inline bool and frame overlap (0062bd80).
  - One store scheduled early (0063e590).

Review at landing:
- These rows needed more than 4 grades: 0063e590 (about 40 variants), 004fec80, 00beab40 and 0062bd80 (parked), and 0063e480 (6).
- Local CWorldMap models are used next to `rebuild/include/engine/CWorldMap.h`, because the header has 16-byte PDB vectors and retail has 12-byte ones. This affects 004fbde0, 00502170, 00502850, 0063a420, 0063e480, 0063e590 and 0063eb20. Each has a `// Layout:` comment.
- New same-TU stand-ins in landed sources (each is a body for a function that also exists elsewhere in retail):
  - 0063e480: CThingSearchTools::GetAllThingsInRoughArea.
  - 00502170: CWorldMap::GetLevelFilesFromWorldFile.
  - 004a0370: CThingSearchTools::GetAllLeafThings.
  - 0063eb20: CDefinitionManager::GetDef, which calls GetPDefFromGlobalIndex 0x9AD9E0.
- 0049f180 is `/GS` and its src now carries `// Flags: /GS`. 0062bd80 (parked) is `/GS` too.
- 00502850 indexes `Maps[loadType]` in the unload test. That is retail's behaviour, not a typo.
- Label rows to fix are in NOTES.md "Label corrections". Among them: 0x0049C490, 0x004FB310, 0x0081E920, 0x0063A9E0, 0x00618550 and 0x0063F3A0 (shared body).

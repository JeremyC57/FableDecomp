# Chunk 224 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- An int `==` evaluates its RIGHT operand first: retail's `call GetRegionNumberMapIsIn ; ... call GetCurrentRegionNumber ;
  cmp current, saved` is `current() == region(x)` (0x0082AE80 / 0x0082B520 / 0x0082B630); `region(x) == current()` calls in
  the same order but compares `cmp saved, current`. (`a + b + c` of three inline Sizeof() calls was evaluated LEFT first in
  0x00A25D20 -- the float/`/` right-first rule does not generalise to every operator.)
- A short member passed as a `long` argument through an inline `long GetX() const { return X; }` getter loads with
  `movsx eax` and frees ecx for `this`; the direct member gives `movsx ecx` and rotates the later registers (0x0082AE80).
- A pointer member used as `this` of a virtual call (`thing->m_pPlacement->SetPosition(p)`) loads into eax and copies to ecx;
  the same access through an inline getter loads straight into ecx (0x008D77A0).
- `x = Data[Position++]` through an inline CArray `operator[](long)` evaluates the index (load, +1, store) before loading the
  element block and changes the loop's register assignment; the raw `Data.Data[Position++]` form loads the block first
  (0x009E9220).
- A float member divisor fetched through an inline getter (`/ stats->GetDuration()`) is loaded before the other call and
  parked in a dead argument slot (right operand first); the plain member is read after the call (0x006F9820).
- A record whose bitfields are ALL assigned on a named local (`e.Index = i; e.ExternallyImported = 1; e.StopEvent = 0;`)
  compiles to one and/or word in a register, and keeps the x87 time on the stack through both range `fcom`s (0x006F9820).
- An STLport vector of class elements destroyed through `_Destroy(first, last)` + `if (start) free(start)` reproduces the
  shared `mov eax,[start]` / `test eax,eax` tail; an in-place loop in the destructor gives a zero-register variant (0x006F9820).
- `ImprisonedFX = CScriptThing();` with an INLINE CCountedPointer::operator= that reads both source words first folds the
  known-zero temporary into ebx stores inside the release block (0x00DA4350); the same assignment in 0x00CF8970 calls the
  out-of-line operator= 0x00704580 (per-TU inline choice).
- An inline virtual of CScriptThing (SetDataString) is devirtualised and inlined on a named local but still dispatched through
  the vtable on a member object (0x00E338E0).
- A `CDefPointer` temporary pair under `a && b` with `Object = 0` in its destructor reproduces retail's single surviving zero
  store per pair (0x00512410).
- `while (memory == 0 && allocSize >= minSize) {...}` (memory zero-initialised) gives retail's top-test/bottom-memory-test
  loop; `while (cond) { ...; if (memory) break; }` rotates the loop and duplicates the error path (0x00A65720).
- `(unsigned char)Flags & 2` in a ternary produces retail's `mov al,[..] ; and al,2 ; neg al ; sbb eax,eax` (0x00A65720).
- A by-value CCharString exception argument: the third CBBBFileException construction in one function is left out of line by
  VC7.1 (0x0099AE50) and copied into the throw object from eax -- write `throw CBBBFileException(msg, type)`, not a named
  exception local (0x009E95F0).
- An STLport `__true_type` tag passed by const reference needs a user-declared empty ctor, else the temporary is zeroed
  with a byte store (0x00547500).
- A converting CIntelligentPointer ctor used for `PEmitter = thing;` needs `__forceinline` in a function this large, otherwise
  VC7.1 calls the inline ctor out of line (0x008D9740).
- 0x0041CFB2 (GetTextBySymbol, called from 0x0041DB84) takes its CWideString result pointer in edi and returns it in eax --
  a link-time custom convention; rows calling it cannot match from a single-TU compile.

Review at landing:
- Rows that used all 4 grades with a concrete untried fix: 0082abd0 and 0082acd0 (apply the 0x0082AE80 getter model;
  0082acd0 additionally an inline OwnedByHero accessor), 008dcb60 (put the nearby-things half in its own block),
  0079fd90 (key as an explicit temporary), 005bec92 (the landed 0x008EC320 out-slot + named pointer + ReleaseDef model may fit).
- Local models next to existing engine headers (the headers are pack(1) / layout-off for retail and lack the member ctors):
  CItem (005bec92), CLandscapeBackgroundPatch (00be5e70, header has no CPatchTesselationEdgeStrip type), CList (00536a30,
  0053b7f0, header is PDB-offset), CMemoryFile (009e9220/009e95f0, header 4 bytes off -- as the landed MovePosition says),
  CMiniMapDisplay (+0xb8 region word, header puts PRegion at +0xd4 -- as the landed UpdateMarkersInternal says).
- The two SSE rows (00a9d480, 00a5cee0) are inline-asm bodies (asm_bake family), not compiler gaps.
- Catalog rows whose labels are wrong (CList::GetDataFromDef, CMesh::~CMesh, CMap::CopyPos, CMeshData::BuildMesh class):
  see NOTES.md "Label corrections"; the landing should name them from the vtables/PDB given there.
- Errors noticed in landed sources: none confirmed; the landed CItem ctor 0x005BEC40 and this chunk's AddItem model CItem
  locally while engine/CItem.h exists (pack(1) raw pointers instead of the CCountedPointer member).

# Chunk 311 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Scoped 2-float copies.** Named 2-float copies inside a `{}` scope give retail's frame, where the copy slots are packed into later ints (`{ C2DVector a(tl.X,tl.Y); ... start.x=(long)a.X+1; }`). Used in 0063b0f0, 0063bf20 and 0063c230.
- **C2DBoxI copy ctor.** A user copy ctor (`TLX(o.TLX),...`) fixed the field-load scheduling and the coordinate-store order in UpdateAllEdges (0063dfd0).
- **Dead Z slot.** `C3DVector d = p - GetPosition();` through an inline `operator-` keeps a 4-byte frame hole for the dead Z (00665260).
- **Repeated int->float expressions.** Repeating `(float)(box.BRX - box.TLX)` inline as constructor arguments (CSE kept on x87) matched retail; named float locals did not, and the inline form also fixed an esi/edi swap (0063bbc0).
- **Switch jump tables.**
  - The table's block order follows source case order. Write each case separately, ordered by the first case of each value group.
  - Grouped fall-through labels produced a two-level byte table instead (00ab3870).
- **~CWorldMap (0x00506940) shapes:**
  - Write the vector-of-objects destroy as an inline `(first, last)` by-value helper. The cached end gives retail's local `push ebx`/`pop ebx` around the loop.
  - Give the `__fastcall _Destroy_range` allocator temporary a user ctor, so no zero byte is stored.
  - Null-check the STLport `_Rb_tree` header free.
  - Write the counted-pointer release as `if (Info) {...} Info = 0; Data = 0;`, with the nulling unconditional inside the outer test.
- **Arg-slot write-backs.** When retail writes cached `this`-like argument registers back into their own argument slots just before a call, the callee receives the addresses of filter temporaries whose value equals the argument. VC7.1 homes `CThingFilter_IsNot(from)` in `from`'s dead slot. Model the filter as `CFilter_Bind2<IsNot, IsNot>` holding const refs to two IsNot temporaries (0063e6f0).
- **CWorldMap TU caching family.** Several CWorldMap rows show whole-program-style register caching across calls:
  - a list head cached across a call that receives `&list` (0063e6f0, 005064c0);
  - `1 << shift` reused from ebp after a member call (005066e0).
  Park these as the known lane gap rather than spending budget on them.
- **Flag bits around `new`.** A ToWideString temporary inside a new-expression's arguments gets VC7.1's conditional-destruction flag bits. Retail UpdateNavMaps (0x00501990) has them; LoadThings (0x00500EB0) names the string first and has none.
- **0xBFEA0E is the CRT malloc thunk.** It is used for STLport node and vector storage, paired with free 0xBFEA14. 0xBFEA1A is operator new and 0xBFE9BC is operator delete.
- **CDataInputStream::Read.** The inline Read that matches this lane's CTexture row compares `size <= SourceChunkBytesRemaining` (`cmp size,[rem]; jg overflow`). The landed `Remaining >= size` spelling gives `cmp [rem],size; jl`.

Review at landing:
- 0063b0f0 now uses the leaf `?GetHighestThemeSizeZInArea@CWorldMap@@QBEMABVC3DVector@@0@Z`. Confirm the landed 0x0063BF20 keeps the camera-offset name, so the two never bind to one symbol.
- 00665260 declares GetPosition 0x006A4D00 on CThingCreatureBase. Check the lander's binding rule for 0x004C73D0 against 0x006A4D00.
- 00506940 declares `operator delete` (0xBFE9BC) and uses `free()` for every 0xBFEA14 site. Retail has both.
- 0063b0f0, 0063bf20, 0063c230, 0063bbc0 and 0063dfd0 carry local CWorldMap models with retail offsets: 12-byte containers, so offsets after +0x14 sit lower than engine/CWorldMap.h. Each file has a `// Layout:` note.

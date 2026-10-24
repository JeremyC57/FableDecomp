# Chunk 339 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A camera-mode setter that stores a byte in the manager and then copies a member (`SingleCut = true; OldCamera = Camera`) keeps the manager pointer in a register only when the code goes through an inline manager helper (`GetCameraManager().SetSingleCut()`). Writing both through `CameraManager->` re-reads the pointer after the byte store (008734f0, 00873570).
- STLport `map::operator[]`: `insert(it, v)` must be an inline forwarder to the out-of-line `_Rb_tree::insert_unique`. Then the iterator is re-read from the call's return slot, as retail does (009ac530).
- The STLport `_Rb_tree_base` constructor, which allocates the header, must be a base class constructor so it runs before the derived `_M_node_count(0)` and `_M_empty_initialize()` (009ac4d0).
- A vector-map lookup written `if (it != end()) return it->second; return 0;` lays out the found arm first, as retail does (00991840).
- A hidden-result C3DVector copy of a list element (`return list.front().first;`) needs a user memberwise C3DVector copy constructor. The implicit copy goes through an esi temp (008fce10).
- Constructor store order follows the body assignment order under `optimize("s")`, not the member order (0057a5dd).
- The camera-point `PeekTC` uses the const LowerBound (0x004365B0) and `GetTC` uses the non-const one (0x0040F020). Both leave the out pointer uninitialised when the bit is clear (00850700, 008540f0).
- CTCCameraPointFixedPoint and CSnapToPoint are `#pragma pack(1)` (bool at +0x40, C3DVector at +0x41; and +0xc0/+0xc1) (00850700).
- A constructor with a `push -1 ; push offset` frame whose unwind map destroys "base, then member" matched on the first try once `--flags /EHsc` / `// Flags: /EHsc` was set, reusing the class model of the landed destructor twin (00c38570 / 0x00C38470). `g.sh` does not read `// Flags:`, only gall.sh does.
- In VC7.1 tests, `std::pow(2.0, float)` is ambiguous under `<cmath>`; use `<math.h>` and `pow(2.0, (double)x)`.
- x87 gap candidate: `1.0f - a / b` followed by `x *= scale` in a tail-duplicated arm; retail drops the dead `b` with `fxch st(1) ; fstp st(0)`, ours with `fstp st(1)`. No source form tried moves it (008eebe0).

Review at landing:
- 009ac4d0 and 009ac530 supersede the landed asm-bake files NBankFileManager_CRetailBank_Constructor_009ac4d0.cpp and NBankFileManager_CContainedBankMap_Index_009ac530.cpp (`__declspec(naked)`). Replace them.
- engine/CGameCameraManager.h is the PDB layout, which does not match retail (SingleCut is at +0xb0 in retail, +0xbc in the PDB; the STLport members are narrower). 008734f0, 00873530 and 00873570 use local models.
- NCameraModes rows model CBase locally: the retail CBase is 0xa4 bytes, the PDB minus 0x24. Regenerate the engine header before retyping.
- The landed CSnapToPoint constructor model lists LookDirection at +0xc4, but retail is +0xc1 (pack 1).
- The landed model of 0x0074CA20 (`__copy`) takes the iterator tag by value, but call sites (0075f4e0) pass it by const ref.
- 00c38570 uses the address-named class CConstraintRepeat_00c38470 to stay consistent with the landed destructor. Unify the name when the class is identified (the CParticleProgram constraint family).
- Rows that needed more than 4 grades: 00854030, 0087c070 and 008eebe0 (all parked); 009ac530 (matched after several insert/forwarder forms).

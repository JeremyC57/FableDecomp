# Chunk 492 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- GFPSizeof-style recursive sizers (0x00AC3910): `if (!GFPIsNull(p)) return sizeof(T) + GFSizeof(*p); return sizeof(T);`
  is the retail shape (two returns, `lea eax,[eax+4]`); a ternary (43v40) or an accumulator local (38v40) does not match.
- An unsigned count passed to a `float` parameter (0x00ADEEA0, C3DBoundingBox::BuildFromPoints(const C3DVector*, float))
  emits `mov [esp+8],eax ; fild ; jge ; fadd [2^32]` reusing the dead argument slot -- check the callee's PDB signature
  before assuming the fild is source-level float math.
- A CCharString-keyed LowerBound/find that keeps the key's data pointer in a register (0x00AE60D0, `mov ebp,[eax]` before
  the loop) needs CBasicString<char>::operator< 0x00429950 defined noinline in the TU (as the landed 0x007564F0): VC7.1
  hoists the load only when it can see the callee writes no memory.
- Element destructor with no vptr store before the out-of-line ~CBaseClass (0x00AF7250, pair<EPSCType, CComponentInstance>):
  the class declares no destructor (compiler-generated dtor drops the vtable reset), and `_Destroy(p)` on the pair is a
  direct, non-virtual destructor call.
- STLport `__uninitialized_copy` / `_M_find` / `__pop_heap_aux` rows with no named owner (0x00AE5730, 0x00B2E900): copy
  the landed byte-twin's text and switch to per-address CUnproven* placeholder types; MATCH first time.
- Console-command setters that tail-jump (`jmp`) into a function (0x00B6774B/5B/7B -> 0x00B676F0) give a quick way to
  locate a class's no-argument helper in the PDB method order (Ego_r: OnReleaseDefaultPoolResources, Clear, commands).
- Loop `for (long i = 0; i < (long)v.size(); ++i)` over a 4-byte STLport vector whose owner is re-read through a global
  after each call (0x00B676F0) produces retail's `test eax,0FFFFFFFCh ; jle` guard and the `sar 2` re-test -- no
  pointer-cast arithmetic is needed (the landed twin 0x00B676A0 uses casts).
- Inline `#include "../tmp/x.inc"` does not reach the lane's compile directory: paste local models into each src file.

Review at landing:
- 0x00AC3910 needed 3 grades (within the cap); no row went past 4.
- Local models next to existing engine headers: CNavNavigableLeafNode (0x00A75A70, 0x00A76520), CNavBlockedQuadTreeNode
  (0x00A76280), C3DAnimation2 (0x00A553F0), CMirroredTextureHighlightsChunk (0x00AC4F30), CEngineScreenEffectGlowRenderer
  (0x00B5EBF0), CEngineWorldMap (0x00B676F0); C3DAnimationTransitionState (0x00A9DEF0 / 0x00A9DF50). Reasons in NOTES.md
  "Header debt".
- Already-landed sources with wrong names/bindings: 0x00A44A70 (`C00a44a70::M`, really vector::back), 0x00A44880
  (global-namespace `vector` stand-in), 0x00A45150 (binds 0x00A44C90 as `BoneMorphLess`), 0x00A80000 (pair bound as `V`),
  0x00B5EE00 (glow method on a derived shell class), 0x00B676A0 (candidates/ header stand-in + pointer casts, needs a
  `virtual` header edit per its own note), 0x00AF7A10 (vector<pair<EPSCType,CComponentInstance>> as a stand-in struct).
- reloc_bindings: 0x00A242C0 (9 spellings), 0x009E6530 (conflicting), 0x00BDC450 (`cl_h_00bdc4f0`), 0x01436E8C (7 global
  spellings; Ego_r layout says g_pEngineWorldMap).
- Catalog rows that hold fragments, not functions: 0x00A446CE, 0x00A44719, 0x00A46594 (skipped).

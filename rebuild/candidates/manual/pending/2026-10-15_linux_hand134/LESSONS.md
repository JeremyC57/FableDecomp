# Lessons from hand chunk 134

- A constructor-shell frame that is zeroed and never read is a dead TEMPORARY that VC7.1 keeps: `member = C4DVector(0,0,0,0)` in an
  inline member constructor body leaves the 16-byte temporary's stores in the frame (0x00BE21F0).
- A bit taken from byte +N with `mov al,[x] ; shr al,7 ; and al,1` is an UNSIGNED SHORT bit field; unsigned char drops the `and`,
  unsigned long loads the whole dword (0x00BBA290).
- A float colour filled R,G,B,A by four `fstp` after loads in A,B,G,R order is an inline four-argument setter (arguments evaluate right
  to left) storing into the member; a constructed temporary assigned to it adds a frame copy (0x00B453D0).
- `a = b = f()` stores `a` LAST: write `ShadowMeshInfo = MeshInfo = f()` when retail stores MeshInfo first (0x00B9D8E0).
- A struct result assembled in a frame temporary and copied out word by word (`mov [esp],eax ...` then four moves) needs the member
  type's memberwise USER copy constructor; with the implicit one VC7.1 forwards the source straight into the destination (0x00BACF00).
- An 8-byte hidden-pointer result with branches is ONE local written field by field; `result = T(a, b)` adds a frame copy (0x00BEB5C0).
- Inlined, a derived constructor stores its vtable BEFORE its own body assignments: members zeroed after the vtable store belong to the
  most-derived constructor body, even if the out-of-line constructor lists them in the base (0x00BDA3D0 vs landed 0x00A8E160).
- A CCountedPointer reset written inline sinks its two zero stores below the next block; an inline member `Reset()` keeps them in place
  (0x00BDC450). The same object's `delete[] p; p = 0;` store sits after or inside the null test depending on whether the source has the
  explicit `if (p)` (0x00BDC450 vs 0x00BDC3C0).
- A by-reference key that retail copies into the dead `mesh` argument slot is the by-value parameter of a `__forceinline` helper
  (`ReleaseID(id)` -> `remove(id); push_back(id)`, 0x00BBCE10), as in GOTCHAS 24b.
- `for_each(v.begin(), v.end(), bind2nd(mem_fun(&CThing::SetInLimbo), flag))` = SGI for_each (/Gr) with the functor result in ecx,
  first in edx, last + an 8-byte {member pointer, bool} binder on the stack, the member pointer pushed as an immediate (0x006A2370).
- Labels: the CEngineInternalPrimitive* family is heavily mislabelled by propagation in the game-code range (0x0043.., 0x006A..,
  0x0077.., 0x00A1..); check `rtti-vtables.tsv` for the ??_G / slot owner and the PDB method list before modelling `this`.
- Lane gaps seen again: vector slot address materialisation (0x00BE2120/0x00BC62D0), a local cached in esi across a call (0x006A2370),
  call-result temporary coalescing (0x00B83750), frame trimming of a partly-constant temporary whose live word is first (0x00A11C20 --
  its Y/Z twins match).

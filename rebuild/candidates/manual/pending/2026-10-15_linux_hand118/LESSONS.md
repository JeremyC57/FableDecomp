# Chunk 118 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A local whose address is only handed to an out-of-line constructor is treated as non-escaping when that
  constructor's BODY is compiled earlier in the same TU: define it (non-template, `#pragma auto_inline(off)`) above the
  caller and VC7.1 keeps the buffer pointer in a callee-saved register across a virtual call and reuses it for the
  inline destructor (`push edi ; call free`); an opaque declaration reloads it from the frame. A template member
  definition is inlined despite `auto_inline(off)` (CAFile::ReadVar 0x00516180/0x00516D90/0x00516E10).
- CCharString literals in a no-EH `&&` condition: IMPLICIT conversions (`IsRegionLoaded("Lit")`) build the temporary
  before the object pointer is fetched and re-address it with `lea`; explicit `CCharString("Lit")` fetches the object
  first and pushes the ctor's eax. The zero store ahead of the first ctor is the temporaries' flag word home; the
  flags then ride in ebx (`mov ebx,1` / `mov ebx,3`, `test bl,2 ; and ebx,-3`) (0x00E33660).
- The wave-25 STLport vector model (`_Vector_base` + `_STLP_alloc_proxy` over an allocator temporary) also fixes the
  "lea ecx scheduled before the last zero store" shape when the vector is the engine CArray inside a heap object built
  by `new` (0x00A455A0) and in a `/Os` definition ctor (0x004EB9E8).
- Hidden-pointer C3DVector results: copying the WHOLE bounding sphere (memberwise copy ctors) gives retail's 16-byte
  frame (the unread radius store is dropped, its slot stays) where a centre-only copy gives 12; reading the products
  through a reference to the sub-member reorders them (0x00A35F80). For `a*x + b*y + c*z` VC7.1 emits the z product
  first; with the components already on the x87 stack, `v.X * m.XAxis.X` (component left) gives `fld [m] ; fmul st(n)`,
  the matrix-left form gives `fld st(n) ; fmul [m]` (0x00A13C60).
- A footprint sum written as one expression evaluates every inlined GetSizeof term before adding (spills, push ecx);
  accumulating `size += ...` statement by statement reproduces retail's running register (0x00A90860).
- A COM device fetched into a named local INSIDE the guarding `if` is loaded before a following `count ? first : this`
  select branch (retail `mov edi,[esi+1D4h]` ahead of `jne`) (0x0098A8E0).
- Reading an adjacent array element's field: a named local for the PREVIOUS element's index (and for the base array)
  before taking the current element's reference keeps retail's `[eax+ebp-4]` load ahead of `lea esi,[eax+ebp]`; a
  reference to the previous material is CSE'd into `[esi-4]` (0x00A8E5C0).
- Constructor zeroing order: words zeroed BEFORE a member matrix's identity stores are a member object with its own
  zeroing ctor; body assignments sink below the member ctors (0x00B09F90). The CEnginePrimitive base stores
  {type, 0, 3 channel bytes}; the VC6 list member is `head = 0 ; malloc ; self-link ; head = node`.
- A by-value counted-pointer member that is default-constructed and then assigned in the body keeps the full inlined
  operator= including the release test on the known-null block (`mov eax,ecx ; cmp eax,edi`) (0x00A44D80).
- `Size() > 0 ? first : (T*)this` gives `test ; ja`, `!= 0` gives `jne` (0x00A2E9A0 vs 0x0098A8E0).
- A float clamp against an inline `flag ? -1.0f : 0.0f` evaluates the ternary twice from one flag byte load
  (`test dl,dl` twice), the upper clamp to a literal becomes an immediate store (0x00A45370).

Review at landing:
- 00516180 needed 5 grades: the fix was found on its byte-identical twin 00516d90 (4 attempts were spent on 00516180
  before); both now carry the same source.
- 00516180 and 00516d90 are two catalogue rows with byte-identical bodies both named ReadVar<int>; the sources are the
  same (one is probably another 4-byte instantiation folded by the linker).
- Local models next to existing engine headers: C3DMeshFileVerticesChunk / C3DMeshFileTrianglesChunk /
  C3DMeshFileTransformationChunk / C3DMeshFileXVertexDataChunk (the landed family model: PDB CArray 0x10 vs retail 0x0c,
  XVertexData factory allocates 0x4c), C3DMeshInfo (as the landed ctor 0x00A248F0), CAIBrain (header 4 bytes high from
  +0x28, as the landed 0x0088B810). C3DMeshFileXTextureIndexChunk and C3DMeshLODInfo use their headers.
- Invented class names: CSoundOutput00c6afc0 (no-RTTI module), CStatBar::ClearGraphics and
  CShaderRenderManager::UpdateBoneMatrices (descriptive method names on correct classes).
- 00a36df0 (parked) never reads `this` and has no direct caller: its owner class is the manifest's, unverified.
- 00e33c90/00e33e00: retail passes one argument more to CGameScriptInterface slot 72 and one fewer to slot 475 than
  ghidra_out/cgsi_slots_aligned.tsv's prototypes -- the aligned slot names may be off by an overload here.

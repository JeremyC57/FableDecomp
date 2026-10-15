# Lessons from chunk 138

- A struct temporary that retail keeps half on the FPU and half in the frame (x/y `fld` kept, z copied as a dword into a frame
  slot) is a BY-VALUE getter result (`C3DVector GetCentre() const { return Centre; }`) used in an expression; reading the member
  directly folds the arithmetic into memory operands (00b52090).
- `CCountedPointer<T> p(new T(...)); return p;` (no NRVO) reproduces the merged `inc ecx ; mov eax,ecx ; dec eax ; mov [esi],ecx ;
  mov [esi],eax` copy-out + release; a `/Gr` free factory takes the hidden return slot in ecx and ends in a bare `ret` (00512c70,
  009d58d0). A counted pointer passed by value from a named local is copied in the argument area and the local is released
  after the following call (00b3ff70).
- A value that retail keeps in a register while also passing its address (`mov [slot],edi` then `cmp edi,[eax]`) is a COPY:
  `unsigned long searchKey = key; LowerBound(&searchKey); ... key < it->key` -- passing &key itself reloads from the slot
  (009ad2e0). Same for a pointer-held scalar that must survive a store: read it into a local first (`long level = value->Level`,
  00c647b0).
- A by-value CScriptThing whose copy reads `[esi+0Ch]` (from `this`) instead of the cached `&Me` register copies through the
  PImp member's own copy constructor (`PImp(other.PImp)`), not field by field (00d36860).
- An `if (p) p->F(...) else Singleton::Get().F(...)` with the shared `lea ; push` hoisted above the test and the object loaded
  straight into ecx needs the owner pointer as the local (`fe->m_p ? ...` via `CFrontEndManager* fe = GetInstance();`), not the
  member value (00530ec0, manager part).
- A two-store float pair `m_cur = m_def = x` is a chained assignment (`fld ; fst ; fstp`); a lone float member assignment is
  an integer move (00c647b0 / 00c64940).
- A clamp with fldz / fld1 meeting in one `fstp` and no store on the in-range path is `if (f < 0) f = 0; else if (f > 1) f = 1;`
  under `#pragma optimize("s", on)`; the speed build stores integer immediates (00453178).
- A loop over two six-word arrays (`a[i] = 0; b[i] = 0;`) in a constructor body is unrolled by VC7.1 into interleaved stores
  (+0x1e4/+0x1fc, +0x1e8/+0x200, ...); a 12-word record memset becomes `rep stosd` sharing the zero register (00c6ca20).
- A user-constructed return type evaluates the constructor's arguments right to left: the retail conversion order A, B, G, R
  means `CRGBFloatColour(R, G, B, A)` (008860d0).

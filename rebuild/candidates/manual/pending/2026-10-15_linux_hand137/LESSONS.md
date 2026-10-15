# Lessons from hand chunk 137
- A store sequence that runs highest-address-first (`+0x2c, +0x28, ... +0x04`, then the diagonal) is a CHAINED assignment
  (`a = b = c = 0.0f;`): VC7.1 evaluates it right to left. An aggregate initialiser stores ascending; `memset` becomes `rep stosd`
  (00b50510, a CMatrix3x4 identity).
- `if (p && p->count == 1) p->Notify(this)` loads the member straight into ecx; retail's `mov eax,[esi+4] ... mov ecx,eax` comes
  from a NULL-SAFE value accessor `long GetRefCount() const { return m_p ? m_p->count : 0; }` compared with 1 (00b93660; the
  same shape as the CEnginePrimitiveHandle `??_G` @ 0x0041C500). The wrong form even changed the enclosing array loop's rotation.
- Two equivalent getters over containers of the SAME type are tail-merged into one block; retail's two separate arms (each with
  its own `+0x28` read) need vectors of different record types (00b2b790).
- A float `fild` from a frame copy rather than from the member is an 8-byte struct local (`C2DCoordI size = g->m_size;`) (00b5a160).
- An empty functor passed BY VALUE (STL `remove_if(pred)`) pushes the uninitialised `push ecx` frame word only when the functor
  has a user (empty) constructor; `T()` on an aggregate adds `mov byte ptr [esp+4],0` (0082b750).
- `#pragma optimize("", off)` bodies still inline `__forceinline` functions, homing the inlined `this` and locals in frame words
  ([ebp-8]/[ebp-4] pairs per inlined member destructor) (009f7330).
- STLport `_Rb_tree` teardown inlined into an owner's destructor: `if (count) { _M_erase(root); left = header; root = 0; right =
  header; count = 0; }` with the header re-read for every store, then `if (header) free(header)`; vectors: `_Destroy(first, last,
  tag)` as a `__fastcall` with the tag in the high byte of the `push ecx` word, then `if (first) free(first)` (0048d7c0, 0052c4b0,
  0098d9f0). Four "FinishWithDynamicTextures" rows were such record destructors -- that label is heavily propagated.
- Reading two fields of an argument record into locals before the first call reproduces retail's up-front loads (00b2ae60).
- A memberwise C3DVector copy constructor makes VC read `[eax+14h]`, `[eax+18h]` before `add eax,14h`; the implicit copy forms
  the address first (00b80840).
- Engine headers whose layout struct carries the vptr as a data word cannot take vtable stores: constructors/destructors need a
  local polymorphic model even when every offset matches (00bfd480); non-virtual members attach fine through a derived shell.
- Lane gaps seen (parked): constant-register allocation of repeated float immediates (00b5bbc0), placement of a hoisted constant
  load (00c0a790), frame-slot order of float locals (00b50e50), esi/edi colouring (00bf2ad0), copy-loop strength reduction (00bf3030).

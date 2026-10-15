# Chunk 153 lessons
- A string argument built after the interface's vtable is read in our build (`mov eax,[esi+40h] ; mov edi,[eax]` before the CCharString ctor) wants a NAMED local (or an inline test helper holding it) -- the call-argument temporary lets VC hoist the vtable load. Two string arguments: declare the second argument first.
- A switch whose case 0 shares the default arm in retail keeps 0 in the jump-table range only with an explicit `case 0: return X;` plus a fall-out `return X;` (`case 0: default:` biases the index with `dec eax`).
- A pair-style user copy ctor whose POD member copies IMPLICITLY gives the re-based `lea esi,[ecx+4] ; mov ebx,esi` copy; a user copy ctor on the member folds it (0x008198C0); same rule picks C3DVector (user) vs CRGBFloatColour (implicit) in 0x00885A80.
- A by-value result copied three times from eax (`mov esi,eax ; rep movsd`) is a getter returning a REFERENCE to a caller-provided scratch object; a by-value return re-forms the frame address each time.
- `fld dword [g] ; fcomp qword [m]` with a constant that is also squared unfolded elsewhere is an extern float VARIABLE (0x0129BA3C = 0.0001f epsilon), not a literal (a literal becomes a double constant and swaps the operands).
- An inline converting constructor `CVolume(p) : m(0) { Init(p); }` reads its by-value argument into esi before the base constructor; `CVolume v; v.Init(p);` reads it after.
- A member-function-pointer table call `(p->*g_table[3])()` gives `mov eax,[g] ; mov ecx,esi ; call [eax+0Ch]`.
- STLport `_M_insert_overflow(pos, x, __true_type(), 1, true)` needs a user-declared `__true_type() {}` (else a zero byte store into the dead argument slot).
- STLport _Rb_tree empty construction inside `new T` matches with the alloc_proxy header built from 0 then assigned malloc's block and `_M_empty_initialize()` through `_M_leftmost()/_M_rightmost()` reference accessors.

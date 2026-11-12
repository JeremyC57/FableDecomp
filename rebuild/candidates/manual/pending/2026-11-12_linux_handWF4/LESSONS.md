## 0x004B3CE0

- (wf4 0x004B3CE0) A "register roles swapped" residue on a local STLport vector was an ESCAPE fact: the callee that takes the vector (same retail source file, defined earlier) needs its REAL body in the TU and _M_insert_overflow must be the visible header body; then _M_finish stays in a register across element copy constructors.
- (wf4 0x004B3CE0) set::erase(key) called in one arm and expanded in the other (no distance call in the expanded copy: the loop is dead) is ONE spelling in both arms; the split comes from c2's site frequencies -- try `while (i < n) { ..; ++i; }` for the enclosing loops before any /d2inlT flag (for: both expanded; while: retail's split).
- (wf4 0x004B3CE0) `mov eax,[ecx] ; mov ecx,<arg> ; call eax` through a struct's function pointer = an inline accessor returning the pointer (look for Get..Func in the PDBs); the accessors' inline temporaries also moved a constant-zero register contest (c2prio: 62 v 68) that no spelling of the contested region itself moved.
- (wf4 0x004B3CE0) `obj.Method(args.., Call())` with the receiver loaded AFTER the argument call: read the receiver's pointer member directly; an inline accessor on the receiver chain is evaluated before the arguments.
- (wf4) xdis.py / any long printer piped into `head` prints a BrokenPipe traceback: redirect to a file and `sed -n` it instead.

## 0x00A90040

C3DVector::Cross with every component parenthesised (FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION) fixes a `fld st(1)` + `fxch ; fstp st(0)` vs `fxch` x87 residue in the N3DPrimitive ConvertToVBFormat*Bump tangent tail (0x00A90040): PAREN tuples move the scheduling-region cut; spellings of the caller do nothing.

## 0x0064546F

- optimize("s") HUD TU: small header inlines (C2DVector::IsNonZero, C2DCoordI::operator!=) are CALLED unless force-inlined in the header opt-in; retail expands them (0x0064546F).
- `C2DVector p(a.ToC2DCoordF() + ref)` with the right operand bound to a named `const C2DVector&` keeps the float temp on the x87 stack and builds the sum straight into the local; `= a + obj.Member` spills the temp and shifts every frame slot (0x0064546F).
- A CIntelligentPointer destroyed BEFORE the next statement is an implicit/explicit temporary of the assignment (`Effects[i] = rawPointer;`), not a named local (dtor two instructions late) (0x0064546F).
- EEngine2DLayer 0x43 is ENGINE_2D_LAYER_0; take the enum from engine/NUISystem_CManager.h, do not invent an enumerator.

## 0x008565E0

x87 dot product whose one term has swapped operands: write the sum out in the row (c2 emits a one-line sum last term first with each product swapped); `test r8,r8 ; setne al` after a bool bitfield read through a temporary handle is `if (bitfield) return true; return false;`

## 0x008F10B0

- A callee that retail calls once out of line and expands elsewhere in the same function (GetCardDef before / inside the loop of 0x008F10B0) needs no pragma: put its real body in the TU and VC7.1's auto-inline makes the same choice (loop calls expand, the straight-line call stays).
- A frame 0xC too big plus a reload of a def pointer after an add-ref = the GetSubDef template body was not visible: include engine/CDefClassBase_GetSubDef.h with FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE.
- `v = v + f()` costs a 12-byte temporary where retail has none: spell `v += f()`; a `Z += 0.0f` survives as a dword copy of Z onto itself.
- A CDefPointer release whose count store is scheduled late among x87 code (load / dec / ... / store / jne): FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE.
- An overlay copy of stlport/fable_containers.h is bypassed when fable_game.h includes it first (quote include resolves next to the includer): include it first in the row.

## 0x00ADDBE0

- VC7.1 orders the two memory loads of a commutative float add itself (source order `a + b` v `b + a` does not matter); reading one operand through its inline accessor (`s.GetRadius()` v `s.Radius`) flips `fld R ; fadd C` to `fld C ; fadd R` (0x00ADDBE0).
- A left-leaning three-term float sum of x87-stack temporaries is emitted in one fixed order whatever its spelling; parenthesise the right pair (`yy + (xx + zz)`) to get Z, X, add, Y, add (0x00ADDBE0).
- A 6-test min/max loop stays rolled when each bound expression is spelled twice over `spheres[i]`; a `float v` temporary, a reference or a walking pointer makes VC7.1 unroll it by 4 (0x00ADDBE0).
- Frame 5 floats + values homed in dead argument slots = separate scalar float locals, not C3DVector locals (those keep 12-byte aggregates and add a `mov eax,ecx` copy pointer) (0x00ADDBE0).

## 0x0065AA6F

- 0x0065AA6F: a by-reference local that must NOT overlay an earlier dead vector (frame 12 bytes larger than the twin's) is a function-scope local; keep only the out-parameter floats in the inner block and carry the result out in a plain float -- VC7.1 spills it back into the block's freed slot.
- 0x0065AA6F: the operand order of one x87 `fadd` between two pointer loads flipped with the number of inline levels expanded earlier in the function (one-argument GetTC<T>(T**) v two-argument GetTC(id, T**)); try the other accessor arity before touching the expression.
- fg.py stops at the first matching variant: put the spelling you prefer first.

## 0x00BBCC70

0x00BBCC70: a parked "411v415 register" residue was a model error -- nested member setters SetTextureUVModeU/V + AttachTextureToStage(1, &GetStipleFadeTexture()) from the shared headers match on the first grade; a reference-returning inline getter fed to a pointer-taking inline keeps the `add reg,off ; je` null test.

## 0x007B8B10

A hand-written local GetTC/lower-bound in a parked draft gives register-web swaps and a missing loop pad; the shared GetTC(id,&ptc) (GETTC_EXPANDED) + CVECTORMAP_INLINE_LOWERBOUND fixed 0x007B8B10 with no other change.


# Retry 8 lessons (formulations that turned a parked DIFFER into a match)

- **Statement order picks the scratch register** (00bc65d0): take the vector-element reference BEFORE indexing a global table
  (`CHandle& h = v[i]; mgr = g_table->Managers[type];`); with the table lookup first VC7.1 loaded the global into edx ahead of the index.
- **Float copies through the FPU** (00a1ab40, 00a1df60): a C2DVector that retail moves with `fld Y ; fld X ; fstp ; fstp` is a
  named local built through a user memberwise copy ctor (`C2DVector p = node->Position; out.SetXY(p.X, p.Y);`) or a
  `C2DVector(x, y)` temporary handed to the PDB ctor `CNavWayPoint(const C2DVector&, uchar, bool)`; plain member reads move dwords.
- **Block-scoped named locals share dead temporary slots** (00a1df60): two way points as named locals inside their own `{}`
  block give retail's 0x1c frame (function-scope locals or call temporaries got separate words / the wrong build order).
- **x87 sum order** (0065f056): `s = Z*Z; s += X*X; s += Y*Y;` in retail's term order (GOTCHA 356 again); the single
  `X*X + Y*Y + Z*Z` expression is reordered z,y,x.
- **Which value sits in which dead argument slot** (00a77a10): write repeated stream writes through an inline by-value helper
  (`static inline void WriteLong(CAFile& f, long v) { f.Write(&v, 4, 0); }`): its parameter is what VC homes in the dead bool
  slot, leaving the other dead slot to the remaining named local.
- **Two counted-release shapes in one function** (00688c90): when the getter's CDefPointer temporary releases with the long
  ReleaseTemporary form but the local releases with `dec [reg+4] ; jne`, the local is a second holder type whose dtor is
  `if (o) { if (--o->m_count == 0) o->Destroy(); }` and whose assignment from CDefPointer is CDefPointer's own operator=.
- **One shared call block for an either/or flip** (008e9950): `if ((a && !b) || (!a && b)) Call();` gives two tests entering
  one shared call; nested if/else gives two call blocks, `a != b` and the ternary fold the test.
- **`do { ... break; ... } while (0)` cross-jumps early exits** (00811760): early `return`s in a ctor each got their own
  `pop edi / mov eax,ebp / ret` tail; `break`s out of a do/while(0) jump into the single epilogue after the inner `pop esi`.
- **Early return keeps a duplicated dtor tail** (00db80c0): `if (IsDone()) return; Remove();` after a timer loop keeps a second
  CTimer-dtor + epilogue copy like retail; `if (!IsDone()) Remove();` lets VC cross-jump the loop exit into the final release.
- **Separate `if (x) return false;` statements place the shared false exit inline** (00bf7f70): one `a || b || c || d || e`
  test moved the `xor al,al ; ret` to the end of the body (long jumps); five statements put it right after the chain.
- **Zero result held in a register across both failure exits** (00aeabe0): `T* result = 0; if (t && t->A() && !t->B()) { result = ...; } return result;`
  keeps the zero in ebx with two `mov eax,ebx` tails in retail order; early `return 0` forms fold the constant (no ebx push).
- **Loop guard + do/while** (00b44f20): stores retail makes after the `count > 0` test from the index register (and the
  alignment on the NEXT loop) come from `long i = 0; if (i < n) { zeros; do { ... } while (++i < n); }`; a for loop with the
  stores before it stores immediates before the test.
- **Construct a call argument class as a temporary** (008d8dd0): `SetMainAnim(CAnimInit(...), a, b)` (not a named local) gives
  retail's AnimIndex/&init registers; the named local rotated them. (The 008d6da0 twin needs more than this.)
- **Value-initialised member for a zero-filled blank record** (0081b160): a non-POD record (CDefString member) that retail
  zero-fills from one xor register and then default-constructs is `CBlank() : Entry() {}` -- a member value-initialised in an
  initialiser list; the lane does not zero-fill `CEntry()` / `T()` temporaries of such a class, and `= { 0 }` scatters the fill.
- **Inline accessor puts a table element straight into ecx** (008ea220): `g->Joysticks[n].Device->Rumble(...)` loaded the device
  through eax; an inline `CInputManager::GetJoystick(n)` accessor loads it into ecx and keeps the temporary's store before `push 0`.
- **Named reference for an intermediate record** (00898340): `CRegion& region = map->GetRegionEntry(n); CCountedPointer<D> def(region.m_pDef.Data);`
  loads the def straight into ecx (inc / flag test / dec / virtual release); building it inside one expression held it in eax.
- 00b201d0 regraded RELOCATION_MATCH unchanged against the registered oracle (its parked x87 DIFFER no longer reproduces).

## Lane gaps confirmed with probes this pass
- An inlined, null-tested counted release (`if (b && --b->count == 0) ...`) always re-compares after `dec [reg]`
  (`cmp [reg],0`) in the lane; the identical out-of-line destructor uses the `dec` flags. Five decrement spellings, a member
  Release(), and the same-register load all keep the cmp (00aeb790, 00699b00, 006cd890, 0069adc0 family).
- The lane jump-threads a re-tested global flag (`g ? a : b` twice) where retail re-tests `cl` (00543ee0).
- VC7.1 canonicalises x87 multiply operand order: swapped operands and parentheses give identical code (00ac7eb0, 00a5d000).

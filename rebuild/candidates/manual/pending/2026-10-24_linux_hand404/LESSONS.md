# Chunk 404 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Two bool results, then `if (a) { if (!b) return true; } else { if (!b) return true; } return false;` -> the dead `test bl,bl` and the exits `jne ; mov al,1 ; ret 8 ; xor al,al ; ret 8`. The identical arms are cross-jumped (0x00874F10). Per-arm `return false; return true;` pairs fold to `sete`; `if (b) return false;` arms swap the two exits.
- A named C3DVector local overwritten by a by-value `operator*` temporary and then returned (`v = C3DVector(v.Y, -v.X, 0) * -1.0f; return v;`) -> two frame vectors and an implicit-operator= dword copy. The constant Z folds to -0.0f (`mov [..],80000000h`). `return C3DVector(...) * -1.0f;` drops both frames (0x00878B60).
- `C3DVector v(x, y, 0.0f)` whose Z is never read -> the 0.0f store is eliminated but the Z word stays in the frame (24-byte frame for two 2D copies). An implicit copy of the argument keeps the dead Z load/store (0x00875A30).
- `float sign = 1.0f; if (cross < 0.0f) sign = -1.0f; return angle * sign;` -> `fld 1.0` before the cross product, as retail. The lane then tail-duplicates the return into the negative arm (`fstp st(0) ; fmul [-1.0f]`), while retail joins at one `fmulp`. A `__forceinline` sign helper does not stop it (0x00875A30, parked).
- Products in a single-expression dot/length over two frame struct copies -> the operand order is not the source order (both declaration orders gave `to`-first). Fixing it needs a different form (0x00875A30, parked; statement-order sums are the next try).
- `thing->GetDefName() == name` with a 4-byte CDefString -> the result is homed in the dead argument slot through the hidden pointer. CDefString needs a user copy ctor and no destructor (0x00877110).
- A namespace function the PDB takes by `const CWorldMap&` / `const C3DVector&` -> identical bytes when written with pointers. Search the PDB name sets for `NCameraModes::` free functions before inventing names (0x00874F40, 0x00874F80, 0x00874F10).
- An empty STLport tag that retail homes in a LIVE argument slot -> the out-of-line callee's real body must be in the TU under `#pragma auto_inline(off)` + `__declspec(noinline)` (GOTCHAS 24l). This held for:
  - __find_if: 0x008AC060, 0x008C34A0
  - _M_insert_overflow: 0x008ADF90, 0x008B8050
  - __destroy_aux: 0x008B3430
  - the LowerBound key word: 0x0088AFF0
- A `??_G` with an inlined vector destructor -> `virtual __forceinline` dtor + `__forceinline` vector/_Vector_base dtors + an out-of-line ctor stand-in that anchors the vtable (0x008B3430).
- A C2DVector delta + inline GetLengthSquared -> X-first squares; named dx/dy floats square Y first (0x008AABC0).
- `(float)atan2(...) * floatConst` -> `fmul dword` (a double product gives qword). An inline `fabsf` plus a named float difference puts `pop edi` after the fcomp (0x008C7E50).
- Memberwise user copy ctors on C3DVector and on the by-value predicate:
  - -> dword-by-dword predicate copies (0x008AC060, 0x008C34A0);
  - -> the swap temporary goes through the frame (0x008E2DB0, 0x008E3150, 0x008E3430).
- `if (x) return true; return false;` -> byte exits on float compares (0x0087C660). An out-of-line `return a && b && c && d;` -> eax 1/0 (0x008D45A0).
- A template leaf `?swap@std@@` never matches the template symbol `??$swap@...`, so q.py silently fell back to the largest function. Use `??$name@` for templates and `??$?R...@` for member-template operators (0x008E2DB0, 0x008D45A0).

Review at landing:
- Landed 0x00878A80 "GetRotatedPosition" is NCameraModes::CProjectileWeapon::GetCamPosOffsetXY(float) const (PDB name set).
- 0x00874EE0 (not in this chunk) is NCameraModes::IsThemeToBlockCameraBetween. Rename the CameraBlockTest_* declarations in landed 0x008754A0.
- Landed 0x008D43C0 looks like CEventFilter_HitEventWithWeapon::operator() (0x008D43A0 = HitEventWithNoWeapon).
- Tests use behavioural stand-ins for out-of-line callees:
  - the unary CTurningAnimCompare 0x0087C6B0
  - CIsUsableIciclePoint 0x008E2E40
  - the script-name filter 0x00811100
  - AngleDifference 0x00A441E0
  - insert_unique 0x00884830
  - _Rebalance 0x0042971D
  - _M_insert_overflow
- Descriptive names (no PDB symbol): CompareActorRotations, SKeyedPair/SKeyGreater, CIsOutsideRadius, SThingUIDRemapComponent, CSoundPair.
- The same-TU stand-ins (0x0040F020 LowerBound, __find_if, __destroy_aux, _M_insert_overflow) are `auto_inline(off)` bodies in the source objects; the lander must pick the row's leaf, not the largest symbol.

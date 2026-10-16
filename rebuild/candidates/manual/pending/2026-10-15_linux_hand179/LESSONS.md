# Lessons from chunk 179
- When retail calls the map getter (virtual PeekMap) AFTER an out-of-line conversion and only then pushes `&local`, take the getter result into a named `const CMap&` local; the direct `PeekMap(n).F(local)` pushes the argument first.
- A conversion temporary that shares the frame slot of a later local (`GetMapNumberAt(C2DCoordI(...))`, `IsPosInMap(C3DVector((float)x,(float)y,0))`) is an unnamed temporary, not a named local.
- `float t = (float)(long)p->X; return t == q->X && ...`: the stored float on the LEFT of `==` gives `fstp [slot] ; fld [q] ; fld [slot] ; fucompp`; on the right VC keeps it with `fst`.
- `(float)a / (float)(b - c)` folds into `fidiv`; retail's `fild ; fdivp` comes from both operands as named float locals.
- Two arms that each end with the same base call: write the base call once after the if/else; separate calls let VC push the known constant (`push 0`) in the zero arm where retail keeps `push ebx`.
- `long n = 0; for (...) if (!used) { n = i; break; } if (n == 0) {...}` reproduces a loop whose fall-out jumps straight past a `test esi,esi`.
- A function defined in the same TU (ConvertWorldMapCoordsToLocalCoords in CWorldMap.cpp) inlines into its siblings while other TUs call it out of line: check the TU before modelling it as a call.
- Import thunks: 0x00BFE9BC is MSVCR71 `??3@YAXPAX@Z` (operator delete) and 0x00BFEA14 `free` -- read the IAT, not the label, when the use looks odd (here `if (!p) operator delete(p)`).

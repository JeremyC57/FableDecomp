## 0x00760E70

- (wf14, 0x00760E70) `fld [mem] ; fmul st(i)` where VC7.1 would normally `fxch ; fmul [mem]` (the register operand left stale, popped by a trailing `fstp st(1)`) is a read through a byte-packed C2DVector: build the value from `C2DVector v(vec3)` (FROM3D) instead of the C3DVector members; no Dot order/paren spelling reproduces it.

## 0x006AC430

CThingPlayerCreature::LearnExpression: a member typed long in the header but CDefIndex in the PDB changes evaluation order of a fastcall arg (conversion before the other call) and thus the whole register allocation -- use the PDB member type (0x006AC430).

## 0x00AF5430

- A counted-pointer release whose loop-carried index sits in a register vs retail's frame slot: try FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER before loop/local respellings (0x00AF5430).

## 0x009C4440

- (wf14, 0x009C4440) A 64-bit compare's ebx/edi swap at the end of a function came from a `!A() || memcmp()` size test far above it: written as two ifs it matched. Fix earlier control flow before register spellings.


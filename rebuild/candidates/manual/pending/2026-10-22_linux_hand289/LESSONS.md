# Chunk 289 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **esi/edi swap in a counted loop** (0x00690030): writing the per-iteration call (`processed.SetHandled()`) in BOTH arms of an if/else raises the parameter's weight so it wins esi over the countdown. VC tail-merges the duplicated calls, so the bytes are unchanged otherwise. Eleven loop-shape variants did not move it.
- **`if (c) f(true); else f(false);`** gives retail's `push 1 / jmp / push 0 ; call` merge; `f(c)` or `f(c ? true : false)` gives setcc/`mov al`.
- **Two-value state tests laid out larger-case-first are a switch**: `cmp 0xb; je far; cmp 0x10; jne exit` is `switch` with case 0x10 written first.
- **A CCharString by-value result compared then destroyed after the if is a named local**. A temporary keeps the bool in a callee-saved register and spills `this`.
- **Inline out-parameter GetInterface for every lookup** (0x005BEDCB) makes the key words share one frame slot, the last one the dead argument slot. Longhand lookups add a frame word.
- **Script Main frames**: putting a counter, a distance and a loop in their own `{}` block lets a later CCharString temporary share the counter's frame word (0x00D907B0, 0x00D90B30).
- **Float abs compares**: `(float)fabs(x)` in an inline float Abs gives fcomp + `test ah,41h`; a double fabs compares in double with fcompp/`test ah,5`. `cos(PI / 180.0 * deg)` with float PI = 3.1415927f reproduces the retail double constant 0x3FC6571855555556.
- **An STLport map/multimap local**:
  - `: _M_header(0)` in the ctor reproduces retail's pre-malloc zero store (`: _M_node_count(0)` moves it).
  - The multimap insert is the inline `insert_equal` descent plus the out-of-line `_M_insert(x, y, v, 0)` with a hidden iterator result.
  - Real `find` with a `!(key < k)` descent and an if-form tail (`if (y == header || k < key(y)) y = header;`) puts the result in edx as retail does.
  - Reading the header into a named `end` local before the find frees esi, so the key stays an immediate.
- **An inlined copy of a header inline function** (CGraphicInfoFrame::GetFrame inside AddIcon, 0x00542780) can need the bank-frame read written as the full expression twice (`Data->TouchedFrame = GraphicInfo->Bank->CurrentFrame`), not via the `frame` local the out-of-line copy (0x0041C260) uses. With constant arguments the folded condition otherwise schedules the CurrentFrame load ahead of Data.
- **Default-argument C2DVector temporaries v member-body assignments**: two temporaries reusing ONE frame slot (zeros first, then 40.0) are sequential statements in a ctor body (`Offset = C2DVector(0,0); Size = C2DVector(40,40);`) with no user copy ctor on C2DVector.
- **/GS rows**: q.py does not read `// Flags: /GS`; pass `--flags /GS` (gall.sh reads the line).

- **An int-to-float pair from two unsigned shorts** (0x00542780): an inline `C2DVector(unsigned short, unsigned short)` ctor loads both shorts early, in source order; named long/float locals load the second one first; plain casts load it late.

Review at landing:
- 0x00BF84C0 / 0x00BF8030 use a named scalar-deleting-destructor stand-in for 0x00BF8C10 (inline-once band); confirm the lander accepts the name binding.
- 0x00690030 came from the A3 advisor ($S/w22/retry_shared/advisor/00690030/final.cpp; test from there too).
- 0x00E02470 / 0x00E02950 share the catalog name CInfectedBalverine::Main but are different script classes (check the symbol binding).
- 0x00537956 (not a chunk-289 source) was seen with `volatile` in an earlier lane file. Check it is not landed.

# Chunk 792 lessons
- An explicit empty `case` keeps its own jump-table entry only if it emits something: VC7.1 folds `case A: case B: break;` into
  the default (one entry fewer). A store (here `addEvent = false;`) keeps the separate index (0x00AB6E40).
- Retail's single shared "add the event" block after a switch is a flag tested after the switch, not the same call pair in every
  case (cross-jumping only merges the last few instructions) (0x00AB6E40).
- A `count = 1` store that precedes a local's constructor call means the variable is declared before that local.
- `new T[n]` emits scalar `??2` unless `operator new[]` is declared; retail binds 0x00BFEB22 -> RELOC_IDENTITY otherwise.
- A C2DBoxI whose stores sink between argument pushes is a temporary built in the call (`&C2DBoxI(...)`), not a named local.
- A two-function oracle row (0x00B5AF80: the row runs past `ret` into a 10-byte setter) has to be graded against a cut copy.
- The CRenderStateManager bookmark test/or flip point depends on the whole function's IR (moving statements before or after
  the setters moves it), so it cannot be fixed locally at the flipping block.
- `C4DVector(tint * contrast * 0.5f)` through CRGBFloatColour's operators gives retail's temporaries for the colour filter.
- Never name a helper script after a stdlib module (`dis.py` in the grading script's directory broke q.py's imports).
- VC7.1's vector<unsigned char>::insert(it, const char*, const char*) picks the (it, n, value) overload: tests must push bytes.

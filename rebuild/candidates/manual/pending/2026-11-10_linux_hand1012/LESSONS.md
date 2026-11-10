# Chunk 1012 lessons (GOTCHAS-ready)

- A shared-header TU that needs an overlay stlport opt-in must `#include "stlport/fable_containers.h"` before fable_string.h. Its quoted include otherwise resolves to the repo copy next to it (0x0049FFD0, grade 1 SRC_FAIL).
- `std::compose2(std::logical_or<bool>(), HasTC(a), HasTC(b))` inside GFFilter_Bind2 reproduces the Ego_r-mangled GetPBestThing instance 0x004AC2C0; add a same-TU noinline GetPBestThing body (0x0049FFD0).
- Write `source = ...; if (PathExists(source)) CopyFileInternal(source, path);` in BOTH arms of the save-type test. c2 then hoists and cross-jumps the pair as retail does (0x004A0780).
- A def index read as a named local (`long theme = GetDefGlobalIndexFromName(..); p->SetAtmosDefIndex(theme);`) keeps retail's order (0x006B2260).
- A same-TU noinline `GetDef<T>(type, index, out)` body drops the release null test and fixes the loop slots (0x006B1A20).
- A retail vector copy that is CALLED (0x008AC540) but whose result keeps start/finish in ebp/edi across the loop means VC7.1 saw the copy ctor's body. Give it a same-TU `__declspec(noinline)` out-of-class body (FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE): nd 96 -> 7 (0x008A8260).
- Retail `mov esi,eax ; lea ecx,[esp+N] ; mov [esp+N],esi ; call IS_THING_ALIVE<CThing*>` with esi reused afterwards means the const& argument binds a temporary: write `IS_THING_ALIVE(&*thing)`. The temporary homes in the dead argument slot (0x0062BEF0).
- `push const` scheduled AFTER the `GetPlayer` call, for `GetPlayer(x)->HasMode(c)`, means a named pointer in its own block: `{ CPlayer* p = GetPlayer(x); if (p->HasMode(c)) return; }` (0x0062BEF0).
- Frame packing for address-taken locals is liveness-based, not scope-based. Still, a local in its own `{}` block can drop a frame slot where the same local at function scope conflicts with everything (0x0062BEF0 camera manager: 0xC -> 8).
- Packer node order among equal IL weights is FIFO by when each node REACHES that weight (the walk is in source order). A late-referenced `this` therefore sorts after an earlier local of equal weight, and a third node joins the later slot (0x0062BEF0, phook trace).
- Two frame slots of equal density (sum of node weights / size) can come out in either order. Change a node's IL reference count, not its declaration order (0x004A34F0: path 11 vs fullPath 7 + heroState 4).
- The 0x004A34F0 catalog extent runs 17 bytes into AutoLoadInternal 0x004A3730 (`jmp LoadGameState`). Grade with QLEN=564 and record it in SPLITS.tsv.
- c2 constant-folds `return found;` on a path where found has a single reaching definition (`xor al,al`) and drops its ebx piece. A retail `mov al,bl` in a duplicated early-exit epilogue is not reproduced by a second `return found;` (0x008A8260, open).

# NF54 lessons (one line each)

- "Folded with the long instantiation" can be checked: follow the grow callee (vector<unsigned long>::_M_insert_overflow 0x00464ACE vs vector<long>'s 0x00440990). Different callees mean different COMDATs, so no fold.
- A callee reached through a `std::vector<T>&` parameter (in the PDB signature) cannot be a CArray<T> wrapper. Use the caller's PDB parameter types to settle vector-vs-CArray claims.
- A long converted to an unsigned long argument creates a temporary (`mov eax,[esp+X] ; mov [esp+Y],eax ; lea`). Retail's `lea` of the local itself shows the local already has the element type.
- "Zero two floats before the base call, so not a constructor" is wrong when the floats are a member with its own constructor (CAnimInit::Frame). Member construction runs before the body.
- A size-optimised TU (/Os, or `#pragma optimize("s")`) leaves the two-level STLport `_Destroy -> __destroy` dispatch out of line, while retail expands it. /O2 expands it but pops with `add esp,4` instead of `pop ecx`. Forceinline the two wrappers only.
- Check the catalog row for a sibling's `// Flags:` line (0x005BC1B0 has `/Os /Oy-`) before trying pragmas for a size TU.
- Removing a stand-in block with a scripted cut can leave half a comment line behind (0x00957F20 grade 3). Cut whole lines.
- Include engine/C3DVector.h (math.h) before engine/CThing.h when FABLE_RETAIL_CINTELLIGENTPOINTER is on, or math.h fails with C2365 'fabs'.
- A local `enum ETCInterfaceType { };` is a drop-in swap for `#include "engine/ETCInterfaceType.h"`: the mangling is identical and the CVectorMap LowerBound binding is unchanged.
- A heredoc with backticks inside a python replacement string runs them as shell commands. Use `<<'EOF'` when the text has backticks.
- A row whose catalog name is wrong (0x004A6AB0 is ~CWorld but catalogued ~CBase) grades only through q.py's "largest function" fallback. Add a `// Leaf:` line.
- STATUS / APPLIED written after every row survived without loss. Recording the pO (landed) grade before rec.sh is what fills RENAMES section 2; record it first.

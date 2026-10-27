# Chunk 503 lessons

## GOTCHAS one-liners
- Some catalog "functions" are only jcc targets (no call or pointer reference) that sit right after the previous body's `ret`, and they read esi/edi/ebx/ebp without setting them. They are blocks of a bigger body. Scan every branch encoding for the target before authoring, then SKIP the row (0x00A0F6EA, 0x00A0F98B).
- VC7.1 addresses a `T(...)` temporary bound to a const reference through the constructor's eax. A named local in its own `{ }` block is read straight from its frame slot and keeps the same build-before-the-call / die-before-the-test order (0x009ECB70 ".ini", 0x00836C50 "", 0x006299C0).
- An `if (x == 1.0f) out = T(a, b(), c); else out = T(a, b(f), c);` pair is tail-merged by VC7.1: the scale is pushed first and each arm pushes only its own argument. A single assignment with a `?:` argument evaluates the conditional before the scale (0x004BC670).
- A return of an `&&` chain (`return p && !p->dead && q && !q->dead && a < b;`) gives one `mov bl,1` / `xor bl,bl` result with a single destructor tail. if/return duplicates the destructors of by-value arguments (0x00926980).
- After `fld x ; fcomp y`, `test ah,5 ; jnp <true>` is `x < y`, `test ah,41h ; je` is `x > y`, and `test bl,ah ; je` (bl = 1) is `x >= y`.
- `cmp ecx,1 ; je` before an unrolled pointer loop is a `for (i = 1; i != size(); ++i)` loop; `<` gives `jbe` (0x007F2120).
- If a counted pointer's operator= keeps the source's info in ebx and its data in a frame slot across the release, the source read both into locals before the self-compare. Reading the members after the release re-loads them (0x00A49E00).
- If retail keeps a list node in a register and re-reads `node->_M_data` at every use, the source holds an iterator, not an element pointer (0x0090AF80).
- An empty iterator tag passed to an out-of-line algorithm needs a no-op user constructor, or VC7.1 stores the tag byte before the call (0x00809650, the same as __false_type).
- Byte-identical accessor COMDATs (0x00487DC0 / 0x00487DD0, both `add ecx,2Ch ; jmp GetPItem`) carry different PDB names. Use the name the bindings already give each address, or vland reports RELOC_CONFLICT.
- A PDB method list is the fastest identity: CMockSpline (2 methods), CThingTrackNode::ConstructFromParams, LSocketServer::GetUDPPacket and OrderByHeight (the only function of its pointer type) were each named from the layout plus one signature.
- In tests, `#define std tstd` after the C headers lets an STLport model compile under g++ next to libstdc++. `__int64`, `__fastcall` and `__declspec` need shims for g++ only.

## Review at landing
- 0x0099A140 defines CUnprovenKey_00999e50 as one `char` (byte copy of the node value). The landed family header says `long`; reconcile them.
- 0x00A0F6EA / 0x00A0F98B should leave the function catalog: they are blocks of 0x00A0F4B0.
- 0x004C3200 is C3DMeshObject::~C3DMeshObject and is unbound. 0x004BC670 calls it by that name.
- 0x0053B717 inherits the CList(long) vtable-install device. Fix it together with 0x0053B63E.
- The 0x00487DC0 naming choice (GetPPlayerCharacter) follows the binding of its twin 0x00487DD0 (GetPControlledCreature). The bytes cannot decide it.
- vector<char>::resize 0x004122D0 and CThing::GetPosition 0x004C73D0 have only stand-in bindings. 0x0090AF80 and 0x00926980 use the plain STLport / PDB spellings.

# Chunk 295 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Global wipes as `xor ecx,ecx` / `xor edx,edx` plus dword stores interleaved between two blocks, while plain int zeros use the function's ebx, come from intrinsic `memset(&g, 0, sizeof g)` calls. Each expansion gets its own zero register and the scheduler interleaves them. Float or double assignments give immediates or fld/fstp, and struct temporaries stay on the stack. The order of the surrounding int stores relative to the memsets sets where the ebx stores land (00c3fd20).
- A polymorphic object whose ctor stores a field BEFORE the vfptr means that field belongs to a non-polymorphic base. MSVC puts the vfptr at +0 and the base at +4, and the base ctor runs first (00c3fd20).
- A positive bool loop test whose temporaries die before the test is `test al,al ; setne bl` only when written `f(...) != false`. A plain `while (f(...))` gives `mov bl,al`, and `== true` gives dec/neg/sbb/inc (00d35ed0, 00e631f0).
- The second inline SpawnFunction in a long body falls out of line. Mark the CSpawnedFunc ctor and SpawnFunction `__forceinline` (00da05d0).
- A dead entry store `mov [esp+X],0` with no later flag test is a conditional-temporary flag whose only user was removed. A constant-false `if (false) SpawnFunction(new CSpawnedFunc(...))` reproduces it, along with the loop-alignment padding (00da0230).
- `fcomp ; test ah,0x41 ; jnp` around a loop is `while (!(x <= c))`, which is true on NaN; `while (x > c)` gives jne (00df4be0).
- A scripted-thing handle ctor called with ecx = handle+4 means the CBaseObject model lacks its vptr (retail vtable 0x0129A7D8) (00d496f0).
- An early `if (x > 0) return;` instead of `if (x <= 0) { ... }` fixed a `pop esi` scheduled into the trailing inline dtor and also swapped eax/edx in an unrelated vcall. Block structure changes register colouring (00d9bb50).
- STLport `back()` as `*(end() - 1)` through an inline end() emits `add eax,-4`, while `*(_M_finish - 1)` emits `sub eax,4` (00d6cdc0).
- A local CTimer needs the INLINE ~CTimer body (DeregisterTimer) even though retail calls the out-of-line copy; only then is the index cached in a register. `v.front()` and `v[0]` schedule an interface reload differently (00d736b0).
- A pre-loop terminate test and the loop-bottom test sharing ONE exit block = top-tested `for (;;) { if (IsActiveThreadTerminating()) return; ... NewScriptFrame(); }`. `#pragma inline_depth(0)` / `#pragma inline_depth()` toggled INSIDE a body chooses which return sites get the inline ~vector / ~CTimer and which call the out-of-line copy (00d20790, 00d9b420, 00da0820).
- A count loaded into a callee-saved register BEFORE an out-of-line temporary's ctor = the one-argument inline `resize(n) { resize(n, T()); }` wrapper (00d347b0).
- A CRGBColour passed by value and built in its argument slot needs a `__forceinline` ctor AND a `__forceinline` copy ctor. CCountedPointer::operator= reads the source's Info and then Data into locals before the compare (00d732f0).

Review at landing:
- 00c3fd20 is not a quest script. Its class is unnamed (`CUnknownSystem1D8`, owner of 0x1D8 bytes, ctor 0xC3E520). The landing session should find its real name (EgoCore / FSE / the 0xC4D3B5 caller) before filing it. Its Leaf is `?Init@CUnknownSystem1D8` because a bare `Init` also matches the undefined InitSubSystemA/B and CSystemDevice::Init symbols.
- 00d496f0 and 00df4be0 carry catalog names ("RunSaveXPCutscene2") that differ from line 1. The leaf comes from line 1 (00df4be0 also has `// Leaf: Main`); both were checked against objdump symbols.
- No quest-script class in this chunk has a rebuild/include/engine header, so all sources use local models with `// Layout:` evidence.
- Several rows took more than 4 grades:
  - 00d20790 (pragma/loop shape)
  - 00d732f0 (CRGBColour)
  - 00d736b0 (CTimer)
  - 00c3fd20 (6 probes: memset discovery and statement order)

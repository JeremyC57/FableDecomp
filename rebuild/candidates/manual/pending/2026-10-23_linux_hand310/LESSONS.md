# Chunk 310 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Retail keeps values in callee-saved registers across a call, but ours reloads them:**
  - Define the callee in the TU under `#pragma auto_inline(off)`, e.g. Compare_00411570 or CDefinitionManager::GetDef<T> 0xCDE280.
  - VC7.1 then knows the callee does not write memory or leak addresses, and keeps the values in registers like retail. This fixed 00ee2a80 and 00ec2240.
- **Many uses of a small inline helper:** VC7.1 emits CCharString::operator== in long compare chains, and the handle operator= (`CBaseObject::operator=(o); PImp = o.PImp;`), out of line. Mark them `__forceinline`.
- **CCountedPointer operator= that matches retail:** copy `other.Info` and `other.Data` into locals before the compare and Release.
- **Scope-end destructors and inline_depth:**
  - Put `#pragma inline_depth()` after a block's closing brace when retail calls that scope's destructors out of line.
  - At function end, an explicit `return;` inside an `inline_depth(0)` pair has the same effect.
- **for(;;) with a top terminate-return vs `while (!IsActiveThreadTerminating())`:** the choice decides which inline cleanup copy jumps and which falls through. It fixed 00ea8c20, 00edbd80 and 00ed7320.
  - When the loop's bottom check is "term; je top" and there is no reversed copy of the loop condition, the loop is `while (!IsActiveThreadTerminating())`. Put the post-loop statements inside the exit branch (00e8cb40).
- **By-value CCharString argument built from a virtual that returns CCharString:**
  - Declare CCharString's copy constructor. Without it, VC7.1 builds a temporary and bit-copies it into the argument slot instead of constructing it in place (00e6da80 `GetVictimIsGuard(victim.GetDefName())`).
- **Cut-scene argument map (`map<CCharString,CCharString>`):**
  - It has the same inline header-node model as the cast map, but malloc(0x18).
  - Its operator[] is 0x009AC700 and its erase is 0x009AA830.
  - Both maps' destructors inline as `if (count) { erase(root); reset header; count = 0; } if (header) free(header);`.
- **A by-value CScriptThing temporary from GetThingWithScriptName:** `.IsAlive()` and `.GetPosition()` are virtual calls (slots 75 and 6), and the destructor 0x004AA840 is a plain out-of-line call. Declare these as non-inline virtuals.
- **The `// Leaf:` line must hold only the symbol.** finish.py matches `^// Leaf: (\S+)$`, so any trailing note silently drops the leaf. Put notes on a following `// Label:` line.
- **Engine STL (AddBlocks):** vectors are 12 bytes and lists are 4 bytes (a node pointer only), so retail = Ego_r - 0x24 for CVertexGroupBlockOptimiser.
  - Hoisting a loop-invariant size needs an explicit `n = size()` local.
  - Reusing a dead parameter for a counter reproduces retail's use of the parameter slot (`[esp+0x54]`) and its frame size.

Review at landing:
- 00edbd80, 00edcb80, 00ee2db0, 00d2f7f0, 00da3f40, 00e1a990 and 00deb000 land under their real class names (leaf given). Correct the catalog and manifest labels listed in NOTES.md.
- 00ed94d0 (one instruction) and 00ad9f10 (register allocation) are parked DIFFERs in differ/.

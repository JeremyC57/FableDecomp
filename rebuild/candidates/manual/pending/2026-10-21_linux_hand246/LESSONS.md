# Chunk 246 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Identical `Send()` arms in a ProcessInput switch are cross-jumped by VC7.1; under /GS the payload lands in the dead event slot. Write each arm out in full and let the compiler merge them; do not hand-merge them. (00690b20, 00691680)
- CScriptThing pimpl handle: define GetPos / SetDataString / IsAlive as inline virtuals inside the class body. VC then devirtualizes the calls on stack locals, and retail shows direct calls. (e4a740, e4b010, e4b430)
- `if (cond) return Temp().IsDead();` with an inline `virtual ~CScriptThing(){}` puts the dtor inline at the first site only; later sites call it out of line. A block-scoped spawned local shares the temporary's frame words. (BanditLeader, e4a740)
- A local vector's dtor is inline at the fall-through end and out of line at the early exit. Scope the vector in `{}` so its dtor runs before NewScriptFrame. (TauntingBandit, e4b430)
- Passing a by-value CCharString to a `CSpawnedFunc2` gives the conditional-temporary flag byte. (e4b430)
- Scoped lock struct with an inline BeginCritical: the dtor is out of line only at the merged failure exit. (a02220, a02730)
- `while (x < y) { if (!a) break; if (!b) break; }` produces a rotated loop. An empty-body `while (a && b && c);` does not rotate. (a03180)
- `(unsigned char)~(flags >> 1) & 1` gives the not/shr/and byte sequence. (689c40)
- `!(GFGetTime() <= start + timeout)` gives `fcompp ; test ah,1 ; je`. (68f320)
- In Defragment-style code, put pointer/flag stores before the memset. Name the locked pointers as locals before the memcpy, because args are evaluated right to left. (ce1af0, ce4600)
- `v * (1.0f / len)` homes the reciprocal in a dead argument slot; `v / len` does not. (d73970, d73ae0)
- When the class has a user copy ctor, copies are memberwise while assignment stays `rep movsd`. Reset before the first user-copy of `prev` to get retail's order. (690fd0)
- VC refactors `value += n*w*value` into `value*(n*w+1)`. A separate increment statement keeps retail's two multiplies. (690830)
- CKeyRedefiner:
  - a reference local reproduces the hoisted address
  - switch cases written as `if (differs) break; return;`
  - the index declared between the copy-out and the assignment
  - the parent list named as a local before the call

  (e5e0c0, jump table)
- If the success path is nested, the pushes stay at entry and the false exit is shared. (009ff710)
- Copying a counted pointer in a full expression gives the +1/-1 refcount sequence inline. (be3660, a57f70)
- A same-TU `#pragma auto_inline(off)` stand-in for a callee (LowerBound / GetDef / push_back / find) changes frame-slot sharing and the home slot. It fixed frame size in 0053822b and 00868ce0, and the home slot in 00be7be0, but none of the three reached a full match.
- A size-optimised frame-pointer TU needs `#pragma optimize("s", on)` and nothing else. (536c64)

Review at landing:
- Several rows use local models even though an engine header exists. Each has a `// Layout:` comment with the evidence. Check whether the shared header should absorb them:
  - CHandleBasedMemoryPool
  - CList
  - CInputProcess* (polymorphic local models with only the vtable slots used)
  - CLandscapeBackgroundTreeNode (member offsets shifted by 8 bytes against the PDB header)
- These rows needed more than 4 grades: e4b430, e4a740, e5e0c0, 690b20, 691680, a02730.
- The landed source for 0x0041E5F2 is asm_bake. It is an oracle, not reconstruction, so it should be re-graded.
- The landed CMainGameComponent_AddGameEvent_0041649c is built with __emit. It should be re-graded as asm_bake.
- CheckSync carries a DIFFER grade but sits in compiled/. It needs reconciling.

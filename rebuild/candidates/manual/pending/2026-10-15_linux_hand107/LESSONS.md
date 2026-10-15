# Chunk 107 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A negated one-bit bitfield that retail materialises (`mov cl,al ; not cl ; test cl,1`) inside an `&&` chain is an inline
  bool accessor (`bool IsAlive() const { return !Dead; }`); the direct `!Dead` lets VC merge neighbouring bit tests into one
  mask (`test cl,21h`) (0x004C7B80).
- `movzx eax,cl ; test eax,eax` after a byte branch is an inline helper returning LONG built from an unsigned char member
  (`if (!reasons && flag) return 1; return reasons;`) inside the caller's `&&`/`||` chain (0x004C78F0, 0x004C7BF0).
- A by-value call result (hidden-pointer C2DVector) read through the returned eax is consumed by an inline operator taking it
  by const reference (`physics->Get() + physics->Position`); naming it, even as `const C2DVector&`, re-reads the frame
  slot (0x004C7950).
- A bool argument pushed from a frame byte (`mov [esp+0Ch],al ; mov eax,[esp+0Ch] ; push eax`) is a NAMED bool local
  holding the value; written inline as the argument it becomes a `movzx` register push (0x004C7FF0).
- A colour (or any byte quad) stored straight into the destination in R, G, B, A order is an inline `Set(r, g, b, a)`;
  assigning a constructed temporary builds it in a frame dword first (0x004C74B0).
- A member pointer used as `this` of a null-tested virtual call lands in ecx when fetched through an inline accessor into a
  condition local (`if (T* d = GetTCDriver()) d->F(x);`); the member form goes through eax (0x004C7540).
- The GetInterface out-parameter template is sometimes NOT inlined by VC7.1 inside an if-arm (it emitted a call to the
  template): mark it `__forceinline` (0x008337B0).
- 14-byte two-byte-aligned POD members copied with `movsd x3 ; movsw` in an operator= are a `#pragma optimize("s", on)` TU
  (the speed form copies through eax/ecx dwords); the unaligned function start (0x00464536) is the hint.
- An out-of-line `bitset::test(n)` with `mov al,1 ; jmp` to a shared `pop esi ; ret 4` tail is a size TU (0x007890EA).
- CThing retail layout (no engine header): PDB offsets minus 0x10 from Components on (CVectorMap 16 bytes, STLport list 4);
  flag bytes +0x91/+0x92/+0x93 follow the PDB bit order exactly; InitialisationStage:3 at +0x94.

Review at landing:
- No row needed more than 4 grades. Two rows parked (004c8c90 register choice, 00830260 store scheduling).
- engine/CThingAICreature.h exists but is all `_pad_*` with PDB offsets (0x30 high at the tail); 006a9520, 0082fe60,
  00830320, 008337b0, 00833a10 and differ/00830260 model the class locally (as the landed 0x00833BA0 does).
- Several catalog labels are propagated (see NOTES "Label corrections"); the sources keep the catalog names on line 1
  so the payload module/name match the catalog, and say so in their Identity comment.
- Landed 0x007512C0 / 0x00617D30 (CThing GetTCDriver<CTCDExperienceOrb>) use old `char pad`/`f28` free-function models
  (`__fastcall` self + dummy edx); worth retyping to the member-template form used here.

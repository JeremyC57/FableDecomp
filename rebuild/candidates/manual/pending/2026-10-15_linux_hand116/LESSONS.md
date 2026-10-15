# Chunk 116 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A vtable store followed by `xor ecx,ecx ; lea edx,[eax+4]` and stores through edx is `memset(m_array, 0, sizeof m_array)` in the constructor body (the intrinsic materialises its destination; init lists, loops, pointer locals, inline Clear() and STLport vector models all fold onto eax) -- 0x00C39F10/0x00C3A110/0x00C3A310.
- Zero stores through `xor eax,eax` before `mov esi,ecx` plus an IMMEDIATE zero store after a call = `memset(this, 0, sizeof *this)`: the intrinsic's zero register is private, while plain zero stores make VC keep 0 in ebx across the call -- 0x00C90F10.
- Check eax at `ret` before modelling a constructor: an "init" whose eax ends as a copied word or a call result is a void member (0x00AF07A0 SetDefault, 0x00C90F10 Init); the void form also frees eax and changes the register choice.
- `Up = C3DVector(0,0,1)` (assignment from a temporary) gives the store-immediate / reload-as-dword / store pattern through a 12-byte frame slot; VC does not forward a float store into an integer load -- 0x00AF07A0.
- VC7.1 multiplies a product chain from the LAST operand: `bpp * W * H * D` emits `imul D ; imul H ; imul W` -- 0x00A674E0.
- A 12-byte hidden-pointer result copied through ecx/edx/ecx is a value constructor `return C3DVectorI(w, h, d)`; field stores into a POD local copy through esi/edi -- 0x00A674B0.
- A def-module virtual Copy slot under optimize("s") must be written as an explicit memberwise copy: the implicit operator= is out-lined under /Os and the slot collapses to a `jmp` -- 0x0045E620.
- A member pointer used for a store and then as `this` of a virtual call is one named local (two member reads reload it after the possibly aliasing store) -- 0x00C336B0.
- STLport `operator[]` as `*(begin() + n)` also fixes element ASSIGNMENT through an intelligent pointer: start loaded before the GetPItem call, index scaled after -- 0x007B85F0.
- A memberwise COM copy `if (other.m_p) m_p->AddRef();` re-reads the OTHER's pointer for the test and AddRefs through this copy -- 0x00A67DD0/0x00A682A0.

Review at landing:
- 0x00C39F10 and 0x00C3A110 were graded more than 4 times in total: their own 4 forms failed; the memset form was found on 0x00C3A310's 4th attempt and transferred to the twins (one grade each).
- Local polymorphic models sit next to existing engine headers: CWADFile.h (0x00A39B40/0x00A39B70/0x00A39C80 -- the header flattens the vptr), CVillageTaskCarryCrate.h / CVillageTaskPatrol.h / CVillageTaskSecuritySweep.h (vptr and CIntelligentPointer members flattened), CVirtualKeyboard.h (CWideString flattened to its data pointer), CParentDefClassBase.h (retail base ends at +0x3c, 4 bytes below the PDB). The non-virtual 0x00A39BB0/0x00A39C40 use the CWADFile.h header directly.
- 0x0099A380 is landed as "CBaseIntelligentPointer::CBaseIntelligentPointer" (vtable 0x0129A7D8 + null +4) but it is the CVillageTask base constructor here (folded body): check that landed name.
- The SGI find front end (0x007FE670) and the counted-handle Clear (0x00A69AD0) look like QFE-vs-RTM scheduling/CSE gaps; recheck on the retail toolchain.

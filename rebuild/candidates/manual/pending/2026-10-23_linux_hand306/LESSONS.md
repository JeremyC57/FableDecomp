# Chunk 306 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Frame-slot packing follows lexical scope. A local or temporary whose scope encloses an inline GetTC/GetInterface key temp cannot share that key's slot. Moving a local into its own `{ }` block, or passing a literal as an implicit `const CCharString&` temp, lets VC7.1 reuse a dead argument slot or the slot of a later local, as retail does. This fixed 0065c781 (emitter local), 007a09b0 (sound name), 007c6aa0 (table/effect locals) and the 0x4B key of 007c7a00.
- Passing a string literal straight to a `const CCharString&` parameter (implicit conversion) builds the string before the other arguments are pushed, as retail does. An explicit `CCharString("...")` temp is built after the pushes. This fixed 007d04f0, 007d00f0, 007c7c40 and 007c6aa0.
- Declaring a CCharString user copy ctor makes a by-value CCharString return (GetText) construct directly into the argument slot.
- Same-TU stand-in bodies under `#pragma auto_inline(off)` for an out-param def getter (GetSubDef 0x00669580 / 0x007D1260 / 0x007D4BD0, GetDef 0x007A35D0) tell VC7.1 that the def's address does not escape. The def then stays in a register and the null test before the release folds. Use one only when retail shows the def in a register with no null test (007a09b0, 007c6aa0, 007d2cc0 releases). It does not apply to 008efb00, where retail keeps the null test.
- `CGameDefinitionManager::Get` 0x0044C6B0 is `static CGameDefinitionManager& __fastcall Get()`, and GetGlobalIndexFromClassIndex(name, 1) takes a named CCharString in its own block. This gives retail's ctor/push order (007a09b0, as in the landed 0x008BC790).
- The register order in a `HasTC(n) && GetTC(n)->field <= 0` test depends on the GetTC form. The `GetInterface(&ptr, n)` out-param form gave retail's eax=component / ecx=field (007a09b0).
- The dword HasTC `return bits[i>>5] & (1UL << (i&31));` (returning unsigned long, no `!= 0`) gives retail's `test dword` / `test byte` / `mov cl; test cl,cl; jns` forms. The bool `!= 0` form emits `shr; test cl,1`.
- In large functions, HasTC, IS_THING_ALIVE and handle-reset helpers must be `__forceinline`, or VC emits an out-of-line call (007bb1f0, 007c6aa0, 0076e6a0).
- `abs()` (intrinsic) gives the cdq/xor/sub form (006cf530).
- VC7.1 in this lane has no native wchar_t: use `const unsigned short*` for the CWideString ctor (006ddd30).
- A virtual `~CBaseIntelligentPointer()` as slot 0 gives the `push 0; call [edx]` virtual dtor call in vector pop_back (007c7c40).
- An inline accessor on the def-pointer wrapper used at one site, plus a named `long mapIndex = ...;` local, fixed the register rotation in 007e00d0 (advisor). A `CThing* parent = ParentThing;` local gives `lea ecx,[eax+0x74]`. Named CIntelligentPointer locals instead of temporaries make VC recompute the address instead of reusing the ctor's eax.
- A derived-to-base conversion temp (`IS_THING_ALIVE(CThing* const&)` called with a `CThingPlayerCreature*`) keeps the pointer in a register (0065c781).
- An explicit C3DVector copy ctor `X(o.X),Y(o.Y),Z(o.Z)` gives member-wise mov copies; assignment stays movsd (0065c781).
- A CRefCounted second base with a `__forceinline ReleaseTemporary` gives the `add eax,4; dec [eax]; cmp [eax],0` release, and `AddRef()` with a named `result` copy gives the inc/inc/dec def copy (0065c781, 0065ad37).
- Under `optimize("s")`, CDefPointer members must be `__forceinline`. CTCVillage rows are size-optimised TUs with an ebp frame.
- The ternary GetTC form `(T*)((it == last || t < it->type) ? last : it)->component` gives the `je; cmp [eax],edi; jg` shape (0065cb6b, 007a00e0, 007a09b0 0x6F).
- An 8-byte CIntelligentPointer local must be modelled as a base with `virtual void v00()`, not a `void* _vfptr` member plus a derived virtual (12 bytes), or the frame alignment changes.
- An `end` local for a list loop gives the jmp-to-test loop form. A do-while avoids loop rotation. An out-param initialised `= 0` gives `xor edi,edi`.
- Open residue family (not solved): the operand order of a strength-reduced index `lea ecx,[ebx+eax]` (retail) v `mov ecx,[esi]; add ecx,ebx` (007c7a00), and the x87 cross-product operand order (007a1870, 008efb00, 008f18c0).

Review at landing:
- src/007a09b0.cpp and src/007c6aa0.cpp each carry a same-TU stand-in body for 0x00669580 (auto_inline off). The lander must take only the target symbol (`// Leaf:` is set).
- src/007a09b0.cpp uses `#pragma pack(push, 1)` for the real unaligned PItemRemoved (+0x11), the same model as the landed ~CTCWillLightning 0x007A07F0.
- The tests for 007a09b0 and 007c6aa0 are plain-struct behaviour models. They passed under g++; check the tall.py (VC7.1) run.
- 007d4410's catalog class (ShoveHaPenny) is wrong: the body is CTCTavernGameCoinGolf::UpdateValues.
- 007a1870 is advisor-routed: it sits in differ/ only.

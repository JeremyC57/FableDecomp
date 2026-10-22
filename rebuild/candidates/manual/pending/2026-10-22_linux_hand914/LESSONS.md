# Chunk 914 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A catch clause's type and frame slot are in the EH FuncInfo (stub `mov eax, funcinfo; jmp __CxxFrameHandler`; tryBlockMap -> HandlerType {adjectives, pType, dispCatchObj}). A catch(T&) with dispCatchObj != 0 must be written as a NAMED reference; `catch (...)` or an unnamed `catch (T&)` drops the 4-byte frame slot. (CWorld::Load 0x004A1840: named `catch (CBBBFileException& exception)` turned `sub esp,0A0h` into retail's 0A4h.)
- A def or counted temporary in a `switch` condition keeps its dtor's `m_p = 0` null store. Writing `long type = GetDef()->X;` as a named local lets VC7.1 drop the store. (0x0083CD70)
- Counted-pointer returns from a sub-def lookup only reproduce with a same-TU `#pragma auto_inline(off)` stand-in for GetSubDef<T> that writes only its out pointer. (0x005D93B0, 0x005D9430)
- A function that ends in a noreturn `_CxxThrowException` call keeps a compiler int3 that the chunk oracle trims as padding, so it grades DIFFER(N+1vN). Check with the int3 appended; if it matches, land against the N+1 oracle. (0x00A0D1A0, 0x00A0D200; agent273 precedent)
- CCharString arguments and results that retail keeps in their own frame words (`lea ecx,[esp+N]` re-derived instead of reusing eax, built before the other pushes) are named locals in their own `{}` block. (CThing::ConstructFromParams 0x004CA010 GetString/AddTC; CWorld::Load L"" and the offline-data file name)
- A class with a vptr and `__int64` members aligns to 8. When retail puts the first member at +4, model the vptr as a plain `void* __vftable` member and use non-virtual methods. (CBaseVideoRenderer::RecordFrameLateness 0x00CA5400)
- VC7.1 does not emit a sibling tail call (`jmp`) when the call passes the address of a temporary or parameter. The retail `mov [esp+0Ch],&tag ; jmp` in STLport `_M_append_dispatch` 0x00567E80 is not reproducible in the RTM lane.
- In /Gr algorithm TUs, free STLport helpers (`__copy_trivial`, `_Destroy_Range`) are `__fastcall`: first argument in ecx, second in edx. (0x00410CB0..0x00410D60)

Review at landing:
- 0x00A0D1A0 and 0x00A0D200 need 96-byte oracles (int3 after the throw); the sources are in differ/ and match against those.
- 0x004A1840 CWorld::Load needed more than 4 grades and is with advisor AdvB3. It uses a local CWorld model although rebuild/include/engine/CWorld.h exists: the retail vtable slots 2/12 and the members at +0x08/+0x14/+0x84 were modelled locally.
- Byte-identical pairs are bound to distinct leaf names: 0x00449BF0/0x00449C10, 0x004CD220/0x004CD230 and 0x00410CB0/0x00410D40 (identical bodies apart from the tag). Check the lander does not bind one symbol to both copies.
- Local models sit next to existing engine headers: CThing (0x004CA010, 0x004CD1E0; retail offsets, Layout comment), CWideString (0x0099B1D0), and CPlayerManager (12-byte retail vector).
- The 0x00567400 propagated label (CTCElectrocutedEffect::InsertStringRange) is wrong; it is the STLport string append.

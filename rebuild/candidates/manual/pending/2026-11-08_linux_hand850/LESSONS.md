# Chunk 850 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- A by-reference helper for matrix products moves VC7.1's x87 operand order: PostMultiply as `CMatrix3x4 a(*this); SetMultiply(a, m);` (FableWin SetMultiply, __forceinline) gives retail's k3,k2,k1 sums with a's element first where the direct in-place body does not; source term order alone has no effect (0x00772BB0, 248 -> 102 lines).
- An inline wrapper that forwards to a 4-argument inline (CMatrix3x4::Set(rhset, pos)) is called out of line by VC7.1 in a big function; retail expands it -> __forceinline (0x00772BB0, 2161 -> 2585 bytes).
- A PDB member sub-object with its own ctor (CConsoleVariable::Var, a CGenericVarRef at +8) schedules its stores after the vptr store; flattening it into plain members lets the lea float above the vptr store (0x009ED190, 2 lines -> RELOCATION_MATCH).
- CIVCountedPointeeBase has a user ctor `IVRefCount(1)`; the +4 = 1 store comes before the derived vtable store (0x00AC5ECB).
- CIVCountedPointer::Reset adopts: Release() then store, no AddRef (0x00AC5A86).
- A named `header_size = in.GetPosition() - start_pos;` local orders the GetPosition call before the argument pushes of the next call (0x00AC5770).
- A compiler-generated destructor (PDB 0x103) must stay implicit even when retail calls it out of line: declaring it makes the default ctor's zero stores opaque to the push_back copy (ours re-reads and AddRefs; retail stores the known nulls) (0x00500540, CLevelFiles 0x0050F980).
- gp.sh-style wrappers that export FABLE_HDR_OVERLAY themselves silently ignore an experimental overlay: give the wrapper an override variable before A/B-testing headers.
- VC7.1 canonicalises `for(;;){if(!c)break;...}`, `while(c){...}` and a goto top-test loop to the same rotated loop; retail's unrotated jmp-to-aligned-top wait loop (0x00500D98) is not reachable by loop spelling.
Review at landing:
- 0x009EF900 symbol conflict (__false_type in src/009ed190.cpp v __true_type in reloc_bindings and landed 0x009EBB30).
- src/009ed190.cpp uses a local CConsole / STLport model (no engine/CConsole.h exists; same as the landed 0x00419D90 / 0x009ECD80 rows).
- Rows that needed more than 4 grades: 0x00AE3C30 (10), 0x00500540 (10), 0x00772BB0 (13); 0x0050A650 was not rebased (SRC_FAIL).
- Landed source spells 0x00414F30 as CConsoleCommandBase_Retail(const CCharString&, const long&) and binds ??_7CConsoleCommandBase_Retail@@6B@ -- placeholders (NAME_FIXES.tsv).

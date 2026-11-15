# Identity and verification

- Exact symbol: `?ClearAll@CParticle@NParticleEngine@@QAEXXZ`; FableWin name-set entry `02F18EA0`.
- Independent body evidence: original FableWin body at that entry clears ParentHandleList first and PrimIDList second. Debug layouts use a newer larger vector; native accesses +0x90/+0x84 identify the same members with 12-byte retail vectors. Ego_r member records and the adjacent named constructor/destructor corroborate the class and member types.
- Retail span is a complete straight-line function ending in ret, followed by alignment padding. It is not an EH unwind helper.
- Calls are precisely `std::vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle> >::erase` at 00440D70 and `std::vector<CArray<long>,std::allocator<CArray<long> > >::erase` at 00AF7910, both existing exact symbols in names/reloc_bindings.tsv. STLport range erase returns iterator and takes two element pointers.
- COFF symbol table confirms the exact public thiscall void leaf. Both REL32 targets were decoded independently from retail and compared to those bindings; no other relocation occurs.
- Behavior fixture copies the whole method body verbatim. Nine combinations of empty/single/multiple containers check order, element release, retained capacity, unaffected state, and repeated clearing. Both VC71 /Od /W3 /GR- without EHsc and g++ -std=c++03 pass. This is a method-logic test with tracing stubs; layout/ABI evidence comes from the separate retail compile.
- Header addition only declares this original method under a fresh Pass22 opt-in; existing methods remain byte-for-byte unchanged. Shifted assert identities are pinned.

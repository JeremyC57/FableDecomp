# Identity

Correct leaf: `?ResetPrimitiveHandles@CTCInventory@@QAEXXZ`, public nonvirtual member. Ego_r005F6D5C and FableWin0212E959 agree. The original FableWin CloseInventory body at02122680 calls thunk01815038, which jumps to0212E959; that body applies a member functor to the main handle vector then clears it. Retail CloseInventory00589D30 calls this row at00589D52 in the same position. The existing CloseMenus label is wrong and is reported for caller respelling.

The original member is InventoryMainPrimitiveHandles, a vector<CEnginePrimitiveHandle>. Ego_r type records put it at0x198, FableWin accesses it at0x1b8, while native uses0x16c. The existing shared native layout agrees; the new opt-in changes only this opaque12-byte member to its actual12-byte vector type and declares the real member function. Both real bases and all other header paths remain unchanged.

All three relocations decoded independently: Clear0041BE70 (both PDBs: Ego_r004291A0/FableWin019091A0), original STLport std::for_each<CEnginePrimitiveHandle*,std::mem_fun_ref_t<void,CEnginePrimitiveHandle> >0043F360 and vector range erase00440D70. The first relocation is a member-function pointer, not an external free function. The complete Clear COMDAT carried by the real shared header independently matches its own57-byte extent and has no relocations. STLport original vendor source and the original template types establish the functor's class key and hidden-result convention. Wrong historical free-function and struct-functor aliases at0043F360 are listed in NAME_FIXES.tsv.

The row's45-byte extent is complete, with a final ret at00587748. This is not an unwind funclet.

The behavior test copies the complete method body.21 combinations of array length0–6 and null/shared/distinct resource patterns check clear-before-destruction, one final notification/deletion per resource, empty result, retained capacity, unchanged unrelated state and repeated cleanup. Both VC71 /Od /GR- and g++ C++03 pass. Actual shared-header layout is checked in production compilation.

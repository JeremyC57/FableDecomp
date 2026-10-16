# Chunk 169 lessons
- A def released AFTER a by-value CCharString return (RVO) without re-reading the escaped out pointer: copy `out.Object` into a tiny holder whose destructor releases it (0x00750240).
- A selected counted pointer read twice through `operator->` (test, then use) gives retail's `mov eax,[eax]` register reuse; a named shape local rotates eax/edx (0x00760910).
- `AddNewShape(CreateShape(type, points))`: a by-value CCountedPointer result is built straight in the next call's argument area (`sub esp,8 ; mov ecx,esp`).
- CEnginePrimitiveHandle member dtors in size TUs need `__forceinline` GetRefCount() (`Object ? RefCount : 0`), IsUnique and Release (0x004DC9D2, 0x004DE44D).
- A bool member assigned from a float && chain needs `? true : false` to stay 8-bit (`mov al,1 / xor al,al`, tail-duplicated return).
- STLport list push_back = insert(end()) with `prev = pos->_M_prev` in a local; reading `pos->_M_prev->_M_next` re-reads the field.
- STLport list in the retail CTCScriptedControl is the bare 4-byte sentinel pointer: Locked +0x18 (PDB +0x20).
- CTCQuestCompletionUI retail sits 20 bytes below the PDB after its containers (PBlack +0xBC, QuittingFadeOut +0xC0).

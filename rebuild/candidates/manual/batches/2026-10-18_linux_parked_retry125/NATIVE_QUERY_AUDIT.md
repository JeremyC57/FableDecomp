# Native query audit

Read-only disassembly: 0048D400 length0x8A and005ED860 length0x8A call0049C770, validate byte91 flags4+8 set and1+20 clear, then006A80A0 HasTC /006A7E30 HasScriptName. Iterator n->Next and registry.Head are reloaded after the predicate and append, preserving callback changes.

On full output, both pass insertion count1 and end position to00436AC0. At436AC4 size=(Finish-Start)/4.436ADD chooses max(size,count),436AE5 adds size. Thus append-one capacity=size+max(size,1), equal to doubling a full nonempty vector and1 for empty. Allocation436AF5 calls00BFEA0E, the malloc thunk through IAT01440158 (manifest confirms malloc). Prefix copying uses memmove00BFEAE6.436B40..436B48 unconditionally writes the inserted item; there is no null check after malloc and no recoverable failure branch. An allocation returning null is dereferenced during prefix copying or insertion. Old allocation is freed via00BFEA14 only afterward, then Finish/Start/End published at436B85..436B90. Nonfull query append checks Finish!=0 before placement, then advances Finish.

Final candidate queries preserve this operation order and null placement guard. They retain same-TU definitions to preserve the compiler's correct nonescaping vector alias analysis. The actual quarantined CThing donor header is included under CThingDonorLayout. Retail native projection is retained because donor bitset/map sizes shift Components from native44 to donor48 and lifecycle storage from native91 to donor a1..a4. Native offsets asserted; expanded host fixture uses the same actual headers with pointer-size assertions disabled only for host.

Historical reservations remain4 each; observed pre-correction grades1/1/4/4/5. No post-phase extension applies.

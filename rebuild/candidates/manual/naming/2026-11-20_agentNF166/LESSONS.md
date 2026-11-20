# NF166 lessons (one line each)
- Census every REFS row first (probe.py, about 1.5 s per compile). Most PRIORITY entries were already done: 10 of 31 entries were done-already, and the WORKLIST was stale for 3 rows.
- Replacing a stand-in global with an inline static accessor (CMainGameComponent::GetConstantFPS()) can hoist the load ahead of a preceding call in the same expression. A named local for the other operand restores retail's order: 5 rows went DIFFER -> RELOCATION_MATCH.
- A test that links the row and defines the stand-in global goes LINK_FAIL after the respell. Define the class static in the test, and set a protected static through a derived accessor struct.
- When a const accessor chain is respelled (GetRoot const -> GetHelpers const -> FindFirstEvent const), every hop follows. Check that each hop's address has a landed QBE self Leaf before moving it: the PDBs hold both overloads at separate retail addresses.
- Free __fastcall stand-ins for vtable slots (CGameScriptInterface_X) port in one grade onto engine/CGameScriptInterface.h. Define FABLE_RETAIL_NSCRIPT, or the member is "not a member" (C2039).
- FABLE_RETAIL_CPARTICLEPROGRAM_READ_PASS24 needs engine/CDataInputStream.h included first (C2061 otherwise).

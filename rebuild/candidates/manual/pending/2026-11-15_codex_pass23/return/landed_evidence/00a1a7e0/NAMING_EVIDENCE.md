# Original identity and shared models

Both original PUBLIC name sets agree on private const GetIntersectionPointsBetweenNodes: Ego_r00b85d40 and FableWin031a8110, exact leaf `?GetIntersectionPointsBetweenNodes@CNavigatorAStar@@ABE?AU?$pair@VC2DVector@@V1@@std@@ABVCANavQuadTreeNode@@0M@Z`. Native sole caller00a1cd30 independently reaches00a1a7e0 and the hidden16-byte pair result/two reference inputs/float fit ret16.

Both PDBs give GetSize's exact `?GetSize@CANavQuadTreeNode@@QBEMXZ` (Ego_r00bbb7f0,FableWin0327f4a0). Native00a75b30 reads QuadTreeLevel at+0c and returns the indexed size-table float. names/reloc_bindings.tsv binds that same symbol to00a75b30. C2DVector and C2DBoxF are the existing shared classes, with original X,Y and TLX,TLY,BRX,BRY. FableWin01f95350 confirms the real center/radius box constructor; the native four boundary computations independently confirm it. Actual C2DVector user copy constructor and STLport pair are used, with their shared proven class keys.

The original epsilon is `?DELTA_NOTIONAL_ZERO@@3MB`, established at0129ba3c; all6 operands point there and native data has float bits38d1b717. The row's test uses the exact binary-representable decimal value, not an approximate alternative threshold. Literal0.5f operands and object data agree independently. No new callee aliases or naming corrections are introduced.

The fixture requires a declaration of the real private CNavigatorAStar constructor, whose exact signature is `??0CNavigatorAStar@@AAE@AAVCNavigatorManager@@ABVCNavigatorExternalInterface@@@Z` in both PDBs (Ego_r00b88fc0,FableWin031a7370). It is in a separate new header opt-in unused by production. Test access instrumentation and derived fixtures invoke the actual shared method/body; all unrelated virtual operations fail if called.

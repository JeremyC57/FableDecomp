# Chunk 682 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- q.py keys its work directory by address: two probes of the SAME address run in parallel clobber each other's object (both
  report the first one's bytes); probe different addresses in parallel, the same address only sequentially (chunk 682).
- A `GetTC<T>(T** out)` out-parameter form leaves the component pointer in eax and adds `mov ecx,eax` before the member call;
  the pointer-returning `GetTC(long)` (landed 0x006702E0 form) on a `CThing* thing = ParentThing` local lands it straight in
  ecx (0x0078891D prologue exact, 0x00783850 stats call exact).
- A region compare written `GetRegionNumberMapIsIn(podium->GetCurrentMapNumber()) != heroRegion` (no named map local) gives
  retail's `movsx ; push ; mov edi,eax` order; a named `podiumMap` local hoists the `mov edi,eax` (0x00783850).
- In a /Os TU (`#pragma optimize("s", on)`), a temporary vector that must be freed before the next statement is a block-scoped
  local; the slot it frees is reused by the next local only when that local is declared inside a following `if { }` block,
  not after an early return (0x00788617: direction/pair temp slots swapped by the early-return form).
- Empty filter functors zeroed with `stosb` are value-initialised temporaries (`CThingFilter_X()`) inside the call expression;
  a named CFilter_Bind4 initialised from temporaries homes them in dead argument slots and drops their stores (0x0078891D).
- A `vector<vector<T>>` local needs the real out-of-line destructor (declared, 0x007898CC); the inline base-destructor model
  frees it inline and shifts every other vector's frame slot (0x0078891D p19).
- `GetPCombatFirstHitLocationIntersectionWithTriangle` takes `const vector<C3DVector>&`, not a CTriangle: three push_back of
  the current pair's two points and the previous pair's first point (0x00788617; the landed 0x008169B0 header is wrong).
- The static-initializer identity of a CCharString array comes from the .CRT$XCU neighbour order against Ego_r's
  `dynamic initializer for` list for the class (0x0121E260: the two CTCWife initializers before it are landed).
- A user empty virtual destructor over members with inline Uninitialise() destructors reproduces the renderer teardown; the
  FABLE_STATIC_ASSERT offsetof needs `<stddef.h>` under the lane (0x00B513B0 SRC_FAIL at probe 1).
- The x87 order of CSE'd squares (`X*X + Y*Y + Z*Z` shared by GetLength and the fast Normalise) does not follow the source
  order, parenthesisation, named locals, copy-ctor order or the alias-read trick: retail keeps X*X on the stack, the lane
  Y*Y (0x00670820, 31 probes) -- lane gap.

Review at landing:
- 0x00783850, 0x00670820, 0x0078891D parked with 26 / 31 / 23 probes; residues in differ/ line 2 and probe/<addr>.log.
- 0x00788617 and the parked 0x0078891D model CTCTrapBase/CTCMeleeTrap/CTrapDef/CWorld/CHitParameters/CThingSearchTools locally
  (no engine header); 0x00B513B0 / 0x00BBBBC0 / 0x00670820 model classes whose engine headers are flattened.
- Already-landed errors: 0x00788AEF UpdateTrap spells 0x0078891D `MeleeTrapHelper` (`__stdcall`); 0x00BBBBA0 spells the dtor
  `CEnginePrimitiveManagerDecal_dtor`; 0x00985F40 spells 0x013BAB40 `g_WifeAppearanceLinePrefix`; the 0x007898CC file is named
  CActiveFile_OnReadFinished (it is the vector<CPointPairVector> destructor); 0x008169B0 calls its vector argument a CTriangle;
  0x00783730 GetCrowdGroup puts CrowdSizesSuppressed at +0x18 (retail +0x1c).
- Catalog rows 0x00F26250 / 0x00F34330 are labelled `Transfer<signed_char>`: both are TransferVectorOfSubComponents
  instantiations (NAME_FIXES.tsv).
- Two CIntelligentPointer instantiations are told apart by the vtable their constructors store (0x01238C6C `<CThing>` at
  0x004A96F0/0x004A9730, 0x01237F1C `<const CThing>` at 0x0056C860): the landed 0x00788D68 file and three other landed
  sources spell 0x004A96F0 as the const form (NAME_FIXES.tsv).

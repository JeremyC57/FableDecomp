
- (chunk Q17, 2026-11-11) Porting a this/iterator-swap draft onto the shared engine headers fixed the swap with no other lever in 3 of 3 cases (0x009711D0, 0x0093C030, 0x00A87700): port before chasing registers.
- (chunk Q17, 2026-11-11) A visible noinline CDefClassBase::GetSubDef body (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE + engine/CDefClassBase_GetSubDef.h) lets VC7.1 drop the CDefPointer release null test (0x00966030, 0x00965640).
- (chunk Q17, 2026-11-11) `==` operand order follows a named local: `CThing* slot_thing = ...; if (slot_thing == P.Get())` reproduces retail's compare order (0x00966030).
- (chunk Q17, 2026-11-11) GFGetWorld2DDistance needs a named `float sqr_dist` local, or fsqrt is scheduled before the stack pops (0x009711D0).
- (chunk Q17, 2026-11-11) Frame 4 bytes too large with a filter object: name the index in a local and build the filter temporary inside the GetAllThingsInRoughArea call (0x0095B820, 0x13c -> 0x138).
- (chunk Q17, 2026-11-11) Assigning a constructed CKinematic temporary emits an extra x87 store pair; `Output.SetVector(expr)` matches retail (0x00A87700).
- (chunk Q17, 2026-11-11) A byte twin of a landed row matches by copying the twin's spelling AND its `// Flags:` inline budget (/d2inlT62) (0x00795090 from 0x0078FAF0).
- (chunk Q17, 2026-11-11) std::min<unsigned long> shows in retail as `lea eax,[a] ; jb ; lea eax,[b] ; mov eax,[eax]`; a hand-written if/else does not (0x00555A90).

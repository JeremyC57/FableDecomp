# Chunk 526 lessons

## GOTCHAS one-liners
- STLport locals passed by reference to an out-of-line template (GetAllThings / GetAllLeafThings / list::remove / vector::_M_insert_overflow): give the callee a same-TU `__declspec(noinline)` body so VC7.1 sees the address does not escape -- list header stays in esi, vector start/size stay hoisted (0x0091B010, 0x00946F90). `#pragma auto_inline(off)` does not stop a template instantiated at the end of the TU from inlining; noinline does.
- STLport 4.6 `list` has no destructor of its own: the out-of-line call is `std::_List_base<T,A>::~_List_base` (ICF-folded across element types: 0x006FE660, 0x00792240).
- STLport 4.6 `vector::_M_insert_overflow(T*, const T&, const std::__true_type&, unsigned int, bool)` is protected (IAE) and takes `std::__true_type` and `unsigned int` fill_len -- not `__true_type` at global scope, not `size_type`.
- GetInterface as a single ternary `*ppOut = (T*)((it == last || type < it->type) ? last : it)->component;` loads the component straight into ecx; the if/else form goes through eax (0x00938A10).
- A C3DVector with a user-declared copy ctor keeps a by-value position copy in memory and frees ebp for a shared zero; the trivial-copy model enregisters it (0x0091B010).
- `if (list.empty()) return; n = list.size();` -- VC folds the first node walk of size() into the empty test; take the GFRandom index before `begin()` or the register order flips.
- VC7.1 decides inline-vs-call of an STLport `~list` per function, not per return site: we could not get "inline on the empty path, call on the found path" (0x00929FC0/0x00947EB0 parked).
- CDataInputStream's reader order in Ego_r (Read2DVector, Read3DVector, Read3DAngles, ReadMatrix3x4) maps 1:1 onto 0x00995030/0x009950F0/0x00995210/0x00995340; the propagated "CPersistTraits<...>::TransferBinaryIn" label on 0x00995210 is wrong (wrong arity).
- A row can over-span into the next function through int3 padding after a noreturn call: check the true extent (0x0099C7E0 is 306 bytes, row 334) before grading; a parity check on the truncated bytes proves the body.
- `CBasicString<char>::ExtendData` needs `__forceinline` on the HighestSetBit binary search (VC7.1 emits it out of line otherwise) and a local copy of StringLength to avoid a reload.
- VC7.1 has no `long long`: tests that need 64-bit use `typedef __int64 int64;` under `_MSC_VER`.
- tall.py and xdis.py read `$S` from the environment: export S or they die with KeyError.

## Why earlier attempts stalled
- Wrong manifest labels: 7 of the 20 rows carried invented or propagated names (SelectRandomElement, FindMarkerByName, MinionSitAtSchool_Update, SelectRandomEnemy, FindRandomNPC, HandyMan_FindClosestThing, CPersistTraits::TransferBinaryIn, CCharString_Insert). The bodies were modelled after the label and never matched. Ego_r donor order + vtable slots gave the real names.
- ICF-propagated callee labels (0x00772110 GetConstantCount, 0x0091AD70 list<CTCBase*>::remove, 0x0099C550 OnInitialActivate) led to the wrong callee types.
- The STLport locals escaped because callees were only declared (no same-TU body), which spilled the list/vector headers.
- The 0x0099C7E0 oracle row over-spans into the next function, so no source can match its length.

## Review at landing
- RELOC_CONFLICT (vland): `vector<const CThingBuilding*>::_M_insert_overflow` -> 0x00436AC0 here, 0x004AC610 in landed 0x004AD780. Settled for this chunk: retail 0x00947780 (Ego_r donor order: the only GetAllLeafThings<const CThingBuilding, vector<const CThingBuilding*>, CThingFilter_HasTC>, right after WanderAroundShops::Update) calls 0x00436AC0 for that vector. The landed CThingSearchTools_GetAllLeafThings_004ad780.cpp (marked "probable") DEFINES the same GetAllLeafThings symbol, and 0x004AC610 is a byte-identical separate copy, so 0x004AD780 is a different instantiation (non-const CThingBuilding or another container). Respell 0x004AD780 and its overflow callee at landing; do not respell 0x00946F90.
- RELOC_NAME 0x007FEF40 (label FreeActionPoint) and 0x00792240 (label std::list::~list): both corrected in NAME_FIXES (SetActionPointAsFree; STLport `_List_base::~_List_base`).
- 0x013BB6E0 / 0x013BB6E8: two landed names exist (g_GetUpName / g_GetUpNameAlternate from OnReplace 0x009387F0 vs g_CCharString_013bb6e0); 0x00938A10 uses g_GetUpName / g_GetUpNameAlternate. Pick one name per address.
- 0x013CA828 g_DefStringTable: bindings hold both `?g_DefStringTable@@3UCDefStringTable@@A` and `...3VCDefStringTable@@A`; 0x0092E300 uses the V (class) spelling. No pdb_class_keys entry for CDefStringTable to settle struct vs class.
- 0x00772110 is ICF-folded (CTCShop::GetNumShoppers == CShaderPreParser::GetConstantCount); vland RELOC_NAME warning. 0x00946F90 binds it as GetNumShoppers (its real callee).
- 0x00947780 vland RELOC_NAME warning: the landed label is a GetAllLeafThings<CThingAICreature...> instantiation; this body proves the CThingBuilding/CThingFilter_HasTC one (NAME_FIXES).
- 0x0091AD70 list::remove is ICF-folded across element types; the label says list<CTCBase*>.
- Stand-in bodies are not retail code: `FindThingInRoughArea` (in src/0091b010.cpp's same-TU GetAllThingsInRoughArea), the GetAllLeafThings / _M_insert_overflow bodies in src/00946f90.cpp, and the GetAllThingsInRoughArea / remove / ~_List_base bodies in src/0091b010.cpp. They exist only to stop the local escaping, and they must not be landed as the retail callees.
- 0x0099C7E0: trim the oracle row to 306 bytes and land differ/0099c7e0.cpp (RELOCATION_MATCH on the true extent).
- CDataInputStream is modelled locally in src/00995210.cpp (the engine header has no methods).
- Caller devices `AttachEffectToStaff_Use` / `KrakenLowAttackLoop_FinishAction_Use` (differ/) are not retail functions.
- Descriptive globals: g_MaxShoppersPerShop 0x0138CE30, g_HyperlinkHovered 0x013BD6F8.
- 22 SHARED_HEADER_AVAILABLE warnings: the headers carry no methods (or PDB layouts that differ from retail), so these rows use local models. See NOTES.md Header debt.

# Chunk 375 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport set/map iterator steps in these TUs are 4.5's static `_Rb_global<bool>::_M_increment/_M_decrement(node)`, inlined through `_M_node = _Rb_global<bool>::_M_increment(_M_node)`: the copy-in/assign-back gives an eax temporary and a trailing `mov esi,eax`. The 4.0 in-class `_M_increment()` that edits `_M_node` in place drops both (0x00527FC0, 99 v 103).
- STLport `advance` over a tree iterator needs `__forceinline` on `__advance`: plain inline leaves a 25-byte forwarder. A size_type distance keeps the dead `while (n++) --i` arm behind a second `je` (0x00528090).
- A function whose found vector is filled by a header template (CThingSearchTools::GetAllThings), with push_back's overflow (the real STLport `_M_insert_overflow`) also written out in the TU: VC7.1 then sees the vector never escapes. begin/end stay in ebp/edi across the per-element calls and the destructor frees the cached start. With GetAllThings declared they are re-read every iteration (0x0051F980, 124 v 120).
- Same-TU `__declspec(noinline)` comparator bodies keep the comparator object enregistered in `__partial_sort` / `__insertion_sort` (0x0051ACF0 108 v 104, 0x00517500 125 v 113), and keep tag/value temporaries overlaid on dead argument slots in `vector(n)` / `_M_insert_overflow` (0x005179C0 80 v 78).
- CReplaceSerialise::GetMaxSize needs the early-return form `if (version == GetClassVersionNumber()) return MAX(size, sizeof(T)); return MAX(TOld::GetMaxSize(version, size), sizeof(T));` with the whole chain in the TU. VC then reuses the dominated duplicate call; a named local or a declared-only chain gives 43 / 131 v 110 (0x00519630).
- CReplaceSerialise::ConvertPreviousUnknownVersionUp for a link whose old version has no fields (Header3, 5 bytes): ConvertFromOld is an empty inline, so no call follows the version test and only the count block of `held` survives the inline operator= (`mov eax,[eax+4]`). The landed 0x0051C2F0 template otherwise matches as is (0x00518040).
- `resize(MAX(index + 1, size()))` gives two 24-byte divides (size() again in the false arm). A local `count = size()` then `(unsigned)(index + 1) > count ? index + 1 : count` gives one divide with `cmp ecx,eax ; jbe`. MAX(size(), index + 1) folds the divide but flips the compare to `cmp eax,ecx ; ja` (0x00523790 v 0x005237F0).
- A strstr replace loop: the retail shape is the first strstr before the loop and the next at its bottom on the advanced hit (`*hit = 0; hit += strlen(key); ...; start = hit; hit = strstr(hit, key);`). `while ((hit = strstr(start, key)) != 0)` keeps the length in ebx and rebuilds start with a lea (0x00527200, 221 v 224).
- CCharString's empty-string accessor spelled `PStringData == 0 ? "" : PStringData->m_pData` loads the literal first (`mov edi,""; je; mov edi,[eax]`); `PStringData ? ... : ""` branches around both (0x00527200).
- STLport list/pair/allocator in these instances are `class` (mangled `V`): with `struct`, the `??0?$pair@...V?$list` leaf misses and q.py silently grades the largest function as MATCH (0x00517590). Check every leaf with objdump.
- C2DVector needs its user copy constructor (`X(v.X), Y(v.Y)`) for list<C2DLineF> node copies and back_insert copies (0x00517480 138 v 122).
- Thing-filter lookup-key homing (parked 0x00518DC0): retail homes GetTC's 0xC2 key and the class-name literal temporary in the dead `thing` argument slot (4-byte frame). Every inline spelling tried gives the key its own frame word. One exception: a temporary bound to `const ETCInterfaceType&` does reach the slot, but its compare then reloads it.

Review at landing:
- Errors in already-landed sources:
  - 0x0052C6A0 is named CMarkerRecordVector::resize / SMarkerRecord, but its 24-byte element is CEntityScriptInfo (the vector<CEntityScriptInfo>::resize(size_type) of CEntityRuntimePersistence's two tables; see 0x00523790/0x005237F0/0x00523910 and the landed 0x00523860).
  - 0x00833A10 is landed as CThingAnimatable::SetSuspendAnimation, but Ego_r names it CThingAICreature::SuspendAI(bool, void*) (GOTCHAS already lists five spellings).
  - The landed 0x008A89D0 calls 0x0052B270 with an empty CFilter_Bind2<None, HasTC> model, while the landed 0x0052B270 says CThingFilter_None (and the Bind2 instance is 0x0052B300). 0x0051F980 (1-byte None filter -> 0x0052B270) and 0x0051FA00 (Bind2 of two references -> 0x0052B300) agree with the 0x0052B270 / 0x0052B300 files; 0x008A89D0's comment is off.
- Local models next to existing engine headers:
  - CThingSearchTools, CWorld, CThingAICreature and CTCBase in 0x0051F980 / 0x0051FA00, and CTCBase in parked 0x00518DC0. None is `this`; the engine headers carry Ego_r layouts, and the rows need retail +0x20/+0x44/+0x6C/+0x91 CThing views.
  - CRegion in 0x0051BEC0: the engine header has the Dinkumware 100-byte layout, retail is 0x58.
- Unproven names:
  - 0x00527200 CCharString::ReplaceAll: compiled in the thing-manager TU (its "" literal is 0x0129AAF4), so it is not the CCharString library's `?Replace@CCharString@@QAEXABV1@0@Z`.
  - 0x0051A040 is an unidentified set+array constructor.
  - The 0x00516F80 / NPCNameMap / building-creature tree classes are descriptive non-template stand-ins for the STLport `_Rb_tree` instances.
- 0x0051F980 / 0x0051FA00 contain GetAllThings / push_back / `_M_insert_overflow` bodies that are not graded (only the leaf is), so they are evidence of shape, not landed reconstructions of 0x0052B270 / 0x0052B300 / 0x00515CC0.

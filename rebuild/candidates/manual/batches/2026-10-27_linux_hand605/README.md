# Chunk 605 (second parked-row chunk from the sweep ranking, 10 same-length rows 36-181 bytes): 2 of 10 landed

Landed (vland --land, 2/2: RELOCATION_MATCH + MATCH, tests PASS, multi-check 0): 0x0057F70F
CPersistContext_TransferableVector<long>::TransferOut (the vtable 0x0124F33C RTTI is `<long>`, not the catalog's
ECreatureGeneratorGenerateType; built on the landed `<int>` twin 0x00566360 with a `__forceinline` CPersistTraits<long>::TransferOut
helper, which also puts the index load before the m_pFirst dereference) and 0x00C88CD0 CMixKernelsSSE::Add (libpk SSE mixer slot 5,
the F32vec4 model of the six landed siblings and `*d++ += *s++;`; descriptive class name shared with those siblings, so landed from
name_review/). Parked 8 (pending/.../differ/): 00754d80 improved 17 -> 9 lines (functor float through a local; the load/store is not
hoisted out of the loop), 0070c4e0 pop/xor order, 00c5e2c0 DeviceIoControl argument registers, 00c75f80 memset count before the
m_pos store, 00c52bb0 cmp 4/jae vs cmp 3/ja, 00a852b0 sentinel store placement, 006afe60 (respelled to the PDB protected const
by-reference form on engine/CTCPhysicsBase.h) velocity load order, 0052ea30 (every callee respelled) 8-byte copy pairing. 13
NAME_FIXES + 3 review rows queued (prefix c605: CPersistTraitsSLONG::TransferOut 0x009E2560 six spellings, NUISystem::CManager
GetInstance/GetUIScale, CUIScreenUnitConverter ConvertVector/UnConvertVector, CComponent slots 31/117, CAnimateThing
GetFirstCombinationAnimFromType, CTCPhysicsBase::RespondToCollision, the std::for_each instantiation 0x00754D80).

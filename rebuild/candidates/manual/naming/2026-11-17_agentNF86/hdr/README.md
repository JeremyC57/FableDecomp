# NF86 header overlays (round 7, slice q11098..q11120)

Each overlay adds `#ifdef` opt-ins only, named after the member they serve. Base copies (the repo headers as of this run) are in `../orig/`;
at hand-back all four base copies still equal the repo headers byte for byte (`cmp`). Without its macros each overlay preprocesses to the
same token stream as the repo header: `../tokid.log` runs `cpp -P` on landed includers with and without the overlay (4 sampled includers,
plus 3 includers that define `FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_WIFEUPDATE`, the branch the nested opt-in sits in, and 2
fable_deque.h includers that define `FABLE_RETAIL_STLP_DEQUE_COPY_CTOR` / `_PASS22_POP`): all IDENTICAL. `pin_asserts.py` was run after every edit; what it pinned is listed per header. The new macro names were grepped across
`rebuild/`: no existing use.

| Overlay | Opt-in macro | Rows using it | What it adds | Asserts pinned |
|---|---|---|---|---|
| `engine/CCreatureAction_GetUpFromSitting.h` | `FABLE_RETAIL_CCREATUREACTION_GETUPFROMSITTING_CTOR_INLINE` | 0x00860150, 0x0085FF10 | Includes engine/CSoundPair.h; the one shared `inline` body of the 4-argument constructor `??0CCreatureAction_GetUpFromSitting@@QAE@AAVCThingCreatureBase@@ABVCCharString@@J_N@Z` (retail 0x00860150): `CCombatActionBase(creature, 0x32, CSoundPair())`, SittingMode, RemoveOnPerformAction, then `SetCombatAnimation(animation)`. Replaces the in-row inline copy NF84 put in 0x0085FF10 (same tokens), so both rows emit the same pick-any COMDAT (selection 2) and no row defines a strong copy (brief, q11102 / q11120). Body only, after the asserts. | 0 |
| `stlport/fable_deque.h` | `FABLE_RETAIL_STLP_DEQUE_UNINIT_COPY_BODY` | 0x0059B49B, 0x0059B2CC, 0x0059AFD7 (one TU) | The STLport 4.6 `_uninitialized.h` body of the FABLE_FASTCALL `__uninitialized_copy(I, I, O, const __true_type&)` declared by NF84's `FABLE_RETAIL_STLP_DEQUE_COPY_CTOR`: `return __copy_aux(first, last, result, _BothPtrType<I,O>::_Ret());`. Body only. | 0 (no asserts in the file) |
| `stlport/fable_deque.h` | `FABLE_RETAIL_STLP_STACK_EMPTY` | 0x0059686D, 0x00597B20 | STLport 4.6 `deque::empty()` (`_M_finish._M_cur == _M_start._M_cur`, only when `FABLE_RETAIL_STLP_DEQUE_PASS22_POP` does not already give it) and `stack::empty()` (`return c.empty();`). Inline bodies only. | (same file) |
| `engine/CAIStateGroup_OpinionReactionRoot.h` | `FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_THINGREACTINGTO` (nested in the `_WIFEUPDATE` branch) | 0x0095EE70 | ThingReactingTo at +0x6B as its Ego_r.pdb type `CIntelligentPointer<CThing>` (8 bytes) plus `_pad_0x73[5]`, in place of the byte arrays; the old member lines kept verbatim in the `#else`. Same offsets and size (the three asserts hold). Needs engine/CIntelligentPointer.h first. | 3 (lines 51 / 52 / 53 -> `FABLE_STATIC_ASSERT_AT(51/52/53, ...)`) |
| `engine/CEngineSubPrimitiveSpriteTrail.h` | `FABLE_RETAIL_CENGINESUBPRIMITIVESPRITETRAIL_IMPLICIT_DTOR` | 0x00BCA060 | `#ifndef` around the declared virtual destructor: retail's destructor chain is implicit (0x00BCA060 calls ~CBaseClassNonCopyable 0x0099A300 directly, like every other sub-primitive `??_G`); used with `FABLE_RETAIL_PASS22_PRIMITIVE_IMPLICIT_DESTRUCTORS`. Comment + guard only; same layout. | 1 (line 13 -> `FABLE_STATIC_ASSERT_AT(13, ...)`) |

Not handed back (drafts in `../work/hdr_drafts/`, not used by any finished row):
`fable_deque_copyscan.h` (out-of-line deque copy ctor / dtor declarations for the blocked 0x005959AB) and a `CCategory.h` draft
(`FABLE_RETAIL_CCATEGORY_INVENTORY_FORWARDERS`, for the blocked q11109..q11111 rows).

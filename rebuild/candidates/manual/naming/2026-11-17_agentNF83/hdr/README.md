# NF83 header overlays (round 7, slice q11010..q11043 without q11015 / q11016)

Each file under `hdr/` replaces the same path under `rebuild/include/`. Base copies are in `orig/`.
Every change is an additive `#ifdef FABLE_RETAIL_*` block. With no opt-in defined, each header is token-identical
to its base copy, apart from the asserts pinned with `pin_asserts.py` (`FABLE_STATIC_ASSERT` -> `FABLE_STATIC_ASSERT_AT(<orig line>, ...)`).
No assert was moved.

| Header | Opt-ins (new) | Used by | Asserts pinned |
|---|---|---|---|
| engine/CTCNoiseDef.h | `FABLE_RETAIL_CTCNOISEDEF_CTOR_INLINE`: an inline `CTCNoiseDef() { }` in place of the out-of-line declaration. It needs `FABLE_RETAIL_DEF_CLASS_BASE_CTORS`, `_CSUBDEFCLASSBASE_CTOR_INLINE` and `_CDEFCLASSBASE_CTOR_SUBDEF_FRIEND`. | 0x00749810 | 1 (line 26) |
| engine/CDefClassBase.h | `FABLE_RETAIL_CDEFCLASSBASE_CTOR_SUBDEF_FRIEND`: `friend class CSubDefClassBase;` inside the CTORS block | 0x00749810 | 0 |
| engine/CThingCreatureBase.h | `FABLE_RETAIL_CTHINGCREATUREBASE_SETCURRENTACTION_COUNTED` (retail branch): `bool SetCurrentAction(const CCountedPointer<CCreatureActionBase>&);`. `_FLAT_ISFREETOPERFORMACTION` and `_FLAT_CLEARCURRENTACTION` (flattened branch, after `class CThingCreatureBase {\npublic:`): PDB-spelt `IsFreeToPerformAction(const CCreatureActionBase&) const` and `ClearCurrentAction()` declarations. Declarations only; no layout change. | SETCURRENTACTION_COUNTED: 0x008D6C80; FLAT_ISFREETOPERFORMACTION: 0x00969AE0; FLAT_CLEARCURRENTACTION: 0x00692DB0 | 0 |
| engine/NCreatureMode_CModeSitFloor.h | `FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_FRAMEUPDATE` (`virtual void FrameUpdate();`, an override with no new slot; needs `FABLE_RETAIL_CMODEBASE_VIRTUALS`). `_CMODESITBASE_REMOVEMODEWITHACTION` (`void RemoveModeWithAction(const CCreatureActionBase&);`). | 0x008D6C80, 0x008D7230 | 3 (lines 66, 67, 69) |
| stlport/fable_deque.h | `FABLE_RETAIL_STLP_DEQUE_MSUBTRACT_BODY`: the `_M_subtract` declaration and its out-of-class body. `FABLE_RETAIL_STLP_DEQUE_POP_BACK_AUX_BODY`: the `_M_pop_back_aux` body, with `BACK_POP_BODIES`. | 0x0059A8F3, 0x0059AD35 | 0 |
| engine/CPlayer.h | `FABLE_RETAIL_CPLAYER_AGGRESSIVEMODE`: `ToggleAggressiveMode` / `SetAggressiveMode`, after the SETUPCONTROLS block | 0x00487E90, 0x00487EB0 | 0 |

The coordinator should merge each overlay over the landed header. The rows in `rows/` define only the opt-ins they need.

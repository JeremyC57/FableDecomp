# agentNF17 naming pass (2026-11-11)

Final grade, one batch over all 112 changed rows (`work/final.out`): 112 RELOCATION_MATCH, 112 VC7.1 tests PASS, every
catalog PassPattern kept. 1 to 4 grades per row (cap 10). No header overlay (`hdr/README.md`). No git, nothing written
under the repo, nothing landed, one compile at a time, BUSY.txt and agentH40f untouched.

## 1. CheckCRC binders (0x00404500): 88 of 94 resolved
The 94 binders were not CheckCRC callers spelled wrong. They are the retail-only **one-argument binary transfers**
(`CheckCRC("")` on the pooled empty literal 0x0122D70E, then only `MODE_LOAD_BINARY` -> `CPersistTraits<T>::TransferBinaryIn(*PLoadStream)`
or `MODE_SAVE_BINARY` -> `TransferBinaryOut(*PSaveStream)`, `ret 4`). They were landed as `CEngineLightingManager::UpdateShadowScene`,
`_global::PersistTransfer_<addr>` or `CEnginePrimitiveRenderer2D::AddPrimitive`.

- **Renamed (RENAMES.tsv section 1, 86 rows):** 57 `Transfer<enum>`, 26 `TransferMap<V,K>`, 1 `TransferVector<ENavigatorType>` (0x006724B0),
  plus `Transfer<bool>` (0x0043314A) and `Transfer<unsigned char>` (0x00473593), which were already named but are now respelled on the header.
- **Name status:** the class, member and template are PDB spellings (Ego_r/FableWin carry `??$Transfer@...@CPersistContext` for
  the same T in the 3-argument form). The **one-argument parameter list is descriptive**: no PDB symbol has it, and the header
  `engine/CPersistContext.h` already declares it (`Transfer(T&)`, `TransferMap(map&)`, `TransferVector(vector&)`).
- **How T was proved (two sources per row, evidence column):**
  1. The caller's field offset in its def `Transfer`, calibrated against the known anchors (LESSONS.md), mapped to the Ego_r member type.
  2. Either the PDB 3-argument instance for the same T, or a landed caller's existing binding, or the adjacent
     `CPersistTraits<T>::TransferBinaryIn`.
- **Already correct:** 0x00431020 and 0x004E59E1 already bound CheckCRC.
- **Left (6):**
  - 0x00451E11 `CEngineGraphic`, 0x0045228F `CDefString`, 0x004522F4 `CDefIndex`, 0x004568BC `C2DVector`, 0x00456903 `C3DVector`:
    the header has no usable `CPersistTraits` specialisation. `CDefIndex`'s default ctor is out of line, `CDefString::TablePos`
    is private and it has no `TransferBinaryOut`, and there are no C2D/C3DVector traits. Each needs a header opt-in; I made none
    this pass.
  - 0x00452B4D: a different shape (virtual calls, TransferSubComponent-like), not identified.
- **Naming inconsistency to settle:** landed 0x004331F9 is spelled `TransferVectorBinary`, while the header and 0x006724B0 use
  `TransferVector`. Both are descriptive. Pick one and respell the other.

## 2. LowerBound 0x0040F020: 19 of 35 respelled
Respelled to the `CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >` specialisation (`LowerBound(const ETCInterfaceType&)`,
`first`/`second`, `_M_finish`): 00689130 00857b70 00859d80 00895ed0 00896e60 00896f60 00896fd0 008986d0 00898840 00898940
00898b30 00899d50 00899db0 0089a070 0089a0e0 0089b390 0089dff0 0089e4c0 008a7980.

- **Tests respelled:** the mocks of 00899d50, 00899db0, 0089a070 and 0089a0e0.
- **Pre-existing failures, unchanged:** 00899d50_test g++ GXX_FAIL is pre-existing (`__fastcall` on x86-64 g++, the repo test
  has the same line), and the VC7.1 test passes. 00857b70 and 00859d80 still bind `GetPItem` returning `Creature*` (target
  0x00A01B50, BIND BAD); that is unchanged from the repo and out of this pass's scope.
- **Left (16):** 004c9d60 00783380 007e5230 008385b0 00898bb0 0089a270 0089bb30 0089c4c0 0089ec60 008af850 00915190 00927c80
  00935160 009547d0 00954e30 00972490. These are bespoke key/iterator shapes, free-helper rows (0089a270, 0089c4c0) and
  NF16's register-swap DIFFER family. Each needs its own rewrite.
- **Const twin:** 0x004365B0 was not reached (32 left).

## 3. CDataOutputStream adapters (0x00993EB0 WriteSLONG / 0x00993E30 WriteEBOOL)
- **Rows changed:** 0046aff4 00472841 00612d30 00632b30 00756540 006d3a30 007ad270.
- **Change:** dropped the methods-only `CDataOutputStreamWriter` struct and the `#define CDataOutputStream CDataOutputStreamLayout`
  derived-class blocks in favour of the default `engine/CDataOutputStream.h`.
- **Effect on the rows' own symbols:** they now take the PDB `AAVCDataOutputStream@@`.
- **Debt:** their container parameters still lack the allocator argument, so the full own name is not yet the PDB spelling.
- **Further binders now OK:** the 57 enum rows also bind WriteSLONG through the traits, and Transfer<bool> binds 0x00403EB0
  and WriteEBOOL.
- **Left on 0x00993EB0:**
  - Reader/Adapter rows (0045df5e 0045e37f 0045ec72 004675f6 007adbf0, also the 0x00403EB0 binders).
  - 005dbXXX struct-S rows.
  - 00614620 (WriteString is retail-only).
  - 0086b210, 00aedaf0, 0077ce00.
  - 00a8e6c0 (WriteUBYTE is not in the header).

## 4. Name fixes worth a look (NAME_FIXES.tsv)
- **Conflict:** 0x00580371 is landed as `Transfer<CDefIndex>` (3-argument), but the RTTI of its text wrapper vtable 0x0124F2F4
  is `.?AV?$CPersistContext_TransferableComponent@W4EHeroTrainingStatus@@@@`, so it is `Transfer<EHeroTrainingStatus>(const char*, E&, const E&)`.
  0x004DDF0C is that T's `TransferBinaryIn`.
- **Conflict:** 0x00412D20 is landed as `GFSerialiseVectorBinaryIn<unsigned long>`, but its only caller 0x00412E80 is
  `Transfer<ETutorialCategory>`, so it is `<ETutorialCategory>`. 0x0040FEF0 is `CPersistTraits<ETutorialCategory>::TransferBinaryIn`.
- **GF helpers:** 004314c8 0043151d 0045df1e 0045df5e 004ecd6b 004ecdac 006724f0 00672530 are respelled to their map/vector
  `GFSerialise*Binary{In,Out}` instances.
- **Callers:** every caller spelling of a renamed row is taken from the landed reloc bindings.

## 5. Queue (134 entries)
Outside the items above, the queue was not reached. This covers the CCharString / CWideString / CBaseIntelligentPointer
mega-families, 006644f0, 013b86a0, 0062e900, the Coda SDK names (no second source found) and the small targets. STATUS.tsv
carries NF16's left lists for them unchanged. Five queue targets have no NF16 status line (00692910 008e5670 0137dad8 0042be50
006aeba0); they were not re-verified this pass.

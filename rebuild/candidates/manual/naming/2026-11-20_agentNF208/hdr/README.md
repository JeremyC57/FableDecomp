# NF208 overlay headers (2026-11-20, FP-VECTORINSERT)

Base copies in ../orig/engine/ (byte-equal to the repo working tree at the start of this pass). Both overlays are add-only `#ifdef`
blocks with member-specific macro names; the default path is token-identical (checked: `cpp -P` of orig vs overlay with no opt-in
defined gives the same md5, a0d1e1ad.. CSymbolMap.h, 73aaa374.. CTCAIScratchpad.h). `pin_asserts.py` pinned 0 asserts in each: every FABLE_STATIC_ASSERT in both headers is already
the line-pinned FABLE_STATIC_ASSERT_AT(<line>, ...) form, so the added lines leave every assert typedef name unchanged.

| header | repo base | opt-ins added | rows using them |
|---|---|---|---|
| engine/CTCAIScratchpad.h | rebuild/include/engine/CTCAIScratchpad.h | `FABLE_RETAIL_CTCAISCRATCHPAD_ADDGOSSIP`, `_ADDGOSSIPVILLAGES`, `_ADDGOSSIPFACTIONS`: declarations only of the public static `__fastcall` members AddGossip / AddGossipVillages / AddGossipFactions(CCharString, CCharString) (FableWin.pdb `?AddGossip@CTCAIScratchpad@@SIXVCCharString@@0@Z` 0x02AAFA70, `?AddGossipVillages@...` 0x02AAFCD0, `?AddGossipFactions@...` 0x02AAFDE0), placed right after the existing GOSSIP_MAPS block (under `public:`) | 0x008AE5E0, 0x008AE810, 0x008AEA40 |
| engine/CSymbolMap.h | rebuild/include/engine/CSymbolMap.h | `FABLE_RETAIL_CSYMBOLMAP_FINDFIRSTSYMBOLWITHVAL_VALUE` (nested in FABLE_RETAIL_CSYMBOLMAP): declaration only of the one-argument `const CSymbolInfo* FindFirstSymbolWithVal(long) const` (Ego_r.pdb 0x00B36AA0 + FableWin.pdb 0x03094030 `?FindFirstSymbolWithVal@CSymbolMap@@QBEPBVCSymbolInfo@1@J@Z`; retail 0x009E5090) | 0x006FC8E0 |

Header users: 47 landed rows include engine/CTCAIScratchpad.h and 9 include engine/CSymbolMap.h
(`grep -rlE 'engine/<header>' rebuild/src/compiled`); none defines the new macros (grep over rebuild/src/compiled and rebuild/tests: 0
files), so the opt-ins change none of them. Neither header is overlaid by NF205 / NF207 (their hdr/ dirs read; no overlapping member).

Existing shared-header opt-ins the rows use (no header edit): stlport/fable_containers.h FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE,
FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY, FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE (NF41, the self row 0x00433530's macro);
stlport/fable_algorithm.h FABLE_RETAIL_STLP_FOR_EACH_BODY; engine/CVectorMap.h FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE +
_INSERT_VALUE_TYPE; engine/CTBaseSingleton.h FABLE_RETAIL_CTBASESINGLETON_GET_DECL; engine/CTCAIScratchpad.h
FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS. No FABLE_FORCEINLINE was added to any header.

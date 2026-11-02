# hdr/ -- chunk 710 shared-header corrections

Both headers use the **opt-in** pattern: the repo text is kept unchanged under `#else // flattened Ego_r layout (the landed default,
unchanged)` and the retail class sits under `#ifdef FABLE_RETAIL_<CLASS>`. A script compared each `#else` body with the repo body: identical
(the only other edits are the RETAIL_MARK first-line wording and the added evidence comment). No default path changed, so there is no
INCLUDERS_<Class>.txt and no `--recheck-landed` run. Each header carries a `// Retail opt-in (chunk 710)` evidence comment and
`FABLE_RETAIL_` macro, so gen_class_headers.py's RETAIL_MARK keeps the edit.

Overlay use was proven for both (never with `#error`):
- Without `FABLE_HDR_OVERLAY`, the including rows fail to compile (0x00741390: C2664 -- the flattened struct has no CThing& ctor;
  0x006F1770: C2146).
- A copy with one extra 4-byte member before the first derived-visible member turned the rows DIFFER (0x00741390 DIFFER(45v42), every
  member offset +4; 0x006F1770 DIFFER(35v35), +0x2C for +0x28). The copies were deleted.
- With the overlay all including rows are MATCH / RELOCATION_MATCH (q.py and the read-only vland run).

| header | macro | used by | what the retail branch holds | evidence (two sources each) |
|---|---|---|---|---|
| engine/CModeStandBase.h | FABLE_RETAIL_CMODESTANDBASE | 0x00740420, 0x00741390, 0x008AC4D0, 0x008E31A0 | `class CModeStandBase : public CTCCreatureModeManager::CModeBase`; ctor `(CThing&)`; out-of-line `virtual ~CModeStandBase();` (0x0073D860); Ego_r members IdleCounter +0x1C .. PSprintAnimation +0x60 with their real types (CCharString, CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> x6); sizeof 0x68. Includes fable_string.h and engine/CCountedPointer.h; the includer supplies CTCCreatureModeManager with its nested CModeBase (no shared header exists) | RTTI CModeStandBase : CTCCreatureModeManager::CModeBase; ??1CModeStandBase@@UAE@XZ in Ego_r (0x00525F00) and FableWin (0x01DD6520), bound to 0x0073D860; the Ego_r layout has no STL container, and the four retail dtors put the first derived member (Filter) at +0x68 |
| engine/CCombatSequenceGeneric.h | FABLE_RETAIL_CCOMBATSEQUENCEGENERIC | 0x006F1770 | `class CCombatSequenceGeneric : public CCombatSequenceBase`; the PDB ctor `(CThingCreatureBase&, CThingCreatureBase&, CDefPointer<const CCombatSequenceDef>&)`; member `CDefPointer<const CCombatSequenceDef> Def` +0x28; sizeof 0x2C. The includer supplies CCombatSequenceBase (no shared header) and engine/CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE) | RTTI CCombatSequenceGeneric : CCombatSequenceBase; vtable 0x01261D74 (20 slots, ??_G 0x006F1750); both PDBs' ctor spelling; Ego_r Def +0x2C over a 16-byte Dinkumware vector = retail +0x28 with the 12-byte STLport vector (retail dtor releases [esi+28h]) |

Coordination: agents 709 / 711 may edit other headers; these two files are touched only inside the new `#ifdef` branch. engine/CBankFile.h
(HOLD) and rebuild_abi.h are not touched.

# agentNF24 notes

## Scope
- Part 1: 25 queue entries, 22 done, 3 partial (0x0095A420, 0x005CC10E, vtable 0x012CCC50), 0 rejected. Part 2: all 21 held-back renames ported onto shared headers.
- 59 rows changed (57 RELOCATION_MATCH, 2 MATCH), every test PASS with the catalog PassPattern; per-row grade counts in STATUS.tsv (max 5, at 0x00797E30).
- No BUSY / RESERVED address touched. 0x00F33DC0 (BUSY) was not edited; only its free caller 0x00F33D90.

## Rows left (gaps)
- 0x005CC10E GetSubDef<CCarryableDef> const: 19 binders still spell the non-const local stand-in. Each carries a local CDefPointer/CDefClassBase body that cannot take `const T`; each needs a port to engine/CDefClassBase_GetSubDef.h as 0x00965640 / 0x00966030 had. Plain CCarryableDef: 006648d0 0069f9e0 007153b0 0078e9c0 00793ec0 00845ce0 00848500 00848730 0084a350 0084a4a0 008ba3c0 009236c0. Wrong T (retail is CCarryableDef): 007037d0 (CDebrisDef), 007eb110 / 007eb1a0 (CPickUpDef), 00966cf0 / 009687b0 (CJugDef). CDefLookup stand-in: 00847ac0 0084a5f0.
- 0x0095A420: 0x0095A6C0 / 0x0095B6F0 reach it through stand-in class CQueenNative; the real class has a header but the row signature (long,float) differs from the PDB (long,EMovementType): a rewrite, not a respell.
- vtable 0x012CCC50: 0x00D34EC0 (_Construct of CFireHeartPatternDef) still binds ??_7CDefWithByteVector@@6B@; needs the vector<bool> PadActive copy 0x00D34F00 modelled, and no PDB spelling for that _Construct was found.
- 0x00CAEBB0: only free binders are the funclets 0x012171E8 / 0x01217218, which include the BUSY 0x00CAF480 source (WATCH: coordinator fixes their includes).

## Follow-ups seen (not in the queue)
- Further binders of 0x00EF7AB0 (now NScript::CIsThingConscious::~CIsThingConscious) with wrong spellings: 0x00EFF4D0, 0x00EFE700, 0x00F1B220, 0x00EF7800.
- 0x00658B20 has conflicting landed spellings (IsCookTime / IsWorkTime / ComputeB0): one address, one name needed.
- 0x009ADA10 has wrong local spellings in the GetSubDef stand-in rows.
- 0x00CDBD20's object also emits ??_GCQ_SunnyvaleMasterData@NScript@@UAEPAXI@Z (0x00CDBD70); the coordinator may merge the two rows.

## Descriptive TU devices kept (not new names)
- 0x00CDBD70: the pre-existing MakeOne TU device (forces the ??_G COMDAT) kept as landed.
- 0x00B3E9D0: FAM07's DestroyMapOf... emitter function kept; it only instantiates the ~map COMDAT and is not a retail symbol.

## Header debt
- Overlay opt-ins to merge (hdr/README.md): FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS_CALLS, FABLE_RETAIL_CPREDICATE_AND_INLINE_DTOR, FABLE_RETAIL_NSCRIPT_DATA_CTOR (4 headers), FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_ALLOC, FABLE_RETAIL_COMBATSEQUENCE_STATGETNAME (3 headers), FABLE_RETAIL_CENGINERESOURCEMANAGER_EXTERNALTEXTURES.
- 0x0095D200 still declares AIAction_ProjectileAttackAndStrafeToPos on a local CAIStateGroupBase (as landed); engine/CAIStateGroupBase.h does not yet declare it.
- 0x00EF84D0 adds an inline CIsThingConscious ctor after the include; the header has no ctor body.

## RENAMES format
- Section 1 FAM07 rows (module "std", name "map<...>::map") were split at the last top-level `::` so that module = full class, name = method only. NF23 rows were already in that form. Section 2 includes NF23's caller respellings for 0x00B3E750 / 0x00B3E8D0.

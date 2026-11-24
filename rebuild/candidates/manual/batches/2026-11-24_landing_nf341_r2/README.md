# Landing nf341_r2 (Account A, 2026-11-24): the GetDef<CMaterialDef> copy repair in four emitter rows (NF341 round 2, item 1)

Rows: 0x008F7950 NCombatActionHelpers::CreateDustEffectForCreature, 0x00703150 CThingObject::PeekPMaterial,
0x007F0300 CTCCarriedActionUseSpade::IsThingAbleToDig, 0x0082D5C0 CThingBuilding::PeekPMaterial.

Change: one line per row, `#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD`, next to the row's
`FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY`. This is an existing CDefPointer.h opt-in. It gives the retail SetDynamicCast form,
which re-reads the source after the release: the form the owner row 0x0066C340 and landed 0x0066B370 use. There is no header change.

## Coordinator ruling R1: r2 (this) over r2b
NF341 built two candidates:
- **r2:** this define.
- **r2b:** a new scaffolding specialization in CDefinitionManager.h, behind its own opt-in.

r2 is chosen. It uses an existing opt-in in the retail form, not scaffolding. It also repairs every other GetDef<T> copy in these rows.
Those repairs make r2's object change wider than "only the fixed copy":
- The SetDynamicCast helper COMDATs of the repaired types change body.
- Their Assign helpers drop.

These helpers have no retail address. No reloc_bindings line names them, and no relocation in 157 compiled HEAD objects references them
(`evidence/helper_refs_scan.txt`). Under the NF341 drop ruling, the change is part of the copy repair.

## Gate
- **Grade:** RELOCATION_MATCH PASS before and after (agent), from the repo after landing (`logs/recheck_coordinator.log`).
- **Pick-any:** both checkers, HEAD against new (`evidence/pk_gate*`).
  - 7 copies fixed:
    - GetDef<CMaterialDef> 0x0066C340 in all four rows;
    - GetDef<CEngineThemeDef> 0x0063F3A0 in 0x008F7950 and 0x007F0300;
    - GetDef<CThingBuildingDef> 0x004AB840 in 0x0082D5C0.
  - 0 copies turned NEQ, 0 new.
  - The reversed control fails, as it should.
  - The coordinator's run on the landed objects agrees. Its one leaf-called NEQ copy is
    CArray<pair<long,C3DVector>>::push_back 0x0076B120 in 0x008F7950. It is unchanged from HEAD, now queued as T3 debt (nf341 0x0076B120).
- **Link** (owner object plus the four objects): 0 duplicates (`evidence/link_r2.txt`).
- **D3:** 123 leaf-called NEQ GetDef<T> copies in 109 rows (`evidence/d3_count.txt`). The T3 D3 line now names this define as the
  per-row fix.

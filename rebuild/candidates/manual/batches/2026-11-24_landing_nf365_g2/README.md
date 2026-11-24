# nf365 g2 / g3: the GetDef swap in the three rows NF365 held for rulings (00868e50; 0060c900, 0060eeb0)

Same change as `../2026-11-24_landing_nf365_b1/README.md` (row-local GetDef template replaced by GLOBAL_BODY + SETDYNAMICCAST_REREAD + the
NF349 size body). Rulings (advisor decision, owner delegated, 2026-11-24):
- **R1 (00868e50, g2): land.** The swap brings `SetDynamicCast<CArmourDef>`, which no landed row defined. This row is its first definer.
  The coordinator checked it in this row's own landed object: 52 B, no relocations, characteristics 0x60501020, byte-equal to the
  `SetDynamicCast<CThingObjectDef>` instance beside it (which equals the landed references). It is the same template body under another
  type name. GetDef<CThingObjectDef> 116 -> 98 B EQ (owner 0x0042B040) and GetDef<CArmourDef> 116 -> 108 B EQ (owner 0x00817B40).
- **R2 (0060c900, 0060eeb0, g3): land as debt.** Each carries a strong same-TU copy of CreateMapMarkers 0x005FF190 that is NEQ and
  leaf-called at HEAD (4476 v retail 4176 B) and stays NEQ after (4420 B): no verdict or reach transition, so the gate does not hold the
  row (an EQ -> NEQ turn would). The real fix is a port of those copies (T3 nf365 0x005FF190).

Coordinator verification, HEAD and repo builds by the coordinator (`$S/v365gH.sh`, `$S/v365gN.sh`): 3/3 RELOCATION_MATCH PASS before and
after, identical WARN sets; the pk_all diff per row is exactly the swap footprint (GetDef copies NEQ -> EQ, SetDynamicCast 50 -> 52, Assign /
IsNull dropped, Get added) plus, in g3, the CreateMapMarkers size change above; 0 new NEQ; pickany_vs_retail 0 new NEQ; reachflip 0. Drop
test: 8 NOADDR helpers with no reloc_bindings line, and the leaf's own `?A0x` name (hash only).
T3 nf303 0x0060EEB0 (an unrelated binder-census port) parked during the landing and restored in place.
Landing note: g2's first run was held by land_scripted's symbol check: the row's `// Leaf:` is the anonymous-namespace prefix
`?GetArmourWeightsFromThing@?A0x` and the graded symbol carries the build-path hash. land_scripted.py now accepts that prefix
convention (hash normalised; nothing else loosened); the rerun landed with RELOCATION_MATCH PASS.

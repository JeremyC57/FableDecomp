# chunk 1006 notes (agent1006)

All grades used `export FABLE_HDR_OVERLAY=$A/hdr` and `timeout 240`. Free probes: `tools/probe.py` (c2prio `--obj`).

| row | result | grades | lever / residue |
|---|---|---|---|
| 0x0056EF2D UpdateMenuScrollButtonsItem | MATCH | 1 | `long statCount = statIndicatorMax;` read at the top of the item branch (puts the member load first, in edi) |
| 0x00570332 AddAbilityToList | RELOCATION_MATCH | 1 | the blank `CWideString` in its own `{}` block (frame 0xD8): a bottom-of-block named object is live for the packer from the block's brace and blocked textBank's slot |
| 0x00571280 AddAbilityToListPC | RELOCATION_MATCH | 1 | same as 0x00570332 |
| 0x005B3E4E CTCInventoryClothing::GlobalUpdate | RELOCATION_MATCH | 1 | same-TU `CDefClassBase::GetSubDef<T>` body under auto_inline(off) (the def local is not address-captured, so the appearance pointer CSEs across the two GetAnim calls); `anim = &defaultAnim` then override (shares DEFAULT's slot); CAnimInit argument ctor + `init.Frame = GetFrame()`; named CAnimateThingUpdate local; overlay headers (hdr/README.md) |
| 0x005CA68E SetupActiveWeaponsOnCarrier | DIFFER(1538v1535), parked | 1 (+3 in chunk 1004) | reg-alloc tie: `this` (prio -72) beats the 0x48 constant (-76); retail keeps 0x48 in ebx through both arms |
| 0x00577AC0 OpenInventoryInternalPC | DIFFER(2157v2157), parked | 1 | 5-instruction operand rotation of PointsGeneral+Strength+Skill+Will; ~110 probes |
| 0x005B9641 DrawPreparePrimitivesForRendering | not built | 0 | header debt (CUIMiscThingsDef, CRightHandedSet, C3DOrientationInterpolator, CEnginePrimitive2DMorphedAnimatingMesh, CEngineTextureMorphSet, CSectionSet); outline in differ/005b9641.cpp |

Relocation targets for 0x005B3E4E checked with tools/rel.py; string literals "DEFAULT" 0x01230554 and "EMBARRASSED_TWIRL" 0x012539DC
match the exe. Catalog length for 0x005B9641 (1,867) overruns the function (1,455); the tail is STLport/CVectorMap helpers.

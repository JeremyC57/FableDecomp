# Landing 0x00680050 CThingObjectDef::Validate (2026-11-19, 439 bytes) -- include-order match, landed by owner decision

hand1 / Fable advisor fable8 / hand2a source: RELOCATION_MATCH + PASS (vland --land, LANDVERIFY_STRICT_HEADERS=1; `logs/vland_land.log`),
test: Codex's 3,072-case fixture, 4/4 mutants killed. The match depends on an include order (engine/CMeshDataBank.h before
engine/CWorld.h), which moves a c1 symbol ordinal and so c2's candidate-record numbering and the reload order at 0x00680196. No
statement-level fix was found in about 155 variants. Owner (2026-11-19): "land with note" -- the source's `// Include-order:` block
records the mechanism and its fragility (recheck after shared-header changes). Catalog name corrected from the decorated stand-in
`CThingObjectDef::?Validate@...` to `CThingObjectDef::Validate`.

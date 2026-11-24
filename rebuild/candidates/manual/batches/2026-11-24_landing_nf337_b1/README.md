# Landing nf337 b1..b4 (Account A, 2026-11-24): 38 Def-class `Copy` rows (and operator= units) onto their PDB members

Agent NF337 took the 48 `?Copy@<Def>@@UAEXPBVCDefClassBase@@@Z` rows NF335 skipped: retail's Copy is (mostly) a 5-byte thiscall `jmp` to the
class's operator=; the landed flat selves were `__fastcall` stand-ins. Hand-back: `pending/2026-11-24_nf337_handback/` (NOTES.md first, incl.
"Round 2"); tools `naming_batch/agent_tools/nf337/`. STATUS: 36 done, 5 skipped-held (b5 / b6, ruled below), 7 skipped-port (classes modelled
only under a `_PASS` switch, NF318 Q2).
- **b1 (18):** jmp-form Copy rows on a local model or an existing header (no edit). **b2 (11):** jmp-form Copy rows on 11 add-only
  `FABLE_RETAIL_<C>_COPY_DECLS` blocks (HEADERS.tsv: override probe, made-up-name control). **b3 (2 units, 4 rows):** 0x00464531 + 0x00464536
  (the operator= row was landed as `??4CThingBaseDef`, 0x00463950's own symbol, so HEAD linked one duplicate; now `??4CThingPhysicalSwitchDef`,
  its base callee the real `??4CThingBaseDef` via an add-only CThingBaseDef.h block; catalog / manifest / files renamed to
  CThingPhysicalSwitchDef::operator=); 0x0045E65B + 0x0045E660 (operator= returns `const&` as both PDBs spell it, QAEABV0; same bytes).
  **b4 (5):** body rows that call `CDefClassBase::operator=` 0x00431F10 and copy members.
- **Proof (agent):** HEAD v new at one work path, grade unchanged; objcheck ALL_EQ (or + exactly libcp) under explicit rename maps; COMDAT sets
  equal, no `??_7` / `??_R`; link 0 duplicates, unresolved sets equal after mapping; moved tests' controls (HEAD + moved test) LINK_FAIL;
  19 add-only header blocks, equiv_check 0 of 1382 includers differ, planted control flags 210 and reaches every header.
- **Coordinator:** `orig/` rows and tests re-diffed against the repo; D-f HEAD entries rebuilt from the repo (12 rows with retail byte-dump
  comments); headers checked against the agent's originals; tests copied before `land_scripted.py --release-t3 nf324,nf301` (38 landed, 0 held,
  0 failed); includer rechecks on the landed tree: b2 111, b3 320, b4 18, all MATCH / RELOCATION_MATCH PASS (`logs/includer_recheck.log` in
  each batch). 0x00464536 rechecked after its rename (RELOCATION_MATCH PASS).
- **0x004605E0:** now binds the PDB `??4CSpecialAbilitiesThunderLightningStormDef`; the two nf301 T3 lines that listed it as a stand-in binder
  now name only 0x004582BA.
- **Rulings:** D-4 accepted (body-row tests define their callees as member stubs, exactly as the test gate allows). D-3 (the
  `?Copy@C@@QAEXPBVCParentDefClassBase@@@Z` scaffolding helpers in the CFactionDef / CCombatDialogueDef / BurntEffect operator= rows, NF318 Q5
  debt) queued T3 `nf337`: the way out is the 0x00E95380 helper form, as in b6. D-1 / D-2: see `batches/2026-11-24_landing_nf337_b5/README.md`.
- **Census note (agent, evidence/b6/census2_operator_family_HEAD.tsv):** all 39 landed operator= rows at the jmp targets equal retail; 34 are
  NODUPLICATES (declared and defined out of line), 5 pick-any. Declared and implicit operator= mangle the same, so the spelling cannot settle
  which retail had; recorded for the object-order build work, no change now.
- **Debt:** 0x00464536's standalone mock test still names the old classes (T3 `nf337`).

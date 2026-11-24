# nf357 b1 / b1m / b2a / b2b / b2r: GetSubDef<T> pick-any copies made retail-shaped (48 rows landed, 3 held for NF358's census locks)

Agent NF357 (Account A naming queue; hand-back `pending/2026-11-24_nf357_handback/`). Each row emitted a speed-mode NEQ pick-any copy of
`CDefClassBase::GetSubDef<T>` whose retail owner is size-mode. Each now defines the per-type size-body opt-in (NF349 / NF352 form,
`// Fidelity: scaffolding, not retail`):
- `engine/CDefinitionManager.h` (inside GETSUBDEF_BODY): new CAPPEARANCEDEF and CWEAPONDEF blocks.
- `engine/CDefClassBase_GetSubDef.h`: new CWEAPONDEF, CAPPEARANCEDEF and CENTITYSOUNDDEF blocks (the last reuses the existing macro name;
  one macro per type across both headers).
- Owners 0x0042B0A2 CAppearanceDef, 0x0042B1A6 CWeaponDef, 0x0065F604 CEntitySoundDef each set `optimize("y", off)` and `optimize("s", on)`
  and have no CompilerFlags entry (checked). Every block has users among the landed rows.
Three rows (0x00667B30, 0x00735FD0, 0x006668A0) move one include line so the explicit specialization precedes its first use (VC7.1 C2908);
the coordinator's move-only controls on 0x00735FD0 and 0x006668A0 (and the agent's on 0x00667B30) are byte-neutral: objects equal except
`.debug$S`.

Rulings (advisor decision, owner delegated, 2026-11-24): Q1 land the 36 recount rows (b2a / b2b); Q2 the include move, where byte-neutral;
Q3 one macro per type, decided by the whole-catalog equiv_check (0 differing of 57,155 rows; planted control seen in 766 rows); Q4
0x005BADE1 (CInventoryItemDef) is not landed: its residue is the declaration-only StatGetTypeName (queued, T3 nf357); Q5 the ten T3-held
rows land with their unrelated holds (binder / signature ports) parked and restored verbatim (sorted diff equal to HEAD); the B pair
0x005E5F30 / 0x00607330 has only stale 2026-11-12/13 packages and no open claim.

Coordinator verification from the repo (`$S/v357.sh`, agent_tools/nf357/verify_coordinator.sh):
- vland `--recheck-landed`: 48/48 RELOCATION_MATCH PASS.
- pk_all `--leaf` HEAD (agent evidence) v new: the only verdict changes are the 48 GetSubDef copies NEQ -> EQ; no new NEQ, no NEQ reach flip
  (0x007BFE30's GetAllThingsInRoughArea copy differs only in its `?A0x` namespace hash, which is build-path dependent: same NEQ copy as
  HEAD). pickany_vs_retail: no new NEQ line in any row.
- One emitter per block (type x header) against the coordinator's build of the owner (comdat_eq): bytes, relocations, characteristics and
  alignment equal; only the COMDAT selection differs (owner strong 1, copy pick-any 2), as at HEAD.
Held: 0x006096E0, 0x005E5F30, 0x00607330 (NF358's census holds their locks); they land from this hand-back once released.
No T3 line closes: the recount found 47 more rows with their own same-TU GetSubDef bodies (24 NEQ copies under the PDB names) and the
CInventoryItemDef residue; counts are on the nf340 lines.

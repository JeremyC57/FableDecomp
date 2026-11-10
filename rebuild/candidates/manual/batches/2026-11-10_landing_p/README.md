# 2026-11-10 landing p -- inventory weapons screens (agentSCR), 2 rows + 1 rename

Source: `pending/2026-11-10_linux_handSCR/`. vland --land: 2 RELOCATION_MATCH, tests PASS.

- 0x005C8459 CTCInventoryWeapons::ConstructScreen (console, 3,235 bytes; chunk 900's DIFFER(3299v3235) draft finished) and
  0x005C9A95 CTCInventoryWeapons::ConstructScreenPC (PC twin). Same TU devices: optimize("s"), the def lookups and the two entry
  builders visible in the TU (Ego_r donor order), the melee / ranged damage blocks written out (retail expands
  GetActiveMeleeWeaponDamage / GetActiveProjectileWeaponDamage here).
- Headers: new `engine/CWeaponDef.h` (Ego_r layout, Damage +0x44, Shot +0xA0) and `engine/CTCInventoryWeapons.h`;
  opt-ins FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE (engine/CDefPointer.h: the `dec [ecx+4] ; jne` release that keeps the
  pointee in a register), FABLE_RETAIL_CTCWEAPON_DAMAGE (engine/CTCWeapon.h), FABLE_RETAIL_CTCINVENTORYITEM_ISAUGMENTATION
  (engine/CTCInventoryItem.h). Default paths unchanged (hdrcheck2 default=same). Notes per opt-in: `hdr/README.md` in the pending folder.
- Rename: landed row 0x005DAAA0 was CTCWeapon::GetAnimationSpeedValue (a propagated label shared by five addresses); it is
  CTCWeapon::GetDamage (Ego_r 127-byte GetDamage v 34-byte GetAnimationSpeedValue; it reads CWeaponDef::Damage +0x44 where
  0x005DA850 reads AnimationSpeedVal +0x40). Every caller already bound ?GetDamage@CTCWeapon@@QBEMXZ. Rechecked RELOCATION_MATCH.

Not landed: 0x005B8743 CTCInventoryClothing::ConstructScreenPC parked as a flow skeleton (`differ/005b8743.cpp`): it needs the
retail CTCInventoryClothing header members, six unnamed helpers and agentNF's set/map respellings first.
Queued (`land1011p`): 0x005C3180 respell to ?GetActiveProjectileWeaponDamage@CTCInventoryWeapons@@QBEMXZ (row re-author);
0x006869D0 CTCBase::GetDefinitionManager sweep (34 bindings under about 11 spellings; identical-code-folded address).
Binding warning kept: 0x0042ABCA (list<CComponent*> v list<CActionParam*> destructor) is the open debt from codex20nf.

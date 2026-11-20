# NF113 header overlay

## engine/CTCWeapon.h
- Base copy: orig/engine/CTCWeapon.h (byte-equal to rebuild/include/engine/CTCWeapon.h at hand-back).
- One opt-in, `FABLE_RETAIL_CTCWEAPON_ISPROJECTILEWEAPONLOADED`, at the top of the generated (default, non-FABLE_RETAIL_CTCWEAPON) class body:
  the declaration `bool IsProjectileWeaponLoaded() const;` (Ego_r.pdb 0x0063A7C0 / FableWin.pdb 0x0239B430 ?IsProjectileWeaponLoaded@CTCWeapon@@QBE_NXZ).
  Declaration only, no layout change. Used by row 0x005DA980 only.
- Without the macro the default path is token-identical to the repo header (the only diff is the 6-line #ifdef block).
- pin_asserts.py: `engine/CTCWeapon.h pinned 0` (no FABLE_STATIC_ASSERT moved).
- Header users: 70 landed rows include CTCWeapon.h (grep -rlE 'CTCWeapon\.h' rebuild/src/compiled); none defines the new macro, so the opt-in
  changes none of them.
- NF111 / NF112 overlays: neither has an engine/CTCWeapon.h copy (checked $S/w22/agentNF111/hdr and $S/w22/agentNF112/hdr), so the member is declared once.

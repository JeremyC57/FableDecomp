// CTCInventoryWeapons::ConstructWeaponDescriptionDamage(CWideString& description, CThing* weapon, float compareDamage) -- appends the weapon's damage line (and the difference against the wielded weapon) to the description
// Grade: DIFFER(-v698) -- not graded: retail rounds two damage floats with the inline fistp helper (`fstp [x] ; fld [x] ; fistp [y]`, no __ftol2), which C++ under VC7.1 cannot emit without inline assembly (lane gap: inline-asm fistp rounding helper)
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C7452, 698 bytes, ebp frame (size TU), `ret 0Ch`.
// Decode (for Fable 5.1; no compiled source kept):
//   GetHero 0x005C30D3; "..." 0x01230018 -> CGameDefinitionManager::Get 0x0044C6B0 ->GetDefGlobalIndexFromName-style lookup
//   0x009AD390 (with 1) and the def fetch 0x0042B312 into a CDefPointer at [ebp-10h];
//   the weapon's def through CTCBase GetDefinitionManager 0x006869D0 -> 0x009AD9E0 (CDefPointer out [ebp-4]);
//   a CCountedPointer at [ebp+0Ch] (0x0042B1A6), a 0x14-byte STLport vector<long> at [ebp-3Ch] built from the def's
//   index list (+0x40/+0x44) element by element; then the weapon def +0x2C test 0x005DA510 picks the damage either from
//   the def +0x44 or from a second def lookup (+0xA0 index -> def +0x54);
//   damage = GetDamage 0x007661C0 * value * k(0x01237F0C) rounded with fistp; compare = arg * k rounded with fistp;
//   difference = damage - compare; the line is built from "..." 0x01254584 through the text-formatting helpers
//   0x009C95E0 / 0x00405F00 / 0x0099BE70 and appended with CWideString::operator+= 0x0099B8D0 (prefix +0x3C,
//   suffix +0x4C, "+" +0x54 when difference > 0, value +0x50); vector storage released with free() 0x00BFEA14.

# ESCALATE 0x00918A40 CAIStateGroup_GazeAtWork::IsRunnable -- DIFFER(114v116)

Constraint: retail has a CFG join after the inlined GetTC store (`mov eax,[eax+4]` in the join block, `pop esi` closing the
esi region, then a NEW block `mov ecx,eax ; call GetShopTC`) but no extra branch. The HasTC-false edge goes straight to
FindFreeGazePos (0x00918A9F). Ours folds the store into `mov ecx,[eax+4]` and closes the esi region after the call (114 bytes).

Measured (probe/918a40_*.cpp, ATTEMPTS.log):
- n: `if (Creature->HasTC(TCI_SHOP_KEEPER)) { CTCShopKeeper* p; Creature->GetTC(&p); ... }` gives exactly retail's join +
  `pop esi` + `mov ecx,eax`, but VC7.1 keeps GetTC's own (redundant) HasTC test (129 bytes): c2 does not thread it.
- GetTC if-form body (opt-in, removed), GetTC(id) reference + static_cast / GFDebugPDynamicCast, ORIGINAL_CHAIN /
  FORCEINLINE_FIND / ORIGINAL_FIND, =0 initialisers, && chains, early returns, dead FABLE_RETAIL_ASSERT arm: all 114.
- Landed rows with the same retail byte pattern (0x0078C860, 0x00899F90 ...) all leave GetTC's result untested; this row
  tests it (HasTC-false path skips GetShopTC), which is the difference.
Ego_d locals: one block {ptc_shopkeeper, ptc_shop}. Ego_r.exe copy exists (0x0093C490, 138 bytes) but no binary on this box.
Needed: a GetTC / HasTC spelling where the presence test is single and the store reaches its use through a join, or
Ego_r 0x0093C490's shape as evidence.

Addendum: landed 0x00929F10 (CAIStateGroup_ApprenticeHero::IsRunnable, same retail byte pattern) has ONE presence test in
retail (`mov dl,[eax+24h] ; mov cl,8 ; test cl,dl`), then an untested GetInterface whose own dword test VC7.1 removed. Its
presence test is a differently spelled HasTC (byte form local model) with an early `return false`. Our probe n spells both
tests through CThing::HasTC (one IL form): c2 CSEs the word and mask into edx/ecx and keeps both branches.
FABLE_RETAIL_CTHING_HASTC_FORCEINLINE on probe n: same 129. A raw bitset spelling is not possible (UsingComponents protected).

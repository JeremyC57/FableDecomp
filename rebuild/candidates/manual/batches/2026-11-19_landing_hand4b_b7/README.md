# Landing hand4b b7 (2026-11-19): CTCSpecialAbilities::IsAbilityAvailable (0x0072C430, 584 bytes)

vland --land, strict headers: RELOCATION_MATCH + PASS; 7 grades. Name proven (catalog "GetAbilityAvailability" was not a PDB name):
?IsAbilityAvailable@CTCSpecialAbilities@@QBE_NW4EHeroAbility@@@Z; landed 0x0072CF40 already calls it by that name. Shared-header port (CTCSPECIALABILITIES_FULL + USE,
CTCINVENTORYABILITIES + ASSIGN, CTCTargetingBase, GETTC_EXPANDED) with the Ego_d record (type, ptc_inventory_abilities, ptc_targeting, ptarget -- `// Fidelity:`)
in FableWin's statement order; first compiling grade. `// Inferred:` no raise-dead case (FableWin calls IsAbleToCastRaiseDead; retail's jump table sends 7 to the
plain return false -- revision difference). New opt-in CTCSpecialAbilities.h ISABLETOUSEANYABILITY (declaration of 0x0072AF30).
Header-user recheck: pending at commit time (run after the catalog-flags recheck); result recorded in the follow-up commit.

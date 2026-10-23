// Grade: DIFFER(1v1083) -- parked unreached (skeleton only): 0x170 frame: random spawn positions around the caster (GFROR13 random, x87 offsets), CREATURE_OK_UNDEAD_SOLDIER_SWORD creation with FACTION_HERO / BRAIN_PLAYER_UNDEAD_SERVANT and an intelligent-pointer list; not reached beyond decode.
// CCreatureAction_CastWillRaiseDeadSpell::PerformAction() -- PARK SKELETON (decoded only)
// Leaf: PerformAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BC1A0, length 1088 bytes.
// Decode: 0x170 frame: random spawn positions around the caster (GFROR13 random, x87 offsets), CREATURE_OK_UNDEAD_SOLDIER_SWORD creation with FACTION_HERO / BRAIN_PLAYER_UNDEAD_SERVANT and an intelligent-pointer list; not reached beyond decode.
struct CCreatureAction_CastWillRaiseDeadSpell {
    unsigned char _pad[0x100];
    void PerformAction();
};

void CCreatureAction_CastWillRaiseDeadSpell::PerformAction()
{
}

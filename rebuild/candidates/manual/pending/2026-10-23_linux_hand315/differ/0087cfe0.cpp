// Grade: DIFFER(3v1447) -- parked unreached (skeleton only): three local STLport vectors with inline push_back/grow (0x5E1420), sort/unique/erase (memmove 0xBFEAE6) and CRT free; def-pointer refcounts; not reached beyond decode.
// CCreatureAction_InterruptableMidAttackAutoTurn::SetupTurningAnimation() -- PARK SKELETON (decoded only)
// Leaf: SetupTurningAnimation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087CFE0, length 1456 bytes.
// Decode: three local STLport vectors with inline push_back/grow (0x5E1420), sort/unique/erase (memmove 0xBFEAE6) and CRT free; def-pointer refcounts; not reached beyond decode.
struct CCreatureAction_InterruptableMidAttackAutoTurn {
    unsigned char _pad[0x100];
    void SetupTurningAnimation(long direction);
};

void CCreatureAction_InterruptableMidAttackAutoTurn::SetupTurningAnimation(long direction)
{
}

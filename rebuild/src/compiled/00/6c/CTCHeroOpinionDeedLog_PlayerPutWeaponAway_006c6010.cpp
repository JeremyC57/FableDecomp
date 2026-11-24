// CDeedOwner_006c6010::ClearDeed() -- when the deed index at +0x28 is set (not -1): remove it from the witness search, destroy it, remove its crime from the village, then reset the index to -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C6010, length 48 bytes.
// Callees: three members on `this` taking the index (@ 0x006C5920 with ecx passed straight through, @ 0x006C59E0, @ 0x006C5F80; the manifest labels
// "RemoveDeedStillSearchingForWitnesses" / "AIState_DestroyWithPointerCleanup" / "RemoveCrimeFromVillage" are propagated). Shape: the index is re-read from the
// member for each call; the reset is an immediate -1. The chunk label is propagated (the 0x006Cxxxx module is game code); the class is named by address.
struct CDeedOwner_006c6010 {
    unsigned char _pad[0x28];
    long          m_deedIndex;                     // +0x28 (-1 = none)
    void RemoveDeedStillSearchingForWitnesses(long deed);   // 0x006C5920
    void DestroyDeed(long deed);                   // 0x006C59E0
    void RemoveCrimeFromVillage(long deed);        // 0x006C5F80
    void ClearDeed();
};

void CDeedOwner_006c6010::ClearDeed()
{
    if (m_deedIndex != -1) {
        RemoveDeedStillSearchingForWitnesses(m_deedIndex);
        DestroyDeed(m_deedIndex);
        RemoveCrimeFromVillage(m_deedIndex);
        m_deedIndex = -1;
    }
}
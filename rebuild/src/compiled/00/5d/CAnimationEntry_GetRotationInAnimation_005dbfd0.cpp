// CAnimationEntry::GetRotationInAnimation(float* pRotation) -- the negated rotation of the hero ability entry this animation's bank index names
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DBFD0, length 36 bytes (+ int3 padding).
// Callees: CTCInventoryAbilities::GetAbilityEntry(long) @ 0x00A242C0 (manifest "GetAbilityLevel"; the bytes read a
// float at +0x24 of what it returns, so it returns an entry pointer).
// Globals: the application object @ 0x013B86A0 (its +0x24 object is the game, whose +0x3c is the hero's
// abilities component -- the same two hops as the landed GetDisplayEngine bodies @ 0x00640000..).
// Shape: BankIndex (+0x00) is the ability argument; the result is loaded, negated (`fchs`) and stored
// through the out pointer, which is read from the stack after the call.
struct CAbilityEntry {
    unsigned char _pad[0x24];
    float m_rotation;                           // +0x24
};

struct CTCInventoryAbilities {
    CAbilityEntry* GetAbilityEntry(long ability);   // 0x00A242C0
};

struct CGame {
    unsigned char _pad[0x3c];
    CTCInventoryAbilities* m_pHeroAbilities;    // +0x3c
};

struct CApplication {
    unsigned char _pad[0x24];
    CGame* m_pGame;                             // +0x24
};

extern CApplication* g_pApplication;            // 0x013B86A0

struct CAnimationEntry {
    long BankIndex;                             // +0x00
    void GetRotationInAnimation(float* pRotation) const;
};

void CAnimationEntry::GetRotationInAnimation(float* pRotation) const
{
    CTCInventoryAbilities* abilities = g_pApplication->m_pGame->m_pHeroAbilities;
    *pRotation = -abilities->GetAbilityEntry(BankIndex)->m_rotation;
}
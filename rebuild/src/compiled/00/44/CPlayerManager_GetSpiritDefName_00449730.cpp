// CPlayerManager::GetSpiritDefName(long player) -- the co-op spirit definition name for a player slot (0 ONE, 2 THREE, 3 FOUR, 1 and anything else TWO)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00449730, length 113 bytes + a 16-byte jump table (0x004497A4).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 into the hidden result pointer.
// Strings: 0x01231C30 "COOP_SPIRIT_PLAYER_ONE", 0x01231C14 "..._THREE", 0x01231BFC "..._FOUR", 0x01231BE4 "..._TWO".
// Shape: a four-entry jump table over 0..3 (retail table 0x004497A4 = ONE, TWO, THREE, FOUR arms). `case 1` and the
// fall-out return build the same string and VC merges them into one arm placed last, so the arm order in the bytes is
// ONE, THREE, FOUR, TWO; reading the cases off that order (as an earlier version did) gets slots 1-3 wrong. `this` is
// unused (`ret 8`: result + index).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
};

class CPlayerManager { public:
    CCharString GetSpiritDefName(long player);
};

CCharString CPlayerManager::GetSpiritDefName(long player)
{
    switch (player) {
    case 0: return CCharString("COOP_SPIRIT_PLAYER_ONE", -1);
    case 1: return CCharString("COOP_SPIRIT_PLAYER_TWO", -1);
    case 2: return CCharString("COOP_SPIRIT_PLAYER_THREE", -1);
    case 3: return CCharString("COOP_SPIRIT_PLAYER_FOUR", -1);
    }
    return CCharString("COOP_SPIRIT_PLAYER_TWO", -1);
}
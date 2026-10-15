// CPlayerManager::GetSpiritDefName(long player) -- the co-op spirit definition name for a player slot (0 ONE, 1 THREE, 2 FOUR, anything else TWO)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00449730, length 113 bytes + a 16-byte jump table (0x004497A4).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 into the hidden result pointer.
// Strings: 0x01231C30 "COOP_SPIRIT_PLAYER_ONE", 0x01231C14 "..._THREE", 0x01231BFC "..._FOUR", 0x01231BE4 "..._TWO".
// Shape: a four-entry jump table over 0..3; `case 3` and the fall-out return build the same string and VC merges
// them into the last arm (the table's entry 3 is the default target). `this` is unused (`ret 8`: result + index).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
};

struct CPlayerManager {
    CCharString GetSpiritDefName(long player);
};

CCharString CPlayerManager::GetSpiritDefName(long player)
{
    switch (player) {
    case 0: return CCharString("COOP_SPIRIT_PLAYER_ONE", -1);
    case 1: return CCharString("COOP_SPIRIT_PLAYER_THREE", -1);
    case 2: return CCharString("COOP_SPIRIT_PLAYER_FOUR", -1);
    case 3: return CCharString("COOP_SPIRIT_PLAYER_TWO", -1);
    }
    return CCharString("COOP_SPIRIT_PLAYER_TWO", -1);
}
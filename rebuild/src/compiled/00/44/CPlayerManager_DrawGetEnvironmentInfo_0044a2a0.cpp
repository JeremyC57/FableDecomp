// CPlayerManager::DrawGetEnvironmentInfo(long playerNumber, CWideString& text) -- debug HUD text for a co-op (non-main) player: "P<joystick device + 1>:<spirit score>  "; false for the main player or a player without a hero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044A2A0, length 256 bytes, `ret 8`.
// Label: 0x00487510 returns CPlayer +0x228 = JoystickDeviceNumber (PDB +0x230 less the 8 bytes of the 4-byte retail
// PlayerModes list); its manifest label "CWorld::DrawGetEnvironment" and the landed "GetPlayerNumber" are wrong.
// Callees: CWideString::operator=(const wchar_t*) 0x0099B800, CPlayerManager::GetPlayer(long) 0x00449910,
// CPlayer::GetHeroThing 0x00487DC0, CPlayer::IsMainPlayer 0x00487520, CPlayer::GetJoystickDeviceNumber 0x00487510,
// IntToWideString(long) 0x00405F00 and operator+(const wchar_t*, const CWideString&) 0x0099BFF0 (both `__fastcall`,
// hidden result in ecx), CWideString::operator+=(const CWideString&) 0x0099B8D0 / operator+=(const wchar_t*) 0x0099B940,
// ~CWideString 0x0099B510, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0 (TC 0xc7, the co-op
// spirit), CTCCoopSpirit::GetScore 0x0066FB20 (Ego_r.pdb Score +0x10). Literals L"" 0x0122D70C, L"P" 0x01231CDC,
// L":" 0x0122DE70, L"  " 0x01231CD4.
// Layout: CThing TC mask +0x20 (0xc7 = word 6 bit 7, the sign bit of byte +0x38), Components +0x44.
// Shape: the uninitialised `CTCCoopSpirit* spirit; if (HasTC) spirit = GetTC;` (the stale value is the find key's
// frame word); all wide-string temporaries and the key live in the two dead argument slots.
typedef unsigned short wchar_t;                        // VC7.1 default: wchar_t is not a native type
struct CWideString {
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
    const CWideString& operator=(const wchar_t* text); // 0x0099B800
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
    const CWideString& operator+=(const wchar_t* text);        // 0x0099B940
};
CWideString __fastcall IntToWideString(long value);                            // 0x00405F00 (hidden result in ecx)
CWideString __fastcall operator+(const wchar_t* left, const CWideString& right);   // 0x0099BFF0 (hidden result in ecx)

struct CTCBase;
struct CTCEntry {
    long     Type;
    CTCBase* TC;
};
struct CTCMap {
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase* Find(long type) const
    {
        CTCEntry* it = LowerBound((unsigned long)type);
        if (it == _M_finish || type < it->Type)
            it = _M_finish;
        return it->TC;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                  // +0x20
    unsigned char _pad40[4];
    CTCMap        Components;                          // +0x44
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) const { return Components.Find(type); }
};
struct CTCCoopSpirit {                                 // TC type 0xc7
    long GetScore() const;                             // 0x0066FB20
};

struct CPlayer {
    CThing* GetHeroThing();                            // 0x00487DC0
    bool IsMainPlayer() const;                         // 0x00487520
    long GetJoystickDeviceNumber() const;                      // 0x00487510
};

struct CPlayerManager {
    CPlayer* GetPlayer(long number);                   // 0x00449910
    bool DrawGetEnvironmentInfo(long playerNumber, CWideString& text);
};

bool CPlayerManager::DrawGetEnvironmentInfo(long playerNumber, CWideString& text)
{
    text = L"";
    CPlayer* player = GetPlayer(playerNumber);
    CThing* hero = player->GetHeroThing();
    if (player->IsMainPlayer() || hero == 0)
        return false;
    text += L"P" + IntToWideString(player->GetJoystickDeviceNumber() + 1);
    text += L":";
    CTCCoopSpirit* spirit;
    if (hero->HasTC(0xc7))
        spirit = (CTCCoopSpirit*)hero->GetTC(0xc7);
    text += IntToWideString(spirit->GetScore());
    text += L"  ";
    return true;
}
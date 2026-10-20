// CPlayerManager::LoadGameState(CPersistContext& context) -- uninitialise every joystick-associated non-main player, read the main player's save record (character UID, region name), re-initialise the main player's character from it and give the main player's joystick to the joystick manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00449E60, length 298 bytes, `ret 4`.
// Callees: CPlayerManager::IsPlayerAssociatedWithJoystickDeviceNumber 0x004499E0, CPlayerManager::GetPlayer(long)
// 0x004498C0 (out of line in the first loop; expanded inline for the main player twice), CPlayer::UninitCharacter
// 0x00487BD0, CCharString() 0x0099E4B0 / CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
// CPersistContext::Transfer(const char*, unsigned __int64&, const unsigned __int64&) 0x0044BB00 and Transfer(const
// char*, CCharString&, const CCharString&) 0x004109A0 (propagated labels), CPlayer::InitCharacterFromSave 0x00487EF0,
// CPlayer::GetJoystickDeviceNumber 0x00487510, CGameJoystickManager::Get 0x0049BB90 (`__fastcall`, no arguments) and
// SetMainPlayerJoystickDeviceNumber 0x0049BB30. Literals "PlayerCharacterUID" 0x01231CAC, "PlayerRegionName" 0x01231C98.
// Layout (retail, Ego_r.pdb with a 12-byte vector): Players (vector<CPlayer*>) +0x0c, PlayerNeutral +0x18, MainPlayer
// +0x1c; CPlayer PlayerID +0x28; CPlayerSave {unsigned __int64 CharacterUID; CCharString RegionName;} (16 bytes).
// Shape: the main-player lookup is GetPlayer's body (count cached before the loop) as a __forceinline helper (chunk 225 precedent: the expanded
// sites through an inline helper, the rest through the out-of-line function); the found case leaves the loop for an
// out-of-band `mov ecx,[edi+ecx*4] ; jmp` block; the UID default is a zero __int64 temporary.
struct CCharString {
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CPlayerSave {                                   // Ego_r.pdb (16 bytes)
    unsigned __int64 CharacterUID;                     // +0x00
    CCharString      RegionName;                       // +0x08
};
struct CPersistContext {
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue);   // 0x0044BB00
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);             // 0x004109A0
};
struct CPlayer {
    unsigned char _pad00[0x28];
    long          PlayerID;                            // +0x28
    void UninitCharacter();                            // 0x00487BD0
    bool InitCharacterFromSave(CPlayerSave* save);     // 0x00487EF0
    long GetJoystickDeviceNumber() const;                      // 0x00487510
};
struct CGameJoystickManager {
    static CGameJoystickManager* __fastcall Get();     // 0x0049BB90
    void SetMainPlayerJoystickDeviceNumber(long device);   // 0x0049BB30
};

template <class T> struct vector {                     // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](long n) { return *(_M_start + n); }
};

struct CPlayerManager {
    void*            __vftable;                        // +0x00
    void*            Component;                        // +0x04
    void*            DefinitionManager;                // +0x08
    vector<CPlayer*> Players;                          // +0x0c
    long             PlayerNeutral;                    // +0x18
    long             MainPlayer;                       // +0x1c
    bool IsPlayerAssociatedWithJoystickDeviceNumber(long device);   // 0x004499E0
    CPlayer* GetPlayer(long id);                       // 0x004498C0
    __forceinline CPlayer* FindPlayer(long id)         // GetPlayer's body, expanded inline
    {
        long count = Players.size();
        for (long i = 0; i < count; ++i)
            if (Players[i]->PlayerID == id)
                return Players[i];
        return Players[PlayerNeutral];
    }
    void LoadGameState(CPersistContext& context);
};

void CPlayerManager::LoadGameState(CPersistContext& context)
{
    for (long i = 0; i < 4; ++i) {
        if (IsPlayerAssociatedWithJoystickDeviceNumber(i) && MainPlayer != i)
            GetPlayer(i)->UninitCharacter();
    }
    CPlayerSave save;
    context.Transfer("PlayerCharacterUID", save.CharacterUID, (unsigned __int64)0);
    context.Transfer("PlayerRegionName", save.RegionName, "");
    FindPlayer(MainPlayer)->InitCharacterFromSave(&save);
    CGameJoystickManager::Get()->SetMainPlayerJoystickDeviceNumber(FindPlayer(MainPlayer)->GetJoystickDeviceNumber());
}

// CWorld::IsDiskSpaceForSaveGame()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049D4B0, oracle length 47 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push ecx ; push ebx ; lea eax,[esp+4] ; push eax ; call CTBaseSingleton<CUserProfileManager>::Get ; mov ecx,eax ; call CUserProfileManager::GetDirectoryNameFromProfileName ; mov edx,eax ; mov ecx,96000h ; call IsRoomForFile ; lea ecx,[esp+4] ; mov bl,al ; call CWideString::~CWideString ; mov al,bl ; pop ebx ; pop ecx ; ret
// `this` is unused. Asks the profile manager singleton (0x0040D2A0, __fastcall, landed) for the
// profile's save directory (0x004071C0, __thiscall, CWideString through a hidden pointer) and
// checks that 0x96000 (614,400) bytes are free there (IsRoomForFile @ 0x00999AB0, __fastcall:
// size in ecx, directory in edx). The directory string is destroyed after the result is saved.

struct CWideString {
    void* PStringData;       // +0x00
    ~CWideString();          // 0x0099B510
};

struct CUserProfileManagerCalls {
    CWideString GetDirectoryNameFromProfileName();     // 0x004071C0
};

extern CUserProfileManagerCalls* __fastcall CUserProfileManager_Get();     // 0x0040D2A0
extern bool __fastcall IsRoomForFile(long byteCount, CWideString* directory);   // 0x00999AB0

#define SAVE_GAME_RESERVED_BYTES 0x96000

struct CWorld_Methods {
    bool IsDiskSpaceForSaveGame();
};

bool CWorld_Methods::IsDiskSpaceForSaveGame()
{
    CWideString directory = CUserProfileManager_Get()->GetDirectoryNameFromProfileName();
    return IsRoomForFile(SAVE_GAME_RESERVED_BYTES, &directory);
}

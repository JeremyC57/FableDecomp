// CWorld::IsDiskSpaceForSaveGame()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049D4B0, oracle length 47 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push ecx ; push ebx ; lea eax,[esp+4] ; push eax ; call CTBaseSingleton<CUserProfileManager>::Get ; mov ecx,eax ; call CUserProfileManager::GetDirectoryNameFromProfileName ; mov edx,eax ; mov ecx,96000h ; call IsRoomForFile ; lea ecx,[esp+4] ; mov bl,al ; call CWideString::~CWideString ; mov al,bl ; pop ebx ; pop ecx ; ret
// `this` is unused. Asks the profile manager singleton (0x0040D2A0, __fastcall, landed) for the
// profile's save directory (0x004071C0, __fastcall, CWideString through a hidden pointer) and
// checks that 0x96000 (614,400) bytes are free there (IsRoomForFile @ 0x00999AB0, __fastcall:
// size in ecx, directory in edx). The directory string is destroyed after the result is saved.
//
// Lane feedback: the directory is an unnamed temporary (`mov ecx,eax` straight from the by-value return) and
// the getter is __stdcall; a named local re-derives the address and a __cdecl call adds
// `add esp,4`. RELOCATION_MATCH.
#include <stddef.h>

class CWideString { public:
    void* PStringData;       // +0x00
    ~CWideString();          // 0x0099B510
    const wchar_t* GetText();   // 0x004071C0 (text accessor on the temporary)
};

// Returns the save-game directory name by value; __stdcall (the hidden result pointer is
// callee-cleaned: no `add esp,4` after the call).
extern CWideString __stdcall GFGetSaveGameDirectoryName();     // 0x0040D2A0
extern bool __fastcall IsRoomForFile(long byteCount, const wchar_t* directory);   // 0x00999AB0

#define SAVE_GAME_RESERVED_BYTES 0x96000

struct CWorld_Methods {
    bool IsDiskSpaceForSaveGame();
};

bool CWorld_Methods::IsDiskSpaceForSaveGame()
{
    // Is there room for a 600 KiB save in the profile's save directory? The directory string is
    // a temporary that lives until the full expression ends (destroyed after the space check).
    return IsRoomForFile(SAVE_GAME_RESERVED_BYTES, GFGetSaveGameDirectoryName().GetText());
}
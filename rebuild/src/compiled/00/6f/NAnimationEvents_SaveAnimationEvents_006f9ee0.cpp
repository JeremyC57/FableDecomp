// NAnimationEvents::SaveAnimationEvents() -- save the game and sound animation-event managers to Data\Misc\game_animation_events.txt / sound_animation_events.txt
// Leaf: ?SaveAnimationEvents@NAnimationEvents@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F9EE0, 161 bytes, plain `ret` (__fastcall, no arguments).
// Callees: CWideString(const wchar_t*) 0x0099B6B0, NResourceDirectoryNames::GetMiscDir() 0x0041A0A0 (__fastcall,
//   hidden result in ecx), operator+(const CWideString&, const CWideString&) 0x0099BE70 (__fastcall), CManager::Save
//   0x006F9BE0, ~CWideString 0x0099B510.
// Data: the two counted manager pointers 0x013BABB4 (game events) / 0x013BABBC (sound events) -- descriptive names;
//   the inline accessors 0x006F90B0 / 0x006F90C0 return `*pointer`.
// Shape: one statement per manager, `GetXManager().Save(GetMiscDir() + CWideString(L"..."))`: VC builds the literal
//   string first, then the directory, then the sum, and destroys the three temporaries in reverse.
// Lane note: wchar_t is not a built-in type in the lane's VC7.1 build, so the literal constructor takes `const unsigned
//   short*` (same mangling-independent code).
class CWideString {                                       // 4 bytes
public:
    void* PStringData;                                    // +0x00
    CWideString(const unsigned short* text);              // 0x0099B6B0
    ~CWideString();                                       // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70

namespace NResourceDirectoryNames {
CWideString __fastcall GetMiscDir();                      // 0x0041A0A0 (L"Data\\Misc\\")
}

class CCPPointerInfo;
template<class T> class CCountedPointer {                 // Ego_r.pdb: 8 bytes
public:
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
    T& operator*() const { return *Data; }
};

namespace NAnimationEvents {
class CManager {
public:
    void Save(const CWideString& fileName);               // 0x006F9BE0
};

extern CCountedPointer<CManager> GameEventManager;        // 0x013BABB4 (descriptive name)
extern CCountedPointer<CManager> SoundEventManager;       // 0x013BABBC (descriptive name)

inline CManager& __fastcall GetGameEventManager() { return *GameEventManager; }     // 0x006F90B0 (inlined here)
inline CManager& __fastcall GetSoundEventManager() { return *SoundEventManager; }   // 0x006F90C0 (inlined here)

void __fastcall SaveAnimationEvents()
{
    GetGameEventManager().Save(NResourceDirectoryNames::GetMiscDir() + CWideString(L"game_animation_events.txt"));
    GetSoundEventManager().Save(NResourceDirectoryNames::GetMiscDir() + CWideString(L"sound_animation_events.txt"));
}
}
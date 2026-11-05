// NAnimationEvents::LoadAnimationEvents(bool displayProgress) -- load the game and sound animation-event managers from Data\Misc\game_animation_events.txt / sound_animation_events.txt, with optional progress text
// Leaf: ?LoadAnimationEvents@NAnimationEvents@@YIX_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FABF0, 259 bytes, plain `ret` (__fastcall, the flag in cl).
// Label: catalog "LoadAnimationEvents" (global) -> NAnimationEvents::LoadAnimationEvents (Ego_r.pdb
//   ?LoadAnimationEvents@NAnima...); the twin of the landed NAnimationEvents::SaveAnimationEvents 0x006F9EE0.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, NProgressDisplay::DisplayProgress
//   0x009E9F40 (__fastcall: text ecx, the float on the stack, a in dl, b on the stack), CWideString(const wchar_t*)
//   0x0099B6B0, NResourceDirectoryNames::GetMiscDir() 0x0041A0A0 (hidden result in ecx), operator+(const CWideString&,
//   const CWideString&) 0x0099BE70, CManager::Load 0x006FA4E0, ~CWideString 0x0099B510.
// Data: the counted manager pointers 0x013BABB4 (game events) / 0x013BABBC (sound events) -- descriptive names, as in
//   the landed SaveAnimationEvents.
// Shape: per manager, `if (displayProgress) DisplayProgress("Loading ...", -1.0f, false, false);` then
//   `GetXManager().Load(GetMiscDir() + CWideString(L"..."))`.
// Lane note: wchar_t is not a built-in type in the lane's VC7.1 build, so the literal constructor takes `const unsigned
//   short*` (same mangling-independent code).
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
};
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}

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

#include "engine/CCountedPointer.h"

namespace NAnimationEvents {
class CManager {
public:
    bool Load(const CWideString& fileName);               // 0x006FA4E0
};

extern CCountedPointer<CManager> GameEventManager;        // 0x013BABB4 (descriptive name)
extern CCountedPointer<CManager> SoundEventManager;       // 0x013BABBC (descriptive name)

inline CManager& __fastcall GetGameEventManager() { return *GameEventManager; }     // 0x006F90B0 (inlined here)
inline CManager& __fastcall GetSoundEventManager() { return *SoundEventManager; }   // 0x006F90C0 (inlined here)

void __fastcall LoadAnimationEvents(bool displayProgress)
{
    if (displayProgress)
        NProgressDisplay::DisplayProgress("Loading Game Animation Events", -1.0f, false, false);
    GetGameEventManager().Load(NResourceDirectoryNames::GetMiscDir() + CWideString(L"game_animation_events.txt"));
    if (displayProgress)
        NProgressDisplay::DisplayProgress("Loading Sound Animation Events", -1.0f, false, false);
    GetSoundEventManager().Load(NResourceDirectoryNames::GetMiscDir() + CWideString(L"sound_animation_events.txt"));
}
}
// CFrontendGameComponent::SetMode(EGameMode mode) @ 0x00495FE0.
// Leaf: ?SetMode@CFrontendGameComponent@@AAEXW4EGameMode@@@Z
// Names (NF197, 2026-11-20; in place in the row's HEAD-local CFrontendGameComponent model):
//   - own symbol: Ego_r.pdb 0x0049ED70 + FableWin.pdb 0x01A45E40 ?SetMode@CFrontendGameComponent@@AAEXW4EGameMode@@@Z (private, the
//     mode an EGameMode; was ?SetMode@CFrontendGameComponent@@QAEXJ@Z); the landed caller Input 0x004963A0 binds the PDB spelling and
//     models EGameMode as the same empty local enum (no shared header has it).
//   - callees: the save / profile list refreshes are the static __fastcall CFrontendGameComponent::GetSaveNames 0x00495830 and
//     GetProfileNames 0x004959E0 (both PDBs, vector<CWideString>&; ecx = &SaveNames +0x1E1C / &ProfileNames +0x1E0C), not the
//     stand-ins CFrontendSaveList_00495fe0 / CFrontendProfileList_00495fe0::Refresh.
//   - members (Ego_r.pdb CFrontendGameComponent less the three 4-byte vector shrinks, as 0x004963A0 lays them out): Mode +0x1C,
//     String[128] +0x168 / StringPos +0x1E8 (the ClearString body), ProfileNames / ProfileNamesOffset +0x1E0C / +0x1E18, SaveNames /
//     SaveNamesOffset +0x1E1C / +0x1E28, WorldToLoad +0x1E40, CursorHighlight +0x1E44 (was m_ModePointerEnabled).

#include <cstring>
#include "stlport/fable_containers.h"

// Naming (naming wave 24): the CWideString at +0x1E40 is Ego_r's WorldToLoad (CFrontendGameComponent PDB +0x1E4C less the three 4-byte
// vector shrinks; the landed GetWorldNames 0x00495890 models it so), cleared
// to the empty wide literal 0x0122D70C through CWideString::operator=(const wchar_t*) 0x0099B800 (??4CWideString@@QAEABV0@PBG@Z); it
// was a CFrontendModePointer_00495fe0 stand-in with an Assign member and a g_EmptyFrontendModePointer_00495fe0 object.
class CWideString
{
    void* PStringData;

public:
    const CWideString& operator=(const unsigned short* text);   // 0x0099B800
};

enum EGameMode { };

class CFrontendGameComponent
{
    unsigned char m_Pad0000[0x1c];
    EGameMode Mode;                                     // +0x1C
    unsigned char m_Pad0020[0x148];
    char String[128];                                   // +0x168
    long StringPos;                                     // +0x1E8
    unsigned char m_Pad01ec[0x1c20];
    std::vector<CWideString> ProfileNames;              // +0x1E0C
    long ProfileNamesOffset;                            // +0x1E18
    std::vector<CWideString> SaveNames;                 // +0x1E1C
    long SaveNamesOffset;                               // +0x1E28
    unsigned char m_Pad1e2c[0x14];
    CWideString WorldToLoad;                            // +0x1E40
    long CursorHighlight;                               // +0x1E44

public:
    static void __fastcall GetSaveNames(std::vector<CWideString>& names);      // 0x00495830
    static void __fastcall GetProfileNames(std::vector<CWideString>& names);   // 0x004959E0
private:
    void SetMode(EGameMode mode);
    void GetWorldNames();   // 0x00495890
};

void CFrontendGameComponent::SetMode(EGameMode mode)
{
    Mode = mode;
    CursorHighlight = 1;
    std::memset(String, 0, sizeof(String));
    StringPos = 0;

    if (Mode == 10)
    {
        GetWorldNames();
        return;
    }

    if (mode == 11)
    {
        GetSaveNames(SaveNames);
        WorldToLoad = L"";
        SaveNamesOffset = 0;
        return;
    }

    if (mode == 12)
    {
        GetProfileNames(ProfileNames);
        ProfileNamesOffset = 0;
    }
}

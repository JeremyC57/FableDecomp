// CSubtitleRenderer::GetTextFromEmbeddedTagGraphic(long graphic) const -- the localised controller-side caption for an embedded button graphic: TEXT_GUI_CONTROLLER_LEFT for the A/X/D-pad-left graphics, TEXT_GUI_CONTROLLER_RIGHT for B/Y/D-pad-right, else the empty wide string
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048DF77, 155 bytes; `ret 8` (hidden result + graphic).
// Callees: CCharString() @ 0x0099E4B0, PeekGuiDef @ 0x00438570 (__fastcall, the player GUI def), CCharString::operator=(const char*)
//          @ 0x0099EFE0, CCharString::operator!=(const char*) @ 0x0099E960, CDataBank::GetTextBySymbol(const CCharString&) @ 0x009C95E0
//          on the main game component's text bank (+0x14 of the global @ 0x013B86A0), CWideString(const CWideString&) @ 0x0099B720
//          (the global empty wide string @ 0x013BCA24), ~CCharString @ 0x0099EAE0.
// Layout: CPlayerGuiDef controller graphics retail +0x66c (A), +0x670 (B), +0x674 (X), +0x678 (Y), +0x684 (D-pad left),
//         +0x688 (D-pad right) = Ego_r.pdb offsets -0x18.
// Shape: frame-pointer size TU; an if/else-if chain whose equal assignments the size optimiser tail-merges.
class CWideString {
public:
    unsigned short* m_pData;
    CWideString(const CWideString& other);         // 0x0099B720
};
extern const CWideString g_EmptyWideString;        // 0x013BCA24
class CCharString {
public:
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
    const CCharString& operator=(const char* text);      // 0x0099EFE0
    bool operator!=(const char* text) const;       // 0x0099E960
};
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
using NGameText::CDataBank;
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                        // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
class CPlayerGuiDef { public:
    unsigned char _pad000[0x66c];
    long          ControllerAGraphic;              // +0x66c
    long          ControllerBGraphic;              // +0x670
    long          ControllerXGraphic;              // +0x674
    long          ControllerYGraphic;              // +0x678
    long          ControllerDPadGraphic;           // +0x67c
    long          ControllerDPadDownGraphic;       // +0x680
    long          ControllerDPadLeftGraphic;       // +0x684
    long          ControllerDPadRightGraphic;      // +0x688
};
class CPlayerGui {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();   // 0x00438570
};      // 0x00438570
class CSubtitleRenderer { public:
    CWideString GetTextFromEmbeddedTagGraphic(long graphic) const;
};

CWideString CSubtitleRenderer::GetTextFromEmbeddedTagGraphic(long graphic) const
{
    CCharString tag;
    const CPlayerGuiDef* gui = (&CPlayerGui::PeekGuiDef());
    if (graphic == gui->ControllerAGraphic)
        tag = "TEXT_GUI_CONTROLLER_LEFT";
    else if (graphic == gui->ControllerBGraphic)
        tag = "TEXT_GUI_CONTROLLER_RIGHT";
    else if (graphic == gui->ControllerXGraphic)
        tag = "TEXT_GUI_CONTROLLER_LEFT";
    else if (graphic == gui->ControllerYGraphic)
        tag = "TEXT_GUI_CONTROLLER_RIGHT";
    else if (graphic == gui->ControllerDPadLeftGraphic)
        tag = "TEXT_GUI_CONTROLLER_LEFT";
    else if (graphic == gui->ControllerDPadRightGraphic)
        tag = "TEXT_GUI_CONTROLLER_RIGHT";
    if (tag != "")
        return g_pMainGameComponent->TextBank->GetTextBySymbol(tag);
    return g_EmptyWideString;
}
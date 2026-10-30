// AddLogbookTutorialEntryPC(const CCharString& tag) -- add the PC variant of a tutorial entry: title and heading from <tag>_TITLE, text from <tag>_PC (category 2), then yield a script frame
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBEA81, 153 bytes; a __fastcall global (PDB ?AddLogbookTutorialEntryPC@@YIXABVCCharString@@@Z).
// Label: the catalog's "CSubtitleRenderer::SetText" is propagated; the PDB names the quest-helper globals beside
//        FadeOutThing / IsCutsceneSkipped / GetHeroTitleAsLHTSTag in this TU.
// Callees: operator+(const CCharString&, const char*) @ 0x0099F600 (__fastcall, hidden result in ecx; strings 0x0122E240 "_TITLE", 0x0122E248 "_PC"),
//          CCharString::ToWideString @ 0x0099E480, CGameScriptInterface::AddLogBookEntry (slot 308, +0x4d0) and NewScriptFrame
//          (slot 7, +0x1c) through the global @ 0x0143E8F8, ~CWideString @ 0x0099B510, ~CCharString @ 0x0099EAE0.
// Shape: frame-pointer size TU; the tag strings are named locals, the three wide conversions are argument temporaries
//        (right to left), the interface vtable is read before them.
// Leaf: ?AddLogbookTutorialEntryPC@@YIXABVCCharString@@@Z
class CWideString {
public:
    unsigned short* m_pData;
    ~CWideString();                                // 0x0099B510
};
class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    CWideString ToWideString() const;              // 0x0099E480
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
enum ECategory { LOGBOOK_CATEGORY_STORY = 1, LOGBOOK_CATEGORY_TUTORIAL = 2 };   // CHeroLogBook::CHeroLogBookEntry::ECategory
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

void __fastcall AddLogbookTutorialEntryPC(const CCharString& tag);

void __fastcall AddLogbookTutorialEntryPC(const CCharString& tag)
{
    CCharString title = tag + "_TITLE";
    CCharString text = tag + "_PC";
    g_pGameScriptInterface->AddLogBookEntry(title.ToWideString(), title.ToWideString(), text.ToWideString(), LOGBOOK_CATEGORY_TUTORIAL);
    g_pGameScriptInterface->NewScriptFrame();
}
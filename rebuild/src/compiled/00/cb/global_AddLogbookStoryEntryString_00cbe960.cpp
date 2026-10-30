// AddLogbookStoryEntryString(const CCharString& tag) -- add a story entry to the hero's log book: title and heading from <tag>_NAME, text from <tag>_DESC (category 1)
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE960, 142 bytes; a __fastcall global (PDB ?AddLogbookStoryEntryString@@YIXABVCCharString@@@Z).
// Label: the catalog's "CSubtitleRenderer::SetText" is propagated; the PDB names the quest-helper globals beside
//        FadeOutThing / IsCutsceneSkipped / GetHeroTitleAsLHTSTag in this TU.
// Callees: operator+(const CCharString&, const char*) @ 0x0099F600 (__fastcall, hidden result in ecx; strings 0x0122D770 "_NAME", 0x0122D768 "_DESC"),
//          CCharString::ToWideString @ 0x0099E480, CGameScriptInterface::AddLogBookEntry (slot 308, +0x4d0) and NewScriptFrame
//          (slot 7, +0x1c) through the global @ 0x0143E8F8, ~CWideString @ 0x0099B510, ~CCharString @ 0x0099EAE0.
// Shape: frame-pointer size TU; the tag strings are named locals, the three wide conversions are argument temporaries
//        (right to left), the interface vtable is read before them.
// Leaf: ?AddLogbookStoryEntryString@@YIXABVCCharString@@@Z
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

void __fastcall AddLogbookStoryEntryString(const CCharString& tag);

void __fastcall AddLogbookStoryEntryString(const CCharString& tag)
{
    CCharString name = tag + "_NAME";
    CCharString desc = tag + "_DESC";
    g_pGameScriptInterface->AddLogBookEntry(name.ToWideString(), name.ToWideString(), desc.ToWideString(), LOGBOOK_CATEGORY_STORY);
}
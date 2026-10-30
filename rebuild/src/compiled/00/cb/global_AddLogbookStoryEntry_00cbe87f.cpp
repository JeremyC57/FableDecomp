// AddLogbookStoryEntry(long index) -- add story entry <index> to the hero's log book: title and heading from TEXT_QST_LOG_STORY_<index>_NAME, text from ..._DESC (category 1)
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE87F, 225 bytes; a __fastcall global (Ego_r.pdb ?AddLogbookStoryEntry@@YIXJ@Z), the
//   numbered sibling of the landed AddLogbookStoryEntryString 0x00CBE960 (same TU).
// Label: the catalog's "CSubtitleRenderer::SetText" is propagated (as on 0x00CBE960).
// Callees: GFIntToCharString(long) @ 0x0099F830 (__fastcall, hidden result in ecx), CCharString::CCharString(const char*, long)
//   @ 0x0099EBF0 ("TEXT_QST_LOG_STORY_" @ 0x012C1754), operator+(const CCharString&, const CCharString&) @ 0x0099F570 and
//   operator+(const CCharString&, const char*) @ 0x0099F600 (both __fastcall, hidden result in ecx; "_" @ 0x01244DB4,
//   "NAME" @ 0x012C174C, "DESC" @ 0x012C1744), CCharString::ToWideString @ 0x0099E480, CGameScriptInterface::AddLogBookEntry
//   (slot 308, +0x4d0) through the global @ 0x0143E8F8, ~CWideString @ 0x0099B510, ~CCharString @ 0x0099EAE0.
// Shape: frame-pointer size TU; the tag is one chained expression evaluated right to left ("_" pushed first, the number
//   converted before the explicit literal temporary, whose returned pointer feeds the first operator+); the three
//   temporaries die at the end of the full expression and their frame words are reused by the wide conversions; the
//   interface vtable is read before the argument temporaries.
// Leaf: ?AddLogbookStoryEntry@@YIXJ@Z
class CWideString {
public:
    unsigned short* m_pData;
    ~CWideString();                                // 0x0099B510
};
class CCharString {
public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    CWideString ToWideString() const;              // 0x0099E480
};
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
enum ECategory { LOGBOOK_CATEGORY_STORY = 1, LOGBOOK_CATEGORY_TUTORIAL = 2 };   // CHeroLogBook::CHeroLogBookEntry::ECategory
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

void __fastcall AddLogbookStoryEntry(long index);

void __fastcall AddLogbookStoryEntry(long index)
{
    CCharString tag = CCharString("TEXT_QST_LOG_STORY_") + GFIntToCharString(index) + "_";
    CCharString name = tag + "NAME";
    CCharString desc = tag + "DESC";
    g_pGameScriptInterface->AddLogBookEntry(name.ToWideString(), name.ToWideString(), desc.ToWideString(), LOGBOOK_CATEGORY_STORY);
}
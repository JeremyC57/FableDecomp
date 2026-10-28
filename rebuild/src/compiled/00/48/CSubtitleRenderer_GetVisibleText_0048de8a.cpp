// CSubtitleRenderer::GetVisibleText(const CWideString& text, CWideString& visible) -- copy the subtitle text, strip a leading "(HC)" marker (hide cursor), cut it to the characters revealed so far (or finish the reveal); true unless the marker was there
// Layout (supersedes any local-model note above): engine/CSubtitleRenderer.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CSubtitleRenderer CSubtitleRendererDonor
#include "engine/CSubtitleRenderer.h"
#undef CSubtitleRenderer
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048DE8A, 237 bytes, `ret 8`. Ego_r.pdb `?GetVisibleText@CSubtitleRenderer@@AAE_NABVCWideString@@AAV2@@Z`.
// Leaf: GetVisibleText
// Callees: CWideString::operator=(const CWideString&) @ 0x0099B7D0, CCharString::CCharString(const char*, long) @ 0x0099EBF0
//   ("(HC)" @ 0x0123172C), CWideString::ConvertToCharString() const @ 0x0099B2C0 (landed name; PDB ToCharString), strncmp
//   @ 0x00BFEAF8, ~CCharString @ 0x0099EAE0, CWideString::GetLength() const @ 0x0099B220, CWideString::GetSubString(unsigned
//   long, unsigned long) const @ 0x0099BDB0 (by value), ~CWideString @ 0x0099B510, CWideString::SetLength(long) @ 0x0099BC50.
// Shape: frame-pointer size TU (as the landed CSubtitleRenderer rows). The marker test is an inline prefix compare: the
//   "(HC)" temporary (built first, in the dead `text` slot) is read for text and length with one null test (the empty string
//   preloaded), the narrowed copy through a null-first accessor; the result byte comes from `strncmp(...) ? false : true`
//   (neg/sbb/inc) and survives both destructors in bl. The sub-string temporary reuses the dead `text` slot too. The
//   revealed count is read into a local before the length call.
// Retail layout (engine/CSubtitleRenderer.h, flags at Ego_r.pdb - 8): TextCharactersShowing +0x24, AllCharactersDisplayed +0xDD,
//   InCutscene +0xDE.
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);   // 0x00BFEAF8

class CCharStringData { public:
    const char*   Text;                                  // +0x00
    long          Length;                                // +0x04
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->Text; }
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                      // 0x0099B510
    const CWideString& operator=(const CWideString& other);    // 0x0099B7D0
    CCharString ConvertToCharString() const;             // 0x0099B2C0
    long GetLength() const;                              // 0x0099B220
    CWideString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099BDB0
    void SetLength(long length);                         // 0x0099BC50
};

static inline bool BeginsWith(const CCharString& text, const CCharString& prefix)
{
    const char* prefixText;
    long prefixLength;
    if (prefix.PStringData == 0) {
        prefixLength = 0;
        prefixText = "";
    } else {
        prefixLength = prefix.PStringData->Length;
        prefixText = prefix.PStringData->Text;
    }
    return strncmp(text.GetPString(), prefixText, prefixLength) ? false : true;
}

class CSubtitleRenderer : public CSubtitleRendererDonor {
public:
    bool GetVisibleText(const CWideString& text, CWideString& visible);
};

bool CSubtitleRenderer::GetVisibleText(const CWideString& text, CWideString& visible)
{
    visible = text;
    bool hideCursor = BeginsWith(visible.ConvertToCharString(), "(HC)");
    if (hideCursor)
        visible = visible.GetSubString(4, visible.GetLength());
    if (!InCutscene && !AllCharactersDisplayed) {
        long showing = TextCharactersShowing;
        if (showing < visible.GetLength()) {
            visible.SetLength(showing);
            return !hideCursor;
        }
    }
    TextCharactersShowing = visible.GetLength();
    AllCharactersDisplayed = true;
    return !hideCursor;
}
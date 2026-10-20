// Grade: DIFFER(197v184) -- the inline CCharString::operator==("") materialises its bool with `sete al` and re-tests it in the lane where retail branches straight off the strcmp / block compare (the chunk 188 lane gap); the rest lines up. Without __forceinline the operator is called out of line.
// CSubtitleRenderer::SetButton2OverrideText(const CCharString& text) -- outside cutscene action mode: an empty text restores the default button-2 caption (the TEXT_GUI_SKIP_SCENE text tag) and clears the override, else the caption becomes the text-tag lookup of the text (or the text itself when it is no tag)
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048F881, 184 bytes, `ret 4`. Ego_r.pdb `?SetButton2OverrideText@CSubtitleRenderer@@QAEXABVCCharString@@@Z`.
// Leaf: SetButton2OverrideText
// Callees: CBasicString<char>::operator==(const char*) @ 0x004115A0 (inside the inline CCharString::operator==, "" @ 0x0122D70E),
//   CCharString::ToWideString() const @ 0x0099E480, CSubtitleRenderer::GetTextFromTextTag(const CWideString&, CWideString&)
//   @ 0x0048F6BD (PDB private, bool), CWideString::operator=(const CWideString&) @ 0x0099B7D0, ~CWideString @ 0x0099B510,
//   CWideString::CWideString(const unsigned short*) @ 0x0099B6B0 (L"TEXT_GUI_SKIP_SCENE" @ 0x01238198).
// Shape: frame-pointer size TU (as the landed CSubtitleRenderer rows); `this` is spilled to [ebp-8] because the lookup's
//   negated result rides in bl across the wide temporary's destructor; the inline operator==("") (strcmp intrinsic on the
//   unassigned case, the block compare otherwise) puts the empty-text arm after the function's epilogue; the wide
//   temporaries live in the frame word and the dead `text` slot.
// Retail layout (the landed CSubtitleRenderer rows): Button2OverrideText +0x74, CutsceneActionMode +0xE2,
//   Button2TextOverridden +0xE4.
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CWideString {
public:
    void* PStringData;
    CWideString(const unsigned short* text);    // 0x0099B6B0 (wchar_t is unsigned short under the era defaults)
    ~CWideString();                             // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CWideString ToWideString() const;           // 0x0099E480
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
};

class CSubtitleRenderer {
public:
    void*         __vftable;                    // +0x00
    unsigned char _pad04[0x70 - 0x04];
    CWideString   Button1OverrideText;          // +0x70
    CWideString   Button2OverrideText;          // +0x74
    unsigned char _pad78[0xe2 - 0x78];
    bool          CutsceneActionMode;           // +0xe2
    bool          Button1TextOverridden;        // +0xe3
    bool          Button2TextOverridden;        // +0xe4
    bool GetTextFromTextTag(const CWideString& tag, CWideString& text);   // 0x0048F6BD
    void SetButton2OverrideText(const CCharString& text);
};

static const unsigned short s_Default[] = { 'T', 'E', 'X', 'T', '_', 'G', 'U', 'I', '_', 'S', 'K', 'I', 'P', '_', 'S', 'C', 'E', 'N', 'E', 0 };   // L"TEXT_GUI_SKIP_SCENE"

void CSubtitleRenderer::SetButton2OverrideText(const CCharString& text)
{
    if (CutsceneActionMode)
        return;
    if (text == "") {
        GetTextFromTextTag(CWideString(s_Default), Button2OverrideText);
        Button2TextOverridden = false;
    } else {
        if (!GetTextFromTextTag(text.ToWideString(), Button2OverrideText))
            Button2OverrideText = text.ToWideString();
        Button2TextOverridden = true;
    }
}

// Behaviour test for 0x0048F881 CSubtitleRenderer::SetButton2OverrideText: an empty text sets the button text from the L"TEXT_GUI_SKIP_SCENE" tag
// (retail 0x01238198) and clears the override flag; a non-empty text looks its own tag up (or is used as is) and sets the flag.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the callees are
// defined under the row's own spellings and record the wide text the row passes, so the test asserts the literal; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
// ---- row declarations (copied from the row source; its function definition left out) ----
// CSubtitleRenderer::SetButton2OverrideText(const CCharString& text) -- an empty text restores the default button-2 caption (the TEXT_GUI_SKIP_SCENE text tag) and clears the override (no cutscene-action-mode guard, unlike button 1), else the caption becomes the text-tag lookup of the text (or the text itself when it is no tag)
//   empty arm (strcmp("", "") on the unassigned case, the block compare otherwise): an inline operator==("") with two
//   returns materialises its bool with `sete al` and re-tests it; the default tag is an implicit CWideString conversion
//   (built before the other arguments and re-addressed with `lea`), not an explicit temporary.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048F881, 184 bytes, `ret 4`. Ego_r.pdb `?SetButton2OverrideText@CSubtitleRenderer@@QAEXABVCCharString@@@Z`.
// Callees: CBasicString<char>::operator==(const char*) @ 0x004115A0 (inside the inline CCharString::operator==, "" @ 0x0122D70E),
//   CCharString::ToWideString() const @ 0x0099E480, CSubtitleRenderer::GetTextFromTextTag(const CWideString&, CWideString&)
//   @ 0x0048F6BD (PDB private, bool), CWideString::operator=(const CWideString&) @ 0x0099B7D0, ~CWideString @ 0x0099B510,
//   CWideString::CWideString(const unsigned short*) @ 0x0099B6B0 (L"TEXT_GUI_SKIP_SCENE" @ 0x01238198).
// Shape: frame-pointer size TU (as the landed CSubtitleRenderer rows); `this` is spilled to [ebp-8] because the lookup's
//   negated result rides in bl across the wide temporary's destructor; the inline operator==("") (strcmp intrinsic on the
//   unassigned case, the block compare otherwise) puts the empty-text arm after the function's epilogue; the wide
//   temporaries live in the frame word and the dead `text` slot.
// Retail layout (the landed CSubtitleRenderer rows): Button2OverrideText +0x74, Button2TextOverridden +0xE4.
#include <string.h>
#pragma pack(push, 1)
template <class T> class CBasicString { public:                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public:                        // 17 bytes
    CBasicString<char> Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CWideString {
public:
    void* PStringData;
    CWideString(const unsigned short* text);    // 0x0099B6B0 (wchar_t is unsigned short under the era defaults)
    ~CWideString();                             // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CWideString ToWideString() const;           // 0x0099E480
    FABLE_FORCEINLINE bool operator==(const char* text) const
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
    private: bool GetTextFromTextTag(const CWideString& tag, CWideString& text); public:   // 0x0048F6BD
    void SetButton2OverrideText(const CCharString& text);
};


// ---- end of row declarations ----
static char g_tag[64]; static void* g_tagTarget = 0; static int g_lookups = 0, g_assigns = 0; static bool g_found = false;
static const unsigned short kOwnTag[] = { 'O', 'W', 'N', 0 };
static void Narrow(const unsigned short* w, char* out) { int i = 0; for (; w && w[i] && i < 63; ++i) out[i] = (char)w[i]; out[i] = 0; }
CWideString::CWideString(const unsigned short* text) { PStringData = (void*)text; }
CWideString::~CWideString() { }
const CWideString& CWideString::operator=(const CWideString& other) { PStringData = other.PStringData; ++g_assigns; return *this; }
template <> bool CBasicString<char>::operator==(const char* text) const { return strcmp(PData, text) == 0; }
CWideString CCharString::ToWideString() const { return CWideString(kOwnTag); }
bool CSubtitleRenderer::GetTextFromTextTag(const CWideString& tag, CWideString& text)
{ Narrow((const unsigned short*)tag.PStringData, g_tag); g_tagTarget = &text; ++g_lookups; return g_found; }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
static double g_obj[64];
int main()
{
    CSubtitleRenderer* r = (CSubtitleRenderer*)g_obj;
    CCharString empty; empty.PStringData = 0;
    r->Button2TextOverridden = true;
    r->SetButton2OverrideText(empty);
    bool ok = g_lookups == 1 && strcmp(g_tag, "TEXT_GUI_SKIP_SCENE") == 0 && g_tagTarget == &r->Button2OverrideText && !r->Button2TextOverridden && g_assigns == 0;
    static CCharStringData data; static char own[] = "OWN"; data.Data.PData = own;
    CCharString named; named.PStringData = &data;
    g_tag[0] = 0; r->SetButton2OverrideText(named);   // tag not found: the text itself is assigned
    ok = ok && g_lookups == 2 && strcmp(g_tag, "OWN") == 0 && r->Button2TextOverridden && g_assigns == 1 && r->Button2OverrideText.PStringData == (void*)kOwnTag;
    if (ok) printf("0048f881_TEST PASS\n");
    else printf("FAIL tag=%s lookups=%d assigns=%d\n", g_tag, g_lookups, g_assigns);
    return ok ? 0 : 1;
}

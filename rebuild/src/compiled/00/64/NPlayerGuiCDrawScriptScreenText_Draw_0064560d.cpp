// NPlayerGui::CDrawScriptScreenText::Draw(CEnginePrimitivePrepareInfo*) -- draw the script-driven screen text: when the player GUI shows script text, draw every free-positioned script text entry (its own font, or the GUI resources' default font when the font name is empty) and then the scrolling script lines from the GUI definition's origin, highlighted lines in grey (0x80,0x80,0x80,0xff) and the rest in the definition colour, advancing the pen by the line step times each entry's line count until the definition's maximum line count is reached
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064560D, length 532 bytes (`ret 4`); frame-pointer /Os UI draw TU.
// Callees: CGameUI::Draw2DText 0x00438010, CFontBankManager::GetFont 0x009E2C80 (@ 0x013B838C, counted-pointer result, dtor 0x004057A0),
//   CBasicString<char>::operator==(const char*) 0x004115A0, PeekGuiDef 0x00438570, GFACoordToVCoord 0x009E21D0, script line
//   IsHighlighted 0x004381B0 / GetText 0x004380A0 (by value), CWideString dtor 0x0099B510.
// Layout: PlayerGui (+4) is the CGameUI: ShowScriptText +0x2bc, ScriptTexts list +0x324, ScriptLines list +0x328 (direct STLport lists);
//   gui definition: ScriptTextPos +0x8e0, ScriptTextColour +0x8e8, ScriptTextFont +0x8f8, ScriptTextLineStep +0x8fc, max lines +0x904;
//   resources (+0x10) default font +0x1614c. Modelled locally (no shared header covers these draw objects).
// Shape: /Os via optimize("y",off)+("s",on); the inline CCharString== is __forceinline (else it is called out of line under /Os); the
//   second loop walks an iterator object with a user copy ctor (retail keeps the node in the frame) and reads a `SScriptLine&` local;
//   the pen advance builds a C2DVector delta first (retail's step.Y*n temp).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <string.h>
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    C2DVector& operator+=(const C2DVector& o) { X += o.X; Y += o.Y; return *this; }
};
class CWideString {
public:
    void* m_pStringData;
    ~CWideString();                                 // 0x0099B510
};
#pragma pack(push, 1)
template <class T> class CBasicString { public:
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;        // 0x004115A0
};
class CCharStringData { public:
    CBasicString<char> Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    FABLE_FORCEINLINE bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
enum EEngine2DLayer { ENGINE_2D_LAYER_TEXT = 0x43 };
class CFontBank;
// NF221 (family q12226 / q12402): the font pointer is the shared CCountedPointer<CFontBank> (engine/CCountedPointer.h,
//   RELEASEDATA_DECL_ONLY): its release 0x004057A0 is the out-of-line ?ReleaseData@?$CCountedPointer@VCFontBank@@@@QAEXXZ (FableWin.pdb; no
//   jmp thunk), called by the temporary's inline destructor; GetFont is CFontManager::GetFont (engine/CFontManager.h; FableWin.pdb / Ego_r.pdb
//   ?GetFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@ABVCCharString@@@Z, the self leaf of 0x009E2C80). Were the local
//   CFontPointer / CFontBankManager stand-ins (??1CFontPointer@@QAE@XZ, ?GetFont@CFontBankManager@@QAE?AUCFontPointer@@ABVCCharString@@@Z).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
#include "engine/CFontManager.h"
extern CFontManager* PFontManager;        // 0x013B838C

struct SScriptScreenText {                          // list element (node +8)
    CWideString Text;                               // +0x00
    C2DVector   Position;                           // +0x04
    CRGBColour  Colour;                             // +0x0c
    CCharString FontName;                           // +0x10
};
struct SScriptTextNode {
    SScriptTextNode* Next;
    SScriptTextNode* Prev;
    SScriptScreenText Data;
};
struct SScriptLine {                                // list element (node +8)
    unsigned char _pad00[0xc];
    long          LineCount;                        // +0x0c
    bool IsHighlighted() const;                     // 0x004381B0
    CWideString GetText() const;                    // 0x004380A0
};
struct SScriptLineNode {
    SScriptLineNode* Next;
    SScriptLineNode* Prev;
    SScriptLine      Data;
};
template <class N> struct CListIter {
    N* _M_node;
    CListIter(N* n) : _M_node(n) {}
    CListIter(const CListIter& o) : _M_node(o._M_node) {}
    CListIter& operator++() { _M_node = _M_node->Next; return *this; }
    bool operator!=(const CListIter& o) const { return _M_node != o._M_node; }
    bool operator==(const CListIter& o) const { return _M_node == o._M_node; }
};
template <class N> struct CList {
    N* _M_node;
    CListIter<N> ibegin() const { return CListIter<N>(_M_node->Next); }
    CListIter<N> iend() const { return CListIter<N>(_M_node); }
    N* begin() const { return _M_node->Next; }
    N* end() const { return _M_node; }
    bool empty() const { return _M_node->Next == _M_node; }
};

struct CGameUI {
    unsigned char _pad00[0x2bc];
    bool          ShowScriptText;                   // +0x2bc
    unsigned char _pad2bd[0x324 - 0x2bd];
    CList<SScriptTextNode>  ScriptTexts;            // +0x324
    CList<SScriptLineNode>  ScriptLines;            // +0x328
};
class CPlayerGuiDef { public:
    unsigned char _pad00[0x8e0];
    C2DVector     ScriptTextPos;                    // +0x8e0
    CRGBColour    ScriptTextColour;                 // +0x8e8
    unsigned char _pad8ec[0x8f8 - 0x8ec];
    CCharString   ScriptTextFont;                   // +0x8f8
    C2DVector     ScriptTextLineStep;               // +0x8fc
    long          ScriptTextMaxLines;               // +0x904
};
// Naming (NF442 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x00438010 ?Draw2DText@CPlayerGui@@QAEXABVC2DVector@@ABVCWideString@@ABVCFontBank@@ABVCRGBColour@@MW4EEngine2DLayer@@_N@Z (was ?Draw2DText@CGameUI@@QAEXABVC2DVector@@ABVCWideString@@PAVCFontBank@@ABVCRGBColour@@MW4EEngine2DLayer@@_N@Z)
class CPlayerGui { public: static const CPlayerGuiDef& __fastcall PeekGuiDef();  public: void Draw2DText(C2DVector const&, CWideString const&, CFontBank const&, CRGBColour const&, float, EEngine2DLayer, bool); };      // 0x00438570
struct CGuiResources {
    unsigned char _pad[0x1614c];
    CFontBank*    DefaultFont;                      // +0x1614c
};
class CEnginePrimitivePrepareInfo;

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x009E21D0 ?GFACoordToVCoord@@YIXABVC2DVector@@AAV1@@Z (was ?GFACoordToVCoord@@YIXPBVC2DVector@@PAV1@@Z)
void __fastcall GFACoordToVCoord(C2DVector const&, C2DVector&);
namespace NPlayerGui {
class CDrawScriptScreenText { public:
    void*          _vtable;
    CGameUI*       PlayerGui;                       // +0x04
    void*          PlayerManager;                   // +0x08
    void*          World;                           // +0x0c
    CGuiResources* Resources;                       // +0x10
    void*          Bank;                            // +0x14
    bool           Drawing;                         // +0x18
    void Draw(CEnginePrimitivePrepareInfo* info);
};

void CDrawScriptScreenText::Draw(CEnginePrimitivePrepareInfo* info)
{
    CGameUI* gui = PlayerGui;
    if (!gui->ShowScriptText)
        return;
    if (!gui->ScriptTexts.empty()) {
        Drawing = true;
        for (SScriptTextNode* node = gui->ScriptTexts.begin(); node != gui->ScriptTexts.end(); node = node->Next) {
            SScriptScreenText& text = node->Data;
            CFontBank* font = text.FontName == "" ? Resources->DefaultFont : PFontManager->GetFont(text.FontName).Data;
            reinterpret_cast<CPlayerGui*>(PlayerGui)->Draw2DText(text.Position, text.Text, *(font), text.Colour, 1.0f, ENGINE_2D_LAYER_TEXT, true);
        }
    }
    CList<SScriptLineNode>& lines = PlayerGui->ScriptLines;
    if (!lines.empty()) {
        const CPlayerGuiDef* def = &CPlayerGui::PeekGuiDef();
        CFontBank* font = PFontManager->GetFont(def->ScriptTextFont).Data;
        C2DVector pos;
        GFACoordToVCoord(*(&def->ScriptTextPos), pos);
        C2DVector step;
        GFACoordToVCoord(*(&def->ScriptTextLineStep), step);
        long count = 0;
        for (CListIter<SScriptLineNode> it = lines.ibegin(); it != lines.iend(); ++it) {
            SScriptLine& line = it._M_node->Data;
            if (line.IsHighlighted())
                reinterpret_cast<CPlayerGui*>(PlayerGui)->Draw2DText(pos, line.GetText(), *(font), CRGBColour(0x80, 0x80, 0x80, 0xff), 1.0f, ENGINE_2D_LAYER_TEXT, true);
            else
                reinterpret_cast<CPlayerGui*>(PlayerGui)->Draw2DText(pos, line.GetText(), *(font), def->ScriptTextColour, 1.0f, ENGINE_2D_LAYER_TEXT, true);
            float n = (float)line.LineCount;
            C2DVector delta(step.X * n, step.Y * n);
            pos.X += delta.X;
            pos.Y += delta.Y;
            count += line.LineCount;
            if (count >= def->ScriptTextMaxLines)
                break;
        }
    }
}
}
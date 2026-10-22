// Behaviour test for NUISystem::CEditBox::UpdateDisplayText (0x00540980): model and body verbatim (plus wide-literal overloads
// for g++), every callee stubbed; covers the missing text component, the password mask, the cursor split with the IME caret and
// composition position, the adjustment clamps, the right-to-left width fit (LeftMostCharacter / offset) and the final
// SetPosition / SetValue arguments.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
// Leaf: UpdateDisplayText
// Grade: RELOCATION_MATCH (AdvB4, 2026-10-22, from chunk 284's ud19): the home-X offset sum is `C2DVector((double)home.X + offset, home.Y)` --
//   a double sum narrowed by the float ctor parameter; retail loads home.X first (`fld [eax+8] ; fadd [esp+10h]`) and stores the X
//   after the temporary's lea. A float sum in either operand order loads the frame float first; an explicit (float) cast fixes the
//   order but stores X before the lea.
struct CWideString {
    void* m_pStringData;
    CWideString();                                          // 0x0099AED0
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
    unsigned long GetLength() const;                        // 0x0099B220
    CWideString& operator=(const unsigned short* text);     // 0x0099B800
    CWideString& operator=(const CWideString& other);       // 0x0099B7D0
    CWideString& operator+=(const unsigned short* text);    // 0x0099B940
    CWideString& operator+=(const CWideString& other);      // 0x0099B8D0
    operator const unsigned short*() const;                 // 0x0099AF10
    unsigned short operator[](long i) const { return ((const unsigned short*)*this)[i]; }
    CWideString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099BDB0
#ifndef _MSC_VER
    CWideString& operator=(const wchar_t* text);
    CWideString& operator+=(const wchar_t* text);
#endif
};
unsigned long __fastcall GetCharWidthW(unsigned short c);   // 0x0054F4B0
struct CIME {
    bool IsOn();                                            // 0x00404BA0
    void SetCompositionPosition(long x, long y);            // 0x00404B80
};
CIME* GFGetIME();                                           // 0x00404A80
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator*(const C2DVector& o) const { return C2DVector(X * o.X, Y * o.Y); }
    C2DVector Offset(float dx) const { return C2DVector(X + dx, Y); }
};
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct SComponentState {
    unsigned char _pad00[8];
    C2DVector     Position;                                 // +0x08
    C2DVector     Size;                                     // +0x10
    float         Alpha;                                    // +0x18
};
struct CStateMap {
    void* m_pHeader; unsigned long m_nodeCount; unsigned char m_compare;
    SComponentState& operator[](const unsigned long& key);  // 0x0042D5B1
};
class CFont {
public:
    virtual void v000();
    virtual const C2DVector& GetStringSizeOut(C2DVector& out, const unsigned short* text, long flags);   // slot 1 (+4)
};
struct CFontPointer {
    CFont* m_pObject;
    void* m_pCount;
    CFont* operator->() const { return m_pObject; }
};
namespace NUISystem {
class CText {
public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void SetPosition(const C2DVector& position);         // slot 15 (+0x3c)
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual CStateMap& GetStateMap();                            // slot 82 (+0x148)
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual C2DVector GetScreenPosition();                     // slot 122 (+0x1e8)
    virtual C2DVector GetScale();                              // slot 123 (+0x1ec)
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();
    virtual void v132();
    virtual void v133();
    virtual void v134();
    virtual void v135();
    virtual void v136();
    virtual void v137();
    virtual void v138();
    virtual void v139();
    virtual void v140();
    virtual void v141();
    virtual void v142();
    virtual void v143();
    virtual void SetValue(const CWideString& value, bool a, bool b);   // slot 144 (+0x240)
    virtual void v145();
    virtual CFontPointer& GetFont();                          // slot 146 (+0x248)
    virtual void v147();
    virtual const C2DBoxF* GetRect();                         // slot 148 (+0x250)
};
class CEditBox {
public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();
    virtual void v132();
    virtual void v133();
    virtual void v134();
    virtual void v135();
    virtual void v136();
    virtual void v137();
    virtual void v138();
    virtual void v139();
    virtual void v140();
    virtual void v141();
    virtual void v142();
    virtual void v143();
    virtual CText* GetTextComponent();                          // slot 144 (+0x240)
    virtual void v145();
    virtual void v146();
    virtual void UpdateDisplayText();                           // slot 147 (+0x24c)
    unsigned char _pad004[0x164 - 4];
    CWideString   Text;                                     // +0x164
    long          CursorPos;                                // +0x168
    long          LeftMostCharacter;                        // +0x16c
    long          RightMostCharacter;                       // +0x170
    long          Adjustment;                               // +0x174
    bool          Active;                                   // +0x178
    bool          IsPasswordBox;                            // +0x179
    bool          UsesIME;                                  // +0x17a
};
void CEditBox::UpdateDisplayText()
{
    CText* text = GetTextComponent();
    if (text == 0)
        return;
    CWideString display(Text);
    if (IsPasswordBox) {
        long length = Text.GetLength();
        display = L"";
        for (long i = 0; i < length; ++i)
            display += L"*";
    }
    CWideString beforeCursor;
    CWideString afterCursor;
    if (CursorPos > 0)
        beforeCursor = display.GetSubString(0, CursorPos);
    if (CursorPos < (long)Text.GetLength())
        afterCursor = display.GetSubString(CursorPos, display.GetLength());
    CWideString line(beforeCursor);
    if (Active && GFGetIME()->IsOn()) {
        if (UsesIME) {
            C2DVector position = GetTextComponent()->GetScreenPosition();
            C2DVector size;
            position.X += text->GetFont()->GetStringSizeOut(size, line, 0).X * text->GetScale().X;
            GFGetIME()->SetCompositionPosition((long)(position.X + 3.0f), (long)(position.Y + 3.0f));
        }
        line += L"|";
    }
    line += afterCursor;
    if (Adjustment > 0)
        Adjustment = 0;
    if (Adjustment < -(long)line.GetLength())
        Adjustment = -(long)line.GetLength();
    RightMostCharacter = line.GetLength() + Adjustment;
    float maxWidth = text->GetRect()->BRX;
    float width = 0.0f;
    float previousWidth = 0.0f;
    LeftMostCharacter = RightMostCharacter;
    while (LeftMostCharacter > 0) {
        unsigned short c = line[LeftMostCharacter - 1];
        previousWidth = width;
        width += (float)GetCharWidthW(c);
        if (width >= maxWidth)
            break;
        --LeftMostCharacter;
    }
    float offset = 0.0f;
    if (LeftMostCharacter != 0)
        offset = maxWidth - previousWidth;
    const C2DVector& home = text->GetStateMap()[0].Position;
    text->SetPosition(C2DVector((double)home.X + offset, home.Y));
    line = line.GetSubString(LeftMostCharacter, RightMostCharacter);
    text->SetValue(line, true, true);
}
}

// ---- stubs and harness ----
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
void NUISystem::CText::v000() {}
void NUISystem::CText::v001() {}
void NUISystem::CText::v002() {}
void NUISystem::CText::v003() {}
void NUISystem::CText::v004() {}
void NUISystem::CText::v005() {}
void NUISystem::CText::v006() {}
void NUISystem::CText::v007() {}
void NUISystem::CText::v008() {}
void NUISystem::CText::v009() {}
void NUISystem::CText::v010() {}
void NUISystem::CText::v011() {}
void NUISystem::CText::v012() {}
void NUISystem::CText::v013() {}
void NUISystem::CText::v014() {}
void NUISystem::CText::v016() {}
void NUISystem::CText::v017() {}
void NUISystem::CText::v018() {}
void NUISystem::CText::v019() {}
void NUISystem::CText::v020() {}
void NUISystem::CText::v021() {}
void NUISystem::CText::v022() {}
void NUISystem::CText::v023() {}
void NUISystem::CText::v024() {}
void NUISystem::CText::v025() {}
void NUISystem::CText::v026() {}
void NUISystem::CText::v027() {}
void NUISystem::CText::v028() {}
void NUISystem::CText::v029() {}
void NUISystem::CText::v030() {}
void NUISystem::CText::v031() {}
void NUISystem::CText::v032() {}
void NUISystem::CText::v033() {}
void NUISystem::CText::v034() {}
void NUISystem::CText::v035() {}
void NUISystem::CText::v036() {}
void NUISystem::CText::v037() {}
void NUISystem::CText::v038() {}
void NUISystem::CText::v039() {}
void NUISystem::CText::v040() {}
void NUISystem::CText::v041() {}
void NUISystem::CText::v042() {}
void NUISystem::CText::v043() {}
void NUISystem::CText::v044() {}
void NUISystem::CText::v045() {}
void NUISystem::CText::v046() {}
void NUISystem::CText::v047() {}
void NUISystem::CText::v048() {}
void NUISystem::CText::v049() {}
void NUISystem::CText::v050() {}
void NUISystem::CText::v051() {}
void NUISystem::CText::v052() {}
void NUISystem::CText::v053() {}
void NUISystem::CText::v054() {}
void NUISystem::CText::v055() {}
void NUISystem::CText::v056() {}
void NUISystem::CText::v057() {}
void NUISystem::CText::v058() {}
void NUISystem::CText::v059() {}
void NUISystem::CText::v060() {}
void NUISystem::CText::v061() {}
void NUISystem::CText::v062() {}
void NUISystem::CText::v063() {}
void NUISystem::CText::v064() {}
void NUISystem::CText::v065() {}
void NUISystem::CText::v066() {}
void NUISystem::CText::v067() {}
void NUISystem::CText::v068() {}
void NUISystem::CText::v069() {}
void NUISystem::CText::v070() {}
void NUISystem::CText::v071() {}
void NUISystem::CText::v072() {}
void NUISystem::CText::v073() {}
void NUISystem::CText::v074() {}
void NUISystem::CText::v075() {}
void NUISystem::CText::v076() {}
void NUISystem::CText::v077() {}
void NUISystem::CText::v078() {}
void NUISystem::CText::v079() {}
void NUISystem::CText::v080() {}
void NUISystem::CText::v081() {}
void NUISystem::CText::v083() {}
void NUISystem::CText::v084() {}
void NUISystem::CText::v085() {}
void NUISystem::CText::v086() {}
void NUISystem::CText::v087() {}
void NUISystem::CText::v088() {}
void NUISystem::CText::v089() {}
void NUISystem::CText::v090() {}
void NUISystem::CText::v091() {}
void NUISystem::CText::v092() {}
void NUISystem::CText::v093() {}
void NUISystem::CText::v094() {}
void NUISystem::CText::v095() {}
void NUISystem::CText::v096() {}
void NUISystem::CText::v097() {}
void NUISystem::CText::v098() {}
void NUISystem::CText::v099() {}
void NUISystem::CText::v100() {}
void NUISystem::CText::v101() {}
void NUISystem::CText::v102() {}
void NUISystem::CText::v103() {}
void NUISystem::CText::v104() {}
void NUISystem::CText::v105() {}
void NUISystem::CText::v106() {}
void NUISystem::CText::v107() {}
void NUISystem::CText::v108() {}
void NUISystem::CText::v109() {}
void NUISystem::CText::v110() {}
void NUISystem::CText::v111() {}
void NUISystem::CText::v112() {}
void NUISystem::CText::v113() {}
void NUISystem::CText::v114() {}
void NUISystem::CText::v115() {}
void NUISystem::CText::v116() {}
void NUISystem::CText::v117() {}
void NUISystem::CText::v118() {}
void NUISystem::CText::v119() {}
void NUISystem::CText::v120() {}
void NUISystem::CText::v121() {}
void NUISystem::CText::v124() {}
void NUISystem::CText::v125() {}
void NUISystem::CText::v126() {}
void NUISystem::CText::v127() {}
void NUISystem::CText::v128() {}
void NUISystem::CText::v129() {}
void NUISystem::CText::v130() {}
void NUISystem::CText::v131() {}
void NUISystem::CText::v132() {}
void NUISystem::CText::v133() {}
void NUISystem::CText::v134() {}
void NUISystem::CText::v135() {}
void NUISystem::CText::v136() {}
void NUISystem::CText::v137() {}
void NUISystem::CText::v138() {}
void NUISystem::CText::v139() {}
void NUISystem::CText::v140() {}
void NUISystem::CText::v141() {}
void NUISystem::CText::v142() {}
void NUISystem::CText::v143() {}
void NUISystem::CText::v145() {}
void NUISystem::CText::v147() {}
void NUISystem::CEditBox::v000() {}
void NUISystem::CEditBox::v001() {}
void NUISystem::CEditBox::v002() {}
void NUISystem::CEditBox::v003() {}
void NUISystem::CEditBox::v004() {}
void NUISystem::CEditBox::v005() {}
void NUISystem::CEditBox::v006() {}
void NUISystem::CEditBox::v007() {}
void NUISystem::CEditBox::v008() {}
void NUISystem::CEditBox::v009() {}
void NUISystem::CEditBox::v010() {}
void NUISystem::CEditBox::v011() {}
void NUISystem::CEditBox::v012() {}
void NUISystem::CEditBox::v013() {}
void NUISystem::CEditBox::v014() {}
void NUISystem::CEditBox::v015() {}
void NUISystem::CEditBox::v016() {}
void NUISystem::CEditBox::v017() {}
void NUISystem::CEditBox::v018() {}
void NUISystem::CEditBox::v019() {}
void NUISystem::CEditBox::v020() {}
void NUISystem::CEditBox::v021() {}
void NUISystem::CEditBox::v022() {}
void NUISystem::CEditBox::v023() {}
void NUISystem::CEditBox::v024() {}
void NUISystem::CEditBox::v025() {}
void NUISystem::CEditBox::v026() {}
void NUISystem::CEditBox::v027() {}
void NUISystem::CEditBox::v028() {}
void NUISystem::CEditBox::v029() {}
void NUISystem::CEditBox::v030() {}
void NUISystem::CEditBox::v031() {}
void NUISystem::CEditBox::v032() {}
void NUISystem::CEditBox::v033() {}
void NUISystem::CEditBox::v034() {}
void NUISystem::CEditBox::v035() {}
void NUISystem::CEditBox::v036() {}
void NUISystem::CEditBox::v037() {}
void NUISystem::CEditBox::v038() {}
void NUISystem::CEditBox::v039() {}
void NUISystem::CEditBox::v040() {}
void NUISystem::CEditBox::v041() {}
void NUISystem::CEditBox::v042() {}
void NUISystem::CEditBox::v043() {}
void NUISystem::CEditBox::v044() {}
void NUISystem::CEditBox::v045() {}
void NUISystem::CEditBox::v046() {}
void NUISystem::CEditBox::v047() {}
void NUISystem::CEditBox::v048() {}
void NUISystem::CEditBox::v049() {}
void NUISystem::CEditBox::v050() {}
void NUISystem::CEditBox::v051() {}
void NUISystem::CEditBox::v052() {}
void NUISystem::CEditBox::v053() {}
void NUISystem::CEditBox::v054() {}
void NUISystem::CEditBox::v055() {}
void NUISystem::CEditBox::v056() {}
void NUISystem::CEditBox::v057() {}
void NUISystem::CEditBox::v058() {}
void NUISystem::CEditBox::v059() {}
void NUISystem::CEditBox::v060() {}
void NUISystem::CEditBox::v061() {}
void NUISystem::CEditBox::v062() {}
void NUISystem::CEditBox::v063() {}
void NUISystem::CEditBox::v064() {}
void NUISystem::CEditBox::v065() {}
void NUISystem::CEditBox::v066() {}
void NUISystem::CEditBox::v067() {}
void NUISystem::CEditBox::v068() {}
void NUISystem::CEditBox::v069() {}
void NUISystem::CEditBox::v070() {}
void NUISystem::CEditBox::v071() {}
void NUISystem::CEditBox::v072() {}
void NUISystem::CEditBox::v073() {}
void NUISystem::CEditBox::v074() {}
void NUISystem::CEditBox::v075() {}
void NUISystem::CEditBox::v076() {}
void NUISystem::CEditBox::v077() {}
void NUISystem::CEditBox::v078() {}
void NUISystem::CEditBox::v079() {}
void NUISystem::CEditBox::v080() {}
void NUISystem::CEditBox::v081() {}
void NUISystem::CEditBox::v082() {}
void NUISystem::CEditBox::v083() {}
void NUISystem::CEditBox::v084() {}
void NUISystem::CEditBox::v085() {}
void NUISystem::CEditBox::v086() {}
void NUISystem::CEditBox::v087() {}
void NUISystem::CEditBox::v088() {}
void NUISystem::CEditBox::v089() {}
void NUISystem::CEditBox::v090() {}
void NUISystem::CEditBox::v091() {}
void NUISystem::CEditBox::v092() {}
void NUISystem::CEditBox::v093() {}
void NUISystem::CEditBox::v094() {}
void NUISystem::CEditBox::v095() {}
void NUISystem::CEditBox::v096() {}
void NUISystem::CEditBox::v097() {}
void NUISystem::CEditBox::v098() {}
void NUISystem::CEditBox::v099() {}
void NUISystem::CEditBox::v100() {}
void NUISystem::CEditBox::v101() {}
void NUISystem::CEditBox::v102() {}
void NUISystem::CEditBox::v103() {}
void NUISystem::CEditBox::v104() {}
void NUISystem::CEditBox::v105() {}
void NUISystem::CEditBox::v106() {}
void NUISystem::CEditBox::v107() {}
void NUISystem::CEditBox::v108() {}
void NUISystem::CEditBox::v109() {}
void NUISystem::CEditBox::v110() {}
void NUISystem::CEditBox::v111() {}
void NUISystem::CEditBox::v112() {}
void NUISystem::CEditBox::v113() {}
void NUISystem::CEditBox::v114() {}
void NUISystem::CEditBox::v115() {}
void NUISystem::CEditBox::v116() {}
void NUISystem::CEditBox::v117() {}
void NUISystem::CEditBox::v118() {}
void NUISystem::CEditBox::v119() {}
void NUISystem::CEditBox::v120() {}
void NUISystem::CEditBox::v121() {}
void NUISystem::CEditBox::v122() {}
void NUISystem::CEditBox::v123() {}
void NUISystem::CEditBox::v124() {}
void NUISystem::CEditBox::v125() {}
void NUISystem::CEditBox::v126() {}
void NUISystem::CEditBox::v127() {}
void NUISystem::CEditBox::v128() {}
void NUISystem::CEditBox::v129() {}
void NUISystem::CEditBox::v130() {}
void NUISystem::CEditBox::v131() {}
void NUISystem::CEditBox::v132() {}
void NUISystem::CEditBox::v133() {}
void NUISystem::CEditBox::v134() {}
void NUISystem::CEditBox::v135() {}
void NUISystem::CEditBox::v136() {}
void NUISystem::CEditBox::v137() {}
void NUISystem::CEditBox::v138() {}
void NUISystem::CEditBox::v139() {}
void NUISystem::CEditBox::v140() {}
void NUISystem::CEditBox::v141() {}
void NUISystem::CEditBox::v142() {}
void NUISystem::CEditBox::v143() {}
void NUISystem::CEditBox::v145() {}
void NUISystem::CEditBox::v146() {}
void CFont::v000() {}
const C2DVector& CFont::GetStringSizeOut(C2DVector& out, const unsigned short*, long) { return out; }
void NUISystem::CText::SetPosition(const C2DVector&) {}
CStateMap& NUISystem::CText::GetStateMap() { static CStateMap m; return m; }
C2DVector NUISystem::CText::GetScreenPosition() { return C2DVector(); }
C2DVector NUISystem::CText::GetScale() { return C2DVector(); }
void NUISystem::CText::SetValue(const CWideString&, bool, bool) {}
CFontPointer& NUISystem::CText::GetFont() { static CFontPointer p = { 0, 0 }; return p; }
const C2DBoxF* NUISystem::CText::GetRect() { return 0; }
NUISystem::CText* NUISystem::CEditBox::GetTextComponent() { return 0; }

static unsigned long wlen(const unsigned short* s) { unsigned long n = 0; while (s[n]) ++n; return n; }
static unsigned short* wdup(const unsigned short* s, unsigned long n) { unsigned short* d = (unsigned short*)malloc((n + 1) * 2); memcpy(d, s, n * 2); d[n] = 0; return d; }
static const unsigned short g_empty[1] = { 0 };
CWideString::CWideString() { m_pStringData = wdup(g_empty, 0); }
CWideString::CWideString(const CWideString& o) { const unsigned short* s = (const unsigned short*)o.m_pStringData; m_pStringData = wdup(s, wlen(s)); }
CWideString::~CWideString() { free(m_pStringData); m_pStringData = 0; }
unsigned long CWideString::GetLength() const { return wlen((const unsigned short*)m_pStringData); }
CWideString& CWideString::operator=(const unsigned short* t) { free(m_pStringData); m_pStringData = wdup(t, wlen(t)); return *this; }
CWideString& CWideString::operator=(const CWideString& o) { if (this != &o) { const unsigned short* s = (const unsigned short*)o.m_pStringData; void* d = wdup(s, wlen(s)); free(m_pStringData); m_pStringData = d; } return *this; }
CWideString& CWideString::operator+=(const unsigned short* t) { const unsigned short* s = (const unsigned short*)m_pStringData; unsigned long a = wlen(s), b = wlen(t); unsigned short* d = (unsigned short*)malloc((a + b + 1) * 2); memcpy(d, s, a * 2); memcpy(d + a, t, b * 2); d[a + b] = 0; free(m_pStringData); m_pStringData = d; return *this; }
CWideString& CWideString::operator+=(const CWideString& o) { return *this += (const unsigned short*)o.m_pStringData; }
CWideString::operator const unsigned short*() const { return (const unsigned short*)m_pStringData; }
CWideString CWideString::GetSubString(unsigned long start, unsigned long end) const { CWideString r; const unsigned short* s = (const unsigned short*)m_pStringData; unsigned long n = wlen(s); if (end > n) end = n; if (start > end) start = end; free(r.m_pStringData); r.m_pStringData = wdup(s + start, end - start); return r; }
#ifndef _MSC_VER
static unsigned short* narrow(const wchar_t* w) { static unsigned short buf[64]; int i = 0; while (w[i] && i < 63) { buf[i] = (unsigned short)w[i]; ++i; } buf[i] = 0; return buf; }
CWideString& CWideString::operator=(const wchar_t* t) { return *this = (const unsigned short*)narrow(t); }
CWideString& CWideString::operator+=(const wchar_t* t) { return *this += (const unsigned short*)narrow(t); }
#endif
static CWideString fromAscii(const char* a) { unsigned short buf[64]; int i = 0; for (; a[i] && i < 63; ++i) buf[i] = (unsigned short)a[i]; buf[i] = 0; CWideString r; r = buf; return r; }
static bool equalsAscii(const CWideString& w, const char* a) { const unsigned short* s = w; unsigned long i = 0; for (; a[i]; ++i) if (s[i] != (unsigned short)a[i]) return false; return s[i] == 0; }
static unsigned long g_charWidth = 10;
unsigned long __fastcall GetCharWidthW(unsigned short c) { return c == (unsigned short)'w' ? 20 : g_charWidth; }
static bool g_imeOn = false; static long g_compX = -1, g_compY = -1; static CIME g_ime;
bool CIME::IsOn() { return g_imeOn; }
void CIME::SetCompositionPosition(long x, long y) { g_compX = x; g_compY = y; }
CIME* GFGetIME() { return &g_ime; }
static SComponentState g_state; static unsigned long g_lastKey = 99;
SComponentState& CStateMap::operator[](const unsigned long& key) { g_lastKey = key; return g_state; }
struct TestFont : CFont {
    C2DVector Out; float Width;
    const C2DVector& GetStringSizeOut(C2DVector& out, const unsigned short* text, long flags) { out.X = Width * wlen(text); out.Y = 7.0f; Out = out; return Out; }
};
static TestFont g_font; static CFontPointer g_fontPointer = { &g_font, 0 };
struct TestText : NUISystem::CText {
    CStateMap Map; C2DBoxF Rect; C2DVector LastPosition; CWideString LastValue; bool LastA, LastB; int SetValueCalls, SetPositionCalls;
    void SetPosition(const C2DVector& position) { LastPosition = position; ++SetPositionCalls; }
    CStateMap& GetStateMap() { return Map; }
    C2DVector GetScreenPosition() { return C2DVector(100.0f, 50.0f); }
    C2DVector GetScale() { return C2DVector(2.0f, 1.0f); }
    void SetValue(const CWideString& value, bool a, bool b) { LastValue = value; LastA = a; LastB = b; ++SetValueCalls; }
    CFontPointer& GetFont() { return g_fontPointer; }
    const C2DBoxF* GetRect() { return &Rect; }
};
struct TestBox : NUISystem::CEditBox {
    NUISystem::CText* Component;
    NUISystem::CText* GetTextComponent() { return Component; }
};
int main()
{
    TestText text; text.SetValueCalls = text.SetPositionCalls = 0; text.Rect.BRX = 25.0f; g_state.Position = C2DVector(3.0f, 4.0f); g_font.Width = 5.0f;
    TestBox box; box.Component = 0; box.CursorPos = 2; box.Adjustment = 0; box.Active = false; box.IsPasswordBox = false; box.UsesIME = false;
    box.Text = fromAscii("hello");
    // no text component: nothing happens
    box.CEditBox::UpdateDisplayText(); CHECK(text.SetValueCalls == 0);
    // plain text, width fit from the right: "lo" fits (10, 20 < 25; 30 >= 25 stops at index 3), offset = 25 - 20
    box.Component = &text; box.LeftMostCharacter = -1; box.RightMostCharacter = -1;
    box.CEditBox::UpdateDisplayText();
    CHECK(text.SetValueCalls == 1 && text.SetPositionCalls == 1 && equalsAscii(text.LastValue, "lo") && text.LastA && text.LastB);
    CHECK(box.RightMostCharacter == 5 && box.LeftMostCharacter == 3 && box.Adjustment == 0 && g_lastKey == 0);
    CHECK(text.LastPosition.X == 8.0f && text.LastPosition.Y == 4.0f);
    // everything fits: LeftMost 0, no offset
    text.Rect.BRX = 1000.0f; box.CEditBox::UpdateDisplayText();
    CHECK(box.LeftMostCharacter == 0 && equalsAscii(text.LastValue, "hello") && text.LastPosition.X == 3.0f);
    // password mask
    box.IsPasswordBox = true; box.CEditBox::UpdateDisplayText(); CHECK(equalsAscii(text.LastValue, "*****")); box.IsPasswordBox = false;
    // adjustment clamps: positive -> 0, below -length -> -length
    box.Adjustment = 3; box.CEditBox::UpdateDisplayText(); CHECK(box.Adjustment == 0 && box.RightMostCharacter == 5);
    box.Adjustment = -9; box.CEditBox::UpdateDisplayText(); CHECK(box.Adjustment == -5 && box.RightMostCharacter == 0 && box.LeftMostCharacter == 0 && equalsAscii(text.LastValue, ""));
    box.Adjustment = -1; box.CEditBox::UpdateDisplayText(); CHECK(box.RightMostCharacter == 4 && equalsAscii(text.LastValue, "hell")); box.Adjustment = 0;
    // active with the IME on: caret inserted after the cursor, composition position from the screen position + string size * scale
    box.Active = true; g_imeOn = true; box.UsesIME = true; box.CEditBox::UpdateDisplayText();
    CHECK(equalsAscii(text.LastValue, "he|llo") && box.RightMostCharacter == 6);
    CHECK(g_compX == (long)(100.0f + 5.0f * 2 * 2.0f + 3.0f) && g_compY == 53);
    // IME on but not used by this box: caret only
    box.UsesIME = false; g_compX = -1; box.CEditBox::UpdateDisplayText(); CHECK(equalsAscii(text.LastValue, "he|llo") && g_compX == -1);
    g_imeOn = false; box.Active = false;
    // cursor at the end: no after-cursor part; cursor at 0: no before part
    box.CursorPos = 5; box.CEditBox::UpdateDisplayText(); CHECK(equalsAscii(text.LastValue, "hello"));
    box.CursorPos = 0; box.CEditBox::UpdateDisplayText(); CHECK(equalsAscii(text.LastValue, "hello"));
    printf("00540980_TEST %s\n", g_fail ? "FAIL" : "PASS");
    return g_fail;
}
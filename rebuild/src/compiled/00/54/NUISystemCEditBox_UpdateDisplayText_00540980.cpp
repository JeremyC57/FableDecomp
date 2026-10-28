// NUISystem::CEditBox::UpdateDisplayText() -- rebuild the text shown by the edit box: asterisks for a password box, the text before the cursor, a "|" caret while active with the IME on (positioning the IME composition window when the box uses it), the text after the cursor; clamp the scroll adjustment, pick the visible window of characters that fit the text component width from the right edge, offset the text component from its state-0 position and set the visible substring
// Leaf: UpdateDisplayText
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00540980, length 873 bytes (`ret`). Callees: CWideString 0x0099AED0/0x0099B720/0x0099B510/0x0099B220/0x0099B800/0x0099B7D0/0x0099B940/0x0099B8D0/0x0099AF10/0x0099BDB0, GFGetIME 0x00404A80, CIME 0x00404BA0 (IME on) / 0x00404B80 (composition position), __ftol2, GetCharWidthW 0x0054F4B0, map::operator[] 0x0042D5B1. Strings L"" 0x0122D70C, L"*" 0x0122DE6C, L"|" 0x0122DE58; 3.0f 0x0122DED4.
// Layout: as AddCharacter 0x0053FE50 (CEditBox members 0x40 below Ego_r.pdb); text component slots from the landed CText::ForceWidth 0x0054FB10 and CComponentContainer::Initialise 0x005461D0.
// Grade note: the home-X offset sum is `C2DVector((double)home.X + offset, home.Y)` -- a double sum narrowed by the float ctor parameter makes VC7.1 load home.X first and keep the X store after the temporary lea (a float sum loads the frame float first in either order; an explicit (float) cast stores X early). Found by advisor B4.
class CWideString { public:
    void* m_pStringData;
    CWideString();                                          // 0x0099AED0
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
    CWideString& operator=(const unsigned short* text);     // 0x0099B800
    const CWideString& operator=(const CWideString& other);       // 0x0099B7D0
    CWideString& operator+=(const unsigned short* text);    // 0x0099B940
    CWideString& operator+=(const CWideString& other);      // 0x0099B8D0
    operator const unsigned short*() const;                 // 0x0099AF10
    unsigned short operator[](long i) const { return ((const unsigned short*)*this)[i]; }
    CWideString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099BDB0
public:
    long GetLength() const;   // 0x0099B220
};
unsigned long __fastcall GetCharWidthW(unsigned short c);   // 0x0054F4B0
struct CIME {
    bool IsOn();                                            // 0x00404BA0
    void SetCompositionPosition(long x, long y);            // 0x00404B80
};
CIME* GFGetIME();                                           // 0x00404A80
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator*(const C2DVector& o) const { return C2DVector(X * o.X, Y * o.Y); }
    C2DVector Offset(float dx) const { return C2DVector(X + dx, Y); }
};
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
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
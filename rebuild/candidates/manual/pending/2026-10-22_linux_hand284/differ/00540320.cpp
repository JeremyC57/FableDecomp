// CEditBox::KeyPressed(wchar_t key) -- edit-box navigation keys: left / right move the cursor (scrolling the view by 6 at the visible edge), and unless the IME is composing: escape and enter run their action lists through the front-end manager, delete removes the character after the cursor (or clears the text after a pending delete-all), home moves to the start and recomputes the scroll from the glyph widths, end moves to the end; the pending delete-all is dropped and the display text refreshed
// Grade: DIFFER(755v755) -- 14 lines, all register/encoding: the three vtable loads take eax where retail takes edx (and back), and the home-key scroll `i - len - 5` is `sub edi,5` where retail has `add edi,-5` (unsigned/signed, `+ -5`, `-= 5`, `- 5u`, parenthesised forms all give sub); control flow exact (shared refresh tail through a label)
// Leaf: KeyPressed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00540320, length 755 bytes (`ret 4`, virtual
// ?KeyPressed@CEditBox@NUISystem@@UAEX_W@Z).
// Callees: as AddCharacter 0x0053FE50 (CWideString), plus operator const wchar_t* 0x0099AF00, GFGetIME 0x00404A80 /
// CIME::IsIMEActive 0x00404C50, CFrontEndManager::GetInstance 0x0041E5F2 (its slot 14, +0x38, takes each action),
// GetCharWidthW 0x0054F4B0, own slots 144 (+0x240, the parent element) / 147 (+0x24c, UpdateDisplayText) and the
// parent's slot 148 (+0x250, its rectangle; +8 the right edge).
// Layout: as AddCharacter 0x0053FE50; ActionsOnEnter / ActionsOnEscape are 4-byte STLport lists at +0x15c / +0x160
// (the PDB's are 12-byte MS lists).
unsigned long __fastcall GetCharWidthW(unsigned short c);   // 0x0054F4B0
struct CIME {
    bool IsIMEActive();                                     // 0x00404C50
};
CIME* GFGetIME();                                           // 0x00404A80
struct CAction;
template <class T> struct CCountedPointer { T* m_pObject; void* m_pCount; };
struct CActionNode {
    CActionNode*              _M_next;
    CActionNode*              _M_prev;
    CCountedPointer<CAction> _M_data;
};
struct CActionList { CActionNode* _M_node; };
struct CFrontEndManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual void PerformAction(const CCountedPointer<CAction>& action);   // slot 14 (+0x38)
    static CFrontEndManager* __fastcall GetInstance();      // 0x0041E5F2
};
struct CWideString {
    void* m_pStringData;
    CWideString();                                          // 0x0099AED0
    CWideString(const unsigned short* text);                       // 0x0099B6B0
    ~CWideString();                                         // 0x0099B510
    CWideString& operator+=(unsigned short c);              // 0x0099B9A0
    bool Contains(const CWideString& text) const;           // 0x0099AF20
    unsigned long GetLength() const;                        // 0x0099B220
    CWideString& operator=(const unsigned short* text);            // 0x0099B800
    CWideString& operator=(const CWideString& other);       // 0x0099B7D0
    operator const unsigned short*() const;                 // 0x0099AF00
    CWideString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099BDB0
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& left, unsigned short c);           // 0x0099C120

struct C2DBoxF { float TLX, TLY, BRX, BRY; };
class CEditBox;
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
    virtual CEditBox* GetParentElement();     /* slot 144 (+0x240) */
    virtual void v145();
    virtual void v146();
    virtual void UpdateDisplayText();         /* slot 147 (+0x24c) */
    virtual const C2DBoxF* GetRect();         /* slot 148 (+0x250) */
    unsigned char _pad004[0x15c - 4];
    CActionList   ActionsOnEnter;                           // +0x15c (STLport list, 4 bytes)
    CActionList   ActionsOnEscape;                          // +0x160
    CWideString   Text;                                     // +0x164
    long          CursorPos;                                // +0x168
    long          LeftMostCharacter;                        // +0x16c
    long          RightMostCharacter;                       // +0x170
    long          Adjustment;                               // +0x174
    bool          Active;                                   // +0x178
    bool          IsPasswordBox;                            // +0x179
    bool          UsesIME;                                  // +0x17a
    bool          DisallowSpaceAsFirstChar;                 // +0x17b
    bool          DeleteAllOnKeyPress;                      // +0x17c
    long          MaxLength;                                // +0x180
    CWideString   AllowedCharacters;                        // +0x184
    CWideString   ForbiddenCharacters;                      // +0x188
    void KeyPressed(unsigned short key);
};

void CEditBox::KeyPressed(unsigned short key)
{
    if (key == 0x6f) {
        if (--CursorPos < 0)
            CursorPos = 0;
        if (CursorPos < LeftMostCharacter)
            Adjustment -= 6;
    } else if (key == 0x70) {
        ++CursorPos;
        if (CursorPos > (long)Text.GetLength())
            CursorPos = Text.GetLength();
        if (CursorPos > RightMostCharacter - 1)
            Adjustment += 6;
    } else {
        if (GFGetIME()->IsIMEActive())
            goto refresh;
        if (key == 1) {
            for (CActionNode* node = ActionsOnEscape._M_node->_M_next; node != ActionsOnEscape._M_node; node = node->_M_next)
                CFrontEndManager::GetInstance()->PerformAction(node->_M_data);
        } else if (key == 0x75) {
            if (DeleteAllOnKeyPress) {
                Text = L"";
                CursorPos = 0;
            } else {
                {
                    CWideString before;
                    CWideString after;
                    if (CursorPos > 0)
                        before = Text.GetSubString(0, CursorPos);
                    if (CursorPos < (long)Text.GetLength())
                        after = Text.GetSubString(CursorPos + 1, Text.GetLength());
                    Text = before + after;
                }
                UpdateDisplayText();
                return;
            }
        } else if (key == 0x6c) {
            CursorPos = 0;
            float width = 0.0f;
            CEditBox* parent = GetParentElement();
            unsigned long i = 0;
            if (Text.GetLength() != 0) {
                do {
                    width += (float)GetCharWidthW(((const unsigned short*)Text)[i]);
                    if (width > parent->GetRect()->BRX)
                        break;
                    ++i;
                } while (i != Text.GetLength());
            }
            long adjustment = i - Text.GetLength() - 5;
            Adjustment = adjustment;
            if (adjustment < 0)
                Adjustment = 0;
        } else if (key == 0x71) {
            CursorPos = Text.GetLength();
            Adjustment = 0;
        } else if (key == 0x1c) {
            for (CActionNode* node = ActionsOnEnter._M_node->_M_next; node != ActionsOnEnter._M_node; node = node->_M_next)
                CFrontEndManager::GetInstance()->PerformAction(node->_M_data);
        } else {
            goto refresh;
        }
    }
    DeleteAllOnKeyPress = false;
refresh:
    UpdateDisplayText();
}

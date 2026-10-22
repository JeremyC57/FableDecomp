// CEditBox::AddCharacter(unsigned short c) -- type one character into the edit box: backslashes, a leading space when disallowed, characters outside the allowed set or inside the forbidden set and control characters are ignored; a pending delete-all clears the text; backspace removes the character before the cursor, anything else is inserted at the cursor while the text is below its maximum length; the display text is then refreshed
// Leaf: AddCharacter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053FE50, length 719 bytes (`ret 4`).
// Callees: CWideString ctor(const wchar_t*) 0x0099B6B0, ctor() 0x0099AED0, operator+=(unsigned short) 0x0099B9A0,
// Contains 0x0099AF20, GetLength 0x0099B220, operator=(const wchar_t*) 0x0099B800, GetSubString(start, end) 0x0099BDB0
// (by value), operator=(const CWideString&) 0x0099B7D0, ~CWideString 0x0099B510, operator+(CWideString, CWideString)
// 0x0099BE70 and operator+(CWideString, unsigned short) 0x0099C120 (both __fastcall, hidden result first);
// UpdateDisplayText (own vtable slot 147, +0x24c). Strings L"\\" 0x01246B44, L"" 0x0122D70C.
// Layout: Ego_r.pdb CEditBox members sit 0x40 lower in retail (Text +0x164 for the PDB's +0x1a4; CursorPos +0x168,
// DisallowSpaceAsFirstChar +0x17b, DeleteAllOnKeyPress +0x17c, MaxLength +0x180, AllowedCharacters +0x184,
// ForbiddenCharacters +0x188) -- modelled locally.
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
    CWideString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099BDB0
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& left, unsigned short c);           // 0x0099C120

class CEditBox {
public:
    virtual void v000();    virtual void v001();    virtual void v002();    virtual void v003();    virtual void v004();    virtual void v005();
    virtual void v006();    virtual void v007();    virtual void v008();    virtual void v009();    virtual void v010();    virtual void v011();
    virtual void v012();    virtual void v013();    virtual void v014();    virtual void v015();    virtual void v016();    virtual void v017();
    virtual void v018();    virtual void v019();    virtual void v020();    virtual void v021();    virtual void v022();    virtual void v023();
    virtual void v024();    virtual void v025();    virtual void v026();    virtual void v027();    virtual void v028();    virtual void v029();
    virtual void v030();    virtual void v031();    virtual void v032();    virtual void v033();    virtual void v034();    virtual void v035();
    virtual void v036();    virtual void v037();    virtual void v038();    virtual void v039();    virtual void v040();    virtual void v041();
    virtual void v042();    virtual void v043();    virtual void v044();    virtual void v045();    virtual void v046();    virtual void v047();
    virtual void v048();    virtual void v049();    virtual void v050();    virtual void v051();    virtual void v052();    virtual void v053();
    virtual void v054();    virtual void v055();    virtual void v056();    virtual void v057();    virtual void v058();    virtual void v059();
    virtual void v060();    virtual void v061();    virtual void v062();    virtual void v063();    virtual void v064();    virtual void v065();
    virtual void v066();    virtual void v067();    virtual void v068();    virtual void v069();    virtual void v070();    virtual void v071();
    virtual void v072();    virtual void v073();    virtual void v074();    virtual void v075();    virtual void v076();    virtual void v077();
    virtual void v078();    virtual void v079();    virtual void v080();    virtual void v081();    virtual void v082();    virtual void v083();
    virtual void v084();    virtual void v085();    virtual void v086();    virtual void v087();    virtual void v088();    virtual void v089();
    virtual void v090();    virtual void v091();    virtual void v092();    virtual void v093();    virtual void v094();    virtual void v095();
    virtual void v096();    virtual void v097();    virtual void v098();    virtual void v099();    virtual void v100();    virtual void v101();
    virtual void v102();    virtual void v103();    virtual void v104();    virtual void v105();    virtual void v106();    virtual void v107();
    virtual void v108();    virtual void v109();    virtual void v110();    virtual void v111();    virtual void v112();    virtual void v113();
    virtual void v114();    virtual void v115();    virtual void v116();    virtual void v117();    virtual void v118();    virtual void v119();
    virtual void v120();    virtual void v121();    virtual void v122();    virtual void v123();    virtual void v124();    virtual void v125();
    virtual void v126();    virtual void v127();    virtual void v128();    virtual void v129();    virtual void v130();    virtual void v131();
    virtual void v132();    virtual void v133();    virtual void v134();    virtual void v135();    virtual void v136();    virtual void v137();
    virtual void v138();    virtual void v139();    virtual void v140();    virtual void v141();    virtual void v142();    virtual void v143();
    virtual void v144();    virtual void v145();    virtual void v146();
    virtual void UpdateDisplayText();                       // slot 147 (+0x24c)
    unsigned char _pad004[0x164 - 4];
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
    void AddCharacter(unsigned short c);
};

void CEditBox::AddCharacter(unsigned short c)
{
    CWideString backslash(L"\\");
    CWideString character;
    character += c;
    if (backslash.Contains(character))
        return;
    if (DisallowSpaceAsFirstChar && CursorPos == 0 && c == ' ')
        return;
    if (c != 8) {
        if (AllowedCharacters.GetLength() != 0 && !AllowedCharacters.Contains(character))
            return;
        if (ForbiddenCharacters.GetLength() != 0 && ForbiddenCharacters.Contains(character))
            return;
        if (c < ' ')
            return;
    }
    if (DeleteAllOnKeyPress) {
        Text = L"";
        CursorPos = 0;
        DeleteAllOnKeyPress = false;
    }
    if (c == 8) {
        CWideString before;
        CWideString after;
        if (CursorPos > 0)
            before = Text.GetSubString(0, CursorPos - 1);
        if (CursorPos < (long)Text.GetLength())
            after = Text.GetSubString(CursorPos, Text.GetLength());
        Text = before + after;
        if (--CursorPos < 0)
            CursorPos = 0;
    } else {
        if ((long)Text.GetLength() >= MaxLength)
            return;
        CWideString before;
        CWideString after;
        if (CursorPos > 0)
            before = Text.GetSubString(0, CursorPos);
        if (CursorPos < (long)Text.GetLength())
            after = Text.GetSubString(CursorPos, Text.GetLength());
        Text = before + c + after;
        ++CursorPos;
    }
    UpdateDisplayText();
}
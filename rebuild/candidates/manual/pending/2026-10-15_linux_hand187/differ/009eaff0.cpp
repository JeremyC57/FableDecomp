// Grade: DIFFER(219v221) -- (1) prologue order: retail copies CurrentLine into the temporary before forming ToLower's hidden result address, ours forms &lowered first; (2) the command-text temporary dies right after the CConsoleCommandLine ctor here, retail destroys it after the guess (one full expression, but that form addresses the source through the ctor's returned this) | tried: copy-init + one-expression assignment, direct-init + named guess local (fixed assignment addressing), named text local (worse) | next: lane gap: temporary lifetime / evaluation order
// CConsole::SetGuessMode() -- enter command-guess mode: look the lower-cased current line up in the command map and, when the found command starts with it, take that command's text as the guessed command line
// Leaf: SetGuessMode
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EAFF0, 221 bytes (224 with padding).
// Callees: CCharString copy ctor 0x0099EC30, CCharString::ToLower 0x0099ED10 (result by hidden pointer), ~CCharString
//   0x0099EAE0, the command map's _Lbound 0x009EEC10 (Dinkumware _Tree<CCharString, ...>), strncmp 0x00BFEAF8, the command's
//   virtual slot 2 (+8, its text by value), CConsoleCommandLine::CConsoleCommandLine(const CCharString&) 0x009EEA10,
//   CCharString::operator= 0x0099EFB0, CBase::~CBase 0x0099A2E0 (the command line's base); "" @ 0x0129AAF4.
// Layout: CConsole: GuessLine (CConsoleCommandLine: vptr, Text +4, flag +8, cursor +0xc) +0x1c, CurrentLine +0x30, the
//   command map +0x40 (header node first), GuessMode +0x65, GuessIterator +0x68; map node key +0x10, value +0x14.
// Shape: c_str()/Length() are the null-first inline accessors (merged null test); the guess is assigned from a named
//   CConsoleCommandLine local (implicit memberwise operator=, inline destructor: text then ~CBase).
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);   // 0x00BFEAF8

struct CCharStringData {
    const char*   Text;                                     // +0x00
    unsigned long Length;                                   // +0x04
};

struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    CCharString ToLower() const;                            // 0x0099ED10
    const char* c_str() const { return PStringData == 0 ? "" : PStringData->Text; }
    unsigned long Length() const { return PStringData == 0 ? 0 : PStringData->Length; }
};

struct CBase {
    virtual void v00();
    ~CBase();                                               // 0x0099A2E0
};

struct CConsoleCommandLine : CBase {
    CCharString Text;                                       // +0x04
    bool          Flag;                                     // +0x08
    long          Cursor;                                   // +0x0c
    CConsoleCommandLine(const CCharString& text);           // 0x009EEA10
};

struct CConsoleCommand {
    virtual void v00();
    virtual void v01();
    virtual CCharString GetCommandText();                   // slot 2 (+8)
};

struct SCommandNode {
    SCommandNode* _Left;
    SCommandNode* _Parent;
    SCommandNode* _Right;
    long          _Color;
    CCharString   first;                                    // +0x10
    CConsoleCommand* second;                                // +0x14
};

struct CCommandMap {                                        // Dinkumware map<CCharString, CConsoleCommand*>
    SCommandNode* _Myhead;                                  // +0x00
    unsigned long _Mysize;
    SCommandNode* _Lbound(const CCharString& key);          // 0x009EEC10
};

struct CConsole {
    virtual void v00();
    unsigned char _pad04[0x1c - 4];
    CConsoleCommandLine GuessLine;                          // +0x1c
    unsigned char _pad2c[0x30 - 0x2c];
    CCharString CurrentLine;                                // +0x30
    unsigned char _pad34[0x40 - 0x34];
    CCommandMap Commands;                                   // +0x40
    unsigned char _pad48[0x65 - 0x48];
    bool GuessMode;                                         // +0x65
    SCommandNode* GuessIterator;                            // +0x68
    void SetGuessMode();
};

void CConsole::SetGuessMode()
{
    GuessMode = true;
    CCharString lowered(CCharString(CurrentLine).ToLower());
    GuessIterator = Commands._Lbound(lowered);
    if (GuessIterator != Commands._Myhead) {
        if (strncmp(GuessIterator->first.c_str(), lowered.c_str(), lowered.Length()) == 0) {
            CConsoleCommandLine guess(GuessIterator->second->GetCommandText());
            GuessLine = guess;
        }
    }
}

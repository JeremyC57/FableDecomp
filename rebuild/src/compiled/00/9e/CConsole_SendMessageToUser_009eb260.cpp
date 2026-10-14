// CConsole::SendMessageToUser(const CCharString&) -- wrap the text in a non-user command line (cursor at its end) and add it to the console's history
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EB260, length 94 bytes (+ int3 padding).
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 and CBaseClass::~CBaseClass @ 0x0099A2E0 (the command
// line's base, both out of line), CCharString::CCharString(const CCharString&) @ 0x0099EC30,
// CConsole::AddCommandLineToHistory(const CConsoleCommandLine&) @ 0x009EAF80 (the manifest labels it
// "CEngineManager::Cleanup", a propagated name), CCharString::~CCharString @ 0x0099EAE0.
// Vtable 0x0129C400 = CConsoleCommandLine (rtti-vtables.tsv); the line is a 16-byte stack object
// {vptr, CurrentEntry +4, UserEntry +8, CursorPosition +0xc}. Its constructor copies the text, clears
// UserEntry and takes the text's length (the inline GetLength: `p ? p->StringLength : 0`, where the
// null pointer itself serves as the zero); the inlined destructor destroys the string and calls the
// base destructor without re-storing the line's vtable.
struct CCharStringData {
    const char*   PData;                                        // +0
    unsigned long StringLength;                                 // +4
};

struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                      // 0x0099EC30
    ~CCharString();                                             // 0x0099EAE0
    long GetLength() const
    {
        if (PStringData == 0)
            return 0;                                           // the null block pointer itself is the zero
        return PStringData->StringLength;
    }
};

struct CBaseClass {
    virtual void v00();                                         // vtable 0x01231710
    CBaseClass();                                               // 0x0099A2D0
    ~CBaseClass();                                              // 0x0099A2E0
};

struct CConsoleCommandLine : CBaseClass {                       // vtable 0x0129C400
    CCharString CurrentEntry;                                   // +0x04
    bool        UserEntry;                                      // +0x08
    long        CursorPosition;                                 // +0x0c
    virtual void v00();
    CConsoleCommandLine(const CCharString& text)
        : CBaseClass(), CurrentEntry(text), UserEntry(false), CursorPosition(text.GetLength())
    {
    }
};

struct CConsole_Retail {
    virtual void v00();
    void AddCommandLineToHistory(const CConsoleCommandLine& line);   // 0x009EAF80
    void SendMessageToUser(const CCharString& message);
};

void CConsole_Retail::SendMessageToUser(const CCharString& message)
{
    CConsoleCommandLine line(message);
    AddCommandLineToHistory(line);
}
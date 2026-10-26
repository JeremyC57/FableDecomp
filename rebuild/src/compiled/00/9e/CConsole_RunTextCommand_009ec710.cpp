// CConsole::RunTextCommand(const CCharString&) -- run a script of ';'-separated console commands: each piece is appended to the command being built; outside quotes the command is put on the front line, run and the line reset, inside quotes the ';' is kept and the next piece appended
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EC710, 371 bytes (384 with padding). `ret 4`.
// Callees: CCharString(const char*, long) 0x0099EBF0, default ctor 0x0099E4B0, dtor 0x0099EAE0, operator=(const
//          CCharString&) 0x0099EFB0, operator=(const char*) 0x0099EFE0, operator+=(const CCharString&) 0x0099F0A0,
//          operator+=(const char*) 0x0099F100; CStringParser(const CCharString&, callback, const CCharString&, long)
//          0x00404720 and its inlined dtor (~list 0x004046B0, two ~CCharString, CBaseClass::~CBaseClass 0x0099A2E0);
//          CStringParser::ReadAsStringUntilString(const CCharString&, CParsedItem&) 0x009BA110 and ReadNextItem
//          0x009BA080; CParsedItem::GetString 0x009B9880; CConsole::RunCommandLine 0x009EB430.
//          Literals: "" 0x0122D70E, ";" 0x0123542C.
//
// Shape: `while (parser.ReadAsStringUntilString(";", item))` -- the ";" converts implicitly (the temporary is
// homed in the dead `text` argument slot and passed by `lea`); the delimiter item is consumed with an ignored
// ReadNextItem. The front line takes the command and its length (inline GetLength), runs, and is reset with
// the inline CConsoleCommandLine::Reset ("" / false / 0) before the command is cleared.
// Layout: the retail CConsole offsets of the landed rows (0x009EA660): FrontCommandLine +0x1c (CurrentEntry
// +0x20, UserEntry +0x24, CursorPosition +0x28); CStringParser as the landed 0x00CB7CB0 with the PDB CCursorPos
// {InQuotes, StringPos, Line} at +0x1c.
extern const char g_emptyText_0129aaf4[];  // 0x0129AAF4 ("", CCharString's empty text)
class CCharStringData { public:
    const char*   PData;                    // +0
    unsigned long StringLength;             // +4
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);  // 0x0099EC30
    ~CCharString();                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    const CCharString& operator=(const char* text);     // 0x0099EFE0
    const char* GetData() const
    {
        if (PStringData == 0)
            return g_emptyText_0129aaf4;
        return PStringData->PData;
    }
    unsigned long GetLength() const
    {
        if (PStringData == 0)
            return 0;
        return PStringData->StringLength;
    }
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
    CCharString& operator+=(const char* text);          // 0x0099F100
};

class CBaseClass { public:
    virtual void v00();
    ~CBaseClass();                          // 0x0099A2E0
};
struct CStringParserCommentList {           // retail std::list<CStringParserCommentBoundaries>: sentinel only
    void* m_pHead;
    ~CStringParserCommentList();            // 0x004046B0
};
struct CCursorPos {
    bool          InQuotes;                 // +0x00
    unsigned long StringPos;                // +0x04
    unsigned long Line;                     // +0x08
};
class CStringParser : public CBaseClass { public:         // retail 0x28 bytes
    char*                    String;        // +0x04
    unsigned long            BaseLineOffset;    // +0x08
    CCharString              Filename;      // +0x0c
    CCharString              TempCharString;    // +0x10
    CStringParserCommentList Comments;      // +0x14
    void (*ErrorCallback)(CCharString*);    // +0x18
    CCursorPos               CursorPos;     // +0x1c
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);     // 0x00404720
    bool ReadNextItem(class CParsedItem& item);    // 0x009BA080
    bool ReadAsStringUntilString(const CCharString& until, class CParsedItem& item);  // 0x009BA110
};
class CParsedItem { public:                        // 12 bytes
    long        Type;                       // +0x00 (1 = symbol, 2 = identifier, 3 = number)
    long        Value;                      // +0x04
    CCharString Text;                       // +0x08
    const CCharString& GetString();         // 0x009B9880
    const CCharString& GetSymbol();         // 0x009B9870
};
class CConsoleInputBase { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual CCharString GetHelp() const;    // slot 4 (+0x10)
};
struct CBaseClassLine : CBaseClass {        // CConsoleCommandLine, vtable 0x0129C400 (16 bytes)
    CCharString CurrentEntry;               // +0x04
    bool        UserEntry;                  // +0x08
    long        CursorPosition;             // +0x0c
    virtual void v00();
    CCharString GetEntry() const { return CurrentEntry; }
    void Reset()                            // an empty, non-user entry with the cursor at the start
    {
        CurrentEntry = "";
        UserEntry = false;
        CursorPosition = 0;
    }
};
struct CConsole_Retail {
    virtual void v00();
    unsigned char  _maps[0x18];             // +0x04 KeyBindingMap, +0x10 StringBindingMap
    CBaseClassLine FrontCommandLine;        // +0x1c
    CBaseClassLine BackCommandLine;         // +0x2c
    CCharString    HelpBarString;           // +0x3c
    void RunCommandLine();                  // 0x009EB430
    void RunTextCommand(const CCharString& text);
};

void CConsole_Retail::RunTextCommand(const CCharString& text)
{
    CStringParser parser(text, 0, CCharString(""), 0);
    CParsedItem item;
    CCharString command;
    while (parser.ReadAsStringUntilString(";", item)) {
        command += item.GetString();
        if (!parser.CursorPos.InQuotes) {
            FrontCommandLine.CurrentEntry = command;
            FrontCommandLine.CursorPosition = command.GetLength();
            RunCommandLine();
            FrontCommandLine.Reset();
            command = "";
        } else {
            command += ";";
        }
        parser.ReadNextItem(item);
    }
}
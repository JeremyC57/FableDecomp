// CConsole::RunCommandLine() -- parse the front command line and run it: the first item must be a string naming a registered input (looked up lower-cased), which is executed with a copy of the whole line; otherwise report "unknown input - <name>" or "command line was not a string" to the user
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EB430, 345 bytes (352 with padding).
// Callees: CCharString copy ctor 0x0099EC30, CCharString(const char*, long) 0x0099EBF0, default ctor 0x0099E4B0,
//          dtor 0x0099EAE0, ToLower 0x0099ED10, `__fastcall` operator+(const char*, const CCharString&) 0x0099F690;
//          CStringParser(const CCharString&, callback, const CCharString&, long) 0x00404720 and its inlined dtor
//          (~list 0x004046B0, two ~CCharString, CBaseClass::~CBaseClass 0x0099A2E0); CStringParser::ReadNextItem
//          0x009BA080; CParsedItem::GetString 0x009B9880; the input map's find 0x009EED20; CConsole::SendMessageToUser
//          0x009EB260; the input's virtual slot 1 (+0x04, Execute(const CCharString&, CConsole*)).
//          Literals: "" 0x0122D70E, "unknown input - " 0x0129C4C8, "command line was not a string" 0x0129C4A8.
//
// Shape: the parser reads a by-value copy of the front entry; the input name is a named copy of the item's
// string (alive across both arms) looked up through a ToLower temporary passed to an inline FindInput wrapper (a
// direct `RegisteredConsoleInputs.find(...)` hoists the map's `lea` above the ToLower call); the counted pointer's
// object is loaded once for the null test and the call.
// Layout: the retail CConsole offsets of the landed rows (0x009EA660): FrontCommandLine +0x1c,
// RegisteredConsoleInputs +0x40; CStringParser as the landed 0x00CB7CB0 (0x28 bytes).
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
    CCharString ToLower() const;            // 0x0099ED10
    ~CCharString();                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    const CCharString& operator=(const char* text);     // 0x0099EFE0
    const char* GetData() const
    {
        if (PStringData == 0)
            return g_emptyText_0129aaf4;
        return PStringData->PData;
    }
};
CCharString __fastcall operator+(const char* text, const CCharString& string);    // 0x0099F690

class CBaseClass { public:
    virtual void v00();
    virtual ~CBaseClass();                          // 0x0099A2E0
};
struct CStringParserCommentList {           // retail std::list<CStringParserCommentBoundaries>: sentinel only
    void* m_pHead;
    ~CStringParserCommentList();            // 0x004046B0
};
class CStringParser : public CBaseClass { public:         // retail 0x28 bytes
    char*                    String;        // +0x04
    unsigned long            BaseLineOffset;    // +0x08
    CCharString              Filename;      // +0x0c
    CCharString              TempCharString;    // +0x10
    CStringParserCommentList Comments;      // +0x14
    void (*ErrorCallback)(CCharString*);    // +0x18
    long                     CursorPos[3];  // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);     // 0x00404720
    bool ReadNextItem(class CParsedItem& item);    // 0x009BA080
};
class CParsedItem { public:                        // 12 bytes
    long        Type;                       // +0x00 (1 = symbol, 2 = identifier, 3 = number)
    long        Value;                      // +0x04
    CCharString Text;                       // +0x08
    const CCharString& GetString();         // 0x009B9880
    const CCharString& GetSymbol();         // 0x009B9870
};
class CConsole;
class CConsoleInputBase { public:
    virtual void v00();
    virtual void Execute(const CCharString& commandLine, CConsole* console);  // slot 1 (+0x04)
};
struct CInputPointer {                      // CCountedPointer<CConsoleInputBase>
    CConsoleInputBase* Object;
    void*              Info;
    CConsoleInputBase* operator->() const { return Object; }
};
struct SInputNode {                         // STLport _Rb_tree_node<pair<const CCharString, CCountedPointer<..>>>
    unsigned char  _M_color;
    SInputNode*    _M_parent;
    SInputNode*    _M_left;
    SInputNode*    _M_right;
    CCharString    first;                   // +0x10
    CInputPointer  second;                  // +0x14
};
struct CInputIterator {
    SInputNode* _M_node;
    SInputNode* operator->() const { return _M_node; }
};
struct CInputMap {                          // map<CCharString, CCountedPointer<CConsoleInputBase>> (12 bytes)
    SInputNode*   _M_header;
    unsigned long _M_node_count;
    unsigned char _M_compare;
    SInputNode* find(const CCharString& key);       // 0x009EED20 (labelled CStringMap::FindLowerBound; the iterator in eax)
};
struct CBaseClassLine : CBaseClass {        // CConsoleCommandLine, vtable 0x0129C400 (16 bytes)
    CCharString CurrentEntry;               // +0x04
    bool        UserEntry;                  // +0x08
    long        CursorPosition;             // +0x0c
    virtual void v00();
    CCharString GetEntry() const { return CurrentEntry; }
};
class CConsole { public:
    virtual void v00();
    unsigned char  _maps[0x18];             // +0x04 KeyBindingMap, +0x10 StringBindingMap
    CBaseClassLine FrontCommandLine;        // +0x1c
    CBaseClassLine BackCommandLine;         // +0x2c
    CCharString    HelpBarString;           // +0x3c
    CInputMap      RegisteredConsoleInputs; // +0x40
    void RunCommandLine();
    __forceinline SInputNode* FindInput(const CCharString& key) { return RegisteredConsoleInputs.find(key); }
public:
    void SendMessageToUser(const CCharString&);   // 0x009EB260
};

void CConsole::RunCommandLine()
{
    CStringParser parser(FrontCommandLine.GetEntry(), 0, CCharString(""), 0);
    CParsedItem item;
    if (parser.ReadNextItem(item)) {
        if (item.Type == 2) {
            CCharString name = item.GetString();
            SInputNode* it = FindInput(name.ToLower());
            CConsoleInputBase* input = it != RegisteredConsoleInputs._M_header ? it->second.Object : 0;
            if (!input)
                SendMessageToUser("unknown input - " + name);
            else
                input->Execute(FrontCommandLine.GetEntry(), this);
        } else
            SendMessageToUser("command line was not a string");
    }
}
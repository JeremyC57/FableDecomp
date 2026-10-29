// CConsoleVariable::ProcessCommandLine(const CCharString& line, CConsole& console) -- parse the command line after the
// variable name: with no value print "<name> = <value>"; otherwise read the target from the parser and, on failure,
// report the error and the variable's type (bool / float / slong / string)
// Leaf: ?ProcessCommandLine@CConsoleVariable
// Callees: CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CStringParser ctor 0x00404720 and
//   its implicit inline dtor (~list 0x004046B0, two ~CCharString, CBaseClass::~CBaseClass 0x0099A2E0), ReadNextItem
//   0x009BA080, NextItemExists 0x009BA4F0, CConsoleTarget::PrintToString 0x00A63C00 / ReadFromParser 0x00A63FC0,
//   operator+ 0x0099F600 / 0x0099F570 / 0x0099F690 (__fastcall), operator+= 0x0099F100, CConsole::SendMessageToUser 0x009EB260.
// Shape: the 12-byte parser item (type, value, text) is constructed first and filled by ReadNextItem; the printed value
//   and the type message reuse the dead parameter slots; the type names are an if / else-if chain (4, 3, 1, 5).
// Layout: CConsoleVariable vtable +0, Name +4, Target +8 (type enum first); CStringParser 0x28 bytes (landed 0x00CB8690).
class CCharString { public:
    void* m_pData;
    CCharString();                                    // 0x0099E4B0
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
    const CCharString& operator+=(const char* text);        // 0x0099F100
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
class CBaseClass { public:
    virtual void v00();
    virtual ~CBaseClass();                                    // 0x0099A2E0
};
struct CStringParserCommentList {
    void* m_pHead;
    ~CStringParserCommentList();                      // 0x004046B0
};
class CStringParser : public CBaseClass { public:                   // retail 0x28 bytes
    struct CItem {                                    // item read by ReadNextItem: type, value, text
        long        Type;
        long        Value;
        CCharString Text;
    };
    char*                    String;                  // +0x04
    unsigned long            BaseLineOffset;          // +0x08
    CCharString              Filename;                // +0x0c
    CCharString              TempCharString;          // +0x10
    CStringParserCommentList Comments;                // +0x14
    void (*ErrorCallback)(CCharString*);              // +0x18
    long                     CursorPos[3];            // +0x1c
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);               // 0x00404720
    void ReadNextItem(CItem& item);                   // 0x009BA080
    bool NextItemExists();                            // 0x009BA4F0
};
class CConsole { public:
    void SendMessageToUser(const CCharString& message);   // 0x009EB260
};
struct CConsoleTarget {
    long Type;                                        // 1 slong, 3 float, 4 bool, 5 string
    void PrintToString(CCharString& out) const;       // 0x00A63C00
    bool ReadFromParser(CStringParser& parser);       // 0x00A63FC0
};
class CConsoleVariable {
public:
    virtual void v00();
    CCharString    Name;                              // +0x04
    CConsoleTarget Target;                            // +0x08
private:
    virtual bool ProcessCommandLine(const CCharString& line, CConsole& console);
};

bool CConsoleVariable::ProcessCommandLine(const CCharString& line, CConsole& console)
{
    CStringParser::CItem item;
    CStringParser parser(line, 0, CCharString(""), 0);
    parser.ReadNextItem(item);
    bool result;
    if (!parser.NextItemExists()) {
        CCharString value;
        Target.PrintToString(value);
        console.SendMessageToUser(Name + " = " + value);
        result = true;
    } else {
        result = Target.ReadFromParser(parser);
        if (!result) {
            console.SendMessageToUser("error - could not read target for variable");
            CCharString message = "type of " + Name + ": ";
            if (Target.Type == 4)
                message += "bool";
            else if (Target.Type == 3)
                message += "float";
            else if (Target.Type == 1)
                message += "slong";
            else if (Target.Type == 5)
                message += "string";
            console.SendMessageToUser(message);
        }
    }
    return result;
}
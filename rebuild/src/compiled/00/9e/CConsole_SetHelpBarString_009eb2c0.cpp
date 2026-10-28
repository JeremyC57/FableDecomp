// CConsole::SetHelpBarString() -- parse the front command line: when it starts with the name of a registered input that is either alone or followed by the "(" symbol, show that input's help in the help bar, otherwise clear the bar
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EB2C0, 368 bytes.
// Callees: CCharString copy ctor 0x0099EC30, CCharString(const char*, long) 0x0099EBF0, default ctor 0x0099E4B0,
//          dtor 0x0099EAE0, operator=(const CCharString&) 0x0099EFB0, operator=(const char*) 0x0099EFE0;
//          CStringParser(const CCharString&, callback, const CCharString&, long) 0x00404720 and its inlined dtor
//          (~list 0x004046B0, two ~CCharString, CBaseClass::~CBaseClass 0x0099A2E0); CStringParser::ReadNextItem
//          0x009BA080; CParsedItem accessors 0x009B9880 / 0x009B9870 (both `lea eax,[ecx+8]`, labelled
//          CModeBlock::GetAnimFilter); CConsole::FindInput(const CCharString&) 0x009EB220 (labelled
//          StringMap_FindLowerBoundAndToLower); the input's virtual slot 4 (+0x10, GetHelp, CCharString by value).
//          Globals: the "(" string g_CCharString_013caa48 (0x013CAA48), the "" literal 0x0122D70E.
//
// Shape: GetData's "" (0x0129AAF4) is an extern array so the "" literal (0x0122D70E) does not pool with it. The parser reads a by-value copy of the front entry; the item is a 12-byte {type, value, text} local.
// The "(" test is an inline CCharString::operator==(char) (null data is false, else the strcmp intrinsic
// against a {c, 0} buffer homed in a dead temporary slot, `__forceinline`). The help is assigned under nested ifs
// with an early return and one trailing `HelpBarString = ""` for every failure: if/else arms that each assign ""
// are cross-jumped too, but with `lea ecx` scheduled above the literal push.
// Fix (sweep 3): CCharString and CParsedItem are classes (the PDB keys) and the two accessors are const, so they bind
// the PDB symbols ?GetString@CParsedItem@@QBEABVCCharString@@XZ 0x009B9880 / ?GetSymbol@...@@QBE... 0x009B9870 (retail body
// order = the PDB member run GetSymbol, GetString, GetInteger, GetFloat). CBaseClass's destructor is virtual, so the
// inlined parser teardown binds ??1CBaseClass@@UAE@XZ 0x0099A2E0 (was the non-virtual QAE spelling). Same bytes.
// Layout: the retail CConsole offsets of the landed rows (0x009EA660): FrontCommandLine +0x1c, HelpBarString
// +0x3c; CStringParser as the landed 0x00CB7CB0 (0x28 bytes).
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

extern const char g_emptyText_0129aaf4[];  // 0x0129AAF4 ("", CCharString's empty text)
class CCharStringData { public:
    const char*   PData;                    // +0
    unsigned long StringLength;             // +4
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);  // 0x0099EC30
    ~CCharString();                         // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    const CCharString& operator=(const char* text);     // 0x0099EFE0
    const char* GetData() const
    {
        if (PStringData == 0)
            return g_emptyText_0129aaf4;
        return PStringData->PData;
    }
    __forceinline bool operator==(char c) const
    {
        if (PStringData == 0)
            return false;
        char text[2] = { c, 0 };
        return strcmp(PStringData->PData, text) == 0;
    }
};
extern CCharString g_CCharString_013caa48;  // 0x013CAA48 ("(")

class CBaseClass { public:
    virtual void v00();
    virtual ~CBaseClass();                  // 0x0099A2E0
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
    bool ReadNextItem(class CParsedItem& item);     // 0x009BA080
};
class CParsedItem {                         // 12 bytes (Ego_r.pdb)
public:
    long        Type;                       // +0x00 (1 = symbol, 2 = identifier, 3 = number)
    long        Value;                      // +0x04
    CCharString Text;                       // +0x08
    const CCharString& GetString() const;   // 0x009B9880
    const CCharString& GetSymbol() const;   // 0x009B9870
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
};
struct CConsole_Retail {
    virtual void v00();
    unsigned char  _maps[0x18];             // +0x04 KeyBindingMap, +0x10 StringBindingMap
    CBaseClassLine FrontCommandLine;        // +0x1c
    CBaseClassLine BackCommandLine;         // +0x2c
    CCharString    HelpBarString;           // +0x3c
    CConsoleInputBase* FindInput(const CCharString& name);    // 0x009EB220
    void SetHelpBarString();
};

void CConsole_Retail::SetHelpBarString()
{
    CStringParser parser(FrontCommandLine.GetEntry(), 0, CCharString(""), 0);
    CParsedItem item;
    if (parser.ReadNextItem(item) && item.Type == 2) {
        CConsoleInputBase* input = FindInput(item.GetString());
        if (!(parser.ReadNextItem(item) && item.Type == 1) || item.GetSymbol() == g_CCharString_013caa48.GetData()[0]) {
            if (input) {
                HelpBarString = input->GetHelp();
                return;
            }
        }
    }
    HelpBarString = "";
}
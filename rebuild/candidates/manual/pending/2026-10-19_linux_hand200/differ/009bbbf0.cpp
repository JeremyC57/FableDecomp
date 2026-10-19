// Grade: DIFFER(ours v retail) -- frame 0x38 v 0x40: retail leaves 8 unused bytes between the three message temporaries and the spare string (every lea offset shifts); instructions otherwise identical
// CPersistContext::CheckUnknownSymbols() -- after a text load, report every item left unparsed in the source text as `Unknown entry "<item>"`
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BBBF0, 207 bytes, `ret`.
// Callees: CStringParser::CStringParser(const CStringParser&) 0x0046381B, CCharString::CCharString() 0x0099E4B0,
//   CStringParser::SkipWhitespace 0x009B9C60, CStringParser::NextItemExists 0x009BA4F0,
//   CStringParser::ReadAsStringUntilWhitespace 0x009BA4A0 (returns CCharString), operator+(const char*, const CCharString&)
//   0x0099F690, operator+(const CCharString&, const char*) 0x0099F600, CPersistContext::Error 0x009BAA90 (catalogued
//   "CDefinitionManager::Error"; it is called on the context with the parser and the message), ~CCharString 0x0099EAE0,
//   list<CStringParserCommentBoundaries>::~list 0x004046B0, CBaseClass::~CBaseClass 0x0099A2E0.
// Shape: the context's parser (PParser +0x8, Ego_r.pdb) is copied so the scan does not move the real cursor; a spare
//   CCharString local declared after it is built and destroyed but never used. In the loop the message is one expression
//   `"Unknown entry \"" + parser.ReadAsStringUntilWhitespace() + "\""` (the closing quote is pushed first, VC evaluates
//   right to left), its three temporaries die after the Error call. The parser's destructor is compiler-generated (no
//   vtable store): comment list, the two strings, then the out-of-line CBaseClass destructor. Retail CStringParser is 0x28
//   bytes (the PDB's 0x30 has the debug-sized list).
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600

class CBaseClass {
public:
    virtual ~CBaseClass();                              // 0x0099A2E0
};

struct CStringParserCommentList {                       // list<CStringParserCommentBoundaries>, retail 8 bytes
    void* m_pHead;
    unsigned long m_size;
    ~CStringParserCommentList();                        // 0x004046B0
};

class CStringParser : public CBaseClass {
public:
    char* String;                                       // +0x04
    unsigned long BaseLineOffset;                       // +0x08
    CCharString Filename;                               // +0x0C
    CCharString TempCharString;                         // +0x10
    CStringParserCommentList Comments;                  // +0x14
    void (*ErrorCallback)(CCharString*);                // +0x1C
    long CursorPos[2];                                  // +0x20
    CStringParser(const CStringParser& other);          // 0x0046381B
    long SkipWhitespace();                              // 0x009B9C60
    bool NextItemExists();                              // 0x009BA4F0
    CCharString ReadAsStringUntilWhitespace();          // 0x009BA4A0
};

struct CPersistContext {
    void*          PDefinitionManager;                  // +0x00
    void*          PSymbolMap;                          // +0x04
    CStringParser* PParser;                             // +0x08
    unsigned char  _pad0c[0x20];
    void Error(CStringParser* parser, const CCharString& message);   // 0x009BAA90
    void CheckUnknownSymbols();
};

void CPersistContext::CheckUnknownSymbols()
{
    CStringParser parser(*PParser);
    CCharString unused;
    parser.SkipWhitespace();
    while (parser.NextItemExists()) {
        Error(&parser, "Unknown entry \"" + parser.ReadAsStringUntilWhitespace() + "\"");
        parser.SkipWhitespace();
    }
}

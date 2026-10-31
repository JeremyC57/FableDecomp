// CPersistContext::CheckUnknownSymbols() -- after a text load, report every item left unparsed in the source text as `Unknown entry "<item>"`
// Grade: DIFFER(207v207) -- frame 0x38 v 0x40: retail leaves 8 unused bytes (+0x10..+0x17) between the three message temporaries (+4/+8/+0xC) and the spare string (+0x18); every lea offset shifts, instructions otherwise identical
// Leaf: ?CheckUnknownSymbols@CPersistContext@@QAEXXZ
// Chunk 631: callee respelled ?PersistError@CPersistContext@@QBEXABVCStringParser@@ABVCCharString@@@Z (was Error(CStringParser*, ...)),
//   the comment list is the bound std::list<CStringParserCommentBoundaries> (4 bytes, retail STLport) and the parser layout follows the
//   landed PersistError model (ErrorCallback +0x18, CursorPos +0x1C). Tried: a named item local (211), a for loop (same frame).
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
class CCharString {
public:
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

class CStringParserCommentBoundaries;
namespace std {
template <class T> class allocator;
template <class T, class A = allocator<T> > class list {   // STLport 4.6 list: one node pointer (4 bytes)
public:
    void* _M_node;
    ~list();                                            // 0x004046B0 (this instantiation)
};
}
struct CCursorPos {
    bool InQuotes;                                      // +0x00
    unsigned long StringPos;                            // +0x04
    unsigned long Line;                                 // +0x08
};

class CStringParser : public CBaseClass {
public:
    char* String;                                       // +0x04
    unsigned long BaseLineOffset;                       // +0x08
    CCharString Filename;                               // +0x0C
    CCharString TempCharString;                         // +0x10
    std::list<CStringParserCommentBoundaries> Comments; // +0x14
    void (__fastcall* ErrorCallback)(const CCharString&);   // +0x18
    CCursorPos CursorPos;                               // +0x1C
    CStringParser(const CStringParser& other);          // 0x0046381B
    long SkipWhitespace();                              // 0x009B9C60
    bool NextItemExists();                              // 0x009BA4F0
    CCharString ReadAsStringUntilWhitespace();          // 0x009BA4A0
};

class CPersistContext {
public:
    void*          PDefinitionManager;                  // +0x00
    void*          PSymbolMap;                          // +0x04
    CStringParser* PParser;                             // +0x08
    unsigned char  _pad0c[0x20];
    void PersistError(const CStringParser& parser, const CCharString& message) const;   // 0x009BAA90
    void CheckUnknownSymbols();
};

void CPersistContext::CheckUnknownSymbols()
{
    CStringParser parser(*PParser);
    CCharString unused;
    parser.SkipWhitespace();
    while (parser.NextItemExists()) {
        PersistError(parser, "Unknown entry \"" + parser.ReadAsStringUntilWhitespace() + "\"");
        parser.SkipWhitespace();
    }
}

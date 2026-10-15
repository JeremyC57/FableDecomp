// CGameTimeManager::Load(const CCharString& text) -- parses the saved time-manager text: builds a string parser over it and a text persist context, then runs the context-based Load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BBBA0, length 127 bytes (padded to 144).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (the "" file name @ 0x0122D70E, -1),
//          CStringParser::CStringParser(const CCharString&, void (*)(CCharString*), const CCharString&, long) @ 0x00404720,
//          CCharString::~CCharString @ 0x0099EAE0, CPersistContext::CPersistContext(CStringParser*, void (*)(CCharString*),
//          bool, CDefinitionManager*, CSymbolMap*, CCharString*) @ 0x009BADD0, CGameTimeManager::Load(CPersistContext&)
//          @ 0x006BBAC0, std::list<CStringParserCommentBoundaries>::~list @ 0x004046B0, CBase::~CBase @ 0x0099A2E0.
// Shape: the CAnimComponentFlags::Initialise (0x005DC3C0) parser shape -- a 0x28-byte CStringParser local built over the
// text with an explicit `CCharString("", -1)` temporary that dies after the declaration, then a 0x2c-byte persist
// context with a trivial destructor (nothing runs for it); the parser's implicit destructor is inlined (comment list,
// two strings, the out-of-line CBase destructor). No EH frame.
// Leaf: ?Load@CGameTimeManager@@QAEXABVCCharString@@@Z
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
struct CBase {
    virtual void v00();
    ~CBase();                                      // 0x0099A2E0
};
struct CStringParserCommentList {                  // retail std::list<CStringParserCommentBoundaries>: sentinel only
    void* m_pHead;
    ~CStringParserCommentList();                   // 0x004046B0
};
struct CStringParser : CBase {                     // retail 0x28 bytes
    char*                    String;               // +0x04
    unsigned long            BaseLineOffset;       // +0x08
    CCharString              Filename;             // +0x0c
    CCharString              TempCharString;       // +0x10
    CStringParserCommentList Comments;             // +0x14
    void (*ErrorCallback)(CCharString*);           // +0x18
    long                     CursorPos[3];         // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);            // 0x00404720
};
struct CDefinitionManager;
struct CSymbolMap;
struct CPersistContext {                           // retail 0x2c bytes, trivial destructor
    unsigned char _pad00[0x2c];
    CPersistContext(CStringParser* parser, void (*errorCallback)(CCharString*), bool reportErrors,
                    CDefinitionManager* definitions, CSymbolMap* symbols, CCharString* sourceName);   // 0x009BADD0
};
extern const char g_emptyString_0122d70e[];       // 0x0122D70E ("")
struct CGameTimeManager {
    unsigned char _pad00[0x40];
    void Load(CPersistContext& context);           // 0x006BBAC0
    void Load(const CCharString& text);
};

void CGameTimeManager::Load(const CCharString& text)
{
    CStringParser parser(text, 0, CCharString(g_emptyString_0122d70e, -1), 0);
    CPersistContext context(&parser, 0, true, 0, 0, 0);
    Load(context);
}
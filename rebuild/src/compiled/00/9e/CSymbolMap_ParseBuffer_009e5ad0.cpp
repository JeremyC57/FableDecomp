// CSymbolMap::ParseBuffer(char* buffer) -- parse a symbol-definition text buffer: for every "enum" token found, parse the enum block (fail on the first bad block)
// Leaf: ParseBuffer
// Retail 0x009E5AD0, 352 bytes, `ret 4`.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E, "enum" 0x0129B144), ~CCharString 0x0099EAE0,
// CCharString() 0x0099E4B0, CBaseClass::CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0, malloc 0x00BFEA0E (list sentinel),
// CStringParser::Init(char*, callback, const CCharString&, long) 0x009B9530, the parser's keyword search 0x009B9BA0
// (bool, const CCharString&; labelled CThingCreatureBase::SerialiseGameState), CSymbolMap::ParseEnum(CStringParser*)
// 0x009E55A0, list<CStringParserCommentBoundaries>::~list 0x004046B0. Vtable 0x0122D874 (CStringParser).
// Layout: Ego_r.pdb CStringParser with the retail one-word list (0x28 bytes): String +4, BaseLineOffset +8, Filename +0xC,
// TempCharString +0x10, Comments +0x14, ErrorCallback +0x18, CursorPos +0x1C.
// Shape: the parser is built by an inline constructor that calls Init, so the "" filename temporary is built before the
// parser's base constructor (an explicit `CCharString("")`, whose ctor result stays in esi); an unused 12-byte token local
// follows (its text CCharString at +8 is the only constructed member: retail's frame has two dead words before it); the parser has no user destructor (no vtable reset in
// the inlined teardown); `"enum"` is a temporary homed in the dead `buffer` slot, the bool taken before its dtor.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
struct CCharString {
    void* PStringData;
    CCharString();                                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
};
struct CBaseClass {
    CBaseClass();                                          // 0x0099A2D0
    virtual ~CBaseClass();                                 // 0x0099A2E0
};
struct SCommentNode {                                      // list<CStringParserCommentBoundaries> node
    SCommentNode* m_next;
    SCommentNode* m_prev;
    char*         PStart;
    char*         PEnd;
};
struct CCommentList {                                      // retail list: the sentinel pointer
    SCommentNode* m_head;
    CCommentList() : m_head(0)
    {
        SCommentNode* node = (SCommentNode*)malloc(sizeof(SCommentNode));
        node->m_next = node;
        node->m_prev = node;
        m_head = node;
    }
    ~CCommentList();                                       // 0x004046B0
};
struct CCursorPos { long Line, Column, Offset; };
typedef void (__cdecl* TErrorCallback)(CCharString*);
struct CStringParser : CBaseClass {                        // vtable 0x0122D874
    char*          String;                                 // +0x04
    unsigned long  BaseLineOffset;                         // +0x08
    CCharString    Filename;                               // +0x0c
    CCharString    TempCharString;                         // +0x10
    CCommentList   Comments;                               // +0x14
    TErrorCallback ErrorCallback;                          // +0x18
    CCursorPos     CursorPos;                              // +0x1c
    CStringParser(char* text, TErrorCallback callback, const CCharString& filename, long baseLine)
    {
        Init(text, callback, filename, baseLine);
    }
    void Init(char* text, TErrorCallback callback, const CCharString& filename, long baseLine);   // 0x009B9530
    bool FindKeyword(const CCharString& keyword);          // 0x009B9BA0
};
struct SParseToken {                                       // 12 bytes: two POD words and the token text
    long        Type;
    long        Value;
    CCharString Text;
};
struct CSymbolMap {
    bool ParseEnum(CStringParser* parser);                 // 0x009E55A0
    bool ParseBuffer(char* buffer);
};

bool CSymbolMap::ParseBuffer(char* buffer)
{
    CStringParser parser(buffer, 0, CCharString(""), 0);
    SParseToken token;
    while (parser.FindKeyword("enum")) {
        if (!ParseEnum(&parser))
            return false;
    }
    return true;
}
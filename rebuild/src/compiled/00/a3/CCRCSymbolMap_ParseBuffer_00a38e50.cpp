// CCRCSymbolMap::ParseBuffer(char* buffer, long context) -- parse a symbol-definition text buffer: for every "enum" token found, parse the enum block into the CRC map (fail on the first bad block)
// Leaf: ParseBuffer
// Retail 0x00A38E50, 350 bytes, `ret 8`. The byte twin of the landed CSymbolMap::ParseBuffer 0x009E5AD0 with a second
// argument (a dword kept in esi across the loop) handed on to the per-block parser.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E, "enum" 0x0129B144), ~CCharString 0x0099EAE0,
// CCharString() 0x0099E4B0, CBaseClass::CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0, malloc 0x00BFEA0E (list sentinel),
// CStringParser::Init(char*, callback, const CCharString&, long) 0x009B9530, CStringParser::FindKeyword 0x009B9BA0,
// CCRCSymbolMap::ParseEnum(CStringParser*, long) 0x00A38670 (catalog name unknown; the second argument's type is not
// recoverable from this caller: a 4-byte value), list<CStringParserCommentBoundaries>::~list 0x004046B0. Vtable 0x0122D874.
// Layout: Ego_r.pdb CStringParser with the retail one-word list (0x28 bytes): String +4, BaseLineOffset +8, Filename +0xC,
// TempCharString +0x10, Comments +0x14, ErrorCallback +0x18, CursorPos +0x1C.
// Shape: as the landed sibling -- the parser is built by an inline constructor that calls Init (the "" filename temporary
// before the base constructor), an unused 12-byte token local whose text member is the only constructed part, "enum" a
// temporary homed in the dead `buffer` slot with the bool taken before its dtor.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
class CCharString { public:
    void* PStringData;
    CCharString();                                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
};
class CBaseClass { public:
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
class CStringParser : public CBaseClass { public:                        // vtable 0x0122D874
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
class CCRCSymbolMap { public:
    bool ParseEnum(CStringParser* parser, long context);   // 0x00A38670
    bool ParseBuffer(char* buffer, long context);
};

bool CCRCSymbolMap::ParseBuffer(char* buffer, long context)
{
    CStringParser parser(buffer, 0, CCharString(""), 0);
    SParseToken token;
    while (parser.FindKeyword("enum")) {
        if (!ParseEnum(&parser, context))
            return false;
    }
    return true;
}
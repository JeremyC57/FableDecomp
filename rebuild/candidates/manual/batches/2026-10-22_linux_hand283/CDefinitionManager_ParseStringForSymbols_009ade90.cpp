// CDefinitionManager::ParseStringForSymbols(const char* text, bool instantiate) -- symbol pre-pass over a definition
// source: optionally instantiate every "#definition_template" and then every "#definition" (class name, instance
// name; unknown classes reported), rewinding the parser after each pass; then parse every "enum" for its symbols
// Leaf: ParseStringForSymbols
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ADE90, length 699 bytes, `ret 8`. Mangled (PDB):
//   ?ParseStringForSymbols@CDefinitionManager@@IAEXPBD_N@Z
// Callees: CBaseClass ctor 0x0099A2D0 / ~ 0x0099A2E0, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / ~
//   0x0099EAE0, malloc 0x00BFEA0E (comment-list head node), CStringParser::Init 0x009B9530 / SkipToItem 0x009B9C00
//   (descriptive) / ReadAsString 0x009BA6F0 / SetStringPos 0x009B9940, the comment list dtor 0x004046B0,
//   InstantiateDef 0x009ADC90, operator+(const char*, const CCharString&) 0x0099F690 (__fastcall), Error 0x009AC9F0,
//   ParseEnumForSymbols 0x009ACEE0.
// Layout: retail CDefinitionManager: ErrorCallback +0xb0, CurrentFilename +0xb4 (PDB +0xcc/+0xd0); the parser is the
//   retail 0x28-byte CStringParser (vtable 0x0122D874; inline default ctor model: landed CTCBase::SerialiseLevelState
//   0x00686E10).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
class CBaseClass {
public:
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                         // 0x0099A2E0
};
class CCharString {
public:
    void* PStringData;
    CCharString();                                 // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
struct CStringParserCommentList {
    struct SNode { SNode* Next; SNode* Prev; char* PStart; char* PEnd; };
    SNode* m_pHead;
    CStringParserCommentList() : m_pHead(0)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        node->Next = node;
        node->Prev = node;
        m_pHead = node;
    }
    ~CStringParserCommentList();                   // 0x004046B0
};
struct CCursorPos {
    bool          InQuotes;
    unsigned long StringPos;
    unsigned long Line;
};
typedef void (__cdecl* TErrorCallback)(CCharString*);
class CStringParser : public CBaseClass {          // vtable 0x0122D874
public:
    char*                    String;               // +0x04
    unsigned long            BaseLineOffset;       // +0x08
    CCharString              Filename;             // +0x0C
    CCharString              TempCharString;       // +0x10
    CStringParserCommentList Comments;             // +0x14
    TErrorCallback           ErrorCallback;        // +0x18
    CCursorPos               CursorPos;            // +0x1C
    CStringParser(const char* text, TErrorCallback callback, const CCharString& filename, long baseLine)
    {
        Init(text, callback, filename, baseLine);
    }
    void Init(const char* text, TErrorCallback callback, const CCharString& filename, long baseLine);   // 0x009B9530
    bool SkipToItem(const CCharString& name);      // 0x009B9C00
    CCharString ReadAsString();                    // 0x009BA6F0
    void SetStringPos(unsigned long pos);          // 0x009B9940
};
struct CDefinitionManager {
    unsigned char  _pad000[0xb0];
    TErrorCallback ErrorCallback;                  // +0xb0
    CCharString    CurrentFilename;                // +0xb4
    bool InstantiateDef(const CCharString& className, const CCharString& instanceName, bool isTemplate);   // 0x009ADC90
    void Error(const CStringParser& parser, const CCharString& message);   // 0x009AC9F0
    void ParseEnumForSymbols(CStringParser& parser);   // 0x009ACEE0
    void ParseStringForSymbols(const char* text, bool instantiate);
};

void CDefinitionManager::ParseStringForSymbols(const char* text, bool instantiate)
{
    CStringParser parser(text, ErrorCallback, CurrentFilename, 0);
    if (instantiate) {
        while (parser.SkipToItem("#definition_template")) {
            CCharString className = parser.ReadAsString();
            CCharString instanceName = parser.ReadAsString();
            if (!InstantiateDef(className, instanceName, true))
                Error(parser, "Unable to find def class called " + className);
        }
        parser.SetStringPos(0);
        while (parser.SkipToItem("#definition")) {
            CCharString className = parser.ReadAsString();
            CCharString instanceName = parser.ReadAsString();
            if (!InstantiateDef(className, instanceName, false))
                Error(parser, "Unable to find def class called " + className);
        }
        parser.SetStringPos(0);
    }
    while (parser.SkipToItem("enum"))
        ParseEnumForSymbols(parser);
}

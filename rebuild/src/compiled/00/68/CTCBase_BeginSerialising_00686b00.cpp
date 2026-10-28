// CTCBase::BeginSerialising(bool saving, CCharString& data, CStringParser& parser, CPersistContext& context) -- open a
// component's "Start<ClassName>;" block: saving appends the header to the level-state string and points the persist
// context at it; loading parses the string, finds the block and hands its body to the context through the parser
// Retail 0x00686B00, 0x211 bytes, `ret 10h`. Callees: virtual slot 22 (+0x58, CCharString GetClassTypeName() const),
// CCharString operator+(const char*, const CCharString&) 0x0099F690 and operator+(const CCharString&, const char*)
// 0x0099F600 (both __fastcall), CCharString(const char*, int) 0x0099EBF0, ~CCharString 0x0099EAE0, operator=
// 0x0099EFB0, operator+= 0x0099F0A0, CStringParser::Init(const char*, callback, const CCharString&, long) 0x009B9530,
// SkipToItem 0x009B9C00 (label CThingCreatureBase::SerialiseGameState is propagated), ReadAsStringUntilString
// 0x009BA330, CPersistContext::Init(CCharString&, CDefinitionManager*) 0x009BAA10 (save) and Init(CStringParser*,
// callback, bool, CDefinitionManager*) 0x009BA9D0 (load), CGameDefinitionManager::Get 0x0044C6B0 (__fastcall).
// Shape: the load path in an `else` arm with its own `return true/false` (one shared `return true` after the if/else
// grows the frame; a single if with fall-through load code shares the saving temporary's slot). The parser's inline Init(const CCharString&, filename = "") keeps the text alive in TempCharString (+0x10)
// then calls the char* Init with the caller's string; the tags are named locals (their operator+ temporaries die
// at the end of each declaration).
// Layout: the retail 0x28-byte CStringParser of the landed caller 0x00686E10 (TempCharString +0x10).
class CCharString {
public:
    struct SData { char* Data; };
    SData* PStringData;
    CCharString(const char* text, int length = -1);    // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    CCharString& operator+=(const CCharString& other); // 0x0099F0A0
    const char* c_str() const { return !PStringData ? "" : PStringData->Data; }
};
CCharString __fastcall operator+(const char* a, const CCharString& b);    // 0x0099F690
CCharString __fastcall operator+(const CCharString& a, const char* b);    // 0x0099F600

typedef void (*TStringParserErrorCallback)(const char*);
class CStringParser {
public:
    void* __vftable;                                   // +0x00
    char* String;                                      // +0x04
    unsigned long BaseLineOffset;                      // +0x08
    CCharString Filename;                              // +0x0C
    CCharString TempCharString;                        // +0x10
    void* Comments;                                    // +0x14
    TStringParserErrorCallback ErrorCallback;          // +0x18
    unsigned char CursorPos[0xc];                      // +0x1C
    void Init(const char* text, TStringParserErrorCallback callback, const CCharString& filename, long baseLine);   // 0x009B9530
    void Init(const CCharString& text, const CCharString& filename = CCharString(""))
    {
        TempCharString = text;
        Init(text.c_str(), 0, filename, 0);
    }
    bool SkipToItem(const CCharString& name);                     // 0x009B9C00
    CCharString ReadAsStringUntilString(const CCharString& end);  // 0x009BA330
};

class CSymbolMap;
class CDefinitionManager { public: unsigned char _body[0xc4]; };
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};

class CPersistContext { public:
    unsigned char _body[0x2c];
    void Init(CCharString& data, CDefinitionManager* defs);                                                       // 0x009BAA10
    void Init(CStringParser* parser, TStringParserErrorCallback callback, bool loading, CDefinitionManager* defs,
              CSymbolMap* symbols = 0, CCharString* name = 0);                                             // 0x009BA9D0
};

class CTCBase {
public:
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15)
    VF(16) VF(17) VF(18) VF(19) VF(20) VF(21)
    virtual CCharString GetClassTypeName() const;      // slot 22 (+0x58)
    void*         ParentThing;                         // +0x04
    bool          ToBeRemoved;                         // +0x08
    bool          IsInGlobalUpdateTCs;                 // +0x09
    bool BeginSerialising(bool saving, CCharString& data, CStringParser& parser, CPersistContext& context);
};

bool CTCBase::BeginSerialising(bool saving, CCharString& data, CStringParser& parser, CPersistContext& context)
{
    if (saving) {
        CCharString header = "Start" + GetClassTypeName() + ";\r\n";
        data += header;
        context.Init(data, &CGameDefinitionManager::Get());
        return true;
    } else {
        parser.Init(data);
        CCharString startTag = "Start" + GetClassTypeName() + ";";
        CCharString endTag = "End" + GetClassTypeName() + ";";
        if (parser.SkipToItem(startTag)) {
            CCharString body = parser.ReadAsStringUntilString(endTag);
            parser.Init(body);
            context.Init(&parser, 0, true, &CGameDefinitionManager::Get());
            return true;
        }
        return false;
    }
}
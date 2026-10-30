// CThing::SerialiseThingInfo(bool saving, CCharString& data, CSerialiseInfo& info) -- serialise a thing's identity record to or from a text block: saving points a persist context at the string, loading parses it; the shared context overload does the fields, then a load also picks up an optional DefinitionSubType item into DefinitionType
// Leaf: ?SerialiseThingInfo@CThing@@SIX_N
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C88E0, 330 bytes, static __fastcall (saving in cl, data in edx, info on the
//   stack), `ret 4`. Ego_r.pdb `?SerialiseThingInfo@CThing@@SIX_NAAVCCharString@@AAVCSerialiseInfo@1@@Z`.
// Callees: CBaseClass ctor 0x0099A2D0 / dtor 0x0099A2E0, CCharString() 0x0099E4B0, (const char*, long) 0x0099EBF0,
//   ~CCharString 0x0099EAE0, operator= 0x0099EFB0, malloc 0x00BFEA0E (the comment list head), CPersistContext::Init(
//   CCharString&, CDefinitionManager*) 0x009BAA10 (save) and Init(CStringParser*, callback, bool, CDefinitionManager*,
//   CSymbolMap*, CCharString*) 0x009BA9D0 (load), CStringParser::Init(const char*, callback, const CCharString&, long)
//   0x009B9530, the context overload SerialiseThingInfo(CPersistContext&, CSerialiseInfo&) 0x004C81F0, SkipToItem
//   0x009B9C00 (manifest "CThingCreatureBase::SerialiseGameState", propagated), ReadAsString 0x009BA6F0, the comment
//   list destructor 0x004046B0. String "DefinitionSubType" 0x01239B78.
// Layout: the retail 0x28-byte CStringParser and the 0x2C-byte CPersistContext of the landed CTCBase::SerialiseLevelState
//   0x00686E10 / BeginSerialising 0x00686B00 models; CThing::CSerialiseInfo (Ego_r.pdb) DefinitionType +8.
// Shape: BeginSerialising's two context set-ups inline (the parser's Init(const CCharString&, filename = "") keeps the
//   text alive in TempCharString), the parser's compiler-generated destructor (no vtable store), the sub-type item
//   name a temporary destroyed before the test.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CCharString {
public:
    struct SData { char* Data; };
    SData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    const char* c_str() const { return !PStringData ? "" : PStringData->Data; }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CStringParserCommentList {                      // retail list<CStringParserCommentBoundaries>: the head pointer only
    struct SNode { SNode* Next; SNode* Prev; char* PStart; char* PEnd; };
    SNode* m_pHead;
    CStringParserCommentList() : m_pHead(0)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        node->Next = node;
        node->Prev = node;
        m_pHead = node;
    }
    ~CStringParserCommentList();                       // 0x004046B0
};
struct CCursorPos {
    bool          InQuotes;                            // +0x0
    unsigned long StringPos;                           // +0x4
    unsigned long Line;                                // +0x8
};
typedef void (*TStringParserErrorCallback)(const char*);
class CStringParser : public CBaseClass {              // vtable 0x0122D874, retail 0x28 bytes
public:
    char*                      String;                 // +0x04
    unsigned long              BaseLineOffset;         // +0x08
    CCharString                Filename;               // +0x0C
    CCharString                TempCharString;         // +0x10
    CStringParserCommentList   Comments;               // +0x14
    TStringParserErrorCallback ErrorCallback;          // +0x18
    CCursorPos                 CursorPos;              // +0x1C
    CStringParser() { String = 0; }
    void Init(const char* text, TStringParserErrorCallback callback, const CCharString& filename, long baseLine);   // 0x009B9530
    void Init(const CCharString& text, const CCharString& filename = CCharString(""))
    {
        TempCharString = text;
        Init(text.c_str(), 0, filename, 0);
    }
    bool SkipToItem(const CCharString& name);          // 0x009B9C00
    CCharString ReadAsString();                        // 0x009BA6F0
};

class CSymbolMap;
class CDefinitionManager;
class CPersistContext { public:                               // Ego_r.pdb, 0x2C bytes, no destructor
    unsigned char _body[0x2c];
    void Init(CCharString& data, CDefinitionManager* defs);                                                       // 0x009BAA10
    void Init(CStringParser* parser, TStringParserErrorCallback callback, bool loading, CDefinitionManager* defs,
              CSymbolMap* symbols = 0, CCharString* name = 0);                                             // 0x009BA9D0
};

class CThing {
public:
    class CSerialiseInfo {                             // Ego_r.pdb, 0x10 bytes
    public:
        unsigned __int64 UID;                          // +0x00
        CCharString      DefinitionType;               // +0x08
        signed char      Player;                       // +0x0C
    };
    static void __fastcall SerialiseThingInfo(CPersistContext& context, CSerialiseInfo& info);    // 0x004C81F0
    static void __fastcall SerialiseThingInfo(bool saving, CCharString& data, CSerialiseInfo& info);
};

void __fastcall CThing::SerialiseThingInfo(bool saving, CCharString& data, CSerialiseInfo& info)
{
    CStringParser parser;
    CPersistContext context;
    if (saving)
        context.Init(data, 0);
    else {
        parser.Init(data);
        context.Init(&parser, 0, true, 0);
    }
    SerialiseThingInfo(context, info);
    if (!saving) {
        if (parser.SkipToItem("DefinitionSubType"))
            info.DefinitionType = parser.ReadAsString();
    }
}
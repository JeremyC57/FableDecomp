// CThing::GetSubTypeFromScript(const char* script) -- parse a thing's script block up to "EndThing", load its serialise record and return the global index of its definition type (0 for "NULL")
// Leaf: ?GetSubTypeFromScript@CThing@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C8A30, 368 bytes, static __fastcall (script in ecx), plain `ret`. Ego_r.pdb
//   `?GetSubTypeFromScript@CThing@@SIJPBD@Z`.
// Callees: CCharString(const char*, int) 0x0099EBF0, CCharString() 0x0099E4B0, ~CCharString 0x0099EAE0, CBaseClass ctor
//   0x0099A2D0 / dtor 0x0099A2E0, malloc 0x00BFEA0E, CStringParser::Init 0x009B9530, ReadAsStringUntilString 0x009BA330,
//   CThing::SerialiseThingInfo(bool, CCharString&, CSerialiseInfo&) 0x004C88E0 (this chunk), CBasicString<char>::
//   operator==(const char*) 0x004115A0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, the comment list
//   destructor 0x004046B0. Globals: g_pThingManager 0x013B8A1C (+0x20 the definition manager), "EndThing" 0x01239B8C,
//   "NULL" 0x012393E4, "" 0x0122D70E.
// Layout: the 0x004C88E0 parser; CThingManager DefinitionManager +0x20 (retail).
// Shape: the parser is built by an inline CStringParser(const char*, filename = "") constructor (the default-argument
//   temporary is constructed first and dies after the declaration; no String = 0 store); the inline CCharString::
//   operator== with the null-block arm `strcmp(text, "")` (the `repe cmpsb`); the definition manager is a named local
//   (the bare object expression would be loaded after the argument push); the "NULL" return block sits at the end.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;           // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, int length = -1);    // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

class CBaseClass {
public:
    CBaseClass();                                      // 0x0099A2D0
    virtual ~CBaseClass();                             // 0x0099A2E0
};
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
    CStringParser(const char* text, const CCharString& filename = CCharString(""))
    {
        Init(text, 0, filename, 0);
    }
    void Init(const char* text, TStringParserErrorCallback callback, const CCharString& filename, long baseLine);   // 0x009B9530
    CCharString ReadAsStringUntilString(const CCharString& end);  // 0x009BA330
};

class CDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CThingManager {
public:
    unsigned char       _pad00[0x20];
    CDefinitionManager* DefinitionManager;             // +0x20 (retail; PDB +0x2C)
};
extern CThingManager* g_pThingManager;                 // 0x013B8A1C

class CPersistContext;
class CThing {
public:
    class CSerialiseInfo {                             // Ego_r.pdb, 0x10 bytes
    public:
        unsigned __int64 UID;                          // +0x00
        CCharString      DefinitionType;               // +0x08
        signed char      Player;                       // +0x0C
    };
    static void __fastcall SerialiseThingInfo(bool saving, CCharString& data, CSerialiseInfo& info);   // 0x004C88E0
    static long __fastcall GetSubTypeFromScript(const char* script);
};

long __fastcall CThing::GetSubTypeFromScript(const char* script)
{
    CStringParser parser(script);
    CCharString body = parser.ReadAsStringUntilString("EndThing");
    CSerialiseInfo info;
    SerialiseThingInfo(false, body, info);
    if (info.DefinitionType == "NULL")
        return 0;
    const CDefinitionManager* defs = g_pThingManager->DefinitionManager;
    return defs->GetDefGlobalIndexFromName(info.DefinitionType);
}
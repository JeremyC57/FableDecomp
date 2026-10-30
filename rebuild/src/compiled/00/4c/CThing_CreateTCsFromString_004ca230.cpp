// CThing::CreateTCsFromString(const CCharString&) -- parse `CreateTC "<name>";` statements from a text block and add each named thing component (activated)
// Leaf: CreateTCsFromString
// Retail 0x004CA230, 349 bytes, ret 4. Label correction: the catalog's "CScriptBase::LoadGameState" is propagated; the
// only caller (0x004CAE62) is CThing code that passes the thing in ecx right after a CThing::AddTC call, and the body
// calls CThing::AddTC 0x004C9D60 on `this`; the method name is descriptive.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CStringParser ctor 0x00404720 and its
// implicit inline dtor (~list 0x004046B0, two ~CCharString, CBaseClass::~CBaseClass 0x0099A2E0), CStringParser::
// NextItemIs 0x009B9BA0 (catalog "SerialiseGameState" propagated), SkipPastNextItemIfIs 0x009B9E00, ReadAsString
// 0x009BA6F0 (by value). Literals: "" 0x0122D70E, "CreateTC" 0x01239BFC, "\"" 0x0122DF24, ";" 0x0123542C.
// Shape: the parser model of the landed CScriptBase::DoScriptDataPersist 0x00CB7CB0 (filename an unnamed "" temporary,
// `push eax`); every keyword is a NAMED CCharString inside small inline helpers (NextItemIsText / SkipPastText) --
// retail passes them with `lea ; push` and destroys each right after its call; the while condition is duplicated at
// the loop bottom (rotated loop).
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CStringParserCommentList {                  // retail std::list<CStringParserCommentBoundaries>: sentinel only
    void* m_pHead;
    ~CStringParserCommentList();                   // 0x004046B0
};
class CStringParser : public CBaseClass { public:                // retail 0x28 bytes
    char*                    String;               // +0x04
    unsigned long            BaseLineOffset;       // +0x08
    CCharString              Filename;             // +0x0c
    CCharString              TempCharString;       // +0x10
    CStringParserCommentList Comments;             // +0x14
    void (*ErrorCallback)(CCharString*);           // +0x18
    long                     CursorPos[3];         // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);            // 0x00404720
    bool NextItemIs(const CCharString& item);              // 0x009B9BA0
    bool SkipPastNextItemIfIs(const CCharString& item);    // 0x009B9E00
    CCharString ReadAsString();                            // 0x009BA6F0
    bool NextItemIsText(const char* text)
    {
        CCharString item(text, -1);
        return NextItemIs(item);
    }
    void SkipPastText(const char* text)
    {
        CCharString item(text, -1);
        SkipPastNextItemIfIs(item);
    }
};
extern const char g_emptyString_0122d70e[];       // 0x0122D70E ""
extern const char g_createTC[];                   // 0x01239BFC "CreateTC"
extern const char g_quote[];                      // 0x0122DF24 "\""
extern const char g_semicolon[];                  // 0x0123542C ";"
class CTCBase;
class CTCParamsBase;
class CThing { public:
    void CreateTCsFromString(const CCharString& text);
public:
    CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*);   // 0x004C9D60
};

void CThing::CreateTCsFromString(const CCharString& text)
{
    CStringParser parser(text, 0, CCharString(g_emptyString_0122d70e, -1), 0);
    while (parser.NextItemIsText(g_createTC)) {
        parser.SkipPastText(g_quote);
        CCharString name = parser.ReadAsString();
        parser.SkipPastText(g_quote);
        AddTC(name, true, 0);
        parser.SkipPastText(g_semicolon);
    }
}
// CScriptBase::LoadGameState(const CCharString&) -- restore the script's own data and its persisted entity scripts from the START_/END_ marked game-state text
// Leaf: ?LoadGameState@CScriptBase
// Retail 0x00CB8690, 400 bytes, ret 4 (the reader of the landed SaveGameState 0x00CB8820). Callees: CCharString(const
// char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CStringParser ctor 0x00404720 and its implicit inline dtor
// (~list 0x004046B0, two ~CCharString, CBaseClass::~CBaseClass 0x0099A2E0), CStringParser::NextItemIs 0x009B9BA0,
// ReadAsStringUntilString 0x009BA330 (by value), CScriptBase::DoScriptDataPersist 0x00CB7CB0 (loading),
// NScript::CRuntimeScriptedEntityPersist::LoadFromGameStateScript 0x00CB8440 on the member at +0x30.
// Literals: "" 0x0122D70E, START_SCRIPT_DATA 0x012C16C8, END_SCRIPT_DATA 0x012C16B8, START_ENTITY_SCRIPTS 0x012C16A0,
// END_ENTITY_SCRIPTS 0x012C168C.
// Shape: the parser model of DoScriptDataPersist; each marker is a NAMED CCharString inside an inline helper (lea/push,
// destroyed straight after its call); the section text is a by-value result of an inline ReadUntilText whose end
// marker is likewise a named local; the closing marker test's result is ignored.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
class CBaseClass { public:                                // the CStringParser root (polymorphic)
    virtual void v00();
    virtual ~CBaseClass();                                 // 0x0099A2E0
};
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
    CCharString ReadAsStringUntilString(const CCharString& end);   // 0x009BA330
    CCharString ReadUntilText(const char* text)
    {
        CCharString end(text, -1);
        return ReadAsStringUntilString(end);
    }
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
extern const char g_startScriptData[];            // 0x012C16C8 "START_SCRIPT_DATA"
extern const char g_endScriptData[];              // 0x012C16B8 "END_SCRIPT_DATA"
extern const char g_startEntityScripts[];         // 0x012C16A0 "START_ENTITY_SCRIPTS"
extern const char g_endEntityScripts[];           // 0x012C168C "END_ENTITY_SCRIPTS"
namespace NScript {
struct CRuntimeScriptedEntityPersist {
    unsigned char _body[0x14];
    void LoadFromGameStateScript(const CCharString& text);   // 0x00CB8440
};
}
class CScriptBase {
public:
    virtual void v00();
    void DoScriptDataPersist(CCharString& data, bool saving);   // 0x00CB7CB0
    void LoadGameState(const CCharString& state);
private:
    unsigned char _pad04[0x30 - 0x04];
    NScript::CRuntimeScriptedEntityPersist RuntimeScriptedEntityPersist;   // +0x30 (retail)
};

void CScriptBase::LoadGameState(const CCharString& state)
{
    CStringParser parser(state, 0, CCharString(g_emptyString_0122d70e, -1), 0);
    if (parser.NextItemIsText(g_startScriptData)) {
        CCharString data = parser.ReadUntilText(g_endScriptData);
        DoScriptDataPersist(data, false);
        parser.NextItemIsText(g_endScriptData);
    }
    if (parser.NextItemIsText(g_startEntityScripts)) {
        CCharString data = parser.ReadUntilText(g_endEntityScripts);
        RuntimeScriptedEntityPersist.LoadFromGameStateScript(data);
        parser.NextItemIsText(g_endEntityScripts);
    }
}
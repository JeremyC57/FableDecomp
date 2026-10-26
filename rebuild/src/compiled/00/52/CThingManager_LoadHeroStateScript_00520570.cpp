// CThingManager::LoadHeroStateScript(CStringParser& parser) -- skip to "NewThing", read the hero's thing script up to "END_THING_LEVEL" and load it as a single thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00520570, length 248 bytes (+ int3 padding), `ret 4`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0,
// CStringParser::ReadAsStringUntilString(const CCharString&) @ 0x009BA3E0 (result dropped) and @ 0x009BA330 (the
// inclusive form, result kept; both catalogued "ReadAsStringUntilString"), CStringParser::CStringParser(const
// CCharString&, callback, const CCharString&, long) @ 0x00404720, CStringParser::SkipWhitespace @ 0x009B9C60,
// CPersistContext text constructor @ 0x009BADD0, CThingManager::LoadSingleThing(CPersistContext&, long) @ 0x0051FD80,
// and the inlined ~CStringParser (~list @ 0x004046B0, two ~CCharString, CBaseClass::~CBaseClass @ 0x0099A2E0).
// Strings: 0x01245A88 "NewThing", 0x01245A78 "END_THING_LEVEL", 0x0122D70E "".
// Shape: the CStringParser / CPersistContext models of the landed CScriptBase::DoScriptDataPersist 0x00CB7CB0 (retail
// parser 0x28 bytes, list 4 bytes; the context has a trivial destructor). The engine header CStringParser.h is the
// flattened PDB layout (12-byte list, 0x30 bytes), so the parser is modelled locally. The second and third string
// temporaries are homed in the dead `parser` argument slot. The two terminators are implicit conversions (CCharString
// ctor with a defaulted length, re-addressed with `lea`), the "" file name an explicit temporary (pushed from eax).
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
class CBaseClass { public:                                // the CStringParser root (polymorphic)
    virtual void v00();
    ~CBaseClass();                                 // 0x0099A2E0
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
    CCharString ReadAsStringUntilString_009ba3e0(const CCharString& terminator);            // 0x009BA3E0
    CCharString ReadAsStringUntilStringInclusive(const CCharString& terminator);   // 0x009BA330
    long SkipWhitespace();                         // 0x009B9C60
};
class CDefinitionManager;
class CSymbolMap;
class CPersistContext { public:                           // retail 0x2c bytes, trivial destructor
    unsigned char _pad00[0x2c];
    CPersistContext(CStringParser* parser, void (*errorCallback)(CCharString*), bool reportErrors,
                    CDefinitionManager* definitions, CSymbolMap* symbols, CCharString* sourceText);   // 0x009BADD0
};
class CThing;
class CThingManager {
public:
    CThing* LoadSingleThing(CPersistContext& context, long mode);   // 0x0051FD80
    void LoadHeroStateScript(CStringParser& parser);
};

extern const char g_strNewThing[];                 // 0x01245A88 "NewThing"
extern const char g_strEndThingLevel[];            // 0x01245A78 "END_THING_LEVEL"
extern const char g_emptyString_0122d70e[];       // 0x0122D70E ("")

void CThingManager::LoadHeroStateScript(CStringParser& parser)
{
    parser.ReadAsStringUntilString_009ba3e0(g_strNewThing);
    CCharString heroState = parser.ReadAsStringUntilStringInclusive(g_strEndThingLevel);
    CStringParser heroParser(heroState, 0, CCharString(g_emptyString_0122d70e, -1), 0);
    heroParser.SkipWhitespace();
    CPersistContext context(&heroParser, 0, true, 0, 0, 0);
    LoadSingleThing(context, 1);
}
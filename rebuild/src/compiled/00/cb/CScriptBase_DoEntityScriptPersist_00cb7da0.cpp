// CScriptBase::DoEntityScriptPersist(CActiveEntityScriptBase& script, CCharString& data, bool saving) -- persist an active entity script's state to or from its text form through a CPersistContext
// Leaf: DoEntityScriptPersist
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB7DA0, length 175 bytes (`ret 0Ch`).
// PDB: ?DoEntityScriptPersist@CScriptBase@@IAEXAAVCActiveEntityScriptBase@@AAVCCharString@@_N@Z.
// Callees: as DoScriptDataPersist 0x00CB7CB0 (CPersistContext @ 0x009BAE10 / 0x009BADD0, CStringParser @ 0x00404720, the
// "" CCharString @ 0x0099EBF0 / 0x0099EAE0, ~list @ 0x004046B0, CBaseClass::~CBaseClass @ 0x0099A2E0); the entity script's
// virtual CEntityScriptBase::OnPersist(CPersistContext&) (slot 4, +0x10) through the active script's +0x34 pointer
// (landed SaveEntityScript 0x00CB83D0).
// Shape: DoScriptDataPersist with the persist call on the entity script (re-read after the constructors; `this` unused).
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
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
};
class CDefinitionManager;
class CSymbolMap;
class CPersistContext { public:                           // retail 0x2c bytes, trivial destructor
    unsigned char _pad00[0x2c];
    CPersistContext(CCharString& output);          // 0x009BAE10 (saving: writes into the string)
    CPersistContext(CStringParser* parser, void (*errorCallback)(CCharString*), bool reportErrors,
                    CDefinitionManager* definitions, CSymbolMap* symbols, CCharString* sourceText);   // 0x009BADD0
};
extern const char g_emptyString_0122d70e[];       // 0x0122D70E ("")
class CEntityScriptBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void OnPersist(CPersistContext& context);   // slot 4 (+0x10)
};
class CActiveEntityScriptBase {
public:
    unsigned char      _pad[0x34];
    CEntityScriptBase* Script;                     // +0x34
};
class CScriptBase {
public:
    virtual void v00();
    void DoEntityScriptPersist(CActiveEntityScriptBase& script, CCharString& data, bool saving);
};

void CScriptBase::DoEntityScriptPersist(CActiveEntityScriptBase& script, CCharString& data, bool saving)
{
    if (saving) {
        CPersistContext context(data);
        script.Script->OnPersist(context);
    }
    else {
        CStringParser parser(data, 0, CCharString(g_emptyString_0122d70e, -1), 0);
        CPersistContext context(&parser, 0, true, 0, 0, &data);
        script.Script->OnPersist(context);
    }
}
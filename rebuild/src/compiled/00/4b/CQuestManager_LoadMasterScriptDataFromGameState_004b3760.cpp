// CQuestManager::LoadMasterScriptDataFromGameState(CScriptDataBase& script_data, const CCharString& quest_name) -- when the runtime persistence holds a saved master-data script for the quest, parse it with a string parser and a loading persist context and let the script data persist itself from it
// Leaf: ?LoadMasterScriptDataFromGameState@CQuestManager@@AAEXAAVCScriptDataBase@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B3760, 173 bytes (176 with padding), `ret 8` (__fastcall).
// Identity: Ego_r.pdb `?LoadMasterScriptDataFromGameState@CQuestManager@@AAEXAAVCScriptDataBase@@ABVCCharString@@@Z`
//   (private); the body is the CScriptBase::DoScriptDataPersist 0x00CB7CB0 loading arm over a looked-up script text.
// Callees: NQuestManager::CRuntimeQuestPersistence::GetQuestMasterDataPersistScript(const CCharString&) const 0x004B27F0
//   (on RuntimeQuestPersistence +0xC; returns `const CCharString*` or 0), CCharString copy ctor 0x0099EC30,
//   CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E), CStringParser ctor 0x00404720, ~CCharString 0x0099EAE0,
//   CGameDefinitionManager::Get 0x0044C6B0 (static __fastcall), CPersistContext ctor 0x009BADD0, the script data's
//   slot 1 (OnPersist), the inlined ~CStringParser (~list 0x004046B0, two ~CCharString, ~CBaseClass 0x0099A2E0).
// Layout (retail): CQuestManager RuntimeQuestPersistence +0xC (Ego_r.pdb +0xC); CStringParser 0x28 bytes, CPersistContext
//   0x2C bytes with a trivial destructor (the landed CGameTimeManager::Load 0x006BBBA0 model).
// Shape: the persisted text is copied into a named local (homed in the dead quest-name argument slot); the parser is
//   built with an explicit `CCharString("", -1)` temporary that dies after its declaration; the context's argument list
//   is evaluated right to left (`&text`, 0, Get(), true, 0, `&parser`), so Get() runs after the two outer pushes.
// Label correction: catalogued as `CQuestManager_LoadActiveQuest`; the real routine is
//   CQuestManager::LoadMasterScriptDataFromGameState. Callee 0x004B27F0 (catalogued `LookupPersistentQuestInfo`) is
//   named GetQuestMasterDataPersistScript by its use here; its twin GetQuestScriptPersistScript has the same shape.
class CCharString {
public:
    void* m_pData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CStringParserCommentList {                    // retail std::list<CStringParserCommentBoundaries>: sentinel only
    void* m_pHead;
    ~CStringParserCommentList();                     // 0x004046B0
};
class CStringParser : public CBaseClass { public:                  // retail 0x28 bytes
    char*                    String;                 // +0x04
    unsigned long            BaseLineOffset;         // +0x08
    CCharString              Filename;               // +0x0c
    CCharString              TempCharString;         // +0x10
    CStringParserCommentList Comments;               // +0x14
    void (*ErrorCallback)(CCharString*);             // +0x18
    long                     CursorPos[3];           // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);              // 0x00404720
};
class CDefinitionManager { };
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
class CSymbolMap;
class CPersistContext { public:                             // retail 0x2c bytes, trivial destructor
    unsigned char _pad00[0x2c];
    CPersistContext(CStringParser* parser, void (*errorCallback)(CCharString*), bool reportErrors,
                    CDefinitionManager* definitions, CSymbolMap* symbols, CCharString* sourceText);   // 0x009BADD0
};
extern const char g_emptyString_0122d70e[];         // 0x0122D70E ("")

class CScriptDataBase {
public:
    virtual void v00();
    virtual void OnPersist(CPersistContext& context);   // slot 1 (+4)
};

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    unsigned char _body[0x20];                       // PersistScripts, MasterDataPersistScripts (CVectorMaps)
    const CCharString* GetQuestMasterDataPersistScript(const CCharString& quest_name) const;   // 0x004B27F0
};
}

class CQuestManager {
public:
    void*         __vftable;                         // +0x00
    unsigned char _pad04[0x0C - 0x04];               // PGameScriptThingManager
    NQuestManager::CRuntimeQuestPersistence RuntimeQuestPersistence;   // +0x0C
private:
    void LoadMasterScriptDataFromGameState(CScriptDataBase& script_data, const CCharString& quest_name);
};

void CQuestManager::LoadMasterScriptDataFromGameState(CScriptDataBase& script_data, const CCharString& quest_name)
{
    const CCharString* persisted = RuntimeQuestPersistence.GetQuestMasterDataPersistScript(quest_name);
    if (persisted) {
        CCharString text(*persisted);
        CStringParser parser(text, 0, CCharString(g_emptyString_0122d70e, -1), 0);
        CPersistContext context(&parser, 0, true, &CGameDefinitionManager::Get(), 0, &text);
        script_data.OnPersist(context);
    }
}
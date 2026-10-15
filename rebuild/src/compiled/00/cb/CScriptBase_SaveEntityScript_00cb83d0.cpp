// CScriptBase::SaveEntityScript(unsigned __int64 entityID, CActiveEntityScriptBase& script) -- persist an active entity script into a string and, when anything was written, record it as the entity's persisted script
// Leaf: SaveEntityScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB83D0, length 103 bytes (+9 int3 padding).
// PDB: ?SaveEntityScript@CScriptBase@@IAEX_KAAVCActiveEntityScriptBase@@@Z.
// Callees: CCharString::CCharString() @ 0x0099E4B0, CPersistContext::CPersistContext(CCharString&) @ 0x009BAE10,
// the entity script's virtual slot 4 (+0x10, persist into a CPersistContext&) through the active script's +0x34
// pointer, CCharString::operator!=(const char*) @ 0x0099E960 against "" (0x0122D70E),
// CRuntimeScriptedEntityPersist::SetEntityPersistScript(unsigned __int64, const CCharString&) @ 0x00CB82F0 on the
// member at +0x30, ~CCharString @ 0x0099EAE0.
// Shape: the landed CQuestManager::SaveMasterScriptDataToGameState form (0x004B5030): string at frame +0, the
// 0x2C-byte context above it with no destructor call.
struct CCharString {
    void* PStringData;
    CCharString();                                   // 0x0099E4B0
    ~CCharString();                                  // 0x0099EAE0
    bool operator!=(const char* text) const;         // 0x0099E960
};

struct CPersistContext {
    unsigned char _body[0x2c];
    CPersistContext(CCharString& output);            // 0x009BAE10
};

struct CEntityScriptBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void Persist(CPersistContext& context);  // slot 4 (+0x10)
};

class CActiveEntityScriptBase {
public:
    unsigned char      _pad[0x34];
    CEntityScriptBase* Script;                       // +0x34
};

namespace NScript {
struct CRuntimeScriptedEntityPersist {
    unsigned char _body[0x14];
    void SetEntityPersistScript(unsigned __int64 entityID, const CCharString& script);   // 0x00CB82F0
};
}

class CScriptBase {
public:
    virtual ~CScriptBase();
    void SaveEntityScript(unsigned __int64 entityID, CActiveEntityScriptBase& script);
private:
    unsigned char _body[0x30 - 4];
    NScript::CRuntimeScriptedEntityPersist RuntimeScriptedEntityPersist;   // +0x30 (retail)
};

void CScriptBase::SaveEntityScript(unsigned __int64 entityID, CActiveEntityScriptBase& script)
{
    CCharString text;
    CPersistContext context(text);
    script.Script->Persist(context);
    if (text != "")
        RuntimeScriptedEntityPersist.SetEntityPersistScript(entityID, text);
}
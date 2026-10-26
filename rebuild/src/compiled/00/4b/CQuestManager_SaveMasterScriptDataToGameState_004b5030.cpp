// CQuestManager::SaveMasterScriptDataToGameState(CScriptDataBase&, const CCharString&) -- persist the master script data into a string through a persist context and record it as the quest's master-data persist script
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B5030, length 77 bytes.
// Callees: CCharString::CCharString() @ 0x0099E4B0, CPersistContext::CPersistContext(CCharString&) @ 0x009BAE10,
// the script data's virtual slot 1 (+4, OnPersist(CPersistContext&)),
// NQuestManager::CRuntimeQuestPersistence::SetQuestMasterDataPersistScript(const CCharString&, const CCharString&)
// @ 0x004B4F30 on the member at +0xc, CCharString::~CCharString @ 0x0099EAE0.
// Identity: FableWin.pdb ?SaveMasterScriptDataToGameState@CQuestManager@@AAEXAAVCScriptDataBase@@ABVCCharString@@@Z.
// Shape: a 0x30-byte frame: the string at +0 and the 0x2c-byte context above it; the context has no destructor
// call (no user destructor in the shipped build), the string is destroyed last; no EH frame.
class CCharString { public:
    void* PStringData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
};

class CPersistContext { public:
    unsigned char _body[0x2c];
    CPersistContext(CCharString& output);          // 0x009BAE10
};

class CScriptDataBase { public:
    virtual void v00();
    virtual void OnPersist(CPersistContext& context);   // slot 1 (+0x04)
};

namespace NQuestManager {
struct CRuntimeQuestPersistence {
    unsigned char _body[0x20];
    void SetQuestMasterDataPersistScript(const CCharString& quest, const CCharString& script);   // 0x004B4F30
};
}

class CQuestManager { public:
    unsigned char                           _pad_0x00[0x0c];
    NQuestManager::CRuntimeQuestPersistence RuntimeQuestPersistence;   // +0x0c
    void SaveMasterScriptDataToGameState(CScriptDataBase& data, const CCharString& quest);
};

void CQuestManager::SaveMasterScriptDataToGameState(CScriptDataBase& data, const CCharString& quest)
{
    CCharString script;
    CPersistContext context(script);
    data.OnPersist(context);
    RuntimeQuestPersistence.SetQuestMasterDataPersistScript(quest, script);
}
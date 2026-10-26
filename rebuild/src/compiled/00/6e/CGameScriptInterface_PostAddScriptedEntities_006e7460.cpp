// CGameScriptInterface::PostAddScriptedEntities() const -- attaches the already-existing scripted entities to the quest script that is running now
// Leaf: PostAddScriptedEntities@CGameScriptInterface
// Retail: Fable.exe @ 0x006E7460, 22 bytes. Calls CQuestManager::AttachExistingScriptedEntitiesToScript(CScriptBase&) @ 0x004B26B0 on the
// quest manager singleton @ 0x013B89FC (named g_pQuestManager by landed code); `this` is unused.
// Layout: retail CQuestManager = Ego_r.pdb with 4-byte STLport lists, 12-byte vectors and 16-byte CVectorMaps:
// PQuestCurrentlyRunning +0x88 (PDB +0xe4). CQuestManager::CActiveQuest (Ego_r.pdb): PScript (CCountedPointer<CScriptBase>) +0x08.
class CScriptBase;
class CScriptInfo;
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    T& operator*() const { return *Data; }
};
class CQuestManager {
public:
    class CActiveQuest {
    public:
        long                         ID;                 // +0x00
        CScriptInfo*                 PScriptInfo;        // +0x04
        CCountedPointer<CScriptBase> PScript;            // +0x08
    };
    unsigned char _pad00[0x88];
    CActiveQuest* PQuestCurrentlyRunning;                // +0x88
    void AttachExistingScriptedEntitiesToScript(CScriptBase& script);   // 0x004B26B0
};
extern CQuestManager* g_pQuestManager;                   // 0x013B89FC
class CGameScriptInterface {
public:
    virtual void PostAddScriptedEntities() const;
};

void CGameScriptInterface::PostAddScriptedEntities() const
{
    g_pQuestManager->AttachExistingScriptedEntitiesToScript(*g_pQuestManager->PQuestCurrentlyRunning->PScript);
}
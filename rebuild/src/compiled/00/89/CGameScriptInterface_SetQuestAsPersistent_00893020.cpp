// CGameScriptInterface::SetQuestAsPersistent
class CCharString;

class CQuestManager
{ public:
    void SetQuestAsPersistent(
        const CCharString& questName,
        bool persistent);
};

extern CQuestManager* DAT_013b89fc;

void __fastcall CGameScriptInterface_SetQuestAsPersistent(
    void*,
    unsigned long,
    const CCharString& questName,
    bool persistent)
{
    DAT_013b89fc->SetQuestAsPersistent(questName, persistent);
}

// Leaf: ?AutoSaveQuestStart@CGameScriptInterface@@UBEXXZ
class CWorld
{
public:
    bool AutoSaveQuestStart();                  // 0x004A0CD0 (PDB returns bool; NF310)
};

class CGameScriptInterface
{
public:
        CWorld* m_pWorld;
    virtual void AutoSaveQuestStart() const;
};

void CGameScriptInterface::AutoSaveQuestStart() const
{
    m_pWorld->AutoSaveQuestStart();
}
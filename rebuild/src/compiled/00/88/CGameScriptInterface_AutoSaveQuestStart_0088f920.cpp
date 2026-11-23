class CWorld
{
public:
    bool AutoSaveQuestStart();                  // 0x004A0CD0 (PDB returns bool; NF310)
};

class CGameScriptInterface
{
public:
    void* m_pVtbl;
    CWorld* m_pWorld;
    void AutoSaveQuestStart() const;
};

void CGameScriptInterface::AutoSaveQuestStart() const
{
    m_pWorld->AutoSaveQuestStart();
}
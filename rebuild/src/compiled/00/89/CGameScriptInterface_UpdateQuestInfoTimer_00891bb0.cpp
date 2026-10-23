namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void UpdateTick_00643f79(long questId, bool enabled);
};
}

struct CGSIUpdateQuestInfoTick_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

class CGameScriptInterface
{
public:
    virtual void UpdateQuestInfoTick(
        long questId,
        bool enabled) const;
};

extern CGSIUpdateQuestInfoTick_GuiOwner*
    CGSIUpdateQuestInfoTick_Gui;

void CGameScriptInterface::UpdateQuestInfoTick(
    long questId,
    bool enabled) const
{
    CGSIUpdateQuestInfoTick_Gui->m_QuestInfo
        ->UpdateTick_00643f79(questId, enabled);
}
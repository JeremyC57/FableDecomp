#pragma optimize("s",on)
class CDisplayManager
{
public:
    bool CopyBackBufferToTexture(long conversationID);
};

class CGameSubSystem
{
public:
    char pad00[0x7C];
    CDisplayManager* m_DisplayManager; // 0x7C
};

class CGameScriptInterface
{
public:
    CGameSubSystem* m_Sub; // 0x04 (vptr occupies 0x00)
    virtual bool IsConversationActive(long conversationID) const;
};

bool CGameScriptInterface::IsConversationActive(long conversationID) const
{
    return m_Sub->m_DisplayManager->CopyBackBufferToTexture(conversationID);
}
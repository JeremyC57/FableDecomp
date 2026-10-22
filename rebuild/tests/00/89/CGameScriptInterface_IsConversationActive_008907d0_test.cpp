#include <cstdio>

class CDisplayManager
{
public:
    bool CopyBackBufferToTexture(long conversationID);
};

class CGameSubSystem
{
public:
    char pad00[0x7C];
    CDisplayManager* m_DisplayManager;
};

class CGameScriptInterface
{
public:
    CGameSubSystem* m_Sub;
    virtual bool IsConversationActive(long conversationID) const;
};

static bool g_called = false;
static CDisplayManager* g_expected = 0;
static long g_arg = 0;

bool CDisplayManager::CopyBackBufferToTexture(long conversationID)
{
    g_called = (this == g_expected);
    g_arg = conversationID;
    return true;
}

bool CGameScriptInterface::IsConversationActive(long conversationID) const
{
    return m_Sub->m_DisplayManager->CopyBackBufferToTexture(conversationID);
}

int main()
{
    CDisplayManager disp;
    CGameSubSystem holder;
    holder.m_DisplayManager = &disp;
    g_expected = &disp;

    CGameScriptInterface gsi;
    gsi.m_Sub = &holder;

    bool r = gsi.IsConversationActive(7);
    if (r && g_called && g_arg == 7)
    {
        std::printf("ISCONV_OK\n");
        return 0;
    }
    std::printf("ISCONV_FAIL\n");
    return 1;
}
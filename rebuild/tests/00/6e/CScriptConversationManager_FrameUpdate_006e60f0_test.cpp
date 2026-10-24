// Behaviour test for 0x006E60F0 (CScriptConversationManager::FrameUpdate: step past each conversation before updating it,
// so an update that erases its own conversation does not break the walk).
#include <cstdio>
#include <map>

struct Manager;
struct Conversation {
    Manager* Owner; long Index; int Updates; bool RemoveSelf;
    void FrameUpdate();
};
struct Manager {
    std::map<long, Conversation> Conversations;
    int Order[8]; int OrderCount;
    void FrameUpdate()
    {
        std::map<long, Conversation>::iterator it = Conversations.begin();
        while (it != Conversations.end()) {
            Conversation& conversation = it->second;
            ++it;
            conversation.FrameUpdate();
        }
    }
};
void Conversation::FrameUpdate()
{
    ++Updates;
    Owner->Order[Owner->OrderCount++] = (int)Index;
    if (RemoveSelf) Owner->Conversations.erase(Index);
}
#define FAIL { std::printf("006e60f0_TEST FAIL\n"); return 1; }
int main()
{
    Manager m; m.OrderCount = 0;
    m.FrameUpdate();
    if (m.OrderCount != 0) FAIL;
    long keys[4] = { 7, 2, 9, 4 };
    for (int i = 0; i < 4; ++i) {
        Conversation c; c.Owner = &m; c.Index = keys[i]; c.Updates = 0; c.RemoveSelf = keys[i] == 4 || keys[i] == 9;
        m.Conversations[keys[i]] = c;
    }
    m.FrameUpdate();
    if (m.OrderCount != 4 || m.Order[0] != 2 || m.Order[1] != 4 || m.Order[2] != 7 || m.Order[3] != 9) FAIL;
    if (m.Conversations.size() != 2 || m.Conversations[2].Updates != 1 || m.Conversations[7].Updates != 1) FAIL;
    m.OrderCount = 0;
    m.FrameUpdate();
    if (m.OrderCount != 2 || m.Order[0] != 2 || m.Order[1] != 7) FAIL;
    std::printf("006e60f0_TEST PASS\n");
    return 0;
}
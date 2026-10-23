// Behaviour test for CTCInventory::SetEditBoxValues @ 0x0058216E (fill each scoreboard edit box from the profile)
#include <cstdio>
#include <cstring>

struct Profile { const char* User; const char* Pass; const char* Proxy; int ProxyPort; int ServerPort; };
struct Box { const char* Name; char Text[32]; int Refreshes; };
static void SetValues(Box* boxes, int count, const Profile& p)
{
    for (int i = 0; i < count; ++i) {
        char text[32]; text[0] = 0;
        Box& b = boxes[i];
        if (std::strcmp(b.Name, "UI_SCOREBOARD_LOGIN_EDITBOX") == 0) std::strcpy(text, p.User);
        if (std::strcmp(b.Name, "UI_SCOREBOARD_PASSWORD_EDITBOX") == 0) std::strcpy(text, p.Pass);
        if (std::strcmp(b.Name, "UI_SCOREBOARD_PROXY_IP_EDITBOX") == 0) std::strcpy(text, p.Proxy);
        if (std::strcmp(b.Name, "UI_SCOREBOARD_PROXY_PORT_EDITBOX") == 0) std::sprintf(text, "%d", p.ProxyPort);
        if (std::strcmp(b.Name, "UI_SCOREBOARD_SERVER_PORT_EDITBOX") == 0) std::sprintf(text, "%d", p.ServerPort);
        std::strcpy(b.Text, text);
        ++b.Refreshes;
    }
}
int main()
{
    Box boxes[6] = { { "UI_SCOREBOARD_LOGIN_EDITBOX", "x", 0 }, { "UI_SCOREBOARD_PASSWORD_EDITBOX", "x", 0 },
        { "UI_SCOREBOARD_PROXY_IP_EDITBOX", "x", 0 }, { "UI_SCOREBOARD_PROXY_PORT_EDITBOX", "x", 0 },
        { "UI_SCOREBOARD_SERVER_PORT_EDITBOX", "x", 0 }, { "OTHER", "x", 0 } };
    Profile p = { "hero", "pw", "1.2.3.4", 3128, 27015 };
    SetValues(boxes, 6, p);
    if (std::strcmp(boxes[0].Text, "hero") != 0 || std::strcmp(boxes[2].Text, "1.2.3.4") != 0 || std::strcmp(boxes[3].Text, "3128") != 0 ||
        std::strcmp(boxes[4].Text, "27015") != 0 || boxes[5].Text[0] != 0 || boxes[5].Refreshes != 1) { std::printf("0058216e_TEST FAIL\n"); return 1; }
    std::printf("0058216e_TEST PASS\n");
    return 0;
}
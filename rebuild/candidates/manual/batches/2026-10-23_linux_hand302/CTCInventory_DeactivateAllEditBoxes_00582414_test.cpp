// Behaviour test for CTCInventory::DeactivateAllEditBoxes @ 0x00582414 (store edit-box texts into the profile, save, deactivate)
#include <cstdio>
#include <cstring>
#include <cstdlib>

struct Profile { char User[32]; char Pass[32]; char Proxy[32]; int ProxyPort; int ServerPort; int Saves; };
struct Box { const char* Name; const char* Text; bool Active; };
static void Deactivate(Box* boxes, int count, bool apply, Profile& p)
{
    for (int i = 0; i < count; ++i) {
        Box& b = boxes[i];
        if (apply) {
            if (std::strcmp(b.Name, "UI_SCOREBOARD_LOGIN_EDITBOX") == 0) std::strcpy(p.User, b.Text);
            if (std::strcmp(b.Name, "UI_SCOREBOARD_PASSWORD_EDITBOX") == 0) std::strcpy(p.Pass, b.Text);
            if (std::strcmp(b.Name, "UI_SCOREBOARD_PROXY_IP_EDITBOX") == 0) std::strcpy(p.Proxy, b.Text);
            if (std::strcmp(b.Name, "UI_SCOREBOARD_PROXY_PORT_EDITBOX") == 0) p.ProxyPort = std::atoi(b.Text);
            if (std::strcmp(b.Name, "UI_SCOREBOARD_SERVER_PORT_EDITBOX") == 0) p.ServerPort = std::atoi(b.Text);
            ++p.Saves;
        }
        b.Active = false;
    }
}
int main()
{
    Box boxes[5] = { { "UI_SCOREBOARD_LOGIN_EDITBOX", "hero", true }, { "UI_SCOREBOARD_PASSWORD_EDITBOX", "pw", true },
        { "UI_SCOREBOARD_PROXY_IP_EDITBOX", "10.0.0.1", true }, { "UI_SCOREBOARD_PROXY_PORT_EDITBOX", "8080", true },
        { "UI_SCOREBOARD_SERVER_PORT_EDITBOX", "1234", true } };
    Profile p; std::memset(&p, 0, sizeof(p));
    Deactivate(boxes, 5, false, p);
    if (p.Saves != 0 || p.User[0] != 0 || boxes[4].Active) { std::printf("00582414_TEST FAIL\n"); return 1; }
    boxes[0].Active = true;
    Deactivate(boxes, 5, true, p);
    if (p.Saves != 5 || std::strcmp(p.User, "hero") != 0 || std::strcmp(p.Proxy, "10.0.0.1") != 0 || p.ProxyPort != 8080 ||
        p.ServerPort != 1234 || boxes[0].Active) { std::printf("00582414_TEST FAIL\n"); return 1; }
    std::printf("00582414_TEST PASS\n");
    return 0;
}

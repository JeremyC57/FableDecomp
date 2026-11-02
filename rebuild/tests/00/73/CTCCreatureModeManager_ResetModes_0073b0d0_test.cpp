// Behaviour test for 0x0073b0d0 CTCCreatureModeManager::ResetModes(): RemoveAllModes (the front mode's OnDeactivate when the
// list is not empty, the list cleared, AddMode(0)), then SetupDefaultMode, then SetupInitialMode -- in that order.
#include <cstdio>
#include <cstring>

static char g_log[64];
static int g_n = 0;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }

class CModeBase {
public:
    CModeBase() : Active(true) { }
    virtual ~CModeBase() { }
    virtual void OnDeactivate() { Active = false; Log('d'); }
    bool Active;
};

struct SNode { SNode* Next; SNode* Prev; CModeBase* Mode; };

class CTCCreatureModeManager {
public:
    CTCCreatureModeManager() { Head.Next = &Head; Head.Prev = &Head; Head.Mode = 0; }
    bool Empty() const { return Head.Next == &Head; }
    void PushBack(SNode* node) { node->Next = &Head; node->Prev = Head.Prev; Head.Prev->Next = node; Head.Prev = node; }
    void Clear() { Head.Next = &Head; Head.Prev = &Head; Log('c'); }
    bool AddMode(unsigned int mode) { Log((char)('0' + mode)); return true; }
    void SetupDefaultMode() { Log('D'); }
    void SetupInitialMode() { Log('I'); }
    void RemoveAllModes()
    {
        if (!Empty())
            Head.Next->Mode->OnDeactivate();
        Clear();
        AddMode(0);
    }
    void ResetModes()
    {
        RemoveAllModes();
        SetupDefaultMode();
        SetupInitialMode();
    }
    SNode Head;
};

int main()
{
    CTCCreatureModeManager manager;
    CModeBase first, second;
    SNode a = { 0, 0, &first }, b = { 0, 0, &second };
    manager.PushBack(&a);
    manager.PushBack(&b);
    manager.ResetModes();
    if (std::strcmp(g_log, "dc0DI") != 0 || first.Active || !second.Active || !manager.Empty()) {
        std::printf("0073b0d0_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    g_n = 0; g_log[0] = 0;
    manager.ResetModes();                      // empty list: no OnDeactivate
    if (std::strcmp(g_log, "c0DI") != 0) {
        std::printf("0073b0d0_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    std::printf("0073b0d0_TEST PASS\n");
    return 0;
}
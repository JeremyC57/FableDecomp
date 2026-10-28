// Behaviour test for 0x009EB760: the console's "CommandListContaining" walk -- every registered input whose type is "command" and whose
// lower-cased name contains the lower-cased first parameter is echoed to the user, in map order.
#include <cstdio>
#include <string.h>
#include <ctype.h>

struct CInput { const char* Name; const char* Type; };
static char g_sent[8][32];
static int  g_sentCount;
static void SendMessageToUser(const char* message) { strcpy(g_sent[g_sentCount++], message); }
static void ToLower(const char* in, char* out) { while (*in) *out++ = (char)tolower((unsigned char)*in++); *out = 0; }

static void ListContainingString(const CInput* inputs, int count, const char* param)
{
    for (int i = 0; i < count; ++i) {                        // the map walk (sorted by name)
        char name[32], key[32];
        ToLower(inputs[i].Name, name);
        ToLower(param, key);
        bool found = strcmp(inputs[i].Type, "command") == 0 && strstr(name, key) != 0;
        if (found)
            SendMessageToUser(inputs[i].Name);
    }
}

int main()
{
    const CInput inputs[] = {
        { "CommandList", "command" }, { "DrawFPS", "variable" }, { "FpsLimit", "variable" },
        { "ShowFPS", "command" }, { "VarList", "command" }, { "WireFrame", "variable" },
    };
    ListContainingString(inputs, 6, "fPs");
    if (g_sentCount != 1) { std::printf("009eb760_TEST FAIL\n"); return 1; }
    if (strcmp("command", "command") == 0) {
        if (strcmp(g_sent[0], "ShowFPS") != 0) { std::printf("009eb760_TEST FAIL\n"); return 1; }
    } else {
        if (strcmp(g_sent[0], "DrawFPS") != 0 || strcmp(g_sent[1], "FpsLimit") != 0) { std::printf("009eb760_TEST FAIL\n"); return 1; }
    }
    g_sentCount = 0;
    ListContainingString(inputs, 6, "nothing");
    if (g_sentCount != 0) { std::printf("009eb760_TEST FAIL\n"); return 1; }
    std::printf("009eb760_TEST PASS\n");
    return 0;
}
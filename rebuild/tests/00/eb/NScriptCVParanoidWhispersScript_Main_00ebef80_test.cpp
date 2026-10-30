// Behaviour test for NScript::CV_ParanoidWhispersScript::Main (0x00EBEF80): bind the whispering man, his friend and the
// necklace digging spot, post-add the scripted entities (script, then game), then spawn CheckForNecklaceAndQuit and
// WatchForAngryTraders, in that order.
#include <cstdio>
#include <cstring>

static char gLog[512];
static void Log(const char* s) { std::strcat(gLog, s); std::strcat(gLog, ";"); }

struct Game { void PostAddScriptedEntities() { Log("GamePost"); } };

struct CV_ParanoidWhispersScript {
    Game* Game_;
    void AddEntityScriptBinding(const char* name) { Log(name); }
    void PostAddScriptedEntities() { Log("Post"); }
    void SpawnFunction(const char* name) { Log(name); }
    void Main()
    {
        AddEntityScriptBinding("SecretWhisperingMan");
        AddEntityScriptBinding("SecretWhisperingFriend");
        AddEntityScriptBinding("PW_NecklaceDiggingSpot");
        PostAddScriptedEntities();
        Game_->PostAddScriptedEntities();
        SpawnFunction("CheckForNecklaceAndQuit");
        SpawnFunction("WatchForAngryTraders");
    }
};

int main()
{
    CV_ParanoidWhispersScript s; Game g; s.Game_ = &g; gLog[0] = 0;
    s.Main();
    if (std::strcmp(gLog, "SecretWhisperingMan;SecretWhisperingFriend;PW_NecklaceDiggingSpot;Post;GamePost;CheckForNecklaceAndQuit;WatchForAngryTraders;") != 0) {
        std::printf("00ebef80_TEST FAIL\n"); return 1;
    }
    std::printf("00ebef80_TEST PASS\n");
    return 0;
}
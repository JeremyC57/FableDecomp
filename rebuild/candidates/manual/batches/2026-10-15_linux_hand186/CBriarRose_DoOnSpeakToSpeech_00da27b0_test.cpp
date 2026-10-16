#include <cstdio>
#include <cstring>
struct Rose {
    long timer; bool done; char said[64]; int calls;
    void DoSpeech(const char* s) { std::strcpy(said, s); ++calls; }
    void DoOnSpeakToSpeech()
    {
        char line[64]; std::strcpy(line, "TEXT_QST_B08_BRIAR_ONSPEAKTO");
        if (timer > 48) { if (done) return; std::strcat(line, "_START"); }
        else if (timer > 24) { if (done) return; std::strcat(line, "_MIDDLE"); }
        else { if (done) return; std::strcat(line, "_END"); }
        DoSpeech(line);
    }
};
int main()
{
    Rose r; r.calls = 0; r.done = false;
    r.timer = 49; r.DoOnSpeakToSpeech(); bool a = std::strcmp(r.said, "TEXT_QST_B08_BRIAR_ONSPEAKTO_START") == 0;
    r.timer = 48; r.DoOnSpeakToSpeech(); bool b = std::strcmp(r.said, "TEXT_QST_B08_BRIAR_ONSPEAKTO_MIDDLE") == 0;
    r.timer = 24; r.DoOnSpeakToSpeech(); bool c = std::strcmp(r.said, "TEXT_QST_B08_BRIAR_ONSPEAKTO_END") == 0;
    r.done = true; r.DoOnSpeakToSpeech();
    if (!a || !b || !c || r.calls != 3) { std::printf("00da27b0_TEST FAIL\n"); return 1; }
    std::printf("00da27b0_TEST PASS\n");
    return 0;
}

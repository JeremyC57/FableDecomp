// Behaviour test for 008a9ae0 CGameScriptInterface::MsgOnBoastsMade: copy (id, text) of each matching message; true when any.
#include <cstdio>
#include <cstring>
struct Payload { long id; const char* text; };
struct Message { long type; long frame; Payload* payload; };
struct Pair { long first; const char* second; };
static bool MsgOnBoastsMade(const Message* all, int n, long frame, long maxFrame, Pair* out, int& count)
{
    const Message* found[8]; int nf = 0;
    for (int i = 0; i < n; ++i) if (all[i].type == 0x29 && all[i].frame >= frame && all[i].frame <= maxFrame) found[nf++] = &all[i];
    for (int i = 0; i < nf; ++i) { Pair p; p.first = found[i]->payload->id; p.second = found[i]->payload->text; out[count++] = p; }
    if (nf != 0) return true;
    return false;
}
int main()
{
    Payload a = { 7, "brave" }, b = { 9, "strong" };
    Message m[3] = { { 0x29, 5, &a }, { 0x28, 5, &b }, { 0x29, 6, &b } };
    Pair out[8]; int count = 0;
    bool r = MsgOnBoastsMade(m, 3, 5, 6, out, count);
    int c2 = 0; bool r2 = MsgOnBoastsMade(m, 3, 10, 11, out, c2);
    if (!r || count != 2 || out[0].first != 7 || std::strcmp(out[1].second, "strong") || r2 || c2) { std::printf("008a9ae0_TEST FAIL\n"); return 1; }
    std::printf("008a9ae0_TEST PASS\n");
    return 0;
}
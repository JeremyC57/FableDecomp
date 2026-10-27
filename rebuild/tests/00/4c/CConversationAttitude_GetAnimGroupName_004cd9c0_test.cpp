// Behaviour test for 004cd9c0 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
class CCharString { public: const char* Text; };
enum EConversationAttitude { A0, A1, A2 };
static CCharString g_Names[3];
static CCharString* g_Begin = g_Names;
const CCharString& GetAnimGroupName(EConversationAttitude a) { return g_Begin[a]; }
int main() {
    g_Names[0].Text = "neutral"; g_Names[1].Text = "happy"; g_Names[2].Text = "angry";
    if (&GetAnimGroupName(A2) != &g_Names[2] || GetAnimGroupName(A1).Text[0] != 'h') { std::printf("004cd9c0_TEST FAIL\n"); return 1; }
    { std::printf("004cd9c0_TEST PASS\n"); return 0; }
}
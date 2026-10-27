// Behaviour test for 0x00C186D0 CMorphableNode_00c185b0::Slot00 (log on a null implementation, then forward to slot 0)
#include <cstdio>
#include <cstring>

static char g_log[256];
static int g_breaks = 0;
static int g_forwarded = 0;
struct Node { virtual void Slot00() { ++g_forwarded; } virtual ~Node() { } };
struct Morphable {
    Node* m_pNode;
    void Slot00()
    {
        if (m_pNode == 0) {
            std::sprintf(g_log, "%s(%d) : %s\n", ".\\PKCTRMorphable.cpp", 26, "Error: NULL implimentation");
            ++g_breaks;
            return;                        // retail breaks into the debugger here; the test stops instead of calling through null
        }
        m_pNode->Slot00();
    }
};

int main()
{
    Node n;
    Morphable m; m.m_pNode = &n;
    m.Slot00();
    if (g_forwarded != 1 || g_breaks != 0 || g_log[0] != 0) { std::printf("00c186d0_TEST FAIL\n"); return 1; }
    m.m_pNode = 0;
    m.Slot00();
    if (g_breaks != 1 || std::strcmp(g_log, ".\\PKCTRMorphable.cpp(26) : Error: NULL implimentation\n") != 0) { std::printf("00c186d0_TEST FAIL\n"); return 1; }
    std::printf("00c186d0_TEST PASS\n");
    return 0;
}
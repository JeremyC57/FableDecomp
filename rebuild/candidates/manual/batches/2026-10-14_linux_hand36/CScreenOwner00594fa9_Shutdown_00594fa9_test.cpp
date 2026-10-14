#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_kb = 0, g_back = 0; static long g_backFlags = -1;
struct CVirtualKeyboard { ~CVirtualKeyboard() { ++g_kb; } };
struct CScreenOwner_00594fa9 { unsigned char _pad[0x60]; CVirtualKeyboard* m_pKeyboard; void GotoPreviousScreen(long f) { ++g_back; g_backFlags = f; }
    void Shutdown(bool goBack) { if (m_pKeyboard) { delete m_pKeyboard; m_pKeyboard = 0; } if (goBack) GotoPreviousScreen(0); } };
int main() {
    CScreenOwner_00594fa9* o = (CScreenOwner_00594fa9*)new char[sizeof(CScreenOwner_00594fa9)]; o->m_pKeyboard = new CVirtualKeyboard; o->Shutdown(false); if (g_kb != 1 || o->m_pKeyboard || g_back) { std::printf("00594fa9_TEST FAIL\n"); return 1; }
    o->Shutdown(true); if (g_kb != 1 || g_back != 1 || g_backFlags != 0) { std::printf("00594fa9_TEST FAIL\n"); return 1; }
    delete[] (char*)o;
    std::printf("00594fa9_TEST PASS\n"); return 0;
}

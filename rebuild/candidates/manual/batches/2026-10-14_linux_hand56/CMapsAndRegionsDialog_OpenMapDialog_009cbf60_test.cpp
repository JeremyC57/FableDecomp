#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct IDialog { virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void Open(long m, long r, long f) { char b[32]; std::sprintf(b, "O%ld,%ld,%ld", m, r, f); std::strcat(g_seq, b); } virtual void v004() { } virtual void Select(long m) { char b[16]; std::sprintf(b, "S%ld", m); std::strcat(g_seq, b); } };
struct CDlg { unsigned char _pad[0x7c]; IDialog* m_pDialog; void OpenMapDialog(long map, long region, long flags) { m_pDialog->Select(map); m_pDialog->Open(region, flags, 0); } };

int main() {
    CDlg d; IDialog i; d.m_pDialog = &i; d.OpenMapDialog(1, 2, 3); if (std::strcmp(g_seq, "S1O2,3,0")) { std::printf("009cbf60_TEST FAIL\n"); return 1; }
    std::printf("009cbf60_TEST PASS\n"); return 0;
}

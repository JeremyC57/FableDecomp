#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static long g_flags = 0; struct CBase { virtual void v00() { } ~CBase() { std::strcat(g_seq, "B"); } }; class CVolume : public CBase { public: void ReleaseVolume(long f) { std::strcat(g_seq, "R"); g_flags = f; } virtual ~CVolume() { ReleaseVolume(1); } };

int main() {
    CVolume* p = new CVolume; delete p; if (std::strcmp(g_seq, "RB") || g_flags != 1) { std::printf("009fcd10_TEST FAIL\n"); return 1; }
    std::printf("009fcd10_TEST PASS\n"); return 0;
}
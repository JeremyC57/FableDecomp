#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CBaseGroup { virtual void v00() { } unsigned char _body[0xf4]; CBaseGroup() { std::strcat(g_seq, "B"); } }; struct SCoverRecord { unsigned char _body[0x40]; };
struct CDerived : CBaseGroup { unsigned char _padf8[0x24]; SCoverRecord* m_pRecordA; long m_120; SCoverRecord* m_pRecordB; SCoverRecord m_recordA, m_recordB; virtual void v00() { } CDerived() : CBaseGroup(), m_pRecordA(&m_recordA), m_pRecordB(&m_recordB) { } };

int main() {
    CDerived d; if (std::strcmp(g_seq, "B") || d.m_pRecordA != &d.m_recordA || d.m_pRecordB != &d.m_recordB) { std::printf("00c44580_TEST FAIL\n"); return 1; }
    std::printf("00c44580_TEST PASS\n"); return 0;
}
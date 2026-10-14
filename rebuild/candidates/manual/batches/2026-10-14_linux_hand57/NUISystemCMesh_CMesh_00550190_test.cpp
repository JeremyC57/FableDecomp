#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_base = 0; static void* g_def = 0; static long g_data = 0; struct IP { virtual void v00() { } }; struct IA { virtual void a00() { } unsigned char _pad[0x10]; }; struct IB { virtual void b00() { } };
static long g_cbase = 0; struct CDraggableInto : IP, IA, IB { unsigned char _pad[0x164 - 0x1c]; CDraggableInto(void* d) { ++g_base; g_def = d; } }; struct CBase { virtual void v00() { } CBase() { ++g_cbase; } };
struct SBody { long m_4; SBody() : m_4(0) { } }; struct CRecord : CBase { virtual void v00() { } SBody m_body; CRecord() : CBase(), m_body() { } };
struct CMesh : CDraggableInto { virtual void v00() { } virtual void a00() { } virtual void b00() { } CRecord m_record; unsigned char _p[0x34]; bool m_1a0; void GetDataFromDef() { ++g_data; } CMesh(void* d) : CDraggableInto(d), m_record(), m_1a0(false) { GetDataFromDef(); } };

int main() {
    int def; CMesh* m = new CMesh(&def); if (g_base != 1 || g_def != &def || g_cbase != 1 || m->m_record.m_body.m_4 || m->m_1a0 || g_data != 1) { std::printf("00550190_TEST FAIL\n"); return 1; } delete m;
    std::printf("00550190_TEST PASS\n"); return 0;
}

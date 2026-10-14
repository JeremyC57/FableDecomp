#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[16] = ""; static const char* g_name = 0; static void* g_map = 0; static void* g_stream = 0;
static const char g_emptyString_0122d70e[] = "";
struct CInitBaseClass { unsigned char _body[8]; ~CInitBaseClass() { std::strcat(g_seq, "b"); } };
struct CTransferScopeMember_004dd470 { unsigned char _body[12]; ~CTransferScopeMember_004dd470() { std::strcat(g_seq, "m"); } };
struct CTransferScope_004e0a05 : CInitBaseClass { CTransferScopeMember_004dd470 m_member; CTransferScope_004e0a05() { std::strcat(g_seq, "c"); } ~CTransferScope_004e0a05() { } };
struct CPersistContext_004e59e1 { unsigned char _pad[8]; unsigned char m_stream[4]; };
struct CDefMap_004e59e1 { int x; };
static void SerialiseMapBinaryIn_004e5a76(CDefMap_004e59e1* m, void* s) { std::strcat(g_seq, "A"); g_map = m; g_stream = s; }
static void SerialiseAllocatedAreas_004e5a39(CDefMap_004e59e1* m, void* s) { std::strcat(g_seq, "B"); g_map = m; g_stream = s; }
struct CFlammableDef { unsigned char _pad[0x18]; long m_kind; unsigned char _pad1c[8]; CDefMap_004e59e1* m_pMapA; CDefMap_004e59e1* m_pMapB; void SetHelperName(const char* n) { g_name = n; std::strcat(g_seq, "N"); }
    void TransferHelper(CPersistContext_004e59e1* ctx) { SetHelperName(g_emptyString_0122d70e); CTransferScope_004e0a05 scope; switch (m_kind) { case 2: SerialiseMapBinaryIn_004e5a76(m_pMapA, ctx->m_stream); break; case 3: SerialiseAllocatedAreas_004e5a39(m_pMapB, ctx->m_stream); break; } } };
int main() {
    CFlammableDef d; std::memset(&d, 0, sizeof d); CPersistContext_004e59e1 ctx; CDefMap_004e59e1 ma, mb; d.m_pMapA = &ma; d.m_pMapB = &mb;
    d.m_kind = 2; d.TransferHelper(&ctx); if (std::strcmp(g_seq, "NcAmb") || g_map != &ma || g_stream != ctx.m_stream || std::strcmp(g_name, "")) { std::printf("004e59e1_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; d.m_kind = 3; d.TransferHelper(&ctx); if (std::strcmp(g_seq, "NcBmb") || g_map != &mb) { std::printf("004e59e1_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; d.m_kind = 1; d.TransferHelper(&ctx); if (std::strcmp(g_seq, "Ncmb")) { std::printf("004e59e1_TEST FAIL\n"); return 1; }
    std::printf("004e59e1_TEST PASS\n"); return 0;
}
// Behaviour test for 004b64e0: body copied from src/004b64e0.cpp (CQuestManager::SaveGameState(CPersistContext&)), string/stream/context callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CCharStringData { public:
    const char*   m_pData;
    long          m_length;
};
char g_EmptyCharString_0129AAF4[1] = { 0 };
static CCharStringData g_stateData = { "quest-state", 11 };
static int g_ctorCalls, g_dtorCalls, g_buildCalls; static bool g_buildNull;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString() : PStringData(0) { ++g_ctorCalls; }
    ~CCharString() { ++g_dtorCalls; }
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->m_length; }
    const char* GetPString() const { return PStringData == 0 ? g_EmptyCharString_0129AAF4 : PStringData->m_pData; }
};
static int g_seq; static int g_ulongAt, g_bufferAt, g_transferAt, g_boastsAt; static unsigned long g_ulong; static const void* g_bufData; static long g_bufSize;
class CDataOutputStream { public:
    void WriteULong(unsigned long value) { g_ulongAt = ++g_seq; g_ulong = value; }
    void WriteBuffer(const void* data, long size) { g_bufferAt = ++g_seq; g_bufData = data; g_bufSize = size; }
};
static CCharString* g_transferred;
class CPersistContext { public:
    unsigned char      _pad00[0x18];
    long               Mode;
    unsigned char      _pad1c[0xc];
    CDataOutputStream* Stream;
    void Transfer_009bab50(CCharString& value) { g_transferAt = ++g_seq; g_transferred = &value; }
};
static CPersistContext* g_boastsContext;
class CQuestManager { public:
    void SaveGameState(CCharString& state) { ++g_buildCalls; state.PStringData = g_buildNull ? 0 : &g_stateData; }
    void TransferBoasts(CPersistContext& context) { g_boastsAt = ++g_seq; g_boastsContext = &context; }
    void SaveGameState(CPersistContext& context);
};
void CQuestManager::SaveGameState(CPersistContext& context)
{
    CCharString state;
    SaveGameState(state);
    bool writing = context.Mode == 2 || context.Mode == 3;
    if (writing) {
        context.Stream->WriteULong(state.GetLength());
        context.Stream->WriteBuffer(state.GetPString(), state.GetLength());
    } else
        context.Transfer_009bab50(state);
    TransferBoasts(context);
}
static void reset() { g_seq = 0; g_ulongAt = g_bufferAt = g_transferAt = g_boastsAt = 0; g_ulong = 99; g_bufData = 0; g_bufSize = -1; g_transferred = 0; g_ctorCalls = g_dtorCalls = g_buildCalls = 0; }
int main()
{
    CQuestManager qm; CDataOutputStream stream; CPersistContext ctx; memset(&ctx, 0, sizeof(ctx)); ctx.Stream = &stream;
    // 1. mode 2 (writing): length + bytes written, then boasts; the string is built and destroyed
    reset(); g_buildNull = false; ctx.Mode = 2; qm.SaveGameState(ctx);
    CHECK(g_buildCalls == 1); CHECK(g_ulongAt == 1); CHECK(g_ulong == 11); CHECK(g_bufferAt == 2); CHECK(g_bufData == g_stateData.m_pData); CHECK(g_bufSize == 11);
    CHECK(g_transferAt == 0); CHECK(g_boastsAt == 3); CHECK(g_boastsContext == &ctx); CHECK(g_ctorCalls == 1); CHECK(g_dtorCalls == 1);
    // 2. mode 3 (writing) with an empty state string: 0 + "" written
    reset(); g_buildNull = true; ctx.Mode = 3; qm.SaveGameState(ctx);
    CHECK(g_ulongAt == 1); CHECK(g_ulong == 0); CHECK(g_bufferAt == 2); CHECK(g_bufData == g_EmptyCharString_0129AAF4); CHECK(g_bufSize == 0); CHECK(g_boastsAt == 3);
    // 3. other modes: transferred through the context, nothing written
    reset(); g_buildNull = false; ctx.Mode = 1; qm.SaveGameState(ctx);
    CHECK(g_ulongAt == 0); CHECK(g_bufferAt == 0); CHECK(g_transferAt == 1); CHECK(g_transferred != 0); CHECK(g_boastsAt == 2); CHECK(g_dtorCalls == 1);
    reset(); ctx.Mode = 0; qm.SaveGameState(ctx); CHECK(g_transferAt == 1); CHECK(g_boastsAt == 2);
    reset(); ctx.Mode = 4; qm.SaveGameState(ctx); CHECK(g_transferAt == 1); CHECK(g_ulongAt == 0);
    if (fails == 0) printf("004b64e0_TEST PASS\n");
    else printf("004b64e0_TEST FAIL (%d)\n", fails);
    return fails;
}
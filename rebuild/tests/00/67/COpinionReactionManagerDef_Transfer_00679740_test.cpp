#include <cstdio>
// Behaviour test for COpinionReactionManagerDef::Transfer (0x00679740): every member goes through the CRC check and the
// mode switch (2 = load from +0x24 stream, 3 = save to +0x28 stream) in declaration order.
struct Log { int ids[64]; int n; Log() : n(0) {} void Put(int v) { ids[n++] = v; } };
static Log g_log;
struct CMemoryDataInputStream { int tag; };
struct CDataOutputStream { int tag; };
struct Map { int id; };
static void MapOut(CDataOutputStream* s, Map* m) { g_log.Put(1000 + s->tag + m->id); }
static void MapIn(CMemoryDataInputStream* s, Map* m) { g_log.Put(2000 + s->tag + m->id); }
struct CPersistContext {
    long m_mode;
    CMemoryDataInputStream* m_pLoadStream;
    CDataOutputStream* m_pSaveStream;
    int crcs;
    void CheckCRC(const char* name) { if (name[0] == 0) ++crcs; }
    void TransferVectorOfSubComponents(Map& v) { g_log.Put(3000 + v.id); }
    void TransferMatches(Map& v) { g_log.Put(4000 + v.id); }
    void TransferComponent(Map& v) { g_log.Put(5000 + v.id); }
    void Transfer(const char* name, Map& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2: MapIn(m_pLoadStream, &value); break;
        case 3: MapOut(m_pSaveStream, &value); break;
        }
    }
};
struct COpinionReactionManagerDef {
    Map m[11];
    void Transfer(CPersistContext& context)
    {
        context.TransferVectorOfSubComponents(m[0]);
        for (int i = 1; i <= 8; ++i) context.Transfer("", m[i]);
        context.TransferMatches(m[9]);
        context.TransferComponent(m[10]);
    }
};
int main()
{
    COpinionReactionManagerDef d;
    for (int i = 0; i < 11; ++i) d.m[i].id = i;
    CMemoryDataInputStream in = { 100 };
    CDataOutputStream out = { 200 };
    int bad = 0;
    for (int mode = 1; mode <= 3; ++mode) {
        CPersistContext c; c.m_mode = mode; c.m_pLoadStream = &in; c.m_pSaveStream = &out; c.crcs = 0;
        g_log.n = 0;
        d.Transfer(c);
        bad |= c.crcs != 8;
        int expectN = mode == 1 ? 3 : 11;
        bad |= g_log.n != expectN;
        bad |= g_log.ids[0] != 3000 || g_log.ids[expectN - 1] != 5010 || g_log.ids[expectN - 2] != 4009;
        if (mode == 2) bad |= g_log.ids[1] != 2101 || g_log.ids[8] != 2108;
        if (mode == 3) bad |= g_log.ids[1] != 1201 || g_log.ids[8] != 1208;
    }
    if (bad) { std::printf("00679740_TEST FAIL\n"); return 1; }
    std::printf("00679740_TEST PASS\n");
    return 0;
}
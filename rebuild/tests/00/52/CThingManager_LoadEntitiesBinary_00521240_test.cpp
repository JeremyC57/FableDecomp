// Behaviour test for CThingManager::LoadEntitiesBinary (0x00521240): a section-count prefixed stream of named,
// size-prefixed sections; wanted sections (by name or because the load-all name is wanted) are read as size-prefixed
// entity blocks until a zero size, each loaded thing appended; the process is suspended every tick interval and once at
// the end; then the things are activated.
#include <cstdio>
#include <cstring>

struct CStream {
    const unsigned char* P;
    long ReadSLONG() { int v; std::memcpy(&v, P, 4); P += 4; return v; }
    const char* ReadString() { const char* s = (const char*)P; P += std::strlen(s) + 1; return s; }
    const unsigned char* ReadConstData(long n) { const unsigned char* d = P; P += n; return d; }
};
struct CProcess { int Suspends; CProcess() : Suspends(0) { } void SuspendSuspendableProcess() { ++Suspends; } };
class CThingManager { public:
    long Loaded[16]; int LoadedCount; long Activated; char CurrentSectionName[16];
    CThingManager() : LoadedCount(0), Activated(-1) { CurrentSectionName[0] = 0; }
    void LoadEntitiesBinary(CStream& stream, const char** wanted, int wantedCount, CProcess* process, unsigned long interval, long level)
    {
        bool hasProcess = process != 0; unsigned long ticks = 0; CurrentSectionName[0] = 0;
        unsigned long sections = (unsigned long)stream.ReadSLONG();
        for (unsigned long i = 0; i < sections; ++i) {
            const char* name = stream.ReadString();
            long size = stream.ReadSLONG();
            CStream section; section.P = stream.ReadConstData(size);
            bool want = false;
            for (int w = 0; w < wantedCount; ++w)
                if (!std::strcmp(wanted[w], name) || !std::strcmp(wanted[w], "XXX_ALL")) want = true;
            if (!want) continue;
            std::strcpy(CurrentSectionName, name);
            for (;;) {
                long entitySize = section.ReadSLONG();
                if (!entitySize) break;
                CStream entity; entity.P = section.ReadConstData(entitySize);
                Loaded[LoadedCount++] = entity.ReadSLONG();
                if (++ticks >= interval) { ticks = 0; if (hasProcess) process->SuspendSuspendableProcess(); }
            }
        }
        if (process) process->SuspendSuspendableProcess();
        Activated = level;
    }
};
static unsigned char g_buf[256]; static int g_len;
static void PutL(long v) { int i = (int)v; std::memcpy(g_buf + g_len, &i, 4); g_len += 4; }
static void PutS(const char* s) { std::strcpy((char*)g_buf + g_len, s); g_len += (int)std::strlen(s) + 1; }
static void Section(const char* name, const long* ids, int n) { PutS(name); PutL(n * 8 + 4); for (int i = 0; i < n; ++i) { PutL(4); PutL(ids[i]); } PutL(0); }

int main()
{
    long a[3] = { 11, 12, 13 }, b[2] = { 21, 22 };
    g_len = 0; PutL(2); Section("SECT_A", a, 3); Section("SECT_B", b, 2);
    const char* wantB[1] = { "SECT_B" };
    CThingManager m; CProcess p; CStream s; s.P = g_buf;
    m.LoadEntitiesBinary(s, wantB, 1, &p, 1, 5);
    bool ok = m.LoadedCount == 2 && m.Loaded[0] == 21 && m.Loaded[1] == 22 && p.Suspends == 3 && m.Activated == 5
        && !std::strcmp(m.CurrentSectionName, "SECT_B");
    const char* wantAll[1] = { "XXX_ALL" };
    CThingManager all; CStream s2; s2.P = g_buf;
    all.LoadEntitiesBinary(s2, wantAll, 1, 0, 2, 7);
    ok = ok && all.LoadedCount == 5 && all.Loaded[2] == 13 && all.Loaded[4] == 22 && all.Activated == 7;
    if (!ok) { std::printf("00521240_TEST FAIL\n"); return 1; }
    std::printf("00521240_TEST PASS\n");
    return 0;
}
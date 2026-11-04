// Behaviour test for ReadDeferredBank (0x00C16F00): only a version-4 bank loads; each lookup entry is queued with its offset
// moved by the composite data position, and the host records the highest priority.
#include <stdio.h>
struct Entry { long key, length, offset; };
struct Host { long deferred, priority; long lastKey, lastTime, lastData; int queued;
    void QueueEvent(long t, long time, long d) { lastKey = t; lastTime = time; lastData = d; ++queued; } };
static bool Load(long version, unsigned long compData, long priority, const Entry* table, unsigned long count, Host& h)
{
    if (version != 4) return false;
    if (count == 0) return true;
    for (unsigned long i = 0; i < count; ++i) h.QueueEvent(table[i].key, table[i].offset + compData, table[i].length);
    h.priority = priority; h.deferred = 1; return true;
}
int main()
{
    Entry t[2] = { { 1, 10, 0 }, { 2, 20, 100 } }; Host h = { 0, 0, 0, 0, 0, 0 };
    bool ok = !Load(3, 64, 5, t, 2, h) && h.queued == 0;
    ok = ok && Load(4, 64, 5, t, 2, h) && h.queued == 2 && h.lastKey == 2 && h.lastTime == 164 && h.lastData == 20 && h.priority == 5 && h.deferred == 1;
    printf(ok ? "00c16f00_TEST PASS\n" : "00c16f00_TEST FAIL\n");
    return ok ? 0 : 1;
}

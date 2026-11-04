// Behaviour test for CSoundEventManager::PlayStreamed 0x00C12E50: no stream -> -1; a resident sample with a driver plays
// through the lookup (its event id, -1 when the lookup finds nothing); otherwise a pool block of offset + size + tail is
// allocated (-1 when refused), locked, a request built and its read queued; a refused read unlocks and releases it (-1);
// else the new id from the manager's counter.
#include <cstdio>
struct Bank { bool stream; bool resident; bool driver; long padding; bool readOk; };
struct Req { long id, offset, tail, handle, size, key, padding; const Bank* bank; };
struct Mgr {
    long nextId; long allocResult; long allocSize, allocPadding; int locks, unlocks, released; long lookupId; Req last;
    long PlayStreamed(const Bank* bank, long position, unsigned long size)
    {
        if (!bank->stream) return -1;
        if (bank->resident && bank->driver) return lookupId;
        long offset = position % 4;                      // block-aligned start
        long tail = (4 - (offset + (long)size) % 4) % 4; // align the end
        allocSize = tail + offset + (long)size; allocPadding = bank->padding;
        long handle = allocResult;
        if (handle < 0) return -1;
        ++locks;
        Req r = { nextId++, offset, tail, handle, (long)size, position, bank->padding, bank };
        last = r;
        if (!bank->readOk) { ++unlocks; ++released; return -1; }
        return r.id;
    }
};
int main()
{
    bool ok = true;
    Mgr m = {}; m.nextId = 10; m.lookupId = 55;
    Bank none = { false }, res = { true, true, true, 0, true }, str = { true, false, false, 8, true }, bad = { true, false, false, 0, false };
    ok = ok && m.PlayStreamed(&none, 0, 16) == -1;
    ok = ok && m.PlayStreamed(&res, 0, 16) == 55;
    ok = ok && m.PlayStreamed(&str, 6, 9) == 10 && m.allocSize == 2 + 9 + 1 && m.allocPadding == 8 && m.last.key == 6 && m.locks == 1;
    ok = ok && m.PlayStreamed(&bad, 4, 4) == -1 && m.unlocks == 1 && m.released == 1 && m.nextId == 12;
    m.allocResult = -3; ok = ok && m.PlayStreamed(&str, 0, 4) == -1 && m.locks == 2;
    if (ok) std::printf("00c12e50_TEST PASS\n"); else std::printf("00c12e50_TEST FAIL\n");
    return ok ? 0 : 1;
}

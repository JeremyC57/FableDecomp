// Behaviour test for CTCNamedUserActionPoints::GetFreeNamedActionPoint (retail 0x007FF190): the same logic through a small
// standalone copy of the model (free / blocked index lists, names, a "blocked now" predicate standing for the virtual slot 28).
#include <cstdio>
#include <cstring>

struct LongList {
    long data[16];
    long count;
    LongList() : count(0) { }
    long* begin() { return data; }
    long* end() { return data + count; }
    long* find(long value) { long* p = begin(); for (; p != end(); ++p) if (*p == value) break; return p; }
    void erase(long* position)
    {
        if (position + 1 != end())
            std::memmove(position, position + 1, (const char*)end() - (const char*)(position + 1));
        --count;
    }
    void push_back(const long& value) { data[count++] = value; }
    bool has(long value) { return find(value) != end(); }
};

struct Model {
    LongList FreeActionPointIndexes;
    LongList BlockedActionPointIndexes;
    long NumberOfActionPoints;
    const char* Names[8];
    void* PThing;
    unsigned long blockedNowMask;     // the virtual IsActionPointIndexBlocked(thing, position, orientation, index)
    long virtualCalls;
    void* virtualOwner;

    bool IsActionPointIndexBlocked(long index) { return BlockedActionPointIndexes.has(index); }
    bool IsActionPointIndexBlockedNow(void* owner, long index)
    {
        ++virtualCalls;
        virtualOwner = owner;
        return (blockedNowMask >> index) & 1;
    }
    bool GetFreeNamedActionPoint(void* thing, const char* name, long& index, long start, bool checkBlocked)
    {
        PThing = thing;
        for (long i = start; i < NumberOfActionPoints; ++i) {
            const char* element = Names[i];
            if (std::strstr(element ? element : "", name ? name : "")) {
                long* last = FreeActionPointIndexes.end();
                long* pos = FreeActionPointIndexes.find(i);
                if (pos != last) {
                    FreeActionPointIndexes.erase(pos);
                    if (!(IsActionPointIndexBlocked(i) && checkBlocked)) {
                        void* owner = PThing;
                        if (!(IsActionPointIndexBlockedNow(owner, i) && checkBlocked)) {
                            index = i;
                            return true;
                        }
                        BlockedActionPointIndexes.push_back(i);
                    }
                }
            }
        }
        return false;
    }
};

static void Setup(Model& m)
{
    m.FreeActionPointIndexes.count = 0;
    m.BlockedActionPointIndexes.count = 0;
    m.NumberOfActionPoints = 6;
    m.Names[0] = "BAR_STOOL_A";
    m.Names[1] = "TABLE_SEAT";
    m.Names[2] = "BAR_STOOL_B";
    m.Names[3] = 0;                 // a null string reads as ""
    m.Names[4] = "BAR_STOOL_C";
    m.Names[5] = "BAR_STOOL_D";
    for (long i = 0; i < 6; ++i)
        m.FreeActionPointIndexes.push_back(i);
    m.PThing = 0;
    m.blockedNowMask = 0;
    m.virtualCalls = 0;
    m.virtualOwner = 0;
}

static int Fail(int n)
{
    std::printf("007ff190_TEST FAIL (%d)\n", n);
    return 1;
}

int main()
{
    Model m;
    int user;
    long index;

    // 1: first matching free point from start 0; taken off the free list, the user remembered and passed to the virtual test.
    Setup(m); index = -1;
    if (!m.GetFreeNamedActionPoint(&user, "BAR_STOOL", index, 0, true) || index != 0) return Fail(1);
    if (m.FreeActionPointIndexes.count != 5 || m.FreeActionPointIndexes.has(0) || m.FreeActionPointIndexes.data[0] != 1) return Fail(2);
    if (m.PThing != &user || m.virtualOwner != &user || m.virtualCalls != 1) return Fail(3);

    // 2: the search starts at `start`, non-matching names are skipped and stay free.
    Setup(m); index = -1;
    if (!m.GetFreeNamedActionPoint(&user, "BAR_STOOL", index, 1, true) || index != 2) return Fail(4);
    if (!m.FreeActionPointIndexes.has(1) || m.FreeActionPointIndexes.has(2) || m.FreeActionPointIndexes.count != 5) return Fail(5);

    // 3: a matching point that is not free is skipped without any test.
    Setup(m); index = -1;
    m.FreeActionPointIndexes.erase(m.FreeActionPointIndexes.find(0));
    if (!m.GetFreeNamedActionPoint(&user, "BAR_STOOL", index, 0, true) || index != 2 || m.virtualCalls != 1) return Fail(6);

    // 4: a point already on the blocked list is taken off the free list and skipped (no virtual test) when checkBlocked...
    Setup(m); index = -1;
    m.BlockedActionPointIndexes.push_back(0);
    if (!m.GetFreeNamedActionPoint(&user, "BAR_STOOL", index, 0, true) || index != 2) return Fail(7);
    if (m.FreeActionPointIndexes.has(0) || m.virtualCalls != 1 || m.BlockedActionPointIndexes.count != 1) return Fail(8);
    // ... and returned when checkBlocked is false (the virtual test still runs, its result ignored).
    Setup(m); index = -1;
    m.BlockedActionPointIndexes.push_back(0);
    m.blockedNowMask = 1;
    if (!m.GetFreeNamedActionPoint(&user, "BAR_STOOL", index, 0, false) || index != 0) return Fail(9);
    if (m.virtualCalls != 1 || m.BlockedActionPointIndexes.count != 1) return Fail(10);

    // 5: points blocked now are appended to the blocked list and the search goes on.
    Setup(m); index = -1;
    m.blockedNowMask = (1 << 0) | (1 << 2);
    if (!m.GetFreeNamedActionPoint(&user, "BAR_STOOL", index, 0, true) || index != 4) return Fail(11);
    if (m.BlockedActionPointIndexes.count != 2 || m.BlockedActionPointIndexes.data[0] != 0 || m.BlockedActionPointIndexes.data[1] != 2) return Fail(12);
    if (m.FreeActionPointIndexes.count != 3 || m.virtualCalls != 3) return Fail(13);

    // 6: nothing qualifies -> false, index untouched; every matching point was still drawn from the free list.
    Setup(m); index = -7;
    m.blockedNowMask = 0x3f;
    if (m.GetFreeNamedActionPoint(&user, "BAR_STOOL", index, 0, true) || index != -7) return Fail(14);
    if (m.BlockedActionPointIndexes.count != 4 || m.FreeActionPointIndexes.count != 2) return Fail(15);
    if (!m.FreeActionPointIndexes.has(1) || !m.FreeActionPointIndexes.has(3)) return Fail(16);

    // 7: no name contains the text -> false, lists untouched; start past the end -> false at once.
    Setup(m); index = -7;
    if (m.GetFreeNamedActionPoint(&user, "CHAIR", index, 0, true) || m.FreeActionPointIndexes.count != 6 || m.virtualCalls != 0) return Fail(17);
    if (m.GetFreeNamedActionPoint(&user, "BAR_STOOL", index, 6, true) || index != -7) return Fail(18);

    // 8: an empty name matches every point, the null-named one included.
    Setup(m); index = -1;
    if (!m.GetFreeNamedActionPoint(&user, 0, index, 3, true) || index != 3) return Fail(19);

    std::printf("007ff190_TEST PASS\n");
    return 0;
}
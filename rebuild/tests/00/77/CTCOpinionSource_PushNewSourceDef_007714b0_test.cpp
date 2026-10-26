// Behaviour test for 007714b0: body copied from src/007714b0.cpp (CTCOpinionSource::PushNewSourceDef), callees stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct SScalarTag { SScalarTag() { } };
static int g_grow = 0; static long g_growValue = 0;
struct CRetailVector_long {
    long* first;
    long* last;
    long* end;
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool atEnd)
    {
        ++g_grow; g_growValue = value; CHECK(where == last && count == 1 && atEnd);
        static long storage[8]; long n = last - first; for (long i = 0; i < n; ++i) storage[i] = first[i]; storage[n] = value; first = storage; last = storage + n + 1; end = storage + 8;
    }
    void push_back(long value)
    {
        if (last != end) {
            new (last) long(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
class CThing;
static bool g_setupOk = true; static int g_setups = 0; static long g_setupDef = -1;
class CTCOpinionSource { public:
    void* __vftable;
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
    CRetailVector_long DefStack;
    bool SetupFromDef(long defIndex) { ++g_setups; g_setupDef = defIndex; return g_setupOk; }
    bool PushNewSourceDef(long defIndex);
};
bool CTCOpinionSource::PushNewSourceDef(long defIndex)
{
    if (SetupFromDef(defIndex)) {
        DefStack.push_back(defIndex);
        return true;
    }
    return false;
}
int main()
{
    long buf[1] = { 0 };
    CTCOpinionSource s; memset(&s, 0, sizeof(s)); s.DefStack.first = buf; s.DefStack.last = buf; s.DefStack.end = buf + 1;
    // setup fails: nothing pushed
    g_setupOk = false; CHECK(!s.PushNewSourceDef(5)); CHECK(g_setups == 1 && g_setupDef == 5 && s.DefStack.last == buf);
    // setup succeeds: the index pushed (room available)
    g_setupOk = true; CHECK(s.PushNewSourceDef(7)); CHECK(s.DefStack.last == buf + 1 && buf[0] == 7 && g_grow == 0);
    // full: the growing insert
    CHECK(s.PushNewSourceDef(9)); CHECK(g_grow == 1 && g_growValue == 9 && s.DefStack.last - s.DefStack.first == 2 && s.DefStack.first[1] == 9);
    if (fails == 0) printf("007714b0_TEST PASS\n"); else printf("007714b0_TEST FAIL\n");
    return fails;
}
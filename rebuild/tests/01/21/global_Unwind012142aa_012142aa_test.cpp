// Behaviour test for PKCTRArray::AddValue (0x00C32CD0): the value goes in at its sorted index (the tail shifted up one),
// the array grows by one slot when full, and a null object / a duplicate trace lines 131 / 134.
#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_traces = 0, g_breaks = 0; static char g_last[256];
struct CString {
    char buf[256];
    CString() { buf[0] = 0; }
    CString& operator<<(const char* t) { std::strcat(buf, t); return *this; }
    CString& operator<<(long v) { char n[32]; std::sprintf(n, "%ld", v); std::strcat(buf, n); return *this; }
};
static void Hook(const char* m) { ++g_traces; std::strcpy(g_last, m); }
static void (*g_pfnAssertHook)(const char*) = Hook;
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };
struct Cmp {
    int EqualValue(const void* a, const void* b) { return *(const int*)a == *(const int*)b; }
    int LessValue(const void* a, const void* b) { return *(const int*)a < *(const int*)b; }
};
struct PKCTRArray {
    void** Entries; unsigned int Capacity; unsigned int Count; int grows;
    int FindValueIndex(Cmp* c, void* v, int* found)
    {
        unsigned int i = 0;
        while (i < Count && c->LessValue(Entries[i], v)) ++i;
        *found = i < Count && c->EqualValue(Entries[i], v);
        return (int)i;
    }
    void ResizeAllocation(unsigned int extra)
    { ++grows; Entries = (void**)std::realloc(Entries, (Capacity + extra) * sizeof(void*)); Capacity += extra; }
    void AddValue(Cmp* comparator, void* value)
    {
        if (value == 0) {
            { CString line; line << ".\\PKCTRArray.cpp" << "(" << 131L << ") : " << "Error: NULL object" << "\n"; if (g_pfnAssertHook) g_pfnAssertHook(line.buf); }
            CDebugBreak();
        }
        int found;
        int index = FindValueIndex(comparator, value, &found);
        if (found) {
            { CString line; line << ".\\PKCTRArray.cpp" << "(" << 134L << ") : " << "Error: this object already exists..." << "\n"; if (g_pfnAssertHook) g_pfnAssertHook(line.buf); }
            CDebugBreak();
        }
        if (Capacity - Count == 0) ResizeAllocation(1);
        if (Capacity - Count < 1) {
            { CString line; line << ".\\PKCTRArray.cpp" << "(" << 139L << ") : " << "Error: Growing did not work?" << "\n"; if (g_pfnAssertHook) g_pfnAssertHook(line.buf); }
            CDebugBreak();
        }
        if (Count != 0) {
            for (int i = (int)Count - 1; i >= index; --i) Entries[i + 1] = Entries[i];
        }
        Entries[index] = value;
        ++Count;
    }
};

int main()
{
    int v[] = { 5, 1, 9, 3 };
    PKCTRArray a = { 0, 0, 0, 0 }; Cmp c;
    for (int i = 0; i < 4; ++i) a.AddValue(&c, &v[i]);
    bool ok = a.Count == 4 && a.Capacity == 4 && a.grows == 4 && g_traces == 0
           && *(int*)a.Entries[0] == 1 && *(int*)a.Entries[1] == 3 && *(int*)a.Entries[2] == 5 && *(int*)a.Entries[3] == 9;
    int dup = 3;
    a.AddValue(&c, &dup);
    ok = ok && g_traces == 1 && g_breaks == 1 && std::strcmp(g_last, ".\\PKCTRArray.cpp(134) : Error: this object already exists...\n") == 0 && a.Count == 5;
    std::free(a.Entries);
    if (!ok) { std::printf("00c32cd0_TEST FAIL\n"); return 1; }
    std::printf("012142aa_TEST PASS\n");
    return 0;
}
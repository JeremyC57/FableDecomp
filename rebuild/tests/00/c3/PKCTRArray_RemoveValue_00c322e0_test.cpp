// Behaviour test for PKCTRArray::RemoveValue (0x00C322E0): binary search by value, trace on null / missing objects,
// close the gap and drop the count.
#include <cstdio>

static int g_traces, g_breaks;
static void Trace(int line) { (void)line; ++g_traces; }
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };

class CUnprovenArrayComparator {
public:
    virtual int EqualValue(const void* a, const void* b) { return *(const int*)a == *(const int*)b; }
    virtual int LessValue(const void* a, const void* b) { return *(const int*)a < *(const int*)b; }
    virtual ~CUnprovenArrayComparator() {}
};

class PKCTRArray {
public:
    void** Entries; unsigned int Capacity; unsigned int Count;
    int FindValueIndex(CUnprovenArrayComparator* cmp, const void* key, int* found)
    {
        if (Count == 0) { *found = 0; return 0; }
        int low = 0, high = (int)Count - 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (cmp->EqualValue(key, Entries[mid])) { *found = 1; return mid; }
            if (cmp->LessValue(key, Entries[mid])) high = mid - 1; else low = mid + 1;
        }
        *found = 0;
        int mid = (low + high) / 2;
        return cmp->LessValue(key, Entries[mid]) ? mid : mid + 1;
    }
    void RemoveValue(CUnprovenArrayComparator* comparator, const void* value)
    {
        if (value == 0) { Trace(154); CDebugBreak(); }
        int found;
        int index = FindValueIndex(comparator, value, &found);
        if (!found) { Trace(157); CDebugBreak(); }
        for (unsigned int i = index; i < Count - 1; ++i)
            Entries[i] = Entries[i + 1];
        --Count;
    }
};

int main()
{
    int v[6] = { 10, 20, 30, 40, 50, 60 };
    void* slots[6];
    int i;
    for (i = 0; i < 6; ++i) slots[i] = &v[i];
    PKCTRArray a; a.Entries = slots; a.Capacity = 6; a.Count = 6;
    CUnprovenArrayComparator cmp;
    int key = 30;
    a.RemoveValue(&cmp, &key);
    if (a.Count != 5 || *(int*)a.Entries[2] != 40 || *(int*)a.Entries[4] != 60 || g_traces != 0) { std::printf("00c322e0_TEST FAIL\n"); return 1; }
    key = 60;
    a.RemoveValue(&cmp, &key);                       // last element: no shift
    if (a.Count != 4 || *(int*)a.Entries[3] != 50 || g_traces != 0) { std::printf("00c322e0_TEST FAIL\n"); return 1; }
    key = 10;
    a.RemoveValue(&cmp, &key);                       // first element
    if (a.Count != 3 || *(int*)a.Entries[0] != 20 || *(int*)a.Entries[2] != 50) { std::printf("00c322e0_TEST FAIL\n"); return 1; }
    key = 45;                                        // missing: traced, still removes at the insertion point
    a.RemoveValue(&cmp, &key);
    if (g_traces != 1 || g_breaks != 1 || a.Count != 2) { std::printf("00c322e0_TEST FAIL\n"); return 1; }
    std::printf("00c322e0_TEST PASS\n");
    return 0;
}
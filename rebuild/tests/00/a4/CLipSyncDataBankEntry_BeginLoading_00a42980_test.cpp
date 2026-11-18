// Behaviour test for CLipSyncDataBankEntry::BeginLoading 0x00A42980 (NF68: restated after the port onto the shared CResource /
// CIVCountedPointer / CCountedPointer / CBankFileAsync / CResourceBank headers): a copy of the function's logic with neutral stand-in
// names. Only an entry with no data resource loads: a new resource (reference count 1, owner and bank recorded, empty counted pointers)
// is adopted, the bank entry for the index is created, its whole data size is read at the lip-sync loading priority into the resource's
// file data, the temporary entry pointer is released, and the resource is queued once on the bank's resource bank. A second call is a no-op.
#include <cstdio>

static int fails = 0, g_calls = 0, g_entryReleased = 0, g_queued = 0, g_priorityUsed = -1;
static unsigned long g_offset = 99, g_size = 0, g_index = 0;
#define CHECK(c) do { if (!(c)) { ++fails; std::printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

struct Counted { int* Info; void* Data; Counted() : Info(0), Data(0) { } };
struct Entry;
struct Bank;
struct Resource { long RefCount; Counted LipSyncData, FileData; Entry* Owner; Bank* Bank_; };
struct ResourceBank { Resource* Queued; void AddToLoadingQueue(Resource& r) { Queued = &r; ++g_queued; ++g_calls; } };
struct AsyncEntry {
    int dummy;
    Counted BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, int priority) {
        g_offset = offset; g_size = size; g_priorityUsed = priority; ++g_calls; Counted c; static int info = 1; c.Info = &info; c.Data = this; return c; }
};
struct Bank {
    ResourceBank* PResourceBank; unsigned long Sizes[4]; AsyncEntry Async;
    AsyncEntry* CreateEntry(unsigned long index) { g_index = index; ++g_calls; return &Async; }
    unsigned long GetEntryDataSize(unsigned long index) const { return Sizes[index & 3]; }
};
static int g_priority = 3;
struct Entry {
    Resource* Data; float Duration;
    void BeginLoading(unsigned long index, Bank& bank) {
        if (Data == 0) {
            Resource* fresh = new Resource; fresh->RefCount = 1; fresh->Owner = this; fresh->Bank_ = &bank;
            Data = fresh;
            AsyncEntry* entry = bank.CreateEntry(index);
            unsigned long size = bank.GetEntryDataSize(index);
            Data->FileData = entry->BeginReadDataAutoAlloc(0, size, g_priority);
            ++g_entryReleased;
            bank.PResourceBank->AddToLoadingQueue(*Data);
        }
    }
};

int main() {
    ResourceBank rb; rb.Queued = 0;
    Bank bank; bank.PResourceBank = &rb; bank.Sizes[0] = 10; bank.Sizes[1] = 20; bank.Sizes[2] = 30; bank.Sizes[3] = 40;
    Entry e; e.Data = 0; e.Duration = 7.0f;
    e.BeginLoading(6, bank);
    CHECK(e.Data != 0 && e.Data->RefCount == 1 && e.Data->Owner == &e && e.Data->Bank_ == &bank);
    CHECK(g_index == 6 && g_offset == 0 && g_size == 30 && g_priorityUsed == 3);
    CHECK(e.Data->FileData.Data == &bank.Async && e.Data->LipSyncData.Data == 0 && e.Data->LipSyncData.Info == 0);
    CHECK(g_entryReleased == 1 && g_queued == 1 && rb.Queued == e.Data && g_calls == 3 && e.Duration == 7.0f);
    Resource* first = e.Data;
    e.BeginLoading(1, bank);
    CHECK(e.Data == first && g_calls == 3 && g_queued == 1 && g_index == 6);
    delete first;
    if (fails == 0) std::puts("00a42980_TEST PASS");
    return fails;
}

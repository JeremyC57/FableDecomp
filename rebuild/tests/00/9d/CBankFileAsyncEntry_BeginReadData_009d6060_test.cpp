// Behaviour test for CBankFileAsyncEntry::BeginReadData (0x009D6060): counted data object over a caller buffer, SetupRead with skip 0.
#include <cstdio>

static int g_destroyed = 0;
struct SCountBlock {
    long References;
    void (*Destroy)(void*);
    void* Object;
};
struct Data {
    void* Buffer; unsigned long Size; void* Bank;
    Data(void* buffer, unsigned long size, void* bank) : Buffer(buffer), Size(size), Bank(bank) {}
};
static void DestroyData(void* object) { delete (Data*)object; ++g_destroyed; }

struct CountedData {
    Data* Ptr;
    SCountBlock* Info;
    CountedData(Data* object) : Ptr(object), Info(0)
    {
        if (object) { Info = new SCountBlock; Info->References = 1; Info->Destroy = DestroyData; Info->Object = object; }
    }
    CountedData(const CountedData& other) : Ptr(other.Ptr), Info(other.Info) { if (Info) ++Info->References; }
    ~CountedData()
    {
        if (Info && --Info->References == 0) { Info->Destroy(Info->Object); delete Info; }
    }
};

struct SetupArgs { unsigned long start; Data* data; unsigned long readSize; unsigned long allocSize; unsigned long skip; int priority; };
static SetupArgs g_setup;

struct Entry {
    void* Bank;
    void SetupRead(unsigned long start, Data* data, unsigned long readSize, unsigned long allocSize, unsigned long skip, int priority)
    {
        g_setup.start = start; g_setup.data = data; g_setup.readSize = readSize;
        g_setup.allocSize = allocSize; g_setup.skip = skip; g_setup.priority = priority;
    }
    CountedData BeginReadData(unsigned long start, void* buffer, unsigned long size, unsigned long bufferSize, int priority)
    {
        CountedData data(new Data(buffer, size, Bank));
        SetupRead(start, data.Ptr, size, bufferSize, 0, priority);
        return data;
    }
};

int main()
{
    int bank = 0;
    char buffer[64];
    Entry entry;
    entry.Bank = &bank;
    {
        CountedData result = entry.BeginReadData(0x1000, buffer, 40, 64, 2);
        if (result.Info == 0 || result.Info->References != 1 || result.Ptr == 0) { std::printf("009d6060_TEST FAIL\n"); return 1; }
        if (result.Ptr->Buffer != buffer || result.Ptr->Size != 40 || result.Ptr->Bank != &bank) { std::printf("009d6060_TEST FAIL\n"); return 1; }
        if (g_setup.start != 0x1000 || g_setup.data != result.Ptr || g_setup.readSize != 40 || g_setup.allocSize != 64 ||
            g_setup.skip != 0 || g_setup.priority != 2 || g_destroyed != 0) { std::printf("009d6060_TEST FAIL\n"); return 1; }
    }
    if (g_destroyed != 1) { std::printf("009d6060_TEST FAIL\n"); return 1; }
    std::printf("009d6060_TEST PASS\n");
    return 0;
}
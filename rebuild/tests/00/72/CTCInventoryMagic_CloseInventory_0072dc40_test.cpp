// Standalone C++03 fixture: original whole method, with instrumented base and element callees.
#include <vector>
#include <stdio.h>
#include <stdlib.h>
static bool Tracking, BaseClosed;
static int BaseCalls, Releases;
static void Check(bool value, int line)
{
    if (!value) { fprintf(stderr, "check failed at %d\n", line); exit(1); }
}
#define CHECK(x) Check((x), __LINE__)
namespace NInventory {
class CTCInventoryBase {
public:
    bool InventoryOpen;
    virtual void CloseInventory() {
        CHECK(Tracking);
        ++BaseCalls;
        InventoryOpen = false;
        BaseClosed = true;
    }
};
}
namespace NHeroInformationScreens {
class CBase { public: virtual void ScreenTestStub() {} };
}
class CEnginePrimitiveHandle {
public:
    int Data;
    explicit CEnginePrimitiveHandle(int data = 0) : Data(data) {}
    ~CEnginePrimitiveHandle() {
        if (Tracking && Data) { CHECK(BaseClosed); ++Releases; }
    }
};
class CTCInventoryMagic : public NInventory::CTCInventoryBase,
                          public NHeroInformationScreens::CBase {
public:
    std::vector<CEnginePrimitiveHandle> InventoryMagicPrimitiveHandles;
    int UnrelatedState;
    virtual void CloseInventory();
};

void CTCInventoryMagic::CloseInventory()
{
    NInventory::CTCInventoryBase::CloseInventory();
    InventoryMagicPrimitiveHandles.clear();
}

int main()
{
    const int counts[3] = {0, 1, 4};
    for (int open = 0; open != 2; ++open) {
        for (int n = 0; n != 3; ++n) {
            CTCInventoryMagic value;
            value.InventoryOpen = open != 0;
            value.UnrelatedState = 32767;
            for (int i = 0; i < counts[n]; ++i)
                value.InventoryMagicPrimitiveHandles.push_back(CEnginePrimitiveHandle(i + 1));
            const size_t capacity = value.InventoryMagicPrimitiveHandles.capacity();
            BaseCalls = Releases = 0; BaseClosed = false; Tracking = true;
            value.CloseInventory();
            Tracking = false;
            CHECK(BaseCalls == 1 && !value.InventoryOpen && Releases == counts[n]);
            CHECK(value.InventoryMagicPrimitiveHandles.empty());
            CHECK(value.InventoryMagicPrimitiveHandles.capacity() == capacity);
            CHECK(value.UnrelatedState == 32767);
            BaseCalls = Releases = 0; BaseClosed = false; Tracking = true;
            value.CloseInventory();
            Tracking = false;
            CHECK(BaseCalls == 1 && Releases == 0 && !value.InventoryOpen);
            CHECK(value.InventoryMagicPrimitiveHandles.empty());
        }
    }
    puts("0072dc40_TEST PASS");
    return 0;
}
#include <cstdio>
class CEngineGraphic { public: unsigned long BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
static long g_lastType = -1;
static CEngineGraphic GetEngineGraphic(long itemType) { g_lastType = itemType; CEngineGraphic g = { (unsigned long)(itemType * 10), 0, 0, false, 0 }; return g; }
struct CLongVector {
    long* first; long* last; long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    long* begin() const { return first; }
    long& operator[](unsigned long n) { return *(begin() + n); }
};
// Behaviour of CTCInventory::GetContextSensitiveItemGraphicIndex (0x00581CFD): the slot's graphic bank index, 0 for an empty (<= 0) slot.
class CTCInventory { public:
    CLongVector ContextSensitiveItems;
    long GetContextSensitiveItemGraphicIndex(long index)
    {
        if (ContextSensitiveItems[index] > 0)
            return GetEngineGraphic(ContextSensitiveItems[index]).BankIndex;
        return 0;
    }
};
int main()
{
    long items[4] = { 0, 7, -1, 3 };
    CTCInventory inv; inv.ContextSensitiveItems.first = items; inv.ContextSensitiveItems.last = items + 4; inv.ContextSensitiveItems.end_of_storage = items + 4;
    if (inv.GetContextSensitiveItemGraphicIndex(0) != 0 || g_lastType != -1) { std::printf("00581cfd_TEST FAIL\n"); return 1; }
    if (inv.GetContextSensitiveItemGraphicIndex(1) != 70 || g_lastType != 7) { std::printf("00581cfd_TEST FAIL\n"); return 1; }
    if (inv.GetContextSensitiveItemGraphicIndex(2) != 0 || inv.GetContextSensitiveItemGraphicIndex(3) != 30) { std::printf("00581cfd_TEST FAIL\n"); return 1; }
    std::printf("00581cfd_TEST PASS\n"); return 0;
}
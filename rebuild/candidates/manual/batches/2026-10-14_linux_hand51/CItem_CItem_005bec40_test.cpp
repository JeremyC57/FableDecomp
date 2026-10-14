#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CPerItemData { unsigned char _body[0x24]; CPerItemData() { } CPerItemData(const CPerItemData& o) { std::memcpy(_body, o._body, 0x24); std::strcat(g_seq, "n"); } }; static CPerItemData* g_reset = 0;
struct CCountedPointer { CPerItemData* m_pObject; void* m_pCount; CCountedPointer() : m_pObject(0), m_pCount(0) { } void Reset(CPerItemData* o) { std::strcat(g_seq, "R"); g_reset = o; m_pObject = o; } };
struct CItem { long ItemType; long NumberOfItems; CCountedPointer PPerItemData; long AverageBoughtPrice; CItem(long type, long count, const CPerItemData* data, long price) : ItemType(type), NumberOfItems(count), PPerItemData(), AverageBoughtPrice(price) { if (data) PPerItemData.Reset(new CPerItemData(*data)); } };

int main() {
    CPerItemData d; std::memset(&d, 7, sizeof d); CItem a(1, 2, &d, 3); if (a.ItemType != 1 || a.NumberOfItems != 2 || a.AverageBoughtPrice != 3 || std::strcmp(g_seq, "nR") || !g_reset || g_reset == &d || g_reset->_body[5] != 7) { std::printf("005bec40_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; CItem b(4, 5, 0, 6); if (b.ItemType != 4 || b.PPerItemData.m_pObject || b.PPerItemData.m_pCount || g_seq[0]) { std::printf("005bec40_TEST FAIL\n"); return 1; }
    std::printf("005bec40_TEST PASS\n"); return 0;
}

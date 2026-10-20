// Behaviour test for 005bec92: src/005bec92.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. NInventory::CItem::AddItem(long, long, long)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NInventory::CItem::AddItem(long, long, long) -- add items of a type
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
#pragma optimize("y", off)
#pragma optimize("s", on)
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
bool __fastcall IsAugmentation(long itemType);     // 0x005D97F0
long __fastcall GetMaxNumberOfItems(long itemType);   // 0x005D99F0 (labelled GetInventoryCategory)
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
    const T& operator[](unsigned long i) const { return _M_start[i]; }
    void resize(unsigned long count, const T& value);   // 0x004428F0
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
    T* Detach() { T* p = Object; Object = 0; return p; }
};
template <class T> struct CDefHolder {             // adopts a reference (no addref)
    T* Object;
    CDefHolder(T* p) : Object(p) { }
    __forceinline ~CDefHolder() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CAugmentationDef : CDefBase {
    unsigned char _pad08[0x28 - 0x08];
    long NumberOfSlots;                            // +0x28
    unsigned char _pad2c[0x40 - 0x2c];
    vector<unsigned long> SlotValues;              // +0x40
};
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out) const;   // 0x0042AF9E
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
    template <class T> bool GetDef(long index, CDefPointer<T>& out) const;   // 0x0042AF3C
};
void* FindDef_s7r_1();
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CGameDefinitionManager::GetDef(long p0, CDefPointer<T>& out) const
{
    void* def = FindDef_s7r_1();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
struct CPerItemData {                              // 0x24 bytes
    vector<unsigned long> SlotValues;              // +0x00
    unsigned char _body[0x24 - 0x0c];
    CPerItemData();                                // 0x005C14A6
};
struct CCountedPointer_PerItemData {
    CPerItemData* m_pObject;
    void*         m_pCount;
    void Reset(CPerItemData* object);              // 0x005C1946
};
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
struct CItem {
    long                        ItemType;          // +0x00
    long                        NumberOfItems;     // +0x04
    CCountedPointer_PerItemData PPerItemData;      // +0x08
    long                        AverageBoughtPrice;   // +0x10
    void AddItem(long itemType, long number, long price);
};

void CItem::AddItem(long itemType, long number, long price)
{
    if (NumberOfItems == 0) {
        AverageBoughtPrice = 0;
        if (IsAugmentation(itemType)) {
            PPerItemData.Reset(new CPerItemData());
            CDefPointer<CDefClassBase> def;
            CGameDefinitionManager::Get().GetDef(itemType, def);
            bool found;
            CAugmentationDef* raw;
            {
                CDefPointer<CAugmentationDef> out;
                found = def->GetSubDef(out);
                raw = out.Detach();
            }
            CDefHolder<CAugmentationDef> augmentation(raw);
            if (found) {
                PPerItemData.m_pObject->SlotValues.resize(augmentation->NumberOfSlots, 0);
                for (unsigned long i = 0; i < augmentation->SlotValues.size() && i < PPerItemData.m_pObject->SlotValues.size(); ++i)
                    PPerItemData.m_pObject->SlotValues[i] = augmentation->SlotValues[i];
            }
        }
    }
    ItemType = itemType;
    long room = GetMaxNumberOfItems(itemType) - NumberOfItems;
    long added = Min(number, room);
    if (price != -1) {
        AverageBoughtPrice = AverageBoughtPrice * NumberOfItems + added * price;
        NumberOfItems += added;
        AverageBoughtPrice /= NumberOfItems;
    } else {
        NumberOfItems += added;
    }
}

// ---- stubs and driver ----
#include <cstdlib>
static void* g_defs[2]; static int g_find0 = 0, g_find1 = 0, g_release = 0, g_destroy = 0, g_reset = 0, g_resize = 0, g_ctor = 0;
static bool g_isAug = false; static long g_max = 10; static CGameDefinitionManager g_manager;
static unsigned long g_slotStore[8]; static unsigned long g_augSlots[3] = { 11, 22, 33 };
void* FindDef_s7r_0() { ++g_find0; return g_defs[0]; }
void* FindDef_s7r_1() { ++g_find1; return g_defs[1]; }
void ReleaseDef_s7(void*) { ++g_release; }
void CDefBase::v00() {}
void CDefBase::Destroy() { ++g_destroy; }
bool __fastcall IsAugmentation(long itemType) { return g_isAug; }
long __fastcall GetMaxNumberOfItems(long itemType) { return g_max; }
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_manager; }
CPerItemData::CPerItemData() { ++g_ctor; SlotValues._M_start = SlotValues._M_finish = SlotValues._M_end_of_storage = g_slotStore; }
void CCountedPointer_PerItemData::Reset(CPerItemData* object) { ++g_reset; m_pObject = object; m_pCount = 0; }
template <> void vector<unsigned long>::resize(unsigned long count, const unsigned long& value)
{ ++g_resize; for (unsigned long i = size(); i < count; ++i) _M_start[i] = value; _M_finish = _M_start + count; _M_end_of_storage = _M_finish; }
int main()
{
    CItem item; item.ItemType = 0; item.NumberOfItems = 0; item.PPerItemData.m_pObject = 0; item.PPerItemData.m_pCount = 0; item.AverageBoughtPrice = 99;
    // plain item, priced
    item.AddItem(5, 4, 10); CHECK(item.ItemType == 5 && item.NumberOfItems == 4 && item.AverageBoughtPrice == 10 && g_reset == 0);
    item.AddItem(5, 4, 20); CHECK(item.NumberOfItems == 8 && item.AverageBoughtPrice == 15);
    item.AddItem(5, 4, 30); CHECK(item.NumberOfItems == 10 && item.AverageBoughtPrice == (15 * 8 + 2 * 30) / 10);   // clamped to the max
    item.AddItem(5, 3, -1); CHECK(item.NumberOfItems == 10);
    // augmentation: per-item data seeded from the def's slot values
    static CDefClassBase thingDef; thingDef.m_refCount = 2;
    static CAugmentationDef aug; aug.m_refCount = 3; aug.NumberOfSlots = 4; aug.SlotValues._M_start = g_augSlots; aug.SlotValues._M_finish = g_augSlots + 3; aug.SlotValues._M_end_of_storage = g_augSlots + 3;
    g_defs[1] = &thingDef; g_defs[0] = &aug; g_isAug = true;
    CItem a; a.ItemType = 0; a.NumberOfItems = 0; a.PPerItemData.m_pObject = 0; a.PPerItemData.m_pCount = 0; a.AverageBoughtPrice = 7;
    a.AddItem(8, 1, -1);
    CHECK(g_reset == 1 && g_ctor == 1 && a.PPerItemData.m_pObject != 0 && g_resize == 1); CHECK(a.PPerItemData.m_pObject->SlotValues.size() == 4);
    CHECK(g_slotStore[0] == 11 && g_slotStore[1] == 22 && g_slotStore[2] == 33 && g_slotStore[3] == 0);
    CHECK(aug.m_refCount == 2 && thingDef.m_refCount == 1 && a.AverageBoughtPrice == 0 && a.NumberOfItems == 1 && a.ItemType == 8);
    // no augmentation sub-def: data object made, not filled; defs released (last references destroy)
    g_defs[0] = 0; thingDef.m_refCount = 1;
    CItem b; b.ItemType = 0; b.NumberOfItems = 0; b.PPerItemData.m_pObject = 0; b.PPerItemData.m_pCount = 0; b.AverageBoughtPrice = 7;
    b.AddItem(9, 2, 6); CHECK(g_reset == 2 && g_resize == 1 && thingDef.m_refCount == 0 && g_destroy == 1 && b.AverageBoughtPrice == 6 && b.NumberOfItems == 2);
    CHECK(g_release == 0);
    if (fails == 0) printf("005bec92_TEST PASS\n"); else printf("005bec92_TEST FAIL\n");
    return fails;
}
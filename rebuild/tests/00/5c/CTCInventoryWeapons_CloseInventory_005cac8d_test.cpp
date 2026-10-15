#include <cstdio>
// Behaviour of CTCInventoryWeapons::CloseInventory (0x005CAC8D).
struct CEnginePrimitiveHandle { long m_id; void* m_pPrimitive; void Clear() { m_id = 0; m_pPrimitive = 0; } };
template <class R, class T> class mem_fun_ref_t {
public:
    explicit mem_fun_ref_t(R (T::*pfn)()) : m_pfn(pfn) {}
    R operator()(T& object) const { return (object.*m_pfn)(); }
private:
    R (T::*m_pfn)();
};
template <class R, class T> inline mem_fun_ref_t<R, T> mem_fun_ref(R (T::*pfn)()) { return mem_fun_ref_t<R, T>(pfn); }
template <class It, class F> F for_each(It first, It last, F f) { for (; first != last; ++first) f(*first); return f; }
struct CPrimitiveHandleVector {
    CEnginePrimitiveHandle* _First; CEnginePrimitiveHandle* _Last; CEnginePrimitiveHandle* _End;
    CEnginePrimitiveHandle* begin() { return _First; }
    CEnginePrimitiveHandle* end() { return _Last; }
};
struct CTCInventoryWeapons {
    bool InventoryOpen; CPrimitiveHandleVector ActiveWeaponPrimitiveHandles; long m_setups;
    void SetupActiveWeaponsOnCarrier() { ++m_setups; }
    void CloseInventory()
    {
        for_each(ActiveWeaponPrimitiveHandles.begin(), ActiveWeaponPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
        SetupActiveWeaponsOnCarrier();
        InventoryOpen = false;
    }
};
int main()
{
    CEnginePrimitiveHandle h[3]; int x = 0;
    for (int i = 0; i < 3; ++i) { h[i].m_id = i + 1; h[i].m_pPrimitive = &x; }
    CTCInventoryWeapons w; w.InventoryOpen = true; w.m_setups = 0;
    w.ActiveWeaponPrimitiveHandles._First = h; w.ActiveWeaponPrimitiveHandles._Last = h + 2; w.ActiveWeaponPrimitiveHandles._End = h + 3;
    w.CloseInventory();
    int bad = (h[0].m_id != 0 || h[1].m_id != 0 || h[1].m_pPrimitive != 0 || h[2].m_id != 3 || w.m_setups != 1 || w.InventoryOpen);
    if (bad) { std::printf("005cac8d_TEST FAIL\n"); return 1; }
    std::printf("005cac8d_TEST PASS\n");
    return 0;
}
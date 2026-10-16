// Behaviour test for 0x005c2f27 OpenInventory (stats / weapons screen)
#include <cstdio>
struct C2DVector { float X, Y; };
struct C2DBoxF { C2DVector TL, Extents; };
struct CInventoryDef { long m_refCount; C2DVector BoxTL, BoxExtents; };
struct CDefPointer {
    CInventoryDef* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_refCount; }
    CDefPointer(CInventoryDef* d) : Object(d) {}
    ~CDefPointer() { if (Object) --Object->m_refCount; }
    CInventoryDef* operator->() const { return Object; }
};
static int g_opened = 0;
struct CInv {
    CDefPointer PDef; C2DBoxF box; bool baseOpened;
    CDefPointer GetPDef() const { return PDef; }
    void SetInventoryAnimRenderBox(const C2DBoxF& b) { box = b; }
    void OpenInventory()
    {
        baseOpened = true;
        C2DBoxF b; b.TL = GetPDef()->BoxTL; b.Extents = GetPDef()->BoxExtents;
        SetInventoryAnimRenderBox(b);
        ++g_opened;
    }
};
int main()
{
    CInventoryDef def = { 1, { 1.0f, 2.0f }, { 3.0f, 4.0f } };
    CInv inv = { CDefPointer(&def), { { 0, 0 }, { 0, 0 } }, false };
    inv.OpenInventory();
    bool ok = inv.baseOpened && inv.box.TL.X == 1.0f && inv.box.TL.Y == 2.0f && inv.box.Extents.X == 3.0f && inv.box.Extents.Y == 4.0f
           && def.m_refCount == 1 && g_opened == 1;
    inv.PDef.Object = 0;
    if (!ok) { std::printf("005c2f27_TEST FAIL\n"); return 1; }
    std::printf("005c2f27_TEST PASS\n");
    return 0;
}

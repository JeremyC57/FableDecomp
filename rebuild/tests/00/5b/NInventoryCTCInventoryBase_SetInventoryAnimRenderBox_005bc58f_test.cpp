// Behaviour test for NInventory::CTCInventoryBase::SetInventoryAnimRenderBox (0x005BC58F): a model of the row -- the four box
// coordinates are copied into the component's render box.
#include <stdio.h>
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CTCInventoryBase { C2DBoxF InventoryAnimRenderBox;
    void SetInventoryAnimRenderBox(const C2DBoxF& box)
    {
        C2DBoxF& dest = InventoryAnimRenderBox;
        dest.TLX = box.TLX; dest.TLY = box.TLY; dest.BRX = box.BRX; dest.BRY = box.BRY;
    }
};
int main()
{
    CTCInventoryBase inv = { { 0, 0, 0, 0 } };
    C2DBoxF box = { 10.0f, 20.0f, 300.0f, 400.0f };
    inv.SetInventoryAnimRenderBox(box);
    int ok = inv.InventoryAnimRenderBox.TLX == 10.0f && inv.InventoryAnimRenderBox.TLY == 20.0f
        && inv.InventoryAnimRenderBox.BRX == 300.0f && inv.InventoryAnimRenderBox.BRY == 400.0f;
    printf(ok ? "005bc58f_TEST PASS\n" : "005bc58f_TEST FAIL\n");
    return ok ? 0 : 1;
}
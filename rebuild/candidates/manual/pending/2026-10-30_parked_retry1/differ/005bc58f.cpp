// NInventory::CTCInventoryBase::SetInventoryAnimRenderBox(const C2DBoxF&) -- stores the screen box the inventory's 3D item animation is rendered into
// Grade: DIFFER(35v35) -- register roles: retail lea eax,[ecx+0BCh] dest first then src in ecx; ours src in eax, add ecx. Tried (retry1): class-keyed C2DVector/C2DBoxF with PDB user copy ctors, memberwise ref-local copy (42v35), *(&m)=*(&b). Names fixed: NInventory::CTCInventoryBase (PDB QAEXABVC2DBoxF), C2DBoxF/C2DVector class-keyed.
// Leaf: ?SetInventoryAnimRenderBox@CTCInventoryBase@NInventory@@QAEXABVC2DBoxF@@@Z
class C2DVector {
public:
    float X, Y;
    C2DVector() {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
};
class C2DBoxF {
public:
    C2DVector TopLeft, BottomRight;
    C2DBoxF() {}
    C2DBoxF(const C2DBoxF& o) : TopLeft(o.TopLeft), BottomRight(o.BottomRight) {}
};
namespace NInventory {
class CTCInventoryBase {
public:
    unsigned char _pad_0x00[0xBC];
    C2DBoxF       InventoryAnimRenderBox;         // +0xBC (Ego_r +0xD0)
    void SetInventoryAnimRenderBox(const C2DBoxF& box);
};
void CTCInventoryBase::SetInventoryAnimRenderBox(const C2DBoxF& box)
{
    *(&InventoryAnimRenderBox) = *(&box);
}
}

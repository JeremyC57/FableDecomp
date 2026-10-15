// CTCInventoryBase::SetInventoryAnimRenderBox(const C2DBoxF&) -- stores the screen box the inventory's 3D item animation is rendered into
// Grade: DIFFER(35v35) -- register roles swapped: retail forms the destination first (`lea eax,[ecx+0BCh]`) and loads the source pointer into ecx, the lane loads the source into eax and adds 0BCh to ecx. Tried: plain POD assignment (35v35), reference local to the member (same), optimize("s") (movsd x4, 21v35), memberwise inline operator= (42v35, no address CSE).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC58F, length 35 bytes.
// Callees: none.
// Layout: CTCInventoryBase has no engine header; InventoryAnimRenderBox (Ego_r.pdb +0xD0) sits at +0xBC in the
//         shipped class (five 16-byte PDB vectors ahead of it are 12 bytes each).
// Shape: a plain assignment of the 16-byte POD box (two C2DVector corners): VC forms the member address once
//        (`lea eax,[ecx+0BCh]`) and copies the four dwords through one register.

struct C2DVector {
    float X;                                      // +0x00
    float Y;                                      // +0x04
};

struct C2DBoxF {
    C2DVector TopLeft;                            // +0x00
    C2DVector BottomRight;                        // +0x08
};

struct CTCInventoryBase_Retail {
    unsigned char _pad_0x00[0xBC];
    C2DBoxF       InventoryAnimRenderBox;         // +0xBC (PDB +0xD0)

    void SetInventoryAnimRenderBox(const C2DBoxF& box);
};

void CTCInventoryBase_Retail::SetInventoryAnimRenderBox(const C2DBoxF& box)
{
    InventoryAnimRenderBox = box;
}

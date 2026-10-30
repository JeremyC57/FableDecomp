// NUISystem::CIconText::GetAlignment() const -- the text alignment from the component flag bits (centred, right, else left)
// Leaf: ?GetAlignment@CIconText@NUISystem@@UBE?AW4ETextAlignement@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00543B10, 26 bytes, virtual, plain `ret`. No callees.
// Identity: NUISystem::CIconText vtable 0x012470CC slot 151 (rtti-vtables); FableWin.pdb
//   ?GetAlignment@CIconText@NUISystem@@UBE?AW4ETextAlignement@2@XZ. Byte twin of the landed CText::GetAlignment 0x0054FFF0
//   (CText vtable slot 150). The parked name "GetTextAlignment" was invented.
// Shape: as 0x0054FFF0 -- bit 4 of FlagsSetTwo returns the centred value 1 at once; otherwise bit 5 selects 2 or 0, folded
//   branchless into `movzx eax,al ; shr eax,4 ; and eax,2`. Two separate `if` tests keep the movzx (a ternary over the
//   right-aligned bit drops it, 23v26). Layout: retail CComponent FlagsSetTwo +0x12e (landed 0x0052EC20 model).
namespace NUISystem {
enum ETextAlignement { TEXT_ALIGN_LEFT = 0, TEXT_ALIGN_CENTRE = 1, TEXT_ALIGN_RIGHT = 2 };

class CIconText {
public:
    virtual ETextAlignement GetAlignment() const;             // slot 151 (+0x25c)
    unsigned char _pad_0x04[0x12e - 0x04];
    unsigned char FlagsSetTwo;                                // +0x12e
    bool IsTextCentred() const { return (FlagsSetTwo & 0x10) != 0; }
    bool IsTextRightAligned() const { return (FlagsSetTwo & 0x20) != 0; }
};

ETextAlignement CIconText::GetAlignment() const
{
    if (IsTextCentred())
        return TEXT_ALIGN_CENTRE;
    if (IsTextRightAligned())
        return TEXT_ALIGN_RIGHT;
    return TEXT_ALIGN_LEFT;
}
}
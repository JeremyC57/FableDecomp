// NInventory::CTCInventoryBase::ProcessButtonXReleased() -- empty base handler
// Leaf: ?ProcessButtonXReleased@CTCInventoryBase@NInventory@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC68F, 1 byte; vtable 0x01253DBC slot 60 (Ego_r.exe 0x0061F503).
// Retail bytes:  ret
// Name: Ego_r.exe defines the CTCInventoryBase input handlers as one run -- ProcessInventoryInputLeft, ...Right, then per button
// A/B/X/Y Pressed, Held, Released, then ProcessDPadUp/Down/Left/RightReleased (0x0061F4AC..0x0061F51F) -- and retail has the
// same run with the same sizes (0x005BC638..0x005BC6AB); the vtable 0x01253DBC (.?AVCTCInventoryBase@NInventory@@) puts
// Pressed in slots 50-53, Held 54-57, Released 58-61 (A, B, X, Y) and the D-pad releases in 62-65 (Ego_r.pdb names all of
// them ?<name>@CTCInventoryBase@NInventory@@UAEXXZ).
// Local model: no engine header for CTCInventoryBase; only the slots matter.
namespace NInventory {

class CTCInventoryBase {
public:
    virtual ~CTCInventoryBase();
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual void _v05();
    virtual void _v06();
    virtual void _v07();
    virtual void _v08();
    virtual void _v09();
    virtual void _v10();
    virtual void _v11();
    virtual void _v12();
    virtual void _v13();
    virtual void _v14();
    virtual void _v15();
    virtual void _v16();
    virtual void _v17();
    virtual void _v18();
    virtual void _v19();
    virtual void _v20();
    virtual void _v21();
    virtual void _v22();
    virtual void _v23();
    virtual void _v24();
    virtual void _v25();
    virtual void _v26();
    virtual void _v27();
    virtual void _v28();
    virtual void _v29();
    virtual void _v30();
    virtual void _v31();
    virtual void _v32();
    virtual void _v33();
    virtual void _v34();
    virtual void _v35();
    virtual void _v36();
    virtual void _v37();
    virtual void ProcessInventoryInputUp();                // slot 38 (+0x98)
    virtual void ProcessInventoryInputDown();              // slot 39 (+0x9c)
    virtual void ProcessInventoryInputLeft();              // slot 40 (+0xa0)
    virtual void ProcessInventoryInputRight();             // slot 41 (+0xa4)
    virtual void _v42();
    virtual void _v43();
    virtual void _v44();
    virtual void _v45();
    virtual void _v46();
    virtual void _v47();
    virtual void _v48();
    virtual void _v49();
    virtual void ProcessButtonAPressed();                  // slot 50 (+0xc8)
    virtual void ProcessButtonBPressed();                  // slot 51 (+0xcc)
    virtual void ProcessButtonXPressed();                  // slot 52 (+0xd0)
    virtual void ProcessButtonYPressed();                  // slot 53 (+0xd4)
    virtual void ProcessButtonAHeld();                     // slot 54 (+0xd8)
    virtual void ProcessButtonBHeld();                     // slot 55 (+0xdc)
    virtual void ProcessButtonXHeld();                     // slot 56 (+0xe0)
    virtual void ProcessButtonYHeld();                     // slot 57 (+0xe4)
    virtual void ProcessButtonAReleased();                 // slot 58 (+0xe8)
    virtual void ProcessButtonBReleased();                 // slot 59 (+0xec)
    virtual void ProcessButtonXReleased();                 // slot 60 (+0xf0)
    virtual void ProcessButtonYReleased();                 // slot 61 (+0xf4)
    virtual void ProcessDPadUpReleased();                  // slot 62 (+0xf8)
    virtual void ProcessDPadDownReleased();                // slot 63 (+0xfc)
    virtual void ProcessDPadLeftReleased();                // slot 64 (+0x100)
    virtual void ProcessDPadRightReleased();               // slot 65 (+0x104)
};

} // namespace NInventory

void NInventory::CTCInventoryBase::ProcessButtonXReleased()
{
}

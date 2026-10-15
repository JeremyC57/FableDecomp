// NGraphicAppearance::CPrimitive::CPrimitive(EPrimitiveType, long) -- construct a graphic-appearance primitive: its type, an empty engine primitive handle and its bank index
// Leaf: ??0CPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00642390, length 47 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (vtable 0x01231710). Vtable 0x0122F598 = CEnginePrimitiveHandle
// (RTTI). Ego_r.pdb ??0CPrimitive@NGraphicAppearance@@QAE@W4EPrimitiveType@1@J@Z.
// Layout (Ego_r.pdb, 16 bytes): Type +0, PrimitiveHandle (CEnginePrimitiveHandle: vptr + data pointer) +4,
// BankIndex +0xc.
// Shape: Type stored after the handle's `lea` (init-list order with scheduling), the handle's inline
// constructor = base call + vtable + zeroed pointer, BankIndex last.
struct CBaseClass {                                         // vtable 0x01231710
    CBaseClass();                                           // 0x0099A2D0
    virtual void v00();
};

struct CEnginePersistentPrimitiveBase;
struct CEnginePrimitiveHandle : CBaseClass {                // vtable 0x0122F598, 8 bytes
    CEnginePersistentPrimitiveBase* Data;                   // +0x04 (CIVCountedPointer)
    CEnginePrimitiveHandle() : Data(0) { }
    virtual void v00();
};

namespace NGraphicAppearance {

struct CPrimitive {
    enum EPrimitiveType { PRIMITIVE_TYPE_0 };
    EPrimitiveType         Type;                            // +0x00
    CEnginePrimitiveHandle PrimitiveHandle;                 // +0x04
    long                   BankIndex;                       // +0x0c
    CPrimitive(EPrimitiveType type, long bankIndex);
};

CPrimitive::CPrimitive(EPrimitiveType type, long bankIndex)
    : Type(type), BankIndex(bankIndex)
{
}

}

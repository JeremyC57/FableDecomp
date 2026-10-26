#pragma optimize("s", on)
// CQuickAccessItemButtonGuiDef::CQuickAccessItemButtonGuiDef(const CQuickAccessItemButtonGuiDef&) -- copy constructor: the base definition copy, own vtable, then ButtonGraphic, Position and Offset copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046EE2D, length 57 bytes.
// Callees: the CParentDefClassBase copy constructor @ 0x0046EE98 (manifest; out of line, right after this body).
// Vtable 0x012360A4 = CQuickAccessItemButtonGuiDef (rtti-vtables.tsv: slot 19 Copy @ 0x0046EF87, slot 20
// GetSizeofClass @ 0x0046EE66, which returns 0x50 = the retail size).
// Layout: PDB CQuickAccessItemButtonGuiDef (84 bytes) ButtonGraphic +0x40, Position (C2DVector) +0x44, Offset
// (C2DVector) +0x4c; the retail base is 0x3c bytes (4 smaller), so +0x3c, +0x40, +0x48 here.
// Shape: a size-optimised TU (unaligned entry): every dword is copied through eax in declaration order after the
// vtable store; `mov eax,esi` returns `this`.
class C2DVector { public:
    float X, Y;
};

class CParentDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x3c - 4];
    CParentDefClassBase(const CParentDefClassBase& other);   // 0x0046EE98
};

class CQuickAccessItemButtonGuiDef : public CParentDefClassBase { public:   // vtable 0x012360A4, retail sizeof 0x50
    long      ButtonGraphic;                    // +0x3c
    C2DVector Position;                         // +0x40
    C2DVector Offset;                           // +0x48
    virtual void v00();
    CQuickAccessItemButtonGuiDef(const CQuickAccessItemButtonGuiDef& other);
};

CQuickAccessItemButtonGuiDef::CQuickAccessItemButtonGuiDef(const CQuickAccessItemButtonGuiDef& other)
    : CParentDefClassBase(other)
    , ButtonGraphic(other.ButtonGraphic)
    , Position(other.Position)
    , Offset(other.Offset)
{
}
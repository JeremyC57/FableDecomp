// NUISystem::CMouseCursor::GetPosition() const -- the cursor position: the sprite position, copied out by value
// Leaf: GetPosition
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055C5D0, 34 bytes (+ int3 padding; the 48-byte catalog row includes 14 of it),
//   `ret 4` (__fastcall, hidden C2DVector result pointer returned in eax).
// Label: the catalog name "CPersistTraits<EInputKey>::TransferIn" is propagated and wrong: the only reference is slot 16
//   (+0x40) of the NUISystem::CMouseCursor vtable 0x0124C22C, the slot landed UI code calls GetPosition() const
//   (C2DVector by hidden pointer, e.g. 0x00544520); the base slot is NUISystem::CSprite::GetPosition @ 0x0052E890
//   (returns the C2DVector at +0x34).
// Callees: NUISystem::CSprite::GetPosition() const @ 0x0052E890 (a qualified, non-virtual call).
// Shape: the base result lands in a named local (its own frame slot), then the result is copied out member-wise through
//   C2DVector's user copy constructor (X via fld/fstp, Y through a GPR). Speed-optimised (default) TU.
class C2DVector { public:
    float X;
    float Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) {}
};

namespace NUISystem {
class CSprite { public:
    virtual void v00();
    virtual C2DVector GetPosition() const;          // slot 16 in the full vtable; 0x0052E890
};
class CMouseCursor : public CSprite { public:
    virtual C2DVector GetPosition() const;
};

C2DVector CMouseCursor::GetPosition() const
{
    C2DVector position = CSprite::GetPosition();
    return position;
}
}
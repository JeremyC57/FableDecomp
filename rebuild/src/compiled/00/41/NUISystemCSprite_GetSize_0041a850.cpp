// NUISystem::CSprite::GetSize() const -- the sprite's on-screen size: an explicit pixel width/height when set, else the component's base size scaled per axis
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041A850, 115 bytes; `ret 4` (hidden C2DVector result). No callees.
// Layout: retail NUISystem::CSprite = engine/CSprite.h offsets -0x30 for the sprite's own members (Width +0x168, Height +0x16c;
//         PDB +0x198/+0x19c); the component's scale (+0x5c) and base size (+0xcc) are named descriptively.
// Shape: the base size comes from an inline by-value getter (X stays on the FPU, Y is copied to the frame); an unsigned
//        width converts with `fild ; jge ; fadd [2^32]`; the result is built by a two-float constructor.
class C2DVector { public:
    float X;
    float Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
namespace NUISystem {
class CComponent { public:
    virtual void v00();
    unsigned char _pad004[0x58];
    C2DVector     Scale;                           // +0x5c
    unsigned char _pad064[0x68];
    C2DVector     BaseSize;                        // +0xcc
    C2DVector GetBaseSize() const { return BaseSize; }
};
class CSprite : public CComponent { public:
    unsigned char _pad0d4[0x94];
    unsigned long Width;                           // +0x168
    unsigned long Height;                          // +0x16c
    virtual C2DVector GetSize() const;
};

C2DVector CSprite::GetSize() const
{
    C2DVector size = GetBaseSize();
    float x;
    if (Width)
        x = (float)Width;
    else
        x = Scale.X * size.X;
    float y;
    if (Height)
        y = (float)Height;
    else
        y = Scale.Y * size.Y;
    return C2DVector(x, y);
}
}
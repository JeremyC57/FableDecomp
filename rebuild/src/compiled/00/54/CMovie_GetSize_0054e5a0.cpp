// NUISystem::CViewport::GetSize() const (was modelled as CMovie::GetSize) -- the size (+0xcc / +0xd0) scaled by the factors at +0x5c / +0x60, returned as a C2DVector by hidden pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054E5A0, length 48 bytes.
// Leaf: ?GetSize@CViewport@NUISystem@@UBE?AVC2DVector@@XZ
// Identity (NF385): retail CViewport vftable 0x01249A5C slot 23 and CScrollingViewport 0x012468D4 slot 23 = FableWin slot 23
// ?GetSize@CViewport@NUISystem@@UBE?AVC2DVector@@XZ; both PDB name sets (ui_viewport.obj). (0x00547BE0 is CMovie::GetSize.)
// Shape: the size copied into the frame (its Y word through an integer register), each axis multiplied by its factor, `fxch` then the two stores into the result, `ret 4`.
class C2DVector { public:
    float X;
    float Y;
    C2DVector() { }
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};

namespace NUISystem {
struct CViewport {
    // +0x00 vfptr
    unsigned char _pad04[0x58];
    C2DVector     m_scale;                         // +0x5c
    unsigned char _pad64[0x68];
    C2DVector     m_size;                          // +0xcc
    virtual C2DVector GetSize() const;
};
}

C2DVector NUISystem::CViewport::GetSize() const
{
    C2DVector size = m_size;
    size.X = size.X * m_scale.X;
    size.Y = size.Y * m_scale.Y;
    return size;
}
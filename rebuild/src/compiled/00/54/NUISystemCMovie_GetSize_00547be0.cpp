// NUISystem::CMovie::GetSize() const -- the movie's size (+0xcc / +0xd0) scaled by the factors at +0x5c / +0x60, returned as a C2DVector by hidden pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00547BE0, length 48 bytes.
// Leaf: GetSize
// Shape: the size copied into the frame (its Y word through an integer register), each axis multiplied by its factor, `fxch` then the two stores into the result, `ret 4`.
class C2DVector { public:
    float X;
    float Y;
    C2DVector() { }
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};

struct CMovie {
    unsigned char _pad00[0x5c];
    C2DVector     m_scale;                         // +0x5c
    unsigned char _pad64[0x68];
    C2DVector     m_size;                          // +0xcc
    C2DVector GetSize() const;
};

C2DVector CMovie::GetSize() const
{
    C2DVector size = m_size;
    size.X = size.X * m_scale.X;
    size.Y = size.Y * m_scale.Y;
    return size;
}
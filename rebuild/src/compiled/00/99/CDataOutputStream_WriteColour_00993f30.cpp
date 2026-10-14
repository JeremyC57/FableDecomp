// CDataOutputStream::WriteColour(const CRGBColour&) -- write the four colour bytes through the virtual Write(const void*, long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00993F30, length 46 bytes.
// Callees: virtual slot 4 Write(const void*, long) on `this`.
// Shape: the colour is copied byte by byte (R, G, B, A: the engine's member-wise copy constructor of the 4-byte
// CRGBColour union) into a local that VC homes in the dead argument slot (`[esp+4..7]`, re-addressed as `[esp+8]`
// after `push 4`), and the local's address is passed. PDB: CRGBColour is a 4-byte union; the bytes are named here.
struct CRGBColour {
    unsigned char R, G, B, A;
    CRGBColour(const CRGBColour& other) : R(other.R), G(other.G), B(other.B), A(other.A) { }
};

struct CDataOutputStream {                     // engine/CDataOutputStream.h: vptr +0, NewLine +4
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length);   // slot 4
    void* NewLine_PStringData;                 // +0x04
    void WriteColour(const CRGBColour& colour);
};

void CDataOutputStream::WriteColour(const CRGBColour& colour)
{
    CRGBColour copy(colour);
    Write(&copy, 4);
}
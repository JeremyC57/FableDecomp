// CDataOutputStream::Write2DVector(const C2DVector&) -- write the X then Y floats, each through the inline WriteFloat (a float copy passed to the virtual Write(const void*, long))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00993F90, length 50 bytes.
// Callees: virtual slot 4 Write(const void*, long) on `this` (twice).
// Shape: WriteFloat(float) is inlined: its by-value parameter becomes a dword local that VC homes in the dead
// argument slot (`mov [esp+0Ch],eax` before the first push, `[esp+10h]` after `push 4`), and the vector pointer
// stays in edi across the first call for the Y read. The 3D sibling @ 0x00993FD0 adds a third component.
struct C2DVector {
    float X, Y;
};

struct CDataOutputStream {                     // engine/CDataOutputStream.h: vptr +0, NewLine +4
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length);   // slot 4
    void* NewLine_PStringData;                 // +0x04
    void WriteFloat(float value) { Write(&value, 4); }
    void Write2DVector(const C2DVector& vector);
};

void CDataOutputStream::Write2DVector(const C2DVector& vector)
{
    WriteFloat(vector.X);
    WriteFloat(vector.Y);
}
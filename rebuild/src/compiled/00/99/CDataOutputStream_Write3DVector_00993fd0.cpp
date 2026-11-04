// CDataOutputStream::Write3DVector(const C3DVector&) -- write the X, Y then Z floats, each through the inline WriteFloat (a float copy passed to the virtual Write(const void*, long))
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00993FD0, length 73 bytes.
// Callees: virtual slot 4 Write(const void*, long) on `this` (three times).
// Shape: WriteFloat(float) is inlined: its by-value parameter becomes a dword local that VC homes in the dead
// argument slot (`mov [esp+0Ch],eax` before the first push, `[esp+10h]` after `push 4`), and the vector pointer
// stays in edi across the calls for the Y and Z reads. The 2D sibling @ 0x00993F90 writes two components.
#include "engine/C3DVector.h"

class CDataOutputStream { public:                     // engine/CDataOutputStream.h: vptr +0, NewLine +4
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length);   // slot 4
    void* NewLine_PStringData;                 // +0x04
    void WriteFloat(float value) { Write(&value, 4); }
    void Write3DVector(const C3DVector& vector);
};

void CDataOutputStream::Write3DVector(const C3DVector& vector)
{
    WriteFloat(vector.X);
    WriteFloat(vector.Y);
    WriteFloat(vector.Z);
}
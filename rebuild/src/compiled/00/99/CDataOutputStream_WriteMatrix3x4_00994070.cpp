// CDataOutputStream::WriteMatrix3x4(const CMatrix3x4&) -- writes the three basis rows and position as twelve float records
// Retail: Fable.exe (TLC, VC7.1), @ 0x00994070, 262 bytes, ret 4.
// Callee: virtual Write(const void*, long), slot 4 at +0x10, twelve calls.
// Layout: uses the production stream header and the engine's 3x4 matrix field order; each float is copied into a
// four-byte local before the stream's virtual write call, matching the retail serialization order.
// The generated header is a flattened layout view (raw vptr + string pointer). Rename it while including
// so the real polymorphic API can be declared using the batch-109 slot evidence below.
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream
#include "fable_string.h"

class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public: C3DVector XAxis, YAxis, ZAxis, Position; };

class CBaseClass { public:
    virtual ~CBaseClass();                         // RTTI base vtable 0x01231710
};

struct CBaseClassNonCopyable : CBaseClass {
    virtual ~CBaseClassNonCopyable();              // destructor restores CBaseClass vtable, 0x0099A300
};

struct CDataStream : CBaseClassNonCopyable {
    virtual ~CDataStream();                        // vtable 0x0129A69C, destructor 0x00996EA0
    virtual void SetPosition(unsigned long position) = 0; // slot 1
    virtual unsigned long GetPosition() const = 0;        // slot 2
    virtual void Close() = 0;                             // slot 3
};

class CDataOutputStream : public CDataStream { public:
    virtual ~CDataOutputStream();                  // slot 0, retail destructor 0x00996F00
    CCharString NewLine;                           // +0x04, actual CCharString member
    virtual void Write(const void* data, long length) = 0; // slot 4
    void WriteFloat(float value) { Write(&value, 4); }
    void WriteMatrix3x4(const CMatrix3x4& matrix);
};

void CDataOutputStream::WriteMatrix3x4(const CMatrix3x4& matrix)
{
    WriteFloat(matrix.XAxis.X);
    WriteFloat(matrix.XAxis.Y);
    WriteFloat(matrix.XAxis.Z);
    WriteFloat(matrix.YAxis.X);
    WriteFloat(matrix.YAxis.Y);
    WriteFloat(matrix.YAxis.Z);
    WriteFloat(matrix.ZAxis.X);
    WriteFloat(matrix.ZAxis.Y);
    WriteFloat(matrix.ZAxis.Z);
    WriteFloat(matrix.Position.X);
    WriteFloat(matrix.Position.Y);
    WriteFloat(matrix.Position.Z);
}
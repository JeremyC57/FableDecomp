// CFileDataOutputStream::Write(const void* data, long length) -- forward to the file: File->vslot4(data, length, 0)
// Leaf: ?Write@CFileDataOutputStream@@UAEXPBXJ@Z
// Naming wave 27 (family fdosw, DECISIONS_W26 D2): CFileDataOutputStream is the polymorphic retail class (engine/
//   CFileDataOutputStream.h FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM: vtable 0x0129A6F8, Write slot 4 UAE, ~ UAE). Was a free __fastcall Write(CFileDataOutputStream*, int, const void*, long).
// Retail: Fable.exe @ 0x00994480 (vtable 0x0129A6F8 slot 4); the file's slot 4 with the two arguments and 0, a tail call.
#include "engine/CBaseClassNonCopyable.h"                       // : CBaseClass; ~ slot 0 0x0099A300
class CAFile : public CBaseClassNonCopyable { public:           // RTTI .?AVCAFile@@
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4(const void* p, long n, int zero);           // slot 4 (+0x10)
};
class CDataStream : public CBaseClassNonCopyable { public:      // RTTI .?AVCDataStream@@ (vtable 0x0129A69C)
    virtual ~CDataStream();                                     // ??1CDataStream@@UAE@XZ
};
class CDataOutputStream : public CDataStream { public:          // vtable 0x0129A6E0 (slots 1-4 _purecall)
    void* NewLine;                                              // +0x04 CCharString NewLine (opaque here)
    virtual void SetPosition(unsigned long position) = 0;      // slot 1
    virtual unsigned long GetPosition() const = 0;              // slot 2
    virtual void Close() = 0;                                   // slot 3
    virtual void Write(const void* data, long length) = 0;      // slot 4
};
#define FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM
#include "engine/CFileDataOutputStream.h"                      // the retail layout (vtable 0x0129A6F8)

void CFileDataOutputStream::Write(const void* data, long length)
{
    File->v4(data, length, 0);
}

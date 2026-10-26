// CFileDataInputStream::`scalar deleting destructor'(unsigned int) -- stores the CFileDataInputStream vtable, closes the stream, the inlined CDataStream destructor stores its vtable, then CBase::~CBase and a free on bit 0
// Leaf: ??_GCFileDataInputStream
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009971A0, length 49 bytes (vtable 0x0129A728 slot 0).
// Callees: CFileDataInputStream::Close @ 0x00994300 (virtual, called directly from the destructor),
// CBase::~CBase @ 0x0099A300, operator delete @ 0x00BFE9BC.
// The catalog label "CThingTrackNode::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the CFileDataInputStream vtable 0x0129A728.
// Layout: engine/CFileDataInputStream.h is a flat struct (vptr, opaque +0x04..+0x17, File +0x18,
// Buffer +0x1c, BufferSize +0x20); the class is modelled locally as a polymorphic CDataStream over
// the same offsets. Shape: the destructor body calls the virtual Close(), which VC7.1 binds
// statically inside a destructor; the intermediate CDataStream's inline destructor stores its
// vtable (0x0129A69C) before the root base's out-of-line destructor.
class CAFile;

namespace N_0099a300 { struct CBase {                                     // root polymorphic base
    CBase();
    virtual ~CBase();                              // 0x0099A300
}; } using N_0099a300::CBase;

struct CDataStream : CBase {                       // vtable 0x0129A69C
    CDataStream() { }
    virtual ~CDataStream() { }
};

class CFileDataInputStream : public CDataStream { public:        // vtable 0x0129A728
    unsigned char  _pad_0x04[0x14];                // +0x04
    CAFile*        File;                           // +0x18
    unsigned char* Buffer;                         // +0x1c
    long           BufferSize;                     // +0x20
    CFileDataInputStream() : File(0), Buffer(0), BufferSize(0) { }
    virtual void Close();                          // 0x00994300
    __forceinline virtual ~CFileDataInputStream() { Close(); }
};

CFileDataInputStream* Create_CFileDataInputStream()
{
    return new CFileDataInputStream;
}
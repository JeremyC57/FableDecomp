// CFileDataInputStream::~CFileDataInputStream() -- closes the file, then unwinds to CDataStream/CBaseClass
// Leaf: ??1CFileDataInputStream@
// Retail: Fable.exe @ 0x00994780, 28 bytes. Stores the CFileDataInputStream vptr (0x0129A728), calls Close() directly
// (slot 3, 0x00994300), stores the CDataStream vptr (0x0129A69C; the intermediate CDataInputStream has an implicit dtor,
// so no store of its own), then tail-jumps to the out-of-line ~CBaseClass (0x0099A300, vptr 0x01231710).
// Layout: engine/CFileDataInputStream.h / CDataInputStream.h are flat data-only headers; the destructor needs the
// polymorphic hierarchy (rtti-vtables.tsv), so the classes are modelled locally with the PDB data members.
class CAFile;
class CBaseClass {
public:
    virtual ~CBaseClass();                               // 0x0099A300 (folded body)
};
class CDataStream : public CBaseClass {
public:
    virtual ~CDataStream() {}
    virtual void _slot01();
    virtual void _slot02();
    virtual void Close() = 0;                            // slot 3
};
class CDataInputStream : public CDataStream {
public:
    unsigned long  StreamPos;                            // +0x04
    long           StreamSize;                           // +0x08
    unsigned char* CurrentSourcePtr;                     // +0x0c
    unsigned long  SourceChunkStreamPos;                 // +0x10
    long           SourceChunkBytesRemaining;            // +0x14
};
class CFileDataInputStream : public CDataInputStream {
public:
    CAFile*        File;                                 // +0x18
    unsigned char* Buffer;                               // +0x1c
    long           BufferSize;                           // +0x20
    virtual ~CFileDataInputStream();
    virtual void Close();                                // slot 3, 0x00994300
};
CFileDataInputStream::~CFileDataInputStream()
{
    Close();
}
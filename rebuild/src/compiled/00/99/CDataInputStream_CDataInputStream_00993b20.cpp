#pragma optimize("s",on)
// CDataInputStream::CDataInputStream() -- empty input stream: position, size, source pointer and chunk counters all zero
// Retail: Fable.exe @ 0x00993B20, 35 bytes; Ego_r.pdb ??0CDataInputStream@@QAE@XZ. Calls the out-of-line base ctor
// 0x0099A2F0 (CBaseClass level, identical-code folded with CBaseClassNonCopyable's), zeroes +0x04..+0x14, then stores
// the CDataInputStream vptr 0x0129A6B0 (the intermediate CDataStream store is dead and dropped).
// Layout: engine/CDataInputStream.h is a flat data-only header; the ctor needs the polymorphic hierarchy
// (CBaseClass <- CDataStream <- CDataInputStream, as 0x00994780), so the classes are modelled locally with the PDB
// data members (Ego_r.pdb, 24 bytes).
class CBaseClass {
public:
    CBaseClass();                                        // 0x0099A2F0 (folded body)
    virtual ~CBaseClass();
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
    CDataInputStream();
    unsigned long  StreamPos;                            // +0x04
    long           StreamSize;                           // +0x08
    unsigned char* CurrentSourcePtr;                     // +0x0c
    unsigned long  SourceChunkStreamPos;                 // +0x10
    long           SourceChunkBytesRemaining;            // +0x14
};
CDataInputStream::CDataInputStream()
    : StreamPos(0), StreamSize(0), CurrentSourcePtr(0), SourceChunkStreamPos(0), SourceChunkBytesRemaining(0)
{
}
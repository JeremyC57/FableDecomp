// CCompressedMemoryDataInputStream::CCompressedMemoryDataInputStream(const void* source, long sourceSize, long bufferSize) -- build the stream state, create the zlib decompressor on the source block, allocate the read buffer and reset the stream position
// Leaf: ??0CCompressedMemoryDataInputStream
// Retail: Fable.exe (TLC, VC7.1) @ 0x00996990, 156 bytes (176 with padding), `ret 0xc`.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0, operator new 0x00BFEA1A (0x58-byte CCompressorZlib),
//   CCompressorZlib::GetWorkspaceSize 0x00A3A7D0, CACompressor::CACompressor(long) 0x00A3AEA0, CCompressorZlib::Init 0x00A3AA10,
//   CCompressorZlib::SetStreamInput 0x00A3AB90 (manifest label), operator new[] 0x00BFEB22; the old compressor is deleted
//   through vtable slot 0 (flag 1). Vtables: 0x0129A758 CCompressedMemoryDataInputStream, 0x01245974 CCompressorZlib.
// Layout: engine/CCompressedMemoryDataInputStream.h (Buffer +0x18, BufferSize +0x1c, Compressor scoped_ptr +0x20) with the
//   CDataInputStream state names (+4..+0x14); modelled locally like the landed ctor 0x00995DC0 (the header is data only).
// Shape: the scoped_ptr reset stores the new pointer before deleting the old one; the trailing ResetStream() is the
//   inline position reset shared with OpenMemorySource (0x00996A40).
void* __cdecl operator new[](unsigned int size);   // MSVCR71 import thunk 0x00BFEB22 (the lane <new> lacks it)
struct CACompressor {                                           // landed 0x00A3AEA0 (vtable 0x0129B424)
    virtual ~CACompressor();                                    // slot 0: scalar deleting destructor (`delete` with flag 1)
    unsigned char _body[0x1c - 4];
    CACompressor(long workspaceSize);                           // 0x00A3AEA0
};

struct CCompressorZlib : CACompressor {                         // vtable 0x01245974, retail 0x58 bytes
    unsigned char _zlib[0x58 - 0x1c];
    static long GetWorkspaceSize();                             // 0x00A3A7D0
    void Init();                                                // 0x00A3AA10
    void SetStreamInput(const void* source, long sourceSize);   // 0x00A3AB90
    virtual ~CCompressorZlib();
    CCompressorZlib() : CACompressor(GetWorkspaceSize()) { Init(); }
};

struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                                    // 0x0099A2F0
};

struct CDataInputStream : CBaseClassNonCopyable {               // CDataStream/CDataInputStream state (engine/CDataInputStream.h names)
    unsigned long StreamPos;                                    // +0x04
    long          StreamSize;                                   // +0x08
    void*         CurrentSourcePtr;                             // +0x0c
    unsigned long SourceChunkStreamPos;                         // +0x10
    long          SourceChunkBytesRemaining;                    // +0x14
    virtual void v00();
    CDataInputStream() : StreamPos(0), StreamSize(0), CurrentSourcePtr(0), SourceChunkStreamPos(0), SourceChunkBytesRemaining(0) { }
    __forceinline void ResetStream()
    {
        StreamPos = 0;
        SourceChunkStreamPos = 0;
        CurrentSourcePtr = 0;
        SourceChunkBytesRemaining = 0;
        StreamSize = -1;
    }
};

template<class T> struct CScopedPointer {                       // scoped_ptr<T>: one owning pointer
    T* ptr;
    CScopedPointer() : ptr(0) { }
    void reset(T* p)
    {
        T* old = ptr;
        ptr = p;
        if (old)
            delete old;
    }
    T* operator->() const { return ptr; }
};

struct CCompressedMemoryDataInputStream_Retail : CDataInputStream {
    unsigned char*                  Buffer;                     // +0x18
    long                            BufferSize;                 // +0x1c
    CScopedPointer<CCompressorZlib> Compressor;                 // +0x20
    virtual void v00();                                         // own vtable 0x0129A758
    CCompressedMemoryDataInputStream_Retail(const void* source, long sourceSize, long bufferSize);
    void OpenMemorySource(const void* source, long sourceSize, long bufferSize);
};

CCompressedMemoryDataInputStream_Retail::CCompressedMemoryDataInputStream_Retail(const void* source, long sourceSize, long bufferSize)
{
    Compressor.reset(new CCompressorZlib);
    Compressor->SetStreamInput(source, sourceSize);
    Buffer = new unsigned char[bufferSize];
    BufferSize = bufferSize;
    ResetStream();
}
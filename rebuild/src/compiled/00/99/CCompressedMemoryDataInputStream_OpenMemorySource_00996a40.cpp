// CCompressedMemoryDataInputStream::OpenMemorySource(const void* source, long sourceSize, long bufferSize) -- (re)open the stream on a compressed memory block: create the zlib decompressor once, resize the read buffer when its size changes, set the source and reset the position
// Leaf: OpenMemorySource
// Retail: Fable.exe (TLC, VC7.1) @ 0x00996A40, 156 bytes (176 with padding), `ret 0xc`.
// Callees: operator new 0x00BFEA1A (0x58-byte CCompressorZlib), CCompressorZlib::GetWorkspaceSize 0x00A3A7D0,
//   CACompressor::CACompressor(long) 0x00A3AEA0, CCompressorZlib::Init 0x00A3AA10, operator delete[] 0x00BFEB1C,
//   operator new[] 0x00BFEB22, CCompressorZlib::SetStreamInput 0x00A3AB90; old compressor deleted through vtable slot 0.
// Layout: engine/CCompressedMemoryDataInputStream.h (Buffer +0x18, BufferSize +0x1c, Compressor +0x20), local model as in
//   the sibling constructor 0x00996990.
// Shape: `delete[] Buffer` sits under the combined `Buffer && size changed` test (no second null test); the buffer is
//   re-tested through the member (`cmp [esi+0x18],0`) before reallocation.
void* __cdecl operator new[](unsigned int size);   // MSVCR71 import thunk 0x00BFEB22 (the lane <new> lacks it)
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)
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

void CCompressedMemoryDataInputStream_Retail::OpenMemorySource(const void* source, long sourceSize, long bufferSize)
{
    if (!Compressor.ptr)
        Compressor.reset(new CCompressorZlib);
    if (Buffer && BufferSize != bufferSize) {
        delete[] Buffer;
        Buffer = 0;
    }
    if (!Buffer) {
        Buffer = new unsigned char[bufferSize];
        BufferSize = bufferSize;
    }
    Compressor->SetStreamInput(source, sourceSize);
    ResetStream();
}
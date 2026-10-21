// CDataOutputStream::WriteZlibCompressed(const void* data, long size, long bufferSize) -- write a placeholder size word, zlib-compress the block through a scratch buffer (clamped to [1024, size + 1024] bytes) chunk by chunk, then seek back and patch the compressed byte count
// Leaf: WriteZlibCompressed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009967E0, length 400 bytes, `ret 0xc`.
// Callees: the stream's virtual SetPosition (slot 1, +4), GetPosition (slot 2, +8) and WriteData (slot 4, +0x10);
// vector<unsigned char>(n) @ 0x00514E20 (the zero-filling size constructor), CCompressorZlib::GetWorkspaceSize @ 0x00A3A7D0,
// CACompressor::CACompressor(long) @ 0x00A3AEA0, CCompressorZlib::Init @ 0x00A3AA10, ::SetStreamInput @ 0x00A3AB90,
// ::HasMoreInput @ 0x00A3A7E0 (label unverified: a 1-byte member test), ::CompressStream @ 0x00A3A7F0 (landed),
// ::FinishCompressStream @ 0x00A3A8C0 (landed), ~CCompressorZlib @ 0x00A3ACC0, free @ 0x00BFEA14.
// Layout: the stack CCompressorZlib is the retail 0x58-byte object (vtable 0x01245974) of the landed OpenMemorySource (0x00996A40);
// the byte buffer is the retail CArray with the GetData idiom (its own address when empty).
// Shape: the compressed size is written through the `bufferSize` PARAMETER (its home slot: the parameter is reassigned after the
// three-way clamp has chosen the chunk size, so the clamp itself stays in edi); `finished` is a frame byte cleared before the do/while.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14

struct CACompressor {                                        // landed 0x00A3AEA0 (vtable 0x0129B424)
    virtual ~CACompressor();
    unsigned char _body[0x1c - 4];
    CACompressor(long workspaceSize);                        // 0x00A3AEA0
};
struct CCompressorZlib : CACompressor {                      // vtable 0x01245974, retail 0x58 bytes
    unsigned char _zlib[0x58 - 0x1c];
    static long GetWorkspaceSize();                          // 0x00A3A7D0
    void Init();                                             // 0x00A3AA10
    void SetStreamInput(const void* source, long sourceSize);   // 0x00A3AB90
    bool HasMoreInput();                                     // 0x00A3A7E0
    long CompressStream(unsigned char* dest, long destSize); // 0x00A3A7F0
    long FinishCompressStream(unsigned char* dest, long destSize, bool* finished);   // 0x00A3A8C0
    virtual ~CCompressorZlib();                              // 0x00A3ACC0
    CCompressorZlib() : CACompressor(GetWorkspaceSize()) { Init(); }
};
struct CByteArray {                                          // retail vector<unsigned char>: begin/end/capacity
    unsigned char* m_begin; unsigned char* m_end; unsigned char* m_capacity;
    CByteArray(unsigned long n);                             // 0x00514E20
    ~CByteArray() { if (m_begin) free(m_begin); }
    unsigned char* GetData() { return m_end - m_begin != 0 ? m_begin : (unsigned char*)this; }
};

struct CDataOutputStream {
    virtual void v0();
    virtual void SetPosition(long position);                 // slot 1 (+4)
    virtual long GetPosition();                              // slot 2 (+8)
    virtual void v3();
    virtual void WriteData(const void* data, unsigned long size);   // slot 4 (+0x10)
    __forceinline void WriteLong(long value) { WriteData(&value, 4); }
    void WriteZlibCompressed(const void* data, long size, long bufferSize);
};

void CDataOutputStream::WriteZlibCompressed(const void* data, long size, long bufferSize)
{
    if (size > 0) {
        long sizePosition = GetPosition();
        WriteLong(0);
        if (bufferSize < 1024)
            bufferSize = 1024;
        else if (bufferSize > size + 1024)
            bufferSize = size + 1024;
        CByteArray buffer(bufferSize);
        CCompressorZlib compressor;
        compressor.SetStreamInput(data, size);
        long dataPosition = GetPosition();
        while (compressor.HasMoreInput()) {
            long written = compressor.CompressStream(buffer.GetData(), bufferSize);
            WriteData(buffer.GetData(), written);
        }
        bool finished = false;
        do {
            long written = compressor.FinishCompressStream(buffer.GetData(), bufferSize, &finished);
            WriteData(buffer.GetData(), written);
        } while (!finished);
        long endPosition = GetPosition();
        SetPosition(sizePosition);
        WriteLong(endPosition - dataPosition);
        SetPosition(endPosition);
    }
}
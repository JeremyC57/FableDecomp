// Grade: DIFFER(564v551) -- calls and block structure match; retail caches the constants 0 (esi) and 4 (ebp) in registers across the whole body (Read process argument, stream-source store, inline-Read size/compare), keeps the first symbol read in the dead filename argument slot and stores each pair from one start-pointer load; the lane uses immediates and a frame word (lane gap: constant register caching). Forms tried: ternary/conditional malloc, count copied to a register local, reference/assignment element stores.
// CCRCSymbolMap::LoadBinary(const CWideString& filename) -- read the whole file into a zero-terminated byte buffer,
//   stream it from memory: a count, then count {CRC, symbol} pairs into the CRC -> symbol map
// Leaf: LoadBinary
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A38C20, 551 bytes (560 with padding); `ret 4`.
// Callees: CDiskFileWin32 ctor 0x0099AD80 / Read 0x0099A960 / Close 0x0099A920, ~CWideString 0x0099B510, ~CBase
//   0x0099A300, malloc 0x00BFEA0E, free 0x00BFEA14, CDataInputStream ctor 0x00993B20, CMemoryDataInputStream
//   OpenMemorySource 0x00994570 / ReadWithSrcChunkOverflow 0x00993CA0 / dtor 0x00993B10 (vtable 0x0123675C),
//   CVectorMap<unsigned long, long>::resize 0x00A39AE0.
// Model: the landed sibling CCRCSymbolMap::SaveBinary 0x00A382E0 (local CDiskFileWin32) and the inline stream Read of
//   the landed CWoundMorphs::TransferBinaryIn 0x00757990.
// Layout: the engine header CMemoryDataInputStream.h names +0x0c/+0x14 by an older guess; retail reads the chunk
//   pointer at +0x0c and the bytes left at +0x14 (as the landed TransferBinaryIn model).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memcpy, memset)
struct CBase {
    CBase();                                       // 0x0099A2F0
    virtual ~CBase();                              // 0x0099A300 (CBase owns the vptr)
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                // 0x0099B510
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
struct CASuspendableProcess;
struct CDiskFileWin32 : CBase {                    // vtable 0x0122D06C
    long        AccessType;                        // +0x04
    long        Position;                          // +0x08
    long        Length;                            // +0x0c
    void*       WinFileHandle;                     // +0x10
    bool        Opened;                            // +0x14
    CWideString Pathname;                          // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Close();                          // 0x0099A920
    virtual void Read(void* data, long size, CASuspendableProcess* process);   // 0x0099A960
};
struct CDataInputStream {
    virtual ~CDataInputStream();
    CDataInputStream();                            // 0x00993B20
};
struct CMemoryDataInputStream : CDataInputStream { // vtable 0x0123675C
    unsigned long        m_position;               // +0x04
    long                 StreamSize;               // +0x08
    const unsigned char* m_pData;                  // +0x0c current chunk read pointer
    long                 SourceChunkStreamPos;     // +0x10
    long                 m_remaining;              // +0x14 bytes left in the current chunk
    void*                StreamSource;             // +0x18
    CMemoryDataInputStream() : StreamSource(0) { }
    ~CMemoryDataInputStream();                     // 0x00993B10
    void OpenMemorySource(const void* source, unsigned long size);   // 0x00994570
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= m_remaining) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
struct CByteVector {                               // STLport vector<char> (12 bytes)
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    __forceinline CByteVector(unsigned long n, char value) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        if (n != 0)
            _M_start = (char*)malloc(n);
        _M_end_of_storage = _M_start + n;
        memset(_M_start, value, n);
        _M_finish = _M_start + n;
    }
    ~CByteVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    char* GetData() { return size() != 0 ? _M_start : (char*)this; }
};
struct CSymbolPair {
    unsigned long first;                           // CRC
    long          second;                          // symbol
};
struct CVectorMap {
    CSymbolPair* m_pFirst;                         // +0
    CSymbolPair* m_pLast;                          // +4
    CSymbolPair* m_pEnd;                           // +8
    void resize(unsigned long count);              // 0x00A39AE0
    CSymbolPair& operator[](unsigned int i) { return *(m_pFirst + i); }
};

class CCRCSymbolMap {
public:
    void*      __vftable;                          // +0x00
    CVectorMap SLONGMap;                           // +0x04
    void LoadBinary(const CWideString& filename);
};

void CCRCSymbolMap::LoadBinary(const CWideString& filename)
{
    CDiskFileWin32 file(filename, FILE_ACCESS_READ, 2);
    CByteVector buffer(file.Length + 1, 0);
    file.Read(buffer.GetData(), file.Length, 0);
    CMemoryDataInputStream stream;
    stream.OpenMemorySource(buffer.GetData(), buffer.size());
    long stored;
    stream.Read(&stored, sizeof(stored));
    long count = stored;
    SLONGMap.resize(count);
    for (long i = 0; i < count; ++i) {
        unsigned long crc;
        long symbol;
        stream.Read(&crc, sizeof(crc));
        stream.Read(&symbol, sizeof(symbol));
        CSymbolPair& entry = SLONGMap[i];
        entry.first = crc;
        entry.second = symbol;
    }
}

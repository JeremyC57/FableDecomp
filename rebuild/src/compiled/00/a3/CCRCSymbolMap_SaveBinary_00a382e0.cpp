// CCRCSymbolMap::SaveBinary(const CWideString& filename) -- serialise the CRC -> symbol map into a memory stream (count, then each key/value pair) and write the buffer to a new disk file
// Leaf: SaveBinary
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A382E0, 198 bytes (ret 4).
// Callees: CMemoryDataOutputStream::CMemoryDataOutputStream @ 0x00996AF0 / ~ @ 0x00996B30, CDataOutputStream::WriteSLONG
//          @ 0x00993EB0, CDiskFileWin32::CDiskFileWin32(const CWideString&, FileAccessType, ulong) @ 0x0099AD80 (access 4 =
//          write, flags 2), CMemoryDataOutputStream::GetSize @ 0x00994B40 / PeekData @ 0x00995E70, CDiskFileWin32::Write
//          @ 0x0099AA90, then the local file's inline destructor (vtable 0x0122D06C, Close @ 0x0099A920, ~CWideString
//          @ 0x0099B510, ~CBaseClassNonCopyable @ 0x0099A300). Manifest labels of 0x00994B40 / 0x00996B30 are propagated.
// Layout (Ego_r.pdb CCRCSymbolMap): SLONGMap CVectorMap<unsigned long, long> +4 (the retail vector {first +4, last +8}),
//          8-byte {key, value} pairs.
// Shape: size() is the inline (last - first) / 8 re-read every iteration; the file's Write arguments are evaluated right to left.
class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();                                       // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                              // 0x0099A300 (CBase owns the vptr)
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                // 0x0099B510
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CASuspendableProcess;
class CDiskFileWin32 : public CBaseClassNonCopyable { public:                    // vtable 0x0122D06C
    long        AccessType;                        // +0x04
    long        Position;                          // +0x08
    long        Length;                            // +0x0c
    void*       WinFileHandle;                     // +0x10
    bool        Opened;                            // +0x14
    CWideString Pathname;                          // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Close();                          // 0x0099A920
    virtual void Write(const void* data, long size, CASuspendableProcess* process);   // 0x0099AA90
};

class CDataOutputStream { public:
    virtual void v00();
    void WriteSLONG(long value);                   // 0x00993EB0
};
class CMemoryDataOutputStream : public CDataOutputStream { public:
    unsigned char _body[0x14];                     // +0x04 .. +0x17
    CMemoryDataOutputStream();                     // 0x00996AF0
    virtual ~CMemoryDataOutputStream();                    // 0x00996B30
    long GetSize();                                // 0x00994B40
    void* PeekData();                              // 0x00995E70
};

struct CSymbolPair {
    unsigned long first;                           // CRC
    long          second;                          // symbol
};
struct CVectorMap {
    CSymbolPair* m_pFirst;                         // +0
    CSymbolPair* m_pLast;                          // +4
    CSymbolPair* m_pEnd;                           // +8
    unsigned int size() const { return m_pLast - m_pFirst; }
    CSymbolPair& operator[](unsigned int i) { return m_pFirst[i]; }
};

class CCRCSymbolMap {
public:
    void*      __vftable;                          // +0x00
    CVectorMap SLONGMap;                           // +0x04
    void SaveBinary(const CWideString& filename);
};

void CCRCSymbolMap::SaveBinary(const CWideString& filename)
{
    CMemoryDataOutputStream stream;
    stream.WriteSLONG(SLONGMap.size());
    for (unsigned int i = 0; i < SLONGMap.size(); ++i) {
        stream.WriteSLONG(SLONGMap[i].first);
        stream.WriteSLONG(SLONGMap[i].second);
    }
    CDiskFileWin32 file(filename, FILE_ACCESS_WRITE, 2);
    file.Write(stream.PeekData(), stream.GetSize(), 0);
}
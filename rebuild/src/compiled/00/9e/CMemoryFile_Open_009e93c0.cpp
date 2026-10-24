// CMemoryFile::Open(void* data, long length) -- open the memory file over a copy of a caller buffer: clear the path name, rewind, set the length, mark it open, size the byte buffer to the length (zero fill) and copy the data in
// Leaf: ?Open@CMemoryFile@@QAEXPAXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E93C0, length 93 bytes. PDB: ?Open@CMemoryFile@@QAEXPAXJ@Z.
// Label correction: the catalog's "BinaryStream_LoadFromWide" is CMemoryFile::Open(void*, long) (`ret 8`).
// Callees: CWideString::operator=(const unsigned short*) @ 0x0099B800 (with L"" @ 0x0122D70C), vector<unsigned char>::resize(
//   size_type, const unsigned char&) @ 0x00486189.
// Layout: the landed CMemoryFile::Open(const CWideString&, ...) 0x009E94A0 local model: AccessType +4, Position +8, Length +0xc,
//   DataBuffer +0x10 (the 12-byte STLport vector), Opened +0x1c, Pathname +0x20 -- engine/CMemoryFile.h is the PDB layout with a
//   16-byte container, so it cannot be used.
// Shape: the four member stores written in place (the landed no-argument Open 0x009E90A0 does the same four with Length 0;
//   calling it inline schedules the Opened byte before the Length store), the zero fill value a byte temporary homed in the
//   dead `length` slot, then the inline memcpy (`rep movsd ; rep movsb`) into the buffer's data, selected by an inline
//   `GetData()` (`size() ? start : (T*)this`, the `jne` over `mov eax,esi`).
#include <string.h>
#include <stddef.h>

class CWideString {
public:
    void* PStringData;
    CWideString& operator=(const unsigned short* text);     // 0x0099B800
};

template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    void resize(unsigned long n, const T& value);       // 0x00486189
    T* GetData() { return size() ? _M_start : (T*)this; }
};

class CMemoryFile {
public:
    virtual ~CMemoryFile();
    long                  AccessType;                   // +0x04 FileAccessType
    long                  Position;                     // +0x08
    long                  Length;                       // +0x0c
    vector<unsigned char> DataBuffer;                   // +0x10
    bool                  Opened;                       // +0x1c
    CWideString           Pathname;                     // +0x20
    void Open(void* data, long length);
};

void CMemoryFile::Open(void* data, long length)
{
    Pathname = (const unsigned short*)L"";
    Position = 0;
    Length = length;
    Opened = true;
    DataBuffer.resize(length, 0);
    memcpy(DataBuffer.GetData(), data, length);
}
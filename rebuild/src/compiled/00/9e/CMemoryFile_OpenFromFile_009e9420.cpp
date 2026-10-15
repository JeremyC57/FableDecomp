// CMemoryFile::OpenFromFile(CAFile*, long, CASuspendableProcess*) -- load `length` bytes of a file into the memory buffer (rounded up to the file's block size) and adopt its pathname
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9420, 128 bytes (padding included).
// Shape: the file's pathname (CAFile vtable slot 11, a CWideString returned through a temporary built
// in the dead `file` argument slot) is assigned to Pathname (+0x20, operator= 0x0099B7D0) and the
// temporary destroyed (0x0099B510); Position = 0, Length = length, Opened = true; the buffer
// (+0x10, STLport vector<unsigned char>) is resized (0x00486189) to `length` rounded up to the
// block size from vtable slot 13 (unsigned div/imul), the fill value `unsigned char()` again in the dead
// argument slot; finally slot 3 reads `length` bytes into the buffer start with the process argument.
// Members per the PDB CMemoryFile (retail packs DataBuffer in 12 bytes: Opened +0x1c, Pathname +0x20).
#include <stddef.h>    // wchar_t

struct CWideStringData;
struct CASuspendableProcess;

struct CWideString {
    CWideStringData* PStringData;
    ~CWideString();                                 // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

struct CAFile {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void Read(void* buffer, long length, CASuspendableProcess* process);   // slot 3
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10();
    virtual CWideString GetPathname();              // slot 11
    virtual void v12();
    virtual unsigned long GetBlockSize();           // slot 13
};

struct CArrayUChar {                                // CArray<unsigned char> (STLport vector)
    unsigned char* m_start;
    unsigned char* m_finish;
    unsigned char* m_endOfStorage;
    void resize(unsigned long newSize, const unsigned char& fill = (unsigned char)0);   // 0x00486189
};

struct CMemoryFile {
    virtual void v00();
    long          AccessType;                       // +0x04
    long          Position;                         // +0x08
    long          Length;                           // +0x0c
    CArrayUChar   DataBuffer;                       // +0x10
    bool          Opened;                           // +0x1c
    CWideString   Pathname;                         // +0x20
    void OpenFromFile(CAFile* file, long length, CASuspendableProcess* process);
};

void CMemoryFile::OpenFromFile(CAFile* file, long length, CASuspendableProcess* process)
{
    Pathname = file->GetPathname();
    Position = 0;
    Length = length;
    Opened = true;
    unsigned long blockSize = file->GetBlockSize();
    DataBuffer.resize((length + blockSize - 1) / blockSize * blockSize);
    file->Read(DataBuffer.m_start, length, process);
}
// CAFile::ReadVar<int>(int* value, CASuspendableProcess* process) -- reads one int from the file; when the file needs aligned reads, through a zeroed byte buffer rounded up to the alignment
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00516D90, length 114 bytes.
// Leaf: ReadVar
// Callees: CAFile virtual slot 13 (+0x34) GetAlignment (0x00401660 for CDiskFileWin32), slot 3 (+0x0c)
// Read(void*, long, CASuspendableProcess*) (0x0099A960), 0x00514E20 std::vector<unsigned char>(size_type) (the
// VC6 vector constructor: malloc + zero fill), 0x00BFEA14 free (the inline vector destructor).
// Second byte-identical copy of the 4-byte instantiation (the catalogue names both ReadVar<int>; its callers
// (0x0051A257, 0x00826956..) are another module's, likely a second 4-byte type folded to the same code).
// Shape: GetAlignment() is called twice (the guard `<= 1` is signed, the rounding `((sizeof(T) + a - 1) / a) * a`
// unsigned: `div` + `imul`); the vector is a 12-byte frame local whose inline destructor frees its first pointer
// without a null test; the value is copied out of the buffer.
#include <string.h>
extern "C" void* __cdecl malloc(unsigned int size);              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14

class CASuspendableProcess;

struct CByteVector {                                             // VC6 Dinkumware vector<unsigned char>: first/last/end
    unsigned char* _First; unsigned char* _Last; unsigned char* _End;
    explicit CByteVector(unsigned long count);                   // 0x00514E20 (defined below)
    ~CByteVector() { free(_First); }
    unsigned char& operator[](unsigned long index) { return _First[index]; }
};

// The vector constructor is compiled in this translation unit ahead of ReadVar and kept out of line: VC7.1 then
// knows that it does not let the buffer's address escape, so the buffer's first pointer survives the virtual Read
// call in edi and feeds the copy and the destructor's free (an opaque declaration reloads it from the frame).
#pragma auto_inline(off)
CByteVector::CByteVector(unsigned long count)
    : _First(0), _Last(0), _End(0)
{
    unsigned char* data = count ? (unsigned char*)malloc(count) : 0;
    _End = data + count;
    _First = data;
    memset(data, 0, count);
    _Last = data + count;
}
#pragma auto_inline(on)

class CAFile {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);   // slot 3 (+0x0c)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual long GetAlignment() const;                                           // slot 13 (+0x34)
    template<class T> void ReadVar(T* value, CASuspendableProcess* process);
};

template<class T> void CAFile::ReadVar(T* value, CASuspendableProcess* process)
{
    if (GetAlignment() <= 1) {
        Read(value, sizeof(T), process);
        return;
    }
    unsigned long alignment = GetAlignment();
    CByteVector buffer(((sizeof(T) + alignment - 1) / alignment) * alignment);
    Read(&buffer[0], sizeof(T), process);
    *value = *(T*)&buffer[0];
}

template void CAFile::ReadVar<int>(int* value, CASuspendableProcess* process);

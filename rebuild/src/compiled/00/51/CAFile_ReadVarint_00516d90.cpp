// CAFile::ReadVar<int>(int* value, CASuspendableProcess* process) -- reads one int from the file; when the file needs aligned reads, through a zeroed byte buffer rounded up to the alignment
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00516D90, length 114 bytes.
// Leaf: ReadVar
// NF112 (2026-11-20, q6210 follow-up): the local CByteVector stand-in (an in-TU vector constructor, ??0CByteVector@@QAE@K@Z) replaced by
//   engine/CArray.h CArray<unsigned char>; the row's own name (a second ReadVar instance, queue line land1119nf93) is unchanged.
// Callees: CAFile virtual slot 13 (+0x34) GetAlignment (0x00401660 for CDiskFileWin32), slot 3 (+0x0c)
// Read(void*, long, CASuspendableProcess*) (0x0099A960), 0x00514E20 CArray<unsigned char>(unsigned int) (malloc +
// zero fill), 0x00BFEA14 free (the inline vector destructor).
// Second byte-identical copy of the 4-byte instantiation (the catalogue names both ReadVar<int>; its callers
// (0x0051A257, 0x00826956..) are another module's, likely a second 4-byte type folded to the same code).
// Shape: GetAlignment() is called twice (the guard `<= 1` is signed, the rounding `((sizeof(T) + a - 1) / a) * a`
// unsigned: `div` + `imul`); the vector is a 12-byte frame local whose inline destructor frees its first pointer
// without a null test; the value is copied out of the buffer.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_SIZE_CTOR_VISIBLE_NOINLINE
#include "engine/CArray.h"

class CASuspendableProcess;

// The buffer is CArray<unsigned char> (engine/CArray.h; FableWin.pdb ??0?$CArray@E@@QAE@I@Z = 0x00514E20, the spelling every other landed
// caller binds; Ego_d types ReadVar's buffer CArray<unsigned char>), as in the byte-identical twin 0x00516180. The size constructor is
// out of line with its body visible (FABLE_RETAIL_CARRAY_SIZE_CTOR_VISIBLE_NOINLINE): VC7.1 then knows it does not let the buffer's
// address escape, so the buffer's first pointer survives the virtual Read call in edi.

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
    CArray<unsigned char> buffer(((sizeof(T) + alignment - 1) / alignment) * alignment);
    Read(&buffer[0], sizeof(T), process);
    *value = *(T*)&buffer[0];
}

template void CAFile::ReadVar<int>(int* value, CASuspendableProcess* process);
// CDataInputStream::ReadPresizedString() -- read a long character count, then that many bytes into a zeroed byte array one longer, and return them as a CCharString (an empty CCharString when the count is zero)
// Leaf: ReadPresizedString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00996390, length 336 bytes (`ret 4`: the hidden result pointer).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0 (landed) for the two inlined Reads,
// vector<unsigned char>(n) @ 0x00411910 (the zero-filling size constructor, catalog "Buffer_AllocateAndZero"), CCharString(const char*, long)
// @ 0x0099EBF0, CCharString(const CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0, free @ 0x00BFEA14. 0x0122D70E is the empty string.
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0x0c, SourceChunkBytesRemaining +0x14).
// Shape: the inlined Read(buffer, size) of the landed ReadPixelFormat (0x00994670) with a leading `size > 0` guard (folded for the
// 4-byte count read, kept for the variable-length one); the count comes from an inlined ReadLong() helper (its frame local is read once, the value stays in ebp); the byte array is the retail CArray with the
// GetData idiom (its own address when empty); the result is a NAMED CCharString (copy-constructed into the hidden pointer, no NRVO).
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CDataInputStreamReader   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
};
namespace N_00411910 { struct CByteArray {                                        // retail vector<unsigned char>: begin/end/capacity
    unsigned char* m_begin; unsigned char* m_end; unsigned char* m_capacity;
    CByteArray(unsigned long n);                           // 0x00411910
    ~CByteArray() { if (m_begin) free(m_begin); }
    unsigned char* GetData() { return m_end - m_begin != 0 ? m_begin : (unsigned char*)this; }
    unsigned char& operator[](long index) { return m_begin[index]; }
}; } using N_00411910::CByteArray;

struct CDataInputStreamReader : CDataInputStream {         // the engine layout plus the retail member functions
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong()
    {
        long value;
        Read(&value, 4);
        return value;
    }
    CCharString ReadPresizedString();
};

CCharString CDataInputStreamReader::ReadPresizedString()
{
    long length = ReadLong();
    if (length) {
        CByteArray buffer(length + 1);
        Read(buffer.GetData(), length);
        buffer[length] = 0;
        CCharString result((const char*)buffer.GetData(), -1);
        return result;
    }
    else
        return CCharString("", -1);
}
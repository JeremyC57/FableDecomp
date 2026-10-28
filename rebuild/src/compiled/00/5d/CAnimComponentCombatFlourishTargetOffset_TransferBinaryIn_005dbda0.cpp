// CAnimComponentCombatFlourishTargetOffset::TransferBinaryIn(CMemoryDataInputStream&) -- read the flag byte and the 2D target offset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DBDA0, length 130 bytes (virtual, `ret 4`).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0 (chunk exhausted),
// CMemoryDataInputStream::ReadC2DVector() @ 0x00995030 (by-value C2DVector through the hidden pointer; manifest
// label "CPersistTraits<CFloatRange>::TransferBinaryIn" is propagated -- the body reads two floats).
//
// Shape: the landed inline CMemoryDataInputStream::Read (0x007AD7F0 family) for ONE byte: `position + 1`
// widened to __int64 and checked against [0, 0x7FFFFFFF] through the inline bool predicate (the zero high
// dword folds into `xor eax,eax` plus never-taken jumps), then the in-chunk copy (`mov dl,[eax] ; inc eax ;
// dec ebp`) or the overflow reader. The byte local is homed in the dead stream-argument slot and is read
// even when the range check fails. TargetOffset (+0x08) = byte != 0 (`test ; setne`), then the by-value
// vector is assigned straight from the call (an unnamed temporary: the copy reads through the returned
// pointer in eax) to the unaligned TargetOffsetVector_X/_Y (+0x09/+0x0d, pack 1) viewed as a C2DVector; a
// named local re-reads the frame and moves `this` from edi to ebp. `this` is the PDB header layout.
// Leaf: TransferBinaryIn
#include "engine/CAnimComponentCombatFlourishTargetOffset.h"

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class C2DVector { public:
    float X, Y;
    C2DVector() {}                              // non-POD: returned through a hidden pointer
};

class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream : public CDataInputStream { public:
    void* m_pVtable;                            // +0x00
    unsigned long m_position;                   // +0x04
    long StreamSize;
    const unsigned char* m_pData;               // +0x0c current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                           // +0x14 bytes left in the current chunk
    C2DVector ReadC2DVector();                  // 0x00995030
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

struct CAnimComponentCombatFlourishTargetOffset_Methods : CAnimComponentCombatFlourishTargetOffset {
    C2DVector& TargetOffsetVector() { return *(C2DVector*)&TargetOffsetVector_X; }
    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void CAnimComponentCombatFlourishTargetOffset_Methods::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    unsigned char targetOffset;
    stream.Read(&targetOffset, 1);
    TargetOffset = targetOffset != 0;
    TargetOffsetVector() = stream.ReadC2DVector();
}
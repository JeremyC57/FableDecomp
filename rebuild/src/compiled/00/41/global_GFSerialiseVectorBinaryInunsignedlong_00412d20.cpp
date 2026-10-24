// _global::GFSerialiseVectorBinaryIn<unsigned long>(CMemoryDataInputStream& stream, std::vector<unsigned long>& out) -- read the element count (inline checked 4-byte read), resize the vector with zeros, then read every element through CPersistTraits
// Leaf: ??$GFSerialiseVectorBinaryIn@K@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00412D20, 140 bytes (+4 int3 pad). Callees:
// CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (count slow path),
// vector<unsigned long>::resize(n, const T&) 0x00412A50, CPersistTraits<unsigned long>::TransferBinaryIn
// 0x0040FEF0 (manifest label CPersistTraits<float>; the bodies are interchangeable 4-byte copies).
//
// Retail bytes:  push ecx ; push ebx ; push ebp ; push esi ; mov esi,ecx ; mov eax,[esi+4] ; push edi ;
//   lea ecx,[eax+4] ; xor edi,edi ; xor eax,eax ; <__int64 range check of position+4: jl/jg/jb ...
//   cmp ecx,0x7fffffff ; ja> ; mov edx,[esi+0x14] ; cmp edx,4 ; jl S ; <fast read: ebx = *chunkBase,
//   chunkBase += 4, chunkLength -= 4, position = position+4> ; jmp C ; S: push 4 ; lea eax,[slot] ;
//   push eax ; call ReadWithSrcChunkOverflow ; F: mov ebx,[slot] ; C: lea ecx,[slot] ; push ecx ;
//   push ebx ; mov ecx,ebp ; mov [slot],edi ; call resize ; cmp ebx,edi ; jle X ;
//   L: mov edx,[ebp] ; lea eax,[edx+edi*4] ; push eax ; push esi ; lea ecx,[slot] ;
//   call TransferBinaryIn ; inc edi ; cmp edi,ebx ; jl L ; X: ... ret
// Identity: one of FableWin.pdb's `??$GFSerialiseVectorBinaryIn@K@@YIXAAVCMemoryDataInputStream@@...`
// / `@M@` / `@VCRGBColour@@` (all 4-byte elements; the manifest's <CRGBColour> is propagated). Its
// only caller is the CPersistContext::TransferVector body at 0x00412E80, whose binary-out arm
// (0x0040FD40) writes every element with the same WriteSLONG 0x00993EB0 as the count, and the resize
// callee carries the vector<unsigned int> label, so unsigned long is used. __fastcall (stream ecx,
// vector edx). One frame word serves three lifetimes: the inline count read's block-scoped value
// (the landed 0x006194A0 ReadCount helper), the resize(n) default temporary `T()` (stored 0 from the
// zero edi), and the block-scoped traits object whose `this` is the same word.
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (CDataInputStream)
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

template <class T>
class CRetailVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
    void resize(unsigned int newSize, const T& value);   // 0x00412A50 / 0x004129F0 (out of line per T)
    void resize(unsigned int newSize) { resize(newSize, T()); }
};

template <class T> struct CPersistTraits {
    void TransferBinaryIn(CMemoryDataInputStreamReader& stream, T* out) const;   // 0x0040FEF0 (CPersistTraits<unsigned long>)
};

__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStreamReader& stream, CRetailVector<T>& out)
{
    long count;
    ReadCount(stream, &count);
    out.resize(count);
    {
        CPersistTraits<T> traits;
        for (long i = 0; i < count; ++i)
            traits.TransferBinaryIn(stream, &out[i]);
    }
}
template void __fastcall GFSerialiseVectorBinaryIn<unsigned long>(CMemoryDataInputStreamReader& stream, CRetailVector<unsigned long>& out);
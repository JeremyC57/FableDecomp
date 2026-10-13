// CPersistTraits<T>::TransferBinaryIn(CMemoryDataInputStream*, T*) -- inlined 4-byte read (struct, movsd copy), 64-bit position range check, ReadWithSrcChunkOverflow @ 0x00993CA0 when the chunk runs out; frame-pointer TU
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE5A7, oracle length 99 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebp ; mov ebp,esp ; mov ecx,[ebp+8] ; mov eax,[ecx+4] ; lea edx,[eax+4] ; xor eax,eax ; jl R ; jg B ; test lo,lo ; jb R ; B: test eax,eax ; jg R ; jl F ; cmp lo,7FFFFFFFh ; ja R ; F: push ebx ; mov ebx,[ecx+14h] ; cmp ebx,4 ; jl S ; mov eax,[ecx+0Ch] ; push esi ; push edi ; mov esi,eax ; lea edi,[ebp+8] ; movsd ; add eax,4 ; add ebx,-4 ; pop edi ; mov [ecx+0Ch],eax ; mov [ecx+14h],ebx ; mov [ecx+4],edx ; pop esi ; jmp J ; S: push 4 ; lea eax,[ebp+8] ; push eax ; call ReadWithSrcChunkOverflow ; J: pop ebx ; R: mov eax,[ebp+0Ch] ; mov ecx,[ebp+8] ; mov [eax],ecx ; pop ebp ; ret 8
// Binary persist read of one 4-byte value: a local `value` (homed in the dead stream-argument
// slot) is filled by the inlined CMemoryDataInputStream::Read(buffer, size) and copied to the
// output. Read widens `position + size` to __int64 and rejects it outside [0, 0x7FFFFFFF]
// (the zero high dword is materialised and compared like any 64-bit value); with enough bytes
// left in the current chunk it memcpy's from the chunk pointer (+0xC), advances it, decrements
// the remaining count (+0x14) and adds the size to the position (+4) -- `+=`, which keeps the
// old position live and gives the `lea` form -- otherwise it defers to
// CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0. A 4-byte struct value makes the memcpy a movsd; the TU keeps frame pointers and optimises for size.
// The manifest label (`CPersistTraits<...>::TransferBinaryIn`) is propagated; the value type is
// modelled by size. One of 70 bodies of this shape.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStream {
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long m_10;
    long m_remaining;                      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
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
struct SPersistValue4 { unsigned short m_low; unsigned short m_high; };

void __stdcall TransferBinaryIn_004de5a7(CMemoryDataInputStream* stream, SPersistValue4* out)
{
    SPersistValue4 value;
    stream->Read(&value, sizeof(value));
    *out = value;
}

// CPersistTraits<T>::TransferBinaryIn(CMemoryDataInputStream*, T*) -- inlined 4-byte read (scalar), 64-bit position range check through an inline bool predicate (its exits share the duplicated return tails the retail way), ReadWithSrcChunkOverflow @ 0x00993CA0 when the chunk runs out
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E258A0, oracle length 129 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx,[esp+4] ; mov eax,[ecx+4] ; push esi ; lea esi,[eax+4] ; xor eax,eax ; jl R ; jg B ; test lo,lo ; jb R ; B: test eax,eax ; jg R ; jl F ; cmp lo,7FFFFFFFh ; ja R ; F: mov edx,[ecx+14h] ; cmp edx,4 ; jl S ; mov eax,[ecx+0Ch] ; push edi ; mov edi,[eax] ; add eax,4 ; mov [ecx+0Ch],eax ; mov eax,[esp+10h] ; add edx,-4 ; mov [ecx+4],esi ; mov [ecx+14h],edx ; mov [eax],edi ; pop edi ; pop esi ; ret 8 ; S: push 4 ; lea edx,[esp+0Ch] ; push edx ; call ReadWithSrcChunkOverflow ; R: mov ecx,[esp+8] ; mov eax,[esp+0Ch] ; mov [eax],ecx ; pop esi ; ret 8  (two more copies of R for the range-check exits)
// Binary persist read of one 4-byte value: a local `value` (homed in the dead stream-argument
// slot) is filled by the inlined CMemoryDataInputStream::Read(buffer, size) and copied to the
// output. Read widens `position + size` to __int64 and rejects it outside [0, 0x7FFFFFFF]
// (the zero high dword is materialised and compared like any 64-bit value); with enough bytes
// left in the current chunk it memcpy's from the chunk pointer (+0xC), advances it, decrements
// the remaining count (+0x14) and adds the size to the position (+4) -- `+=`, which keeps the
// old position live and gives the `lea` form -- otherwise it defers to
// CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0. A scalar value lets the memcpy forward straight into the output; the TU has no frame pointer.
// The manifest label (`CPersistTraits<...>::TransferBinaryIn`) is propagated; the value type is
// modelled by size. One of 31 bodies of this shape.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStream {
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
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

void __stdcall TransferBinaryIn_00e258a0(CMemoryDataInputStream* stream, long* out)
{
    long value;
    stream->Read(&value, sizeof(value));
    *out = value;
}